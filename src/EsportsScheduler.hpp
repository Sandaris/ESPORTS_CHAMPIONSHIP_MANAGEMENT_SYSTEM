#ifndef ESPORTS_SCHEDULER_HPP
#define ESPORTS_SCHEDULER_HPP

#include <iostream>
#include <string>
#include <algorithm> // For std::swap in basic sort, or use manual swap
#include "File_exe.hpp" // Your CsvToolkit library

using namespace CsvToolkit;

namespace EsportsScheduler {

    // --- Constants for CSV Files ---
    const std::string PLAYERS_CSV_FILE = "player.csv";
    const std::string STAGES_CSV_FILE = "tournament_stages.csv";
    const std::string MATCHES_CSV_FILE = "matches.csv";

    // --- Data Structures for Task 1 ---

    struct PlayerProfile {
        char* player_id;
        char* player_name;
        int ranking_points;
        // Add other relevant fields if needed, e.g., university_name

        PlayerProfile() : player_id(nullptr), player_name(nullptr), ranking_points(0) {}

        // Constructor for easy initialization from CsvToolkit data
        PlayerProfile(const char* id, const char* name, int rank) {
            player_id = CsvToolkit::duplicateString(id);
            player_name = CsvToolkit::duplicateString(name);
            ranking_points = rank;
        }

        // Copy constructor
        PlayerProfile(const PlayerProfile& other) {
            player_id = CsvToolkit::duplicateString(other.player_id);
            player_name = CsvToolkit::duplicateString(other.player_name);
            ranking_points = other.ranking_points;
        }

        // Assignment operator
        PlayerProfile& operator=(const PlayerProfile& other) {
            if (this == &other) return *this;
            delete[] player_id;
            delete[] player_name;
            player_id = CsvToolkit::duplicateString(other.player_id);
            player_name = CsvToolkit::duplicateString(other.player_name);
            ranking_points = other.ranking_points;
            return *this;
        }

        ~PlayerProfile() {
            delete[] player_id;
            delete[] player_name;
        }
    };

    struct TournamentStageInfo {
        char* stage_id;
        char* tournament_id;
        char* stage_name;
        int stage_order;
        char* stage_format; // e.g., "Single Elimination", "Round Robin"
        int number_of_expected_matches;

        TournamentStageInfo() : stage_id(nullptr), tournament_id(nullptr), stage_name(nullptr),
                                stage_order(0), stage_format(nullptr), number_of_expected_matches(0) {}
        
        // Constructor, copy, assignment, destructor similar to PlayerProfile needed for char*
        ~TournamentStageInfo() {
            delete[] stage_id; delete[] tournament_id; delete[] stage_name; delete[] stage_format;
        }
         // Simplified constructor for example
        TournamentStageInfo(const char* sid, const char* tid, const char* sname, int order, const char* format, int expected) {
            stage_id = CsvToolkit::duplicateString(sid);
            tournament_id = CsvToolkit::duplicateString(tid);
            stage_name = CsvToolkit::duplicateString(sname);
            stage_order = order;
            stage_format = CsvToolkit::duplicateString(format);
            number_of_expected_matches = expected;
        }
    };

    struct ScheduledMatch {
        char* match_id;
        char* tournament_id;
        char* stage_id;
        char* match_round_number;
        char* scheduled_datetime;
        // actual_start_datetime, actual_end_datetime (can be added from matches.csv)
        char* player1_id;
        char* player2_id;
        char* winner_player_id; // Null if not played or draw
        int player1_score;
        int player2_score;
        char* match_status; // e.g., "Scheduled", "Ongoing", "Completed"
        char* bracket_position_info;


        ScheduledMatch() : match_id(nullptr), tournament_id(nullptr), stage_id(nullptr),
                           match_round_number(nullptr), scheduled_datetime(nullptr),
                           player1_id(nullptr), player2_id(nullptr), winner_player_id(nullptr),
                           player1_score(0), player2_score(0), match_status(nullptr),
                           bracket_position_info(nullptr) {}

