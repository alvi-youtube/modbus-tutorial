
// #include <Windows.h>
#include <errno.h>
#include <iostream>

#include "../libmodbus/modbus.h"

const int NB_BITS = 16;            // number of bits or coils
const int NB_INPUT_BITS = 16;      // number of input bits
const int NB_REGISTERS = 16;       // number of holding registers
const int NB_INPUT_REGISTERS = 16; // number of input registers

const uint8_t UT_INPUT_BITS_TAB[] = {0xFF, 0xDB};

int main()
{
    int s = -1;
    int rc = 0;
    int i = 0;

    uint8_t buf[MODBUS_TCP_MAX_ADU_LENGTH] = {};
    modbus_t *ctx;
    modbus_mapping_t *map;

    // create context
    ctx = modbus_new_tcp(NULL, 1502);

    // create mapping
    map = modbus_mapping_new_start_address(0, NB_BITS, 0, NB_INPUT_BITS, 0, NB_REGISTERS, 0, NB_INPUT_REGISTERS);

    if (map == NULL)
    {
        std::cout << "Failed to allocate the mapping: " << modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        return -1;
    }
    else
    {
        std::cout << "Mapping created" << std::endl;
        std::cout << "Bits: " << NB_BITS << std::endl;
        std::cout << "Input bits: " << NB_INPUT_BITS << std::endl;
        std::cout << "Registers: " << NB_REGISTERS << std::endl;
        std::cout << "Input registers: " << NB_INPUT_REGISTERS << std::endl;
    }

    map->tab_input_registers[0] = 15;
    map->tab_input_registers[1] = 16;
    map->tab_input_registers[2] = 17;
    map->tab_input_registers[3] = 18;

    map->tab_registers[0] = 10;
    map->tab_registers[1] = 11;
    map->tab_registers[2] = 12;
    map->tab_registers[3] = 13;

    while (1)
    {
        s = modbus_tcp_listen(ctx, 1);

        if (s < 0)
        {
            std::cout << "Failed to listen: " << modbus_strerror(errno) << std::endl;
            Sleep(5000);
        }
        else
        {
            std::cout << "Connected" << std::endl;
            modbus_tcp_accept(ctx, &s);
            break;
        }
    }

    struct mb_tcp
    {
        uint16_t transact;
        uint16_t protocol;
        uint16_t length;
        uint8_t unit;
        uint8_t fcode;
        uint8_t data[];
    };

    while (1)
    {
        do
        {
            rc = modbus_receive(ctx, buf);
            std::cout << "Received: " << rc << std::endl;

        } while (rc == 0);

        if (rc < 0)
        {
            std::cout << "Error: " << modbus_strerror(errno) << std::endl;
            closesocket(s);
            break;
        }

        // struct mb_tcp *mb = (struct mb_tcp *)buf;
        rc = modbus_reply(ctx, buf, rc, map);
        if (rc < 0)
        {
            std::cout << "Error: " << modbus_strerror(errno) << std::endl;
        }
        Sleep(2000);
    }
    closesocket(s);
    modbus_free(ctx);
    modbus_mapping_free(map);

    std::cout << "Slave" << std::endl;
    return 0;
}
