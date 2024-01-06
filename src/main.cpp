#include <iostream>
#include <cstring>

#include "memory.hpp"
#include "registers.hpp"
#include "interpreter.hpp"
#include "instructions.hpp"

int main()
{
    // Initialize
    uint32_t memory_size = 0xbeef;
    auto memory = Memory(memory_size);
    auto s = "Hello, World!";
    auto s2 = "I'm Chris!";
    memory.write(0x00, (uint8_t *)s, strlen(s));
    memory.write(0xbedb, (uint8_t *)s2, strlen(s2));

    auto rf = RegisterFile{
        .registers = {
            0xdead, // R0
            0xbeef, // R1
            0xaaaa, // R2
            0xbbbb, // R3
            0x0000, // IP
        },
    };

    Interpreter interpreter = {
        .memory = memory,
        .registers = rf,
    };

    interpreter.memory.inspect();
    interpreter.registers.inspect();

    auto program = Memory(0x1000);
    auto main_addr = 0x00;
    auto addr = main_addr;

    // MOV_LIT_REG 0x05 R1
    addr += program.write8(addr, (uint8_t)OpCode::MOV_LIT_REG);
    addr += program.write32(addr, 0x12121212);
    addr += program.write8(addr, (uint8_t)Register::R1);

    // STORE_LIT_MEM 0xffffffff 0xdead
    addr += program.write8(addr, (uint8_t)OpCode::STORE_LIT_MEM);
    addr += program.write32(addr, 0xffffffff);
    addr += program.write32(addr, 0x1234);

    // RETURN
    addr += program.write8(addr, (uint8_t)OpCode::RETURN); // RETURN

    std::cout << "Program:" << std::endl;
    program.inspect();
    std::cout << std::endl;

    // Run
    auto ret = interpreter.run(&program, main_addr);
    std::cout << "R0: " << hexstr16(ret) << std::endl;

    // Inspect
    interpreter.memory.inspect();
    interpreter.registers.inspect();
}
