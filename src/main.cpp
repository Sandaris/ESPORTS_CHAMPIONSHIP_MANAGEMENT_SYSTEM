#include <iostream>
#include "File_exe.hpp"         // Your CsvToolkit library
#include "EsportsAnalytics.hpp" // For Task 4 (Performance & Analytics)
#include "EsportsScheduler.hpp" // For Task 1 (Scheduling & Progression)
#include "spectator_management.hpp" // <-- ADD THIS INCLUDE for your Task 3 declarations

// Using namespaces
using namespace CsvToolkit;
using namespace std;
using namespace EsportsAnalytics;
using namespace EsportsScheduler;
using namespace SpectatorSystem;   

// Forward declarations or ensure they are in the headers for other tasks
// void manage_scheduling_and_progression(); // From EsportsScheduler.hpp
// void record_N_Analysis();                // From EsportsAnalytics.hpp
// void manage_registration_and_players(); // Placeholder for Task 2


int main() {
    bool running = true;
    while (running) {
        clearTerminal(); 
        const char* menuOptions[] = {
            "Scheduling & Progression",         
            "Registration & Player Management", 
            "Live Stream & Spectator Management",
            "Performance & Analytics",          
            "Exit"
        };
        int choice = displayMenu("System Main Menu", menuOptions, 5); 

        switch (choice) {
            case 1:
                // manage_scheduling_and_progression(); // Task 1 call
                displaySystemMessage("Task 1: Scheduling & Progression - Module called.", 2);
                break;
            case 2:
                // manage_registration_and_players(); // Task 2 call
                displaySystemMessage("Task 2: Registration & Player Management - Module called.", 2);
                break;
            case 3:
                // --- This is where you call your Task 3 code ---
                { // Scope block for manager instance
                    SpectatorManager task3Manager; // Create an object of your class
                    task3Manager.runMainLoop();    // Call its main function
                }
                // --- End of Task 3 call ---
                break;
            case 4:
                // record_N_Analysis(); // Task 4 call
                displaySystemMessage("Task 4: Performance & Analytics - Module called.", 2);
                break;
            case 5:
                running = false;
                break;
            default:
                displaySystemMessage("Invalid choice, please try again.", 2); 
                break;
        }
    }
    clearTerminal(); 
    std::cout << "\nExiting ESPORTS CHAMPIONSHIP MANAGEMENT SYSTEM, See you...\n" << std::endl;

    return 0;
}