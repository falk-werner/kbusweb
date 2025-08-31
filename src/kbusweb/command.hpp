#ifndef KBUSWEB_COMMAND_HPP
#define KBUSWEB_COMMAND_HPP

#include <cinttypes>

namespace kbusweb
{

struct command
{
    uint32_t offset;
    bool value;
};

}

#endif
