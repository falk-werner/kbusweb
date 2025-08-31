#ifndef KBUSWEB_DEVICE_HPP
#define KBUSWEB_DEVICE_HPP

extern "C"
{
	#include <dal/adi_application_interface.h>
}

namespace kbusweb
{

class device
{
    device(device const &) = delete;
    device& operator=(device const &) = delete;
    device(device &&) = delete;
    device& operator=(device &&) = delete;
public:
    device(tApplicationDeviceInterface * adi, tDeviceId id);
    ~device();
    bool push_kbus();
    void write_start();
    void write_end();
    void write_bool(uint32_t offset, bool value);
private:
    tApplicationDeviceInterface * adi_;
    tDeviceId id_;
};

}


#endif
