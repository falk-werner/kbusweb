#ifndef KBUSWEB_KBUS_HPP
#define KBUSWEB_KBUS_HPP

#include "kbusweb/kbus_status.hpp"
#include "kbusweb/terminal_info.hpp"
#include "kbusweb/command.hpp"

#include <mutex>
#include <vector>

namespace kbusweb
{

class kbus
{
    kbus(kbus const &) = delete;
    kbus& operator=(kbus const &) = delete;
    kbus(kbus &&) = delete;
    kbus& operator=(kbus &&) = delete;
public:
    kbus();
    ~kbus();

    kbus_status get_status();
    std::vector<terminal_info> get_terminal_infos();
    void write_bool(uint32_t offset, bool value);

    void set_status(kbus_status const & status);
    void set_terminal_infos(std::vector<terminal_info> const & terminal_infos);
    std::vector<command> get_commands();

private:
    std::mutex mut;
    kbus_status status_;
    std::vector<terminal_info> terminal_infos_;
    std::vector<command> commands;
};

}

#endif
