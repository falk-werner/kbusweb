
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMICROHTTPD REQUIRED IMPORTED_TARGET libmicrohttpd)

find_path(OSLINUX_HEADERS OsCommon.h
	/usr/include/OsLinux)


add_executable(kbusweb
	src/kbusweb/device_manager.cpp
	src/kbusweb/device.cpp
	src/kbusweb/kbus_info.cpp
	src/kbusweb/kbus.cpp
	src/kbusweb/webserver.cpp
	src/main.cpp
)

target_include_directories(kbusweb PRIVATE
	src
	"${OSLINUX_HEADERS}")

target_link_libraries(kbusweb PRIVATE
	dal dbuskbuscommon PkgConfig::LIBMICROHTTPD pthread)
#	dal libloader oslinux dbuskbuscommon ffi)

install(TARGETS kbusweb RUNTIME DESTINATION bin)

