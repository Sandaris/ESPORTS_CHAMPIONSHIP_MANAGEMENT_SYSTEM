#include <iostream>
#include "File_exe.hpp"
#include "EsportsAnalytics1.cpp"
#include "EsportsScheduler.hpp" 
#include "EsportsPlayerManagement.hpp"

using namespace CsvToolkit;
using namespace std;
using namespace EsportsAnalytics;
using namespace EsportsScheduler;

#include "File_exe.hpp"          // Your CsvToolkit library/ The Task 4 module defined above

int main() {

    srand(time(0));
   
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
            // Using displayMenu for user interaction
            int choice = displayMenu("System Main Menu", menuOptions, 5);

            switch (choice) 
            {   
                //manage_scheduling_and_progression
                case 1: manageMatchSchedulingAndProgression(); break;
                case 2: RegistrationPlayerManagement(); break;
                case 3: printf("Live Stream & Spectator Management"); break;
                case 4: record_N_Analysis(); break;
                case 5: running = false; break;
                default: displaySystemMessage("Invalid choice, please try again.", 2); break;
            }
        }
        clearTerminal();
        std::cout << "\nExiting ESPORTS CHAMPIONSHIP MANAGEMENT SYSTEM, See you...\n" << std::endl;

    return 0;


}