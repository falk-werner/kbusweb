#include "kbusweb/kbus.hpp"

namespace kbusweb
{

kbus::kbus()
{

}

kbus::~kbus()
{
}

kbus_status kbus::get_status()
{
    std::lock_guard lock(mut);
    return status_;
}

std::vector<terminal_info> kbus::get_terminal_infos()
{
    std::lock_guard lock(mut);
    return terminal_infos_;
}

void kbus::write_bool(uint32_t offset, bool value)
{
    std::lock_guard lock(mut);
    commands.push_back({offset, value});

}

void kbus::set_status(kbus_status const & status)
{
    std::lock_guard lock(mut);
    status_ = status;

}
void kbus::set_terminal_infos(std::vector<terminal_info> const & terminal_infos)
{
    std::lock_guard lock(mut);
    terminal_infos_ = terminal_infos;
}

std::vector<command> kbus::get_commands()
{
    std::lock_guard lock(mut);
    auto result = commands;
    commands = {};
    return result;
}

}