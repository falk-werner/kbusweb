#ifndef KBUSWEB_DEVICE_MANAGER_HPP
#define KBUSWEB_DEVICE_MANAGER_HPP

extern "C"
{
	#include <dal/adi_application_interface.h>
}

#include "kbusweb/device.hpp"
#include <string>

namespace kbusweb
{

class device_manager
{
    device_manager(device_manager const &) = delete;
    device_manager& operator=(device_manager const &) = delete;
    device_manager(device_manager &&) = delete;
    device_manager& operator=(device_manager &&) = delete;
public:
    device_manager();
    ~device_manager();
    device open_device(std::string const & name);
    void set_appstate_unconfigured();
    void set_appstate_running();
private:
    tApplicationDeviceInterface * adi;

};

}


#endif
