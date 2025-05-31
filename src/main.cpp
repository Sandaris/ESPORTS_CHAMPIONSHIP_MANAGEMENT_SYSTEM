#include <iostream>
#include "File_exe.hpp"         // Your CsvToolkit library
#include "EsportsAnalytics.hpp" // For Task 4 (Performance & Analytics)
#include "EsportsScheduler.hpp" // For Task 1 (Scheduling & Progression)
// #include "spectator_management.hpp" // We will include this later when Task 3 is called

// Using namespaces from the provided common files and standard library
using namespace CsvToolkit;
using namespace std;
// Assuming these are the namespaces for your teammates' modules
using namespace EsportsAnalytics;
using namespace EsportsScheduler;
// We will add 'using namespace SpectatorSystem;' later or call with SpectatorSystem::

// Forward declare or include actual function declarations from teammate's .hpp files for these
// For now, as placeholders if their full .hpp are not integrated here:
// void manage_scheduling_and_progression(); // This would come from EsportsScheduler.hpp
// void record_N_Analysis();                // This would come from EsportsAnalytics.hpp
// void manage_registration_and_players(); // Placeholder for Task 2

int main() {
    bool running = true;
    while (running) {
        clearTerminal(); // From CsvToolkit
        const char* menuOptions[] = {
            "Scheduling & Progression",         // Task 1
            "Registration & Player Management", // Task 2
            "Live Stream & Spectator Management",// Task 3
            "Performance & Analytics",          // Task 4
            "Exit"
        };
        int choice = displayMenu("System Main Menu", menuOptions, 5); // From CsvToolkit

        switch (choice) {
            case 1:
                // Call for Task 1
                // manage_scheduling_and_progression(); // Example actual call
                displaySystemMessage("Task 1: Scheduling & Progression - Module called.", 2);
                break;
            case 2:
                // Call for Task 2
                // manage_registration_and_players(); // Example actual call
                displaySystemMessage("Task 2: Registration & Player Management - Module called.", 2);
                break;
            case 3:
                // Placeholder for your Task 3.
                // Later, you will create a SpectatorSystem::SpectatorManager object
                // and call its main loop function here.
                displaySystemMessage("Task 3: Live Stream & Spectator Management - Module called.", 2);
                // Example of what will go here eventually:
                // {
                // SpectatorSystem::SpectatorManager task3Manager;
                // task3Manager.runMainLoop();
                // }
                break;
            case 4:
                // Call for Task 4
                // record_N_Analysis(); // Example actual call
                displaySystemMessage("Task 4: Performance & Analytics - Module called.", 2);
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