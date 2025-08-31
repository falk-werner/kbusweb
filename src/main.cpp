#include "kbusweb/kbus.hpp"
#include "kbusweb/webserver.hpp"
#include "kbusweb/device_manager.hpp"
#include "kbusweb/device.hpp"
#include "kbusweb/kbus_info.hpp"

#include <getopt.h>
#include <unistd.h>

#include <csignal>
#include <iostream>

using kbusweb::device_manager;
using kbusweb::device;
using kbusweb::kbus_info;
using kbusweb::kbus;
using kbusweb::webserver;

namespace
{

enum class command
{
	show_help,
	run
};


struct context
{
	context(int argc, char* argv[])
	: cmd(command::run)
	, exit_code(EXIT_SUCCESS)
	, port(8080)
	{
		option const long_opts[] =
		{
			{"port", required_argument, nullptr, 'p'},
			{"help", no_argument, nullptr, 'h'},
			{nullptr, 0, nullptr, 0}
		};

		optind = -1;
		opterr = 0;

		bool done = false;
		while (!done)
		{
			int option_index = 0;
			int const c = getopt_long(argc, argv, "p:h", long_opts, &option_index);
			switch (c)
			{
				case -1:
					done = true;
					break;
				case 'p':
					port = std::stoi(optarg);
					break;
				case 'h':
					cmd = command::show_help;
					break;
				default:
					std::cerr << "error: invalid argument" << std::endl;
					cmd = command::show_help;
					exit_code = EXIT_FAILURE;
					done = true;
					break;
			}
		}
	}

	command cmd;
	int exit_code;
	uint16_t port;
};

void print_usage()
{
	std::cout << R"(kbusweb, (C) 2025 Falk Werner
Simple HTTP access to KBus.

Usage:
	kbusweb [-p PORT]

Options:
	-p, --port - Optional. Set webserver port (default: 8080)
)";
}

bool shutdown_requested = false;
void on_shutdown_requested(int signal_id)
{
	(void) signal_id;
	shutdown_requested = true;
}

void run(uint16_t port)
{
	signal(SIGHUP, SIG_IGN);
	signal(SIGINT, &on_shutdown_requested);
	signal(SIGTERM, &on_shutdown_requested);
	constexpr const useconds_t cycle_interval = 10 * 1000;

	device_manager manager;
	device kbus_device = manager.open_device("libpackbus");
	manager.set_appstate_unconfigured();

	kbus bus;
	kbus_info info;
	bus.set_status(info.get_status());

	webserver web(port, &bus);

	constexpr int const kbus_prio = 40;
	struct sched_param s_param;
    s_param.sched_priority = kbus_prio;
    sched_setscheduler(0, SCHED_FIFO, &s_param);	

	manager.set_appstate_running();
	while (!shutdown_requested)
	{
		kbus_device.push_kbus();
		bus.set_status(info.get_status());

		auto const commands = bus.get_commands();
		if (!commands.empty())
		{
			kbus_device.write_start();
			for(auto const & command: commands)
			{
				kbus_device.write_bool(command.offset, command.value);
			}
			kbus_device.write_end();
		}

		usleep(cycle_interval);
	}
}

}

int main(int argc, char * argv[])
{
	int exit_code = EXIT_SUCCESS;

	try
	{
		context ctx(argc, argv);
		exit_code = ctx.exit_code;

		switch(ctx.cmd)
		{
			case command::run:
				run(ctx.port);
				break;
			case command::show_help:
				print_usage();
				break;
		}
	}
	catch (std::exception const &  ex)
	{
		std::cerr << "error: " << ex.what() << std::endl;
		exit_code = EXIT_FAILURE;
	}
	catch (...)
	{
		std::cerr << "fatal: " << std::endl;
		exit_code = EXIT_FAILURE;
	}

	return exit_code;
}