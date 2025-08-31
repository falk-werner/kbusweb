#ifndef KBUSWEB_TERMINAL_INFO_HPP
#define KBUSWEB_TERMINAL_INFO_HPP

#include <cinttypes>

namespace kbusweb
{

struct terminal_info
{
    uint16_t type;
    uint16_t offset_input_bits;
    uint16_t offset_output_bits;
    uint16_t size_input_bits;
    uint16_t size_output_bits;
    uint16_t channel_count;
    uint16_t pi_format;
};

}

#endif