        // Constructor, copy, assignment, destructor similar to PlayerProfile needed for char*
         ~ScheduledMatch() {
            delete[] match_id; delete[] tournament_id; delete[] stage_id; delete[] match_round_number;
            delete[] scheduled_datetime; delete[] player1_id; delete[] player2_id;
            delete[] winner_player_id; delete[] match_status; delete[] bracket_position_info;
        }
        // Simplified constructor for example
        ScheduledMatch(const char* mid, const char* tid, const char* sid, const char* p1id, const char* p2id, const char* sdt, const char* status = "Scheduled", const char* round = "1", const char* bracket_pos = "") {
            match_id = CsvToolkit::duplicateString(mid);
            tournament_id = CsvToolkit::duplicateString(tid);
            stage_id = CsvToolkit::duplicateString(sid);
            player1_id = CsvToolkit::duplicateString(p1id);
            player2_id = CsvToolkit::duplicateString(p2id);
            scheduled_datetime = CsvToolkit::duplicateString(sdt);
            match_status = CsvToolkit::duplicateString(status);
            match_round_number = CsvToolkit::duplicateString(round);
            bracket_position_info = CsvToolkit::duplicateString(bracket_pos);
            winner_player_id = nullptr; // Init to null
            player1_score = 0; player2_score = 0;
        }


        void getAsCsvRow(const char** rowBuffer, int bufferSize,
                         std::string& p1ScoreStr_temp, std::string& p2ScoreStr_temp) const {
            // matches.csv headers: match_id,tournament_id,stage_id,match_round_number,scheduled_datetime,actual_start_datetime,actual_end_datetime,player1_id,player2_id,winner_player_id,player1_score,player2_score,match_status,bracket_position_info
            // This needs to match the target structure for matches.csv
            if (bufferSize < 14) return;
            p1ScoreStr_temp = std::to_string(player1_score);
            p2ScoreStr_temp = std::to_string(player2_score);

            rowBuffer[0]  = match_id ? match_id : "";
            rowBuffer[1]  = tournament_id ? tournament_id : ""; // Needs to be set
            rowBuffer[2]  = stage_id ? stage_id : "";
            rowBuffer[3]  = match_round_number ? match_round_number : "";
            rowBuffer[4]  = scheduled_datetime ? scheduled_datetime : "";
            rowBuffer[5]  = ""; // actual_start_datetime - empty for new
            rowBuffer[6]  = ""; // actual_end_datetime - empty for new
            rowBuffer[7]  = player1_id ? player1_id : "";
            rowBuffer[8]  = player2_id ? player2_id : "";
            rowBuffer[9]  = winner_player_id ? winner_player_id : "";
            rowBuffer[10] = p1ScoreStr_temp.c_str();
            rowBuffer[11] = p2ScoreStr_temp.c_str();
            rowBuffer[12] = match_status ? match_status : "";
            rowBuffer[13] = bracket_position_info ? bracket_position_info : "";
        }
    };

    // --- Helper Functions ---

    // Basic Bubble Sort for PlayerProfile array by ranking_points (descending)
    void sortPlayersByRank(PlayerProfile* players, int count) {
        if (!players || count < 2) return;
        bool swapped;
        for (int i = 0; i < count - 1; i++) {
            swapped = false;
            for (int j = 0; j < count - i - 1; j++) {
                if (players[j].ranking_points < players[j + 1].ranking_points) {
                    // Swap PlayerProfile objects
                    PlayerProfile temp = players[j]; // Uses copy constructor
                    players[j] = players[j + 1];   // Uses assignment operator
                    players[j + 1] = temp;         // Uses assignment operator
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    // Load players from CSV into a dynamically allocated array of PlayerProfile
    PlayerProfile* loadPlayers(int& playerCount) {
        playerCount = 0;
        CsvToolkit::dataContainer2D playerData = CsvToolkit::getData(PLAYERS_CSV_FILE);
        if (playerData.error || playerData.y == 0) {
            std::cerr << "Error loading players or no players found in " << PLAYERS_CSV_FILE << std::endl;
            CsvToolkit::deleteDataContainer2D(playerData);
            return nullptr;
        }

        // Find column indices
        int idCol = -1, nameCol = -1, rankCol = -1;
        for (int i = 0; i < playerData.x; ++i) {
            if (strcmp(playerData.fields[i], "player_id") == 0) idCol = i;
            else if (strcmp(playerData.fields[i], "player_name") == 0) nameCol = i;
            else if (strcmp(playerData.fields[i], "current_ranking_points") == 0) rankCol = i;
        }

        if (idCol == -1 || nameCol == -1 || rankCol == -1) {
            std::cerr << "Error: Required player columns (player_id, player_name, current_ranking_points) not found in " << PLAYERS_CSV_FILE << std::endl;
            CsvToolkit::deleteDataContainer2D(playerData);
            return nullptr;
        }

        PlayerProfile* profiles = new PlayerProfile[playerData.y];
        playerCount = playerData.y;
        for (int i = 0; i < playerData.y; ++i) {
            profiles[i] = PlayerProfile(
                playerData.data[i][idCol],
                playerData.data[i][nameCol],
                atoi(playerData.data[i][rankCol] ? playerData.data[i][rankCol] : "0")
            );
        }
        CsvToolkit::deleteDataContainer2D(playerData);
        return profiles;
    }
    
    // Load tournament stages
    TournamentStageInfo* loadStages(int& stageCount) {
        stageCount = 0;
        CsvToolkit::dataContainer2D stageData = CsvToolkit::getData(STAGES_CSV_FILE);
        if (stageData.error || stageData.y == 0) {
            std::cerr << "Error loading stages or no stages found in " << STAGES_CSV_FILE << std::endl;
            CsvToolkit::deleteDataContainer2D(stageData);
            return nullptr;
        }

        // stage_id,tournament_id,stage_name,stage_order,stage_format,number_of_expected_matches
        int idCol=-1, tidCol=-1, nameCol=-1, orderCol=-1, formatCol=-1, expMatchesCol=-1;
        for(int i=0; i<stageData.x; ++i) {
            if(strcmp(stageData.fields[i], "stage_id")==0) idCol=i;
            else if(strcmp(stageData.fields[i], "tournament_id")==0) tidCol=i;
            else if(strcmp(stageData.fields[i], "stage_name")==0) nameCol=i;
            else if(strcmp(stageData.fields[i], "stage_order")==0) orderCol=i;
            else if(strcmp(stageData.fields[i], "stage_format")==0) formatCol=i;
            else if(strcmp(stageData.fields[i], "number_of_expected_matches")==0) expMatchesCol=i;
        }
        if(idCol==-1||tidCol==-1||nameCol==-1||orderCol==-1||formatCol==-1||expMatchesCol==-1){
            std::cerr << "Error: Missing required columns in " << STAGES_CSV_FILE << std::endl;
            CsvToolkit::deleteDataContainer2D(stageData); return nullptr;
        }

        TournamentStageInfo* stages = new TournamentStageInfo[stageData.y];
        stageCount = stageData.y;
        for (int i = 0; i < stageData.y; ++i) {
            stages[i] = TournamentStageInfo(
                stageData.data[i][idCol],
                stageData.data[i][tidCol],
                stageData.data[i][nameCol],
                atoi(stageData.data[i][orderCol] ? stageData.data[i][orderCol] : "0"),
                stageData.data[i][formatCol],
                atoi(stageData.data[i][expMatchesCol] ? stageData.data[i][expMatchesCol] : "0")
            );
        }
        CsvToolkit::deleteDataContainer2D(stageData);
        return stages;
    }


    // --- Task 1 Core Functions ---
    void generateScheduleForStage(const TournamentStageInfo& stage, PlayerProfile* allPlayers, int totalPlayerCount) {
        CsvToolkit::clearTerminal();
        std::cout << "--- Generating Schedule for Stage: " << (stage.stage_name ? stage.stage_name : "N/A") << " ---" << std::endl;
        std::cout << "Format: " << (stage.stage_format ? stage.stage_format : "N/A") << std::endl;

        if (!allPlayers || totalPlayerCount < 2) {
            std::cout << "Not enough players to generate matches for this stage." << std::endl;
            CsvToolkit::getString("Press Enter to continue...");
            return;
        }

        // For this example, we'll focus on "Single Elimination" for simplicity
        // And assume all loaded players are participating in this first eligible stage.
        // A real system would filter players based on registration for THIS tournament/stage or prior progression.
        if (stage.stage_format && strcmp(stage.stage_format, "Single Elimination") == 0) {
            // Ensure even number of players for simple pairing, or handle byes (more complex)
            // For simplicity, let's assume totalPlayerCount is a power of 2 or at least even.
            // Or take the top N players where N is a power of 2.
            // For this example: use up to stage.number_of_expected_matches * 2 players.
            
            int playersForThisStage = stage.number_of_expected_matches * 2;
            if (playersForThisStage > totalPlayerCount) playersForThisStage = totalPlayerCount;
            if (playersForThisStage % 2 != 0 && playersForThisStage > 1) playersForThisStage--; // Make it even for simple pairing

            if (playersForThisStage < 2) {
                std::cout << "Not enough players (" << playersForThisStage << ") for single elimination pairings." << std::endl;
                CsvToolkit::getString("Press Enter to continue...");
                return;
            }

            std::cout << "Scheduling " << playersForThisStage << " players for this stage." << std::endl;

            // Sort eligible players by rank for seeding
            // For this demo, we'll sort the first 'playersForThisStage' players from the allPlayers list.
            // A copy might be better if allPlayers needs to retain original order elsewhere.
            PlayerProfile* stagePlayers = new PlayerProfile[playersForThisStage];
            for(int i=0; i<playersForThisStage; ++i) stagePlayers[i] = allPlayers[i]; // Assumes allPlayers is already somewhat ordered or we take top N

            sortPlayersByRank(stagePlayers, playersForThisStage); // Sort the subset for this stage

            std::cout << "\nGenerating matches..." << std::endl;
            int matchesToGenerate = playersForThisStage / 2;
            ScheduledMatch* newMatches = new ScheduledMatch[matchesToGenerate];
            
            // Ensure matches.csv header exists
            const char* matchHeaders[] = {"match_id","tournament_id","stage_id","match_round_number","scheduled_datetime","actual_start_datetime","actual_end_datetime","player1_id","player2_id","winner_player_id","player1_score","player2_score","match_status","bracket_position_info"};
            ensureCsvHeader(MATCHES_CSV_FILE, matchHeaders, 14); // Using ensureCsvHeader from Task 4 for convenience

            for (int i = 0; i < matchesToGenerate; ++i) {
                std::string matchIdStr = std::string(stage.stage_id ? stage.stage_id : "S") + "-M" + std::to_string(i + 1); // Simple Match ID
                std::string scheduledTime = "YYYY-MM-DD HH:MM"; // Placeholder
                std::string roundNumStr = "1"; // Assuming round 1 of this stage
                std::string bracketPosStr = "R1-Match" + std::to_string(i+1);


                // Simple seeding: Top half vs Bottom half (1vN, 2vN-1, etc.)
                // Player indices for pairing: stagePlayers[i] vs stagePlayers[playersForThisStage - 1 - i]
                newMatches[i] = ScheduledMatch(
                    matchIdStr.c_str(),
                    stage.tournament_id, // Use tournament ID from stage
                    stage.stage_id,
                    stagePlayers[i].player_id,
                    stagePlayers[playersForThisStage - 1 - i].player_id,
                    scheduledTime.c_str(),
                    "Scheduled",
                    roundNumStr.c_str(),
                    bracketPosStr.c_str()
                );

                // Write this new match to matches.csv
                const int NUM_MATCH_CSV_FIELDS = 14;
                const char* matchCsvRow[NUM_MATCH_CSV_FIELDS];
                std::string p1s_temp, p2s_temp;
                newMatches[i].getAsCsvRow(matchCsvRow, NUM_MATCH_CSV_FIELDS, p1s_temp, p2s_temp);
                CsvToolkit::writeNewDataRow(MATCHES_CSV_FILE, NUM_MATCH_CSV_FIELDS, matchCsvRow);
                
                std::cout << "Scheduled: " << matchIdStr << " - "
                          << (stagePlayers[i].player_name ? stagePlayers[i].player_name : "P1") << " vs "
                          << (stagePlayers[playersForThisStage - 1 - i].player_name ? stagePlayers[playersForThisStage - 1 - i].player_name : "P2") << std::endl;
            }
            std::cout << matchesToGenerate << " matches scheduled and saved to " << MATCHES_CSV_FILE << std::endl;

            delete[] newMatches; // ScheduledMatch destructors called
            delete[] stagePlayers;

        } else {
            std::cout << "Scheduling for format '" << (stage.stage_format ? stage.stage_format : "Unknown") << "' is not implemented in this example." << std::endl;
        }
        CsvToolkit::getString("Press Enter to continue...");
    }

    void viewCurrentSchedule() {
        CsvToolkit::clearTerminal();
        std::cout << "--- Current Match Schedule (" << MATCHES_CSV_FILE << ") ---" << std::endl;
        CsvToolkit::dataContainer2D schedule = CsvToolkit::getData(MATCHES_CSV_FILE);
        if (schedule.error || schedule.y == 0) {
            std::cout << "No schedule found or error reading file." << std::endl;
        } else {
            // Filter to show only "Scheduled" or "Ongoing" matches for brevity, or select by stage
            // For now, display all
            CsvToolkit::displayTabulatedData(schedule);
        }
        CsvToolkit::deleteDataContainer2D(schedule);
        CsvToolkit::getString("Press Enter to continue...");
    }

    void updateMatchResultAndProgress() {
        CsvToolkit::clearTerminal();
        std::cout << "--- Update Match Result & Player Progression ---" << std::endl;
        
        // 1. Display scheduled/ongoing matches
        CsvToolkit::dataContainer2D allMatches = CsvToolkit::getData(MATCHES_CSV_FILE);
        if (allMatches.error) {
            std::cout << "Error loading matches. Cannot update." << std::endl;
            CsvToolkit::deleteDataContainer2D(allMatches);
            CsvToolkit::getString("Press Enter to continue...");
            return;
        }

        // Filter or let user select a match_id to update
        // For simplicity, let's ask for match_id
        char* matchIdToUpdate_cstr = CsvToolkit::getString("Enter Match ID to update: ");
        if (!matchIdToUpdate_cstr || strlen(matchIdToUpdate_cstr) == 0) {
            delete[] matchIdToUpdate_cstr;
            CsvToolkit::deleteDataContainer2D(allMatches);
            std::cout << "No Match ID entered." << std::endl;
            CsvToolkit::getString("Press Enter to continue...");
            return;
        }
        std::string matchIdToUpdate(matchIdToUpdate_cstr);
        delete[] matchIdToUpdate_cstr;

        // Find the match (assuming match_id is in column 0)
        int matchRow = -1;
        int matchIdCol = -1, p1IdCol = -1, p2IdCol = -1, winnerCol = -1, statusCol = -1, p1ScoreCol = -1, p2ScoreCol = -1;

        for(int i=0; i<allMatches.x; ++i) {
            if(strcmp(allMatches.fields[i], "match_id")==0) matchIdCol=i;
            else if(strcmp(allMatches.fields[i], "player1_id")==0) p1IdCol=i;
            else if(strcmp(allMatches.fields[i], "player2_id")==0) p2IdCol=i;
            else if(strcmp(allMatches.fields[i], "winner_player_id")==0) winnerCol=i;
            else if(strcmp(allMatches.fields[i], "match_status")==0) statusCol=i;
            else if(strcmp(allMatches.fields[i], "player1_score")==0) p1ScoreCol=i;
            else if(strcmp(allMatches.fields[i], "player2_score")==0) p2ScoreCol=i;
        }

        if(matchIdCol == -1 || p1IdCol == -1 || p2IdCol == -1 || winnerCol == -1 || statusCol == -1 || p1ScoreCol == -1 || p2ScoreCol == -1){
            std::cout << "Error: Essential columns not found in matches.csv." << std::endl;
            CsvToolkit::deleteDataContainer2D(allMatches);
            CsvToolkit::getString("Press Enter to continue...");
            return;
        }


        for (int i = 0; i < allMatches.y; ++i) {
            if (allMatches.data[i][matchIdCol] && strcmp(allMatches.data[i][matchIdCol], matchIdToUpdate.c_str()) == 0) {
                matchRow = i;
                break;
            }
        }

        if (matchRow == -1) {
            std::cout << "Match ID '" << matchIdToUpdate << "' not found." << std::endl;
        } else {
            std::cout << "Updating Match: " << matchIdToUpdate << std::endl;
            std::cout << "Player 1: " << allMatches.data[matchRow][p1IdCol] << " vs Player 2: " << allMatches.data[matchRow][p2IdCol] << std::endl;
            
            char* winnerId_cstr = CsvToolkit::getString("Enter Winner Player ID (or DRAW): ");
            int p1Score_val = CsvToolkit::getInt("Enter Player 1 Score: ");
            int p2Score_val = CsvToolkit::getInt("Enter Player 2 Score: ");
            std::string p1Score_str = std::to_string(p1Score_val);
            std::string p2Score_str = std::to_string(p2Score_val);


            // Update the dataContainer2D in memory
            delete[] allMatches.data[matchRow][winnerCol];
            allMatches.data[matchRow][winnerCol] = CsvToolkit::duplicateString(winnerId_cstr);
            
            delete[] allMatches.data[matchRow][statusCol];
            allMatches.data[matchRow][statusCol] = CsvToolkit::duplicateString("Completed");

            delete[] allMatches.data[matchRow][p1ScoreCol];
            allMatches.data[matchRow][p1ScoreCol] = CsvToolkit::duplicateString(p1Score_str.c_str());
            delete[] allMatches.data[matchRow][p2ScoreCol];
            allMatches.data[matchRow][p2ScoreCol] = CsvToolkit::duplicateString(p2Score_str.c_str());


            // Add actual end time (could be prompted or auto-generated)
            // delete[] allMatches.data[matchRow][actualEndDtCol];
            // allMatches.data[matchRow][actualEndDtCol] = CsvToolkit::duplicateString("YYYY-MM-DD HH:MM:SS_NOW");


            // Rewrite the entire matches.csv file with the updated data
            if (CsvToolkit::writeData(MATCHES_CSV_FILE, allMatches) == 0) {
                std::cout << "Match result updated successfully." << std::endl;

                // Basic Progression: If a winner, announce they progress.
                // A real system would identify the next match in the bracket for this winner.
                if (winnerId_cstr && strcmp(winnerId_cstr, "DRAW") != 0 && strlen(winnerId_cstr) > 0) {
                    std::cout << "Player " << winnerId_cstr << " progresses!" << std::endl;
                }
            } else {
                std::cout << "Error writing updated match data to file." << std::endl;
            }
            delete[] winnerId_cstr;
        }
        CsvToolkit::deleteDataContainer2D(allMatches);
        CsvToolkit::getString("Press Enter to continue...");
    }


    // --- Main function for Task 1 ---
    void manage_scheduling_and_progression() {
        int playerCount = 0;
        PlayerProfile* allPlayers = loadPlayers(playerCount);
        
        int stageCount = 0;
        TournamentStageInfo* allStages = loadStages(stageCount);

        bool running = true;
        while (running) {
            CsvToolkit::clearTerminal();
            const char* menuOptions[] = {
                "Generate Schedule for a Stage",
                "View Current Full Schedule",
                "Update Match Result & Progression",
                "Return to Main Menu"
            };
            int choice = CsvToolkit::displayMenu("Match Scheduling & Player Progression", menuOptions, 4);

            switch (choice) {
                case 1:
                    if (allStages && stageCount > 0) {
                        // Let user pick a stage or default to first one for simplicity
                        std::cout << "Available Stages:" << std::endl;
                        for(int i=0; i<stageCount; ++i) {
                            std::cout << (i+1) << ". " << (allStages[i].stage_name ? allStages[i].stage_name : "Unknown Stage") << std::endl;
                        }
                        int stageChoice = CsvToolkit::getInt("Select stage number to schedule: ");
                        if(stageChoice > 0 && stageChoice <= stageCount) {
                            generateScheduleForStage(allStages[stageChoice-1], allPlayers, playerCount);
                        } else {
                            CsvToolkit::displaySystemMessage("Invalid stage selection.", 2);
                        }
                    } else {
                        CsvToolkit::displaySystemMessage("No stages loaded to schedule.", 2);
                    }
                    break;
                case 2:
                    viewCurrentSchedule();
                    break;
                case 3:
                    updateMatchResultAndProgress();
                    break;
                case 4:
                    running = false;
                    break;
                default:
                    CsvToolkit::displaySystemMessage("Invalid choice.", 2);
                    break;
            }
        }
        std::cout << "Exiting Match Scheduling module." << std::endl;
        
        // Cleanup
        if (allPlayers) delete[] allPlayers; // PlayerProfile destructors called
        if (allStages) {
            // Need proper destructors or manual cleanup for TournamentStageInfo char* if not trivially copyable
            for(int i=0; i<stageCount; ++i) {
                // TournamentStageInfo destructor handles its members
            }
            delete[] allStages;
        }
    }

} // namespace EsportsScheduler

#endif // ESPORTS_SCHEDULER_HPP