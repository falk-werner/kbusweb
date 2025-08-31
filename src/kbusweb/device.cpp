#include "kbusweb/device.hpp"
#include <stdexcept>

namespace kbusweb
{

namespace
{

constexpr uint32_t const task_id = 0;

}

device::device(tApplicationDeviceInterface * adi, tDeviceId id)
: adi_(adi)
, id_(id)
{
    auto const result = adi_->OpenDevice(id_);
    if (result != DAL_SUCCESS)
    {
        throw std::runtime_error("failed to open ADI device");
    }
}

device::~device()
{
    adi_->CloseDevice(id_);
}

bool device::push_kbus()
{
    uint32_t retval = 0;
    auto const result = adi_->CallDeviceSpecificFunction("libpackbus_Push", &retval);
    if (DAL_SUCCESS != result)
    {
        throw std::runtime_error("failed to call device specific funcion");
    }

    return (DAL_SUCCESS == retval);
}

void device::write_start()
{
    adi_->WriteStart(id_, task_id);
}

void device::write_end()
{
    adi_->WriteEnd(id_, task_id);

}

void device::write_bool(uint32_t offset, bool value)
{
    adi_->WriteBool(id_, task_id, offset, value);
}


}
