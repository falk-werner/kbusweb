#include "kbusweb/kbus_info.hpp"

extern "C"
{
	#include <ldkc_kbus_information.h>
    #include <ldkc_kbus_register_communication.h>
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

std::vector<terminal_info> kbus_info::get_terminal_infos()
{
    ldkc_KbusInfo_TerminalInfo terminals[LDKC_KBUS_TERMINAL_COUNT_MAX];
    size_t count = 0;
    auto result = ldkc_KbusInfo_GetTerminalInfo(LDKC_KBUS_TERMINAL_COUNT_MAX, terminals, &count);
    if (result != KbusInfo_Ok)
    {
        throw std::runtime_error("failed to get terminal info");
    }

    uint16_t terminal_types[LDKC_KBUS_TERMINAL_COUNT_MAX];
    count = 0;
    result = ldkc_KbusInfo_GetTerminalList(LDKC_KBUS_TERMINAL_COUNT_MAX, terminal_types, &count);
    if (result != KbusInfo_Ok)
    {
        throw std::runtime_error("failed to get terminal info");
    }

    std::vector<terminal_info> infos;
    for(size_t i = 0; i < count; i++)
    {
        terminal_info info;
        info.type = terminal_types[i];

        auto const & terminal = terminals[i];
        info.offset_input_bits = terminal.OffsetInput_bits;
        info.offset_output_bits = terminal.OffsetOutput_bits;
        info.size_input_bits = terminal.SizeInput_bits;
        info.size_output_bits = terminal.SizeOutput_bits;
        info.channel_count = terminal.AdditionalInfo.ChannelCount;
        info.pi_format = terminal.AdditionalInfo.PiFormat;

        infos.emplace_back(info);
    }

    return infos;
}

}