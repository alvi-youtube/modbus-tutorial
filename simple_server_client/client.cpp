#include <iostream>

#include "../libmodbus/modbus.h"

int main()
{
    modbus_t *mb;
    uint16_t tab_reg[64]; // Array to store register data
    uint32_t old_response_to_sec;
    uint32_t old_response_to_usec;

    // Create a new TCP Modbus connection
    mb = modbus_new_tcp("127.0.0.1", 1502);
    modbus_set_response_timeout(mb, 10, 0);
    if (modbus_connect(mb) == -1)
    {
        std::cerr << "Connection failed: " << modbus_strerror(errno) << std::endl;
        modbus_free(mb);
        return -1;
    }

    // Read 4 holding registers starting from address 0
    int num = modbus_read_registers(mb, 0, 4, tab_reg);
    std::cout << "Number of registers read: " << num << std::endl;
    for (int i = 0; i < num; i++)
    {
        std::cout << "Register " << i << ": " << tab_reg[i] << std::endl;
    }

    num = modbus_read_input_registers(mb, 0, 4, tab_reg);
    for (int i = 0; i < num; i++)
    {
        std::cout << "Input Register " << i << ": " << tab_reg[i] << std::endl;
    }
    std::cout << "Number of Input registers read: " << num << std::endl;

    modbus_get_response_timeout(mb, &old_response_to_sec, &old_response_to_usec);
    std::cout << "Response timeout: " << old_response_to_sec << "s " << old_response_to_usec << "us" << std::endl;

    modbus_close(mb);
    modbus_free(mb);

    return 0;
}
