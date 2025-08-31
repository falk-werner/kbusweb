#include "kbusweb/device_manager.hpp"
#include <stdexcept>

namespace kbusweb
{

namespace
{

tDeviceId get_device_by_name(tApplicationDeviceInterface * adi, std::string const & name)
{
    adi->ScanDevices();
    constexpr size_t const max_devices = 10;

    tDeviceInfo devices[max_devices];
    size_t count = 0;
    adi->GetDeviceList(max_devices, devices, &count);
    for (size_t i = 0; i < count; i++)
    {
        if (name == devices[i].DeviceName)
        {
            return devices[i].DeviceId;
        }
    }

    throw std::runtime_error("failed to get device");
}

void set_appstate(tApplicationDeviceInterface * adi, tApplicationState state)
{
    tApplicationStateChangedEvent event;
    event.State = state;

    auto const result = adi->ApplicationStateChanged(event);
    if (result != DAL_SUCCESS)
    {
        throw std::runtime_error("failed to set application state");
    }

}

}

device_manager::device_manager()
{
    adi = adi_GetApplicationInterface();
    adi->Init();
}

device_manager::~device_manager()
{
    adi->Exit();
}

device device_manager::open_device(std::string const & name)
{
    auto device_id = get_device_by_name(adi, name);
    return device(adi, device_id);
}

void device_manager::set_appstate_unconfigured()
{
    set_appstate(adi, ApplicationState_Unconfigured);
}

void device_manager::set_appstate_running()
{
    set_appstate(adi, ApplicationState_Running);
}


}