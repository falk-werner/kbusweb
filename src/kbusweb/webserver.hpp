#ifndef KBUSWEB_WEBSERVER_HPP
#define KBUSWEB_WEBSERVER_HPP

#include "kbusweb/kbus.hpp"
#include <microhttpd.h>

#include <cinttypes>

namespace kbusweb
{

class webserver
{
    webserver(webserver const &) = delete;
    webserver& operator=(webserver const &) = delete;
    webserver(webserver &&) = delete;
    webserver& operator=(webserver &&) = delete;
public:
    webserver(uint16_t port, kbus * bus);
    ~webserver();
private:
    MHD_Daemon * daemon;
};

}

#endif
