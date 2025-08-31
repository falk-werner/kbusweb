#ifndef KBUSWEB_KBUS_STATUS_HPP
#define KBUSWEB_KBUS_STATUS_HPP

#include <cinttypes>

namespace kbusweb
{

struct kbus_status
{
    uint16_t bit_count;
    uint16_t terminal_count;
    uint16_t error_code;
    uint16_t error_arg;
    uint16_t error_pos;
    uint16_t bit_count_analog_input;
    uint16_t bit_count_analog_output;
    uint16_t bit_count_digital_input;
    uint16_t bit_count_digital_output;
};

}

#endif
