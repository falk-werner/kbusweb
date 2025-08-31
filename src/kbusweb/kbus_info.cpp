#include "kbusweb/kbus_info.hpp"

extern "C"
{
	#include <ldkc_kbus_information.h>
}

#include <stdexcept>

namespace kbusweb
{

kbus_info::kbus_info()
{
    ldkc_KbusInfo_Create();
}

kbus_info::~kbus_info()
{
    ldkc_KbusInfo_Destroy();

}

kbus_status kbus_info::get_status()
{
    tldkc_KbusInfo_Status status;
    auto const result = ldkc_KbusInfo_GetStatus(&status);
    if (KbusInfo_Ok != result)
    {
        throw std::runtime_error("failed to get KBus status");
    }

    kbus_status retval;
    retval.bit_count = status.KbusBitCount;
    retval.terminal_count = status.TerminalCount;
    retval.error_code = status.ErrorCode;
    retval.error_arg = status.ErrorArg;
    retval.error_pos = status.ErrorPos;
    retval.bit_count_analog_input = status.BitCountAnalogInput;
    retval.bit_count_analog_output = status.BitCountAnalogOutput;
    retval.bit_count_digital_input = status.BitCountDigitalInput;
    retval.bit_count_digital_output = status.BitCountDigitalOutput;

    return retval;
}

}