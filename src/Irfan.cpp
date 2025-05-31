#include "File_exe.hpp"

using namespace std;
using namespace CsvToolkit;
const std::string Teams_CSV_FILE = "teams.csv";
const std::string Players_CSV_FILE = "player.csv";
void menu();

char** TeamRegistration() {
    using namespace CsvToolkit;
    clearTerminal();
    displaySystemMessage("Team Registration", 3);
    clearTerminal();
    
    dataContainer2D TeamList = getData(Teams_CSV_FILE);
    dataContainer2D EarlyBirdTeamList = Search1FieldValue(Teams_CSV_FILE, "TeamType", "EarlyBird"); 

    char** newTeam = new char*[11];

    // Auto-generate TeamID
    int maxTeamNum = 0;
    for (int i = 0; i < TeamList.y; ++i) {
        std::string tid(TeamList.data[i][0]);
        if (tid.length() > 1 && tid[0] == 'T') {
            int num = std::stoi(tid.substr(1));
            if (num > maxTeamNum) maxTeamNum = num;
        }
    }
    std::string newTeamID = "T0" + std::to_string(maxTeamNum + 1);
    newTeam[0] = duplicateString(newTeamID.c_str());
    std::cout << "Team ID              : " << newTeam[0] << "\n";

    // Basic input
    newTeam[1] = getString("Enter Team Name      : ");
    newTeam[2] = getString("Enter University     : ");
    newTeam[3] = getString("Enter Country        : ");

    if (EarlyBirdTeamList.y < 12) {
        newTeam[4] = duplicateString("EarlyBird");
        std::cout << "Team Type            : EarlyBird\n";
    } else {
        newTeam[4] = getString("Enter Team Type      : ");
    }

    // Date & Time
    time_t now = time(0);
    tm* local = localtime(&now);
    char dateStr[11], timeStr[9];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", local);
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", local);
    newTeam[5] = duplicateString(dateStr);
    cout << "Registration Date    : " << dateStr << "\n";
    newTeam[6] = duplicateString(timeStr);
    cout << "Registration Time    : " << timeStr << "\n";

    newTeam[7] = duplicateString("0");
    cout << "Ranking Point            : 0\n";
    newTeam[8] = duplicateString(EarlyBirdTeamList.y < 12 ? "Registered" : "WaitingList");
    cout << "Team Status          : " << newTeam[8] << "\n";
    newTeam[9] = duplicateString("");
    cout << "Checked In Time      : " << newTeam[9] << "\n";
    newTeam[10] = duplicateString("Pending");
    cout << "Check in Status      : " << newTeam[10] << "\n";

    return newTeam;
}

char** PlayerRegistration(const char* teamID, const char* university, const char* country, const char* teamStatus) {
    using namespace CsvToolkit;

    static int maxID = -1;
if (maxID == -1) {
        dataContainer2D playerList = getData(Players_CSV_FILE);
    for (int i = 0; i < playerList.y; ++i) {
            std::string pid(playerList.data[i][0]);  // e.g., "P098"
        if (pid.length() > 1 && pid[0] == 'P') {
            int num = std::stoi(pid.substr(1));
            if (num > maxID) maxID = num;
        }
    }
    deleteDataContainer2D(playerList);
}

    maxID++;
    std::ostringstream oss;
    oss << "P" << std::setw(3) << std::setfill('0') << maxID;  // Always 3 digits
    std::string newPID = oss.str();

char** newPlayer = new char*[12];
newPlayer[0] = duplicateString(newPID.c_str());
    std::cout << "Player ID           : " << newPlayer[0] << "\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    newPlayer[1] = getString("Enter Player Name   : ");
    
    newPlayer[2] = duplicateString(teamID);
    cout << "Team ID             : " << teamID << "\n";
    
    char* role;
    do {
        role = getString("Enter Role (Main/Sub): ");
    } while (strcmp(role, "Main") != 0 && strcmp(role, "Sub") != 0);
    newPlayer[3] = duplicateString(role);

    newPlayer[4] = duplicateString(university);
    cout << "University          : " << university << "\n";
    newPlayer[5] = duplicateString(country);
    cout << "Country             : " << country << "\n";
    newPlayer[6] = getString("Enter In-Game Name  : ");
    newPlayer[7] = getString("Enter Email         : ");
    
    int rp = getInt("Enter Ranking Point : ");
    std::string rpStr = std::to_string(rp);
    newPlayer[8] = duplicateString(rpStr.c_str());

    newPlayer[9] = duplicateString("Pending");
    cout << "Player Status : " << newPlayer[9] << "\n";
    newPlayer[10] = duplicateString(strcmp(teamStatus, "Registered") == 0 ? "Eligible" : "Standby");
    cout << "Eligibility   : " << newPlayer[10] << "\n";
    newPlayer[11] = duplicateString("");
    cout << "Checked In Time : " << newPlayer[11] << "\n";

    return newPlayer;
}

void WholeRegistration() {

    using namespace CsvToolkit;
    char** newTeam = TeamRegistration();

    clearTerminal();

    int i=0;
    char confirm = 'n';
    char PlayerConfirm = 'n';

    std::cout << "\n--- Team Registration Summary ---\n";
    std::cout << "Team ID              : " << newTeam[0] << "\n";
    std::cout << "Team Name            : " << newTeam[1] << "\n";
    std::cout << "University           : " << newTeam[2] << "\n";
    std::cout << "Country              : " << newTeam[3] << "\n";
    std::cout << "Team Type            : " << newTeam[4] << "\n";
    std::cout << "Registration Date    : " << newTeam[5] << "\n";
    std::cout << "Registration Time    : " << newTeam[6] << "\n";
    std::cout << "Ranking Point        : " << newTeam[7] << "\n";
    std::cout << "Team Status          : " << newTeam[8] << "\n";
    std::cout << "Checked In Time      : " << newTeam[9] << "\n";
    std::cout << "Check in Status      : " << newTeam[10] << "\n";
    std::cout << "---------------------------------\n";
    std::cout << "Are you sure you want to register this team? (y/n) : ";
    std::cin >> confirm;
        

    if(confirm == 'y' || confirm == 'Y') {

        while(i<7){
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear leftover input
            const char** player = const_cast<const char**>(PlayerRegistration(newTeam[0],newTeam[2],newTeam[3],newTeam[8]));
            CsvToolkit::clearTerminal();
            std::cout << "\n--- Player Registration Summary ---\n";
            std::cout << "Player ID         : " << player[0] << "\n";
            std::cout << "Player Name       : " << player[1] << "\n";
            std::cout << "Team ID           : " << player[2] << "\n";
            std::cout << "Role              : " << player[3] << "\n";
            std::cout << "University        : " << player[4] << "\n";
            std::cout << "Country           : " << player[5] << "\n";
            std::cout << "In-Game Name      : " << player[6] << "\n";
            std::cout << "Email             : " << player[7] << "\n";
            std::cout << "Ranking Point     : " << player[8] << "\n";
            std::cout << "Player Status     : " << player[9] << "\n";
            std::cout << "Eligibility Status: " << player[10] << "\n";
            std::cout << "Checked In Time   : " << player[11] << "\n";
            std::cout << "---------------------------------\n";

            std::cout << "Are you sure you want to register this player? (y/n) : ";
            std::cin >> PlayerConfirm;

            if (PlayerConfirm == 'y' || PlayerConfirm == 'Y') {
                CsvToolkit::writeNewDataRow(Players_CSV_FILE, 12, player);
                i++;

                if (i >= 5 && i < 7) {
                    std::cout << "Do you want to register another player? (y/n): ";
                    std::cin >> PlayerConfirm;
                    if (PlayerConfirm == 'n' || PlayerConfirm == 'N') break;
                } else if (i == 7) {
                    std::cout << "Maximum number of players registered.\n";
                    break;
                }
            } else {
                std::cout << "Player registration cancelled.\n";
            }
        }
            CsvToolkit::writeNewDataRow(Teams_CSV_FILE, 11, const_cast<const char**>(newTeam));
            
            displaySystemMessage("Team successfully registered.", 2);
            clearTerminal();
            menu();
    }
    else{
        CsvToolkit::clearTerminal();
        CsvToolkit::displaySystemMessage("Registration canceled.", 2);
        clearTerminal();
        menu();
    }
}

void ViewPriorityList(){

    const dataContainer2D WildCardList = Search1FieldValue(Teams_CSV_FILE, "TeamType"," Wildcard");
    dataContainer2D SortedWCList = sortByTwoFields(WildCardList, "RegisterDate","RegisterTime", true);

    dataContainer2D StandardList = Search1FieldValue(Teams_CSV_FILE, "TeamType", "Standard");
    dataContainer2D SortedSTList = sortByTwoFields(StandardList, "RegisterDate","RegisterTime", true);

    dataContainer2D EarlyBirdList = Search1FieldValue(Teams_CSV_FILE, "TeamType", "EarlyBird");
    dataContainer2D SortedEBList = sortByTwoFields(EarlyBirdList, "RegisterDate","RegisterTime", true);

    displayTabulatedData(SortedWCList);
    displayTabulatedData(SortedEBList);
    displayTabulatedData(SortedSTList);

    cout << "---------------------------------\n";
    cout << "Press any key to continue..."; cin.ignore();
    menu();

}

void PromoteWaitingTeams() {
    using namespace CsvToolkit;

    const int TEAM_LIMIT = 12;

    // Step 1: Get all Registered EarlyBird teams (not Withdrawn)
    dataContainer2D registeredEB = Search2FieldValue(Teams_CSV_FILE, "TeamType", "EarlyBird", "TeamStatus", "Registered");
    int countEB = registeredEB.y;

    // Step 2: Get WaitingList Standard teams
    dataContainer2D waitingStandard = Search2FieldValue(Teams_CSV_FILE, "TeamType", "Standard", "TeamStatus", "WaitingList");
    dataContainer2D sortedStandard = sortByTwoFields(waitingStandard, "RegisterDate", "RegisterTime", true);
    deleteDataContainer2D(waitingStandard);

    int remainingSpots = TEAM_LIMIT - countEB;
    int promoted = 0;

    for (int i = 0; i < sortedStandard.y && promoted < remainingSpots; ++i) {
        const char* teamID = sortedStandard.data[i][0];

        // Promote Standard team
        dataContainer1D teamRow = queryKey(Teams_CSV_FILE, teamID);
        if (!teamRow.error) {
            teamRow.data[8] = duplicateString("Registered");  // TeamStatus
            updateDataByUniqueKey(Teams_CSV_FILE, teamID, 0, teamRow.fields_count, const_cast<const char**>(teamRow.data));
        }
        deleteDataContainer1D(teamRow);

        // Promote players of the team
        dataContainer2D players = Search1FieldValue(Players_CSV_FILE, "TeamID", teamID);
        for (int j = 0; j < players.y; ++j) {
            players.data[j][10] = duplicateString("Eligible"); // EligibilityStatus
            updateDataByUniqueKey(Players_CSV_FILE, players.data[j][0], 0, players.x, const_cast<const char**>(players.data[j]));
        }
        deleteDataContainer2D(players);

        promoted++;
    }

    deleteDataContainer2D(sortedStandard);
    deleteDataContainer2D(registeredEB);

    if (promoted > 0) {
        std::cout << promoted << " Standard team(s) promoted to Registered to fill quota.\n";
    }
}

void Withdrawal() {
    using namespace CsvToolkit;

    clearTerminal();
    displaySystemMessage("Withdraw Team", 3);

    dataContainer2D teamList = getData(Teams_CSV_FILE);
    if (teamList.error || teamList.y == 0) {
        std::cerr << "No team data available.\n";
        return;
    }

    displayTabulatedData(teamList);
    deleteDataContainer2D(teamList);  // Clean up early

    char* teamID = getString("Enter Team ID to withdraw: ");
    std::cout << "---------------------------------\n";
    dataContainer2D team = Search1FieldValue(Teams_CSV_FILE, "TeamID", teamID);
    displayTabulatedData(team);
    std::cout << "Are you sure you want to withdraw this team? (y/n): ";
    char confirm;
    std::cin >> confirm;

    if (tolower(confirm) == 'y') {
        // Step 1: Update the team’s status
        int teamUpdateResult = updateFieldByKey(Teams_CSV_FILE, teamID, "TeamStatus", "Withdraw");
        if (teamUpdateResult != 0) {
            std::cerr << "Failed to update team status.\n";
            return;
        }

        // Step 2: Update all players in the team to Withdrawn

        if (team.error || team.y == 0) {
            std::cerr << "No players found for this team.\n";
        } else {
            int eligibilityCol = -1;
            for (int i = 0; i < team.x; ++i) {
                if (strcmp(team.fields[i], "EligibilityStatus") == 0) {
                    eligibilityCol = i;
                    break;
                }
            }

            if (eligibilityCol == -1) {
                std::cerr << "EligibilityStatus column not found in player.csv.\n";
            } else {
                for (int i = 0; i < team.y; ++i) {
                    const char* playerID = team.data[i][0];  // assuming PlayerID is column 0
                    updateFieldByKey(Players_CSV_FILE, playerID, "EligibilityStatus", "Withdrawn");
                }
            }

            deleteDataContainer2D(team);
        }
        PromoteWaitingTeams();
        displaySystemMessage("Team and players withdrawn successfully.", 2);
        menu();    
    } else {
        displaySystemMessage("Withdrawal cancelled.", 2);
        menu();
    
    }

    std::cout << "Press Enter to return to menu...";
    std::cin.ignore();
    std::cin.get();
    menu();
}

void TeamCheckIn() {
    using namespace CsvToolkit;
    clearTerminal();
    displaySystemMessage("Team Check-In", 3);

    // Step 1: Get all Registered teams that are not Withdrawn
    dataContainer2D eligibleTeams = Search1FieldValue(Teams_CSV_FILE, "TeamStatus", "Registered");

    if (eligibleTeams.y == 0) {
        std::cout << "No teams are available for check-in.\n";
        deleteDataContainer2D(eligibleTeams);
        std::cout << "Press Enter to return to menu..."; std::cin.ignore(); std::cin.get();
        return;
    }

    // Step 2: Display eligible teams
    displayTabulatedData(eligibleTeams);

    // Step 3: Prompt for team ID
    char* teamID = getString("Enter Team ID to check-in: ");

    // Step 4: Show all players from that team
    dataContainer2D teamPlayers = Search1FieldValue(Players_CSV_FILE, "TeamID", teamID);
    
    if (teamPlayers.y == 0) {
        std::cerr << "No players found for this team.\n";
        deleteDataContainer2D(eligibleTeams);
        deleteDataContainer2D(teamPlayers);
        delete[] teamID;
        std::cout << "Press Enter to return to menu..."; std::cin.ignore(); std::cin.get();
        return;
    }

    displayTabulatedData(teamPlayers);

    // Step 5: Prompt to check-in each player
    for (int i = 0; i < teamPlayers.y; ++i) {
        if (strcmp(teamPlayers.data[i][9], "Pending") == 0) {
            std::cout << "Player " << teamPlayers.data[i][1] << " (" << teamPlayers.data[i][0] << ") - Check in now? (1=Yes, 2=Absent, 0=Skip): ";
            int choice = getInt("");
            if (choice == 1) {
                // Format: YYYY-MM-DD HH:MM
                time_t now = time(0);
                tm* local = localtime(&now);
                char formattedTime[17];
                strftime(formattedTime, sizeof(formattedTime), "%Y-%m-%d %H:%M", local);
                teamPlayers.data[i][9] = duplicateString("CheckedIn");
                teamPlayers.data[i][11] = duplicateString(formattedTime);
            } else if (choice == 2) {
                teamPlayers.data[i][9] = duplicateString("Absent");
                teamPlayers.data[i][11] = duplicateString("NULL");
            } else {
                std::cout << "Skipped.\n";
            }

            updateDataByUniqueKey("player.csv", teamPlayers.data[i][0], 0, teamPlayers.x, const_cast<const char**>(teamPlayers.data[i]));
        }
    }

    // Step 6: Re-query players to verify all are CheckedIn or Absent
    dataContainer2D updatedPlayers = Search1FieldValue(Players_CSV_FILE, "TeamID", teamID);
    bool allChecked = true;
    for (int i = 0; i < updatedPlayers.y; ++i) {
        if (strcmp(updatedPlayers.data[i][9], "Pending") == 0) {
            allChecked = false;
            break;
        }
    }

    // Step 7: If all players are checked in or absent, mark team as CheckedIn
    if (allChecked) {
        dataContainer1D teamRow = queryKey(Teams_CSV_FILE, teamID);
        if (!teamRow.error) {
            teamRow.data[10] = duplicateString("CheckedIn");
            // Set CheckedInTime
            time_t now = time(0);
            tm* local = localtime(&now);
            char checkInTime[9];
            strftime(checkInTime, sizeof(checkInTime), "%H:%M:%S", local);
            teamRow.data[9] = duplicateString(checkInTime);

            updateDataByUniqueKey(Teams_CSV_FILE, teamID, 0, teamRow.fields_count, const_cast<const char**>(teamRow.data));
        }
        deleteDataContainer1D(teamRow);
        std::cout << "Team successfully checked in.\n";
    } else {
        std::cout << "Team cannot be fully checked-in. Some players are still pending.\n";
    }

    // Cleanup
    delete[] teamID;
    deleteDataContainer2D(eligibleTeams);
    deleteDataContainer2D(teamPlayers);
    deleteDataContainer2D(updatedPlayers);

    std::cout << "Press Enter to return to menu..."; 
    std::cin.ignore(); 
    std::cin.get();
    menu();
}

void ViewEligibleTeam() {
    using namespace CsvToolkit;
    clearTerminal();
    displaySystemMessage("Final Eligible Teams", 3);

    dataContainer2D allTeams = getData(Teams_CSV_FILE);
    if (allTeams.error || allTeams.y == 0) {
        std::cerr << "No team data found.\n";
        std::cout << "Press Enter to return..."; std::cin.ignore(); std::cin.get();
        return;
    }

    const char* types[] = {"EarlyBird", "WildCard", "Standard"};
    bool displayed = false;

    for (int t = 0; t < 3; ++t) {
        const char* type = types[t];

        // Filter eligible teams based on type-specific logic
        dataContainer2D eligible;
        eligible.x = allTeams.x;
        eligible.y = 0;
        eligible.fields = new char*[eligible.x];
        for (int i = 0; i < eligible.x; ++i)
            eligible.fields[i] = duplicateString(allTeams.fields[i]);
        eligible.data = new char**[allTeams.y];

        for (int i = 0; i < allTeams.y; ++i) {
            if (strcmp(allTeams.data[i][4], type) == 0 && 
                strcmp(allTeams.data[i][10], "CheckedIn") == 0) {  // CheckedIn required for all

                if (strcmp(type, "WildCard") == 0) {
                    // WildCard: Only CheckInStatus matters
                    eligible.data[eligible.y] = new char*[eligible.x];
                    for (int j = 0; j < eligible.x; ++j)
                        eligible.data[eligible.y][j] = duplicateString(allTeams.data[i][j]);
                    eligible.y++;
                } else {
                    // EarlyBird / Standard: Require Registered + CheckedIn
                    if (strcmp(allTeams.data[i][8], "Registered") == 0) {
                        eligible.data[eligible.y] = new char*[eligible.x];
                        for (int j = 0; j < eligible.x; ++j)
                            eligible.data[eligible.y][j] = duplicateString(allTeams.data[i][j]);
                        eligible.y++;
                    }
                }
            }
        }

        if (eligible.y > 0) {
            dataContainer2D sorted = sortByTwoFields(eligible, "RegisterDate", "RegisterTime", true);
            std::cout << "\n--- " << type << " Teams ---\n";
            displayTabulatedData(sorted);
            deleteDataContainer2D(sorted);
            displayed = true;
        } else {
            deleteDataContainer2D(eligible);
        }
    }

    deleteDataContainer2D(allTeams);

    if (!displayed)
        std::cout << "\nNo teams are currently eligible to play.\n";

    std::cout << "---------------------------------------\n";
    std::cout << "Press Enter to return to menu...";
    std::cin.ignore(); 
    std::cin.get();
    menu();
}

void menu(){
    using namespace CsvToolkit;
    CsvToolkit::clearTerminal();
    std::cout << "\n--- Menu ---\n";
    std::cout << "1. Registration\n";
    std::cout << "2. View Priority List\n";
    std::cout << "3. Withdrawal\n";
    std::cout << "4. Team CheckIn\n";
    std::cout << "5. View Eligible Team\n";
    std::cout << "6. Exit\n";
    std::cout << "----------------\n";
    int choice;
    choice = getInt("Enter your choice: ");
    switch(choice){
        case 1: WholeRegistration(); break;
        case 2: ViewPriorityList(); break;
        case 3: Withdrawal(); break;
        case 4: TeamCheckIn(); break;
        case 5: ViewEligibleTeam(); break;
        case 6: exit(0); break;
    }
}

int main(){
    menu();
    return 0;
}

