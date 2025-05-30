#include <iostream>
#include "File_exe.hpp"          // Your CsvToolkit library
#include "EsportsAnalytics.hpp"  // Assumed for Task 4
#include "EsportsScheduler.hpp"  // Assumed for Task 1
#include "spectator_management.hpp" // Your new header for Task 3

using namespace CsvToolkit;
using namespace std;
// Assuming these namespaces are defined in their respective headers
// using namespace EsportsAnalytics; // Already in user's code
// using namespace EsportsScheduler; // Already in user's code
using namespace SpectatorSystem;   // Namespace for your Task 3 code

// Forward declarations for functions from other modules if not handled by their headers
// These are based on your existing main.cpp
// void manage_scheduling_and_progression(); // Should be in EsportsScheduler.hpp
// void record_N_Analysis();                // Should be in EsportsAnalytics.hpp


int main() {
    bool running = true;
    while (running) {
        clearTerminal(); // From CsvToolkit
        const char* menuOptions[] = {
            "Scheduling & Progression",
            "Registration & Player Management",
            "Live Stream & Spectator Management",
            "Performance & Analytics",
            "Exit"
        };
        // Using displayMenu for user interaction (from CsvToolkit)
        int choice = displayMenu("System Main Menu", menuOptions, 5);

        switch (choice) {
            case 1:
                // manage_scheduling_and_progression(); // Call Task 1 function
                EsportsScheduler::manage_scheduling_and_progression(); // Assuming it's in this namespace
                break;
            case 2:
                // Placeholder for Task 2 - Tournament Registration & Player Queueing
                // You would call your Task 2 function here
                displaySystemMessage("Task 2: Registration & Player Management - Not yet implemented.", 2);
                // Example: Task2::manage_registration();
                break;
            case 3:
                // --- This is where you integrate Task 3 ---
                { // Scope block for manager instance
                    SpectatorManager task3_manager; // Create an instance of your manager
                    task3_manager.runMainLoop();    // Call its main operational loop
                }
                break;
            case 4:
                // record_N_Analysis(); // Call Task 4 function
                EsportsAnalytics::record_N_Analysis(); // Assuming it's in this namespace
                break;
            case 5:
                running = false;
                break;
            default:
                displaySystemMessage("Invalid choice, please try again.", 2); // From CsvToolkit
                break;
        }
    }
    clearTerminal(); // From CsvToolkit
    std::cout << "\nExiting ESPORTS CHAMPIONSHIP MANAGEMENT SYSTEM, See you...\n" << std::endl;

    return 0;
}