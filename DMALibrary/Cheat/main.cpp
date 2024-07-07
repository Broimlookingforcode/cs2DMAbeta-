#include "pch.h" // Include the precompiled header

#include <iostream>
#include <DMALibrary/Memory/Memory.h>
#include "kmbox_interface.hpp"
#include <windows.h> 




inline void km_space() {
    std::string command = "km.space(" + std::to_string(1) + ")\r\n"; // space bar down
    Sleep(10); // to stop it from crashing idk
    std::string command1 = "km.space(" + std::to_string(0) + ")\r\n"; // space bar up
    send_command(hSerial, command.c_str());
    send_command(hSerial, command1.c_str());
}

int main() {
    // Find and open the kmbox port
    std::string port = find_port("USB-SERIAL CH340"); // name of the kmbox port without ( COM )
    if (port.empty()) {
        std::cout << "\n\t[!] No port found.." << std::endl;
        return 1;
    }

    if (!open_port(hSerial, port.c_str(), CBR_115200)) { // CBR_115200 is the baud rate
        std::cout << "\n\t[!] Opening the port failed!" << std::endl;
        return 2;
    }

    std::cout << "\n\t[+] Connected to the kmbox with " + port << std::endl;

  
    if (!mem.Init("cs2.exe", true, true)) {
        std::cout << "Failed to initialize DMA" << std::endl;
        return 3;
    }
    else
    {
        std::cout << "DMA initialized" << std::endl;
    }


    if (!mem.GetKeyboard()->InitKeyboard()) {
        std::cout << "Failed to initialize keyboard hotkeys through kernel." << std::endl;
        return 4;
    }

    uintptr_t base = mem.GetBaseDaddy("client.dll");


 
    while (true)
    {

        Sleep(1);



        uintptr_t localPlayer = mem.Read<uintptr_t>(base + 0x1823A08);

        int32_t m_fFlags = mem.Read<int32_t>(localPlayer + 0x63);

        // Check if the space bar is pressed and the player is on the ground
        if (mem.GetKeyboard()->IsKeyDown(0x20) && (m_fFlags & (1 << 0))) {
            km_space(); // Call the km_space function to simulate space bar press
        }
    }

    if (!mem.FixCr3()) {
        std::cout << "Failed to fix CR3" << std::endl;
    }
    else {
        std::cout << "CR3 fixed" << std::endl;
    }

    return 10; // Ensure main returns an integer
}

