#ifndef KBUSWEB_KBUS_INFO_HPP
#define KBUSWEB_KBUS_INFO_HPP

#include "kbusweb/kbus_status.hpp"

namespace kbusweb
{

class kbus_info
{
    kbus_info(kbus_info const &) = delete;
    kbus_info& operator=(kbus_info const &) = delete;
    kbus_info(kbus_info &&) = delete;
    kbus_info& operator=(kbus_info &&) = delete;
public:
    kbus_info();
    ~kbus_info();
    kbus_status get_status();
};

}

#endif
