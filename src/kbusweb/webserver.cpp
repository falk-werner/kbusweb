#include "kbusweb/webserver.hpp"

#include <microhttpd.h>
#include <nlohmann/json.hpp>


#include <cstring>
#include <stdexcept>

using json = nlohmann::json;

namespace kbusweb
{

namespace
{

int respond(MHD_Connection * connection, std::string const & text, std::string content_type)
{
    struct MHD_Response * response = MHD_create_response_from_buffer(
        text.size(),
        reinterpret_cast<void *>(const_cast<char*>(text.c_str())),
        MHD_RESPMEM_MUST_COPY);
        MHD_add_response_header(response, "Content-Type", content_type.c_str());
    int result = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);

    return result;

}

int handle_root(MHD_Connection * connection)
{
    std::string const text = R"(<!doctype html>
<html>
<head>
    <title>KBus Web</title>
    <style type="text/css">
        th {
            text-align: left;
        }
    </style>
    <script type="text/javascript">

    async function startup() {
        const response = await fetch("/status");
        const status = await response.json();
        document.querySelector("#bitcount").textContent = status.BitCount;
        document.querySelector("#terminal_count").textContent = status.TerminalCount;
        document.querySelector("#error_code").textContent = status.ErrorCode;
        document.querySelector("#error_arg").textContent = status.ErrorArg;
        document.querySelector("#error_pos").textContent = status.ErrorPos;
        document.querySelector("#bitcount_ai").textContent = status.BitCountAnalogInput;
        document.querySelector("#bitcount_ao").textContent = status.BitCountAnalogOuput;
        document.querySelector("#bitcount_di").textContent = status.BitCountDigitalInput;
        document.querySelector("#bitcount_do").textContent = status.BitCountDigitalOutput;

        document.querySelector("#write_bool").addEventListener("click", write_bool);
    }

    async function write_bool() {
        const offset = document.querySelector("#write_bool_offset").value;
        const value = document.querySelector("#write_bool_value").checked ? "1" : "0";

        fetch(`/write/bool?offset=${offset}&value=${value}`);
    }

    </script>
</head>
<body onload="startup();">
    <h1>KBus Web</h1>

    <h2>Status</h2>
    <table>
        <tr><th>KBus Bit Count</th><td id="bitcount"></td></tr>
        <tr><th>Terminal Count</th><td id="terminal_count"></td></tr>
        <tr><th>Error Code</th><td id="error_code"></td id=""></tr>
        <tr><th>Error Arg</th><td id="error_arg"></td></tr>
        <tr><th>Error Pos</th><td id="error_pos"></td></tr>
        <tr><th>Bit Count Analog Input</th><td id="bitcount_ai"></td></tr>
        <tr><th>Bit Count Analog Output</th><td id="bitcount_ao"></td></tr>
        <tr><th>Bit Count Digital Input</th><td id="bitcount_di"></td></tr>
        <tr><th>Bit Count Digital Output</th><td id="bitcount_do"></td></tr>
    </table>

    <h2>Process Data</h2>
    <h3>Write Bool</h3>
    <div>
        <label for="write_bool_offset">Offset:</label>
        <input name="write_bool_offset" id="write_bool_offset" type="text" value="0" />
    </div>
    <div>
        <label for="write_boot_value">Value:</label>
        <input name="write_bool_value" id="write_bool_value" type="checkbox" />
    </div>
    <div>
        <input name="write_bool" id="write_bool" type="button" value="Write" />
    </div>
</body>
</html>)";

    return respond(connection, text, "text/html");
}

int handle_status(MHD_Connection * connection, kbus & bus)
{
    auto const status = bus.get_status();
    json data;
    data["BitCount"] = status.bit_count;
    data["TerminalCount"] = status.terminal_count;
    data["ErrorCode"] = status.error_code;
    data["ErrorArg"] = status.error_arg;
    data["ErrorPos"] = status.error_pos;
    data["BitCountAnalogInput"] = status.bit_count_analog_input;
    data["BitCountAnalogOutput"] = status.bit_count_analog_output;
    data["BitCountDigitalInput"] = status.bit_count_digital_input;
    data["BitCountDigitalOutput"] = status.bit_count_digital_output;
    auto text = data.dump();

    return respond(connection, text, "application/plain");
}

int handle_write_bool(MHD_Connection * connection, kbus & bus)
{
    try
    {
        char const * offset_str = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "offset");
        if (nullptr == offset_str)
        {
            throw std::runtime_error("missing required argument: offset");
        }
        uint16_t const offset = std::stoi(offset_str);

        char const * value_str = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "value");
        if (nullptr == value_str)
        {
            throw std::runtime_error("missing required argument: value");
        }
        bool const value = (0 == strcmp(value_str, "1")) || (0 == strcmp(value_str, "true"));

        bus.write_bool(offset, value);

        json data;
        data["status"] = "OK";
        auto const text = data.dump();
        return respond(connection, text, "application/json");
    }
    catch (std::exception const & ex)
    {
        json error;
        error["code"] = -1;
        error["message"] = ex.what();

        json data;
        data["error"] = error;

        auto const text = data.dump();
        return respond(connection, text, "application/json");
    }
}

int handle_request(
    void * cls,
    struct MHD_Connection * connection,
    const char * url,
    const char * method,
    const char * version,
    const char * upload_data,
    size_t * upload_data_size,
    void ** req_cls)
{
    kbus * bus = reinterpret_cast<kbus*>(cls);

    if (0 == strcmp(method, "GET"))
    {
        *req_cls = nullptr;
        if (0 == strcmp(url, "/"))
        {
            return handle_root(connection);
        }
        else if (0 == strcmp(url, "/status"))
        {
            return handle_status(connection, *bus);
        }
        else if (0 == strcmp(url, "/write/bool"))
        {
            return handle_write_bool(connection, *bus);
        }
        else
        {
            char const * response_text = "Not Found";
            struct MHD_Response * response = MHD_create_response_from_buffer(
                strlen(response_text),
                reinterpret_cast<void *>(const_cast<char*>(response_text)),
                MHD_RESPMEM_PERSISTENT);
            MHD_add_response_header(response, "Content-Type", "text/plain");
            int result = MHD_queue_response(connection, MHD_HTTP_NOT_FOUND, response);
            MHD_destroy_response(response);

            return result;
        }
    }


    return MHD_NO;    
}


}


webserver::webserver(uint16_t port, kbus * bus)
{
    void * data = reinterpret_cast<void*>(bus);

    daemon = MHD_start_daemon(
        MHD_USE_AUTO | MHD_USE_INTERNAL_POLLING_THREAD | MHD_USE_ERROR_LOG,
        port,
        nullptr, nullptr,
        &handle_request, data,
        MHD_OPTION_CONNECTION_TIMEOUT, (unsigned int) 120,
        MHD_OPTION_END);
    if (daemon == nullptr)
    {
        throw std::runtime_error("failed to start webserver");
    }

}

webserver::~webserver()
{
    MHD_stop_daemon(daemon);
}

}