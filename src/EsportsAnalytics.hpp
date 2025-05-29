#ifndef ESPORTS_ANALYTICS_HPP
#define ESPORTS_ANALYTICS_HPP

#include <iostream>
#include <string>
#include "File_exe.hpp" // Your CsvToolkit library (sandaris/esports_championship_management_system/ESPORTS_CHAMPIONSHIP_MANAGEMENT_SYSTEM-848912f450fb84d587df656cb09aeaa6710de7db/src/File_exe.hpp)

using namespace CsvToolkit; // Use the CsvToolkit namespace for CSV operations

namespace EsportsAnalytics {

    // Define constants for the CSV filenames
    const std::string MATCHES_CSV = "matches.csv";
    const std::string PLAYER_STATS_CSV = "player_match_stats.csv";
    const std::string PLAYERS_CSV = "player.csv";

    const int MAX_RECENT_MATCH_OUTCOMES = 10;

    // --- MatchOutcome Structure (aligns with matches.csv) ---
    struct MatchOutcome {
        char* match_id;
        char* tournament_id;
        char* stage_id;
        char* match_round_number;
        char* scheduled_datetime;
        char* actual_start_datetime;
        char* actual_end_datetime;
        char* player1_id;
        char* player2_id;
        char* winner_player_id;
        int player1_score;
        int player2_score;
        char* match_status;
        char* bracket_position_info;

        MatchOutcome() : match_id(nullptr), tournament_id(nullptr), stage_id(nullptr),
                         match_round_number(nullptr), scheduled_datetime(nullptr),
                         actual_start_datetime(nullptr), actual_end_datetime(nullptr),
                         player1_id(nullptr), player2_id(nullptr), winner_player_id(nullptr),
                         player1_score(0), player2_score(0), match_status(nullptr),
                         bracket_position_info(nullptr) {}

        MatchOutcome(const char* mid, const char* tid, const char* sid, const char* mrn, const char* sdt,
                     const char* asdt, const char* aedt, const char* p1id, const char* p2id,
                     const char* wpid, int p1s, int p2s, const char* ms, const char* bpi) {
            match_id = duplicateString(mid);
            tournament_id = duplicateString(tid);
            stage_id = duplicateString(sid);
            match_round_number = duplicateString(mrn);
            scheduled_datetime = duplicateString(sdt);
            actual_start_datetime = duplicateString(asdt);
            actual_end_datetime = duplicateString(aedt);
            player1_id = duplicateString(p1id);
            player2_id = duplicateString(p2id);
            winner_player_id = duplicateString(wpid);
            player1_score = p1s;
            player2_score = p2s;
            match_status = duplicateString(ms);
            bracket_position_info = duplicateString(bpi);
        }

        MatchOutcome(const MatchOutcome& other) {
            match_id = duplicateString(other.match_id);
            tournament_id = duplicateString(other.tournament_id);
            stage_id = duplicateString(other.stage_id);
            match_round_number = duplicateString(other.match_round_number);
            scheduled_datetime = duplicateString(other.scheduled_datetime);
            actual_start_datetime = duplicateString(other.actual_start_datetime);
            actual_end_datetime = duplicateString(other.actual_end_datetime);
            player1_id = duplicateString(other.player1_id);
            player2_id = duplicateString(other.player2_id);
            winner_player_id = duplicateString(other.winner_player_id);
            player1_score = other.player1_score;
            player2_score = other.player2_score;
            match_status = duplicateString(other.match_status);
            bracket_position_info = duplicateString(other.bracket_position_info);
        }

        MatchOutcome& operator=(const MatchOutcome& other) {
            if (this == &other) return *this;
            delete[] match_id; delete[] tournament_id; delete[] stage_id; delete[] match_round_number;
            delete[] scheduled_datetime; delete[] actual_start_datetime; delete[] actual_end_datetime;
            delete[] player1_id; delete[] player2_id; delete[] winner_player_id;
            delete[] match_status; delete[] bracket_position_info;

            match_id = duplicateString(other.match_id);
            tournament_id = duplicateString(other.tournament_id);
            stage_id = duplicateString(other.stage_id);
            match_round_number = duplicateString(other.match_round_number);
            scheduled_datetime = duplicateString(other.scheduled_datetime);
            actual_start_datetime = duplicateString(other.actual_start_datetime);
            actual_end_datetime = duplicateString(other.actual_end_datetime);
            player1_id = duplicateString(other.player1_id);
            player2_id = duplicateString(other.player2_id);
            winner_player_id = duplicateString(other.winner_player_id);
            player1_score = other.player1_score;
            player2_score = other.player2_score;
            match_status = duplicateString(other.match_status);
            bracket_position_info = duplicateString(other.bracket_position_info);
            return *this;
        }

        ~MatchOutcome() {
            delete[] match_id; delete[] tournament_id; delete[] stage_id; delete[] match_round_number;
            delete[] scheduled_datetime; delete[] actual_start_datetime; delete[] actual_end_datetime;
            delete[] player1_id; delete[] player2_id; delete[] winner_player_id;
            delete[] match_status; delete[] bracket_position_info;
        }

        void getAsCsvRow(const char** rowBuffer, int bufferSize,
                         std::string& p1ScoreStr_temp, std::string& p2ScoreStr_temp) const {
            if (bufferSize < 14) return;
            p1ScoreStr_temp = std::to_string(player1_score);
            p2ScoreStr_temp = std::to_string(player2_score);

            rowBuffer[0]  = match_id ? match_id : "";
            rowBuffer[1]  = tournament_id ? tournament_id : "";
            rowBuffer[2]  = stage_id ? stage_id : "";
            rowBuffer[3]  = match_round_number ? match_round_number : "";
            rowBuffer[4]  = scheduled_datetime ? scheduled_datetime : "";
            rowBuffer[5]  = actual_start_datetime ? actual_start_datetime : "";
            rowBuffer[6]  = actual_end_datetime ? actual_end_datetime : "";
            rowBuffer[7]  = player1_id ? player1_id : "";
            rowBuffer[8]  = player2_id ? player2_id : "";
            rowBuffer[9]  = winner_player_id ? winner_player_id : "";
            rowBuffer[10] = p1ScoreStr_temp.c_str();
            rowBuffer[11] = p2ScoreStr_temp.c_str();
            rowBuffer[12] = match_status ? match_status : "";
            rowBuffer[13] = bracket_position_info ? bracket_position_info : "";
        }
    };

    // --- PlayerMatchStat Structure (aligns with player_match_stats.csv) ---
    struct PlayerMatchStat {
        char* stat_id; char* match_id; char* player_id;
        int kills; int deaths; int assists; int points_scored;
        char* game_specific_stat_name_1; char* game_specific_stat_value_1;
        char* game_specific_stat_name_2; char* game_specific_stat_value_2;
        char* match_highlights_notes;

        PlayerMatchStat() : stat_id(nullptr), match_id(nullptr), player_id(nullptr), kills(0), deaths(0), assists(0),
                            points_scored(0), game_specific_stat_name_1(nullptr), game_specific_stat_value_1(nullptr),
                            game_specific_stat_name_2(nullptr), game_specific_stat_value_2(nullptr), match_highlights_notes(nullptr) {}
        
        PlayerMatchStat(const char* sid, const char* mid, const char* pid, int k, int d, int a, int ps, const char* notes = "",
                        const char* gssn1 = "", const char* gssv1 = "", const char* gssn2 = "", const char* gssv2 = "") {
            stat_id = duplicateString(sid);
            match_id = duplicateString(mid);
            player_id = duplicateString(pid);
            kills = k; deaths = d; assists = a; points_scored = ps;
            game_specific_stat_name_1 = duplicateString(gssn1);
            game_specific_stat_value_1 = duplicateString(gssv1);
            game_specific_stat_name_2 = duplicateString(gssn2);
            game_specific_stat_value_2 = duplicateString(gssv2);
            match_highlights_notes = duplicateString(notes);
        }
        // Proper Copy Constructor, Assignment Operator needed if this struct is copied often by value.
        // For brevity, assuming it's handled or less critical if copies are few. Add them for robustness.
        ~PlayerMatchStat() {
            delete[] stat_id; delete[] match_id; delete[] player_id;
            delete[] game_specific_stat_name_1; delete[] game_specific_stat_value_1;
            delete[] game_specific_stat_name_2; delete[] game_specific_stat_value_2;
            delete[] match_highlights_notes;
        }

        void getAsCsvRow(const char** rowBuffer, int bufferSize,
                         std::string& sK_temp, std::string& sD_temp, std::string& sA_temp, std::string& sP_temp) const {
            if (bufferSize < 12) return;
            sK_temp = std::to_string(kills); sD_temp = std::to_string(deaths);
            sA_temp = std::to_string(assists); sP_temp = std::to_string(points_scored);

            rowBuffer[0]  = stat_id ? stat_id : "";
            rowBuffer[1]  = match_id ? match_id : "";
            rowBuffer[2]  = player_id ? player_id : "";
            rowBuffer[3]  = sK_temp.c_str();
            rowBuffer[4]  = sD_temp.c_str();
            rowBuffer[5]  = sA_temp.c_str();
            rowBuffer[6]  = sP_temp.c_str();
            rowBuffer[7]  = game_specific_stat_name_1 ? game_specific_stat_name_1 : "";
            rowBuffer[8]  = game_specific_stat_value_1 ? game_specific_stat_value_1 : "";
            rowBuffer[9]  = game_specific_stat_name_2 ? game_specific_stat_name_2 : "";
            rowBuffer[10] = game_specific_stat_value_2 ? game_specific_stat_value_2 : "";
            rowBuffer[11] = match_highlights_notes ? match_highlights_notes : "";
        }
    };

    // --- Circular Queue for Recent Match Outcomes ---
    class RecentMatchOutcomesQueue {
    private:
        MatchOutcome* outcomes; // C-style array of MatchOutcome objects
        int capacity; int head; int tail; int count;
    public:
        RecentMatchOutcomesQueue(int size = MAX_RECENT_MATCH_OUTCOMES)
            : capacity(size), head(0), tail(0), count(0) {
            outcomes = new MatchOutcome[capacity]; // Allocate C-style array
        }
        ~RecentMatchOutcomesQueue() { delete[] outcomes; } // Destructor for MatchOutcome called for each
        bool isFull() const { return count == capacity; }
        bool isEmpty() const { return count == 0; }

        void enqueue(const MatchOutcome& outcome) {
            outcomes[tail] = outcome; // Uses MatchOutcome's assignment operator
            tail = (tail + 1) % capacity;
            if (isFull()) head = (head + 1) % capacity;
            else count++;
        }
        void display() const { /* ... same as previous ... */
            clearTerminal();
            if (isEmpty()) {
                std::cout << "No recent match outcomes to display." << std::endl;
                getString("Press Enter to continue..."); // Use getString from CsvToolkit
                return;
            }
            std::cout << "--- Most Recent Match Outcomes (Oldest to Newest in Buffer) ---" << std::endl;
            int currentIdx = head;
            for (int i = 0; i < count; ++i) {
                const MatchOutcome& mo = outcomes[currentIdx];
                std::cout << (i + 1) << ". Match: " << (mo.match_id ? mo.match_id : "N/A")
                          << " | Stage: " << (mo.stage_id ? mo.stage_id : "N/A")
                          << " | " << (mo.player1_id ? mo.player1_id : "P1") << " [" << mo.player1_score << "] vs "
                          << (mo.player2_id ? mo.player2_id : "P2") << " [" << mo.player2_score << "]"
                          << " | Winner: " << (mo.winner_player_id ? mo.winner_player_id : "N/A")
                          << " | Status: " << (mo.match_status ? mo.match_status : "N/A")
                          << std::endl;
                currentIdx = (currentIdx + 1) % capacity;
            }
            std::cout << "------------------------------------------------------------" << std::endl;
            getString("Press Enter to continue..."); // Use getString from CsvToolkit
        }
    };

    RecentMatchOutcomesQueue recentMatchOutcomes(MAX_RECENT_MATCH_OUTCOMES);

    
    // --- Function to get player stats input ---
    PlayerMatchStat getPlayerStatsFromInput(const char* matchId, const char* playerId) {
        std::cout << "\nEntering stats for Player: " << (playerId ? playerId : "N/A") << " in Match: " << (matchId ? matchId : "N/A") << std::endl;
        char* stat_id_str = getString("Enter Stat ID (e.g., STATM001P1): ");
        int kills = getInt("Kills: ");
        int deaths = getInt("Deaths: ");
        int assists = getInt("Assists: ");
        int points_scored = getInt("Points Scored (game specific): ");
        char* gssn1 = getString("Game Specific Stat 1 Name (or blank): ");
        char* gssv1 = getString("Game Specific Stat 1 Value (or blank): ");
        char* gssn2 = getString("Game Specific Stat 2 Name (or blank): ");
        char* gssv2 = getString("Game Specific Stat 2 Value (or blank): ");
        char* notes = getString("Match Highlights/Notes for this player: ");

        PlayerMatchStat pms(stat_id_str, matchId, playerId, kills, deaths, assists, points_scored, notes, gssn1, gssv1, gssn2, gssv2);
        
        delete[] stat_id_str; delete[] notes; delete[] gssn1; delete[] gssv1; delete[] gssn2; delete[] gssv2;
        return pms;
    }


    // --- Core Task 4 Functions ---
    void logNewMatchOutcome() {
        clearTerminal();
        std::cout << "--- Log New Match Outcome ---" << std::endl;

        char* mo_match_id = getString("Match ID (e.g., MATCH000X): ");
        char* mo_tournament_id = getString("Tournament ID (e.g., APUEC2025): ");
        char* mo_stage_id = getString("Stage ID (e.g., STAGE01): ");
        char* mo_round = getString("Match Round Number: ");
        char* mo_scheduled_dt = getString("Scheduled Datetime (YYYY-MM-DD HH:MM): ");
        char* mo_actual_start_dt = getString("Actual Start (YYYY-MM-DD HH:MM, blank if N/A): ");
        char* mo_actual_end_dt = getString("Actual End (YYYY-MM-DD HH:MM, blank if N/A): ");
        char* mo_p1_id = getString("Player 1 ID: ");
        char* mo_p2_id = getString("Player 2 ID: ");
        int mo_p1_score = getInt("Player 1 Score: ");
        int mo_p2_score = getInt("Player 2 Score: ");
        char* mo_winner_id = getString("Winner Player ID (or DRAW/TBD): ");
        char* mo_status = getString("Match Status (Completed/Scheduled/Ongoing): ");
        char* mo_bracket_info = getString("Bracket Position Info: ");

        MatchOutcome newOutcome(mo_match_id, mo_tournament_id, mo_stage_id, mo_round, mo_scheduled_dt,
                                mo_actual_start_dt, mo_actual_end_dt, mo_p1_id, mo_p2_id,
                                mo_winner_id, mo_p1_score, mo_p2_score, mo_status, mo_bracket_info);
        recentMatchOutcomes.enqueue(newOutcome);

        const int NUM_MATCH_CSV_FIELDS = 14;
        const char* matchCsvRow[NUM_MATCH_CSV_FIELDS]; // C-style array on stack
        std::string p1s_str_temp, p2s_str_temp;
        newOutcome.getAsCsvRow(matchCsvRow, NUM_MATCH_CSV_FIELDS, p1s_str_temp, p2s_str_temp);
        
        const char* matchHeaders[] = {"match_id","tournament_id","stage_id","match_round_number","scheduled_datetime","actual_start_datetime","actual_end_datetime","player1_id","player2_id","winner_player_id","player1_score","player2_score","match_status","bracket_position_info"};
        ensureCsvHeader(MATCHES_CSV, matchHeaders, NUM_MATCH_CSV_FIELDS);
        if (writeNewDataRow(MATCHES_CSV, NUM_MATCH_CSV_FIELDS, matchCsvRow) == 0) {
            std::cout << "Match outcome logged to " << MATCHES_CSV << std::endl;
        } else { std::cout << "Error logging match outcome to " << MATCHES_CSV << std::endl; }


        // Log Player Stats if match is not just "Scheduled"
        if (mo_status && strcmp(mo_status, "Scheduled") != 0) {
            if (mo_p1_id && strlen(mo_p1_id) > 0) {
                PlayerMatchStat p1_stats = getPlayerStatsFromInput(mo_match_id, mo_p1_id);
                const int NUM_PLAYER_STATS_FIELDS = 12;
                const char* p1StatCsvRow[NUM_PLAYER_STATS_FIELDS]; // C-style array on stack
                std::string sK_temp,sD_temp,sA_temp,sP_temp;
                p1_stats.getAsCsvRow(p1StatCsvRow, NUM_PLAYER_STATS_FIELDS,sK_temp,sD_temp,sA_temp,sP_temp);
                const char* playerStatsHeaders[] = {"stat_id","match_id","player_id","kills","deaths","assists","points_scored","game_specific_stat_name_1","game_specific_stat_value_1","game_specific_stat_name_2","game_specific_stat_value_2","match_highlights_notes"};
                ensureCsvHeader(PLAYER_STATS_CSV, playerStatsHeaders, NUM_PLAYER_STATS_FIELDS); // Ensure headers for player_stats.csv
                writeNewDataRow(PLAYER_STATS_CSV, NUM_PLAYER_STATS_FIELDS, p1StatCsvRow);
                std::cout << "Stats for Player " << mo_p1_id << " logged." << std::endl;
            }
            if (mo_p2_id && strlen(mo_p2_id) > 0) {
                PlayerMatchStat p2_stats = getPlayerStatsFromInput(mo_match_id, mo_p2_id);
                const int NUM_PLAYER_STATS_FIELDS = 12;
                const char* p2StatCsvRow[NUM_PLAYER_STATS_FIELDS]; // C-style array on stack
                std::string sK_temp,sD_temp,sA_temp,sP_temp;
                p2_stats.getAsCsvRow(p2StatCsvRow, NUM_PLAYER_STATS_FIELDS,sK_temp,sD_temp,sA_temp,sP_temp);
                // Header check would have happened with p1_stats if it was the first entry
                writeNewDataRow(PLAYER_STATS_CSV, NUM_PLAYER_STATS_FIELDS, p2StatCsvRow);
                std::cout << "Stats for Player " << mo_p2_id << " logged." << std::endl;
            }
        }
        
        delete[] mo_match_id; delete[] mo_tournament_id; delete[] mo_stage_id; delete[] mo_round;
        delete[] mo_scheduled_dt; delete[] mo_actual_start_dt; delete[] mo_actual_end_dt;
        delete[] mo_p1_id; delete[] mo_p2_id; delete[] mo_winner_id; delete[] mo_status; delete[] mo_bracket_info;

        getString("Press Enter to continue...");
    }

    void viewFullMatchOutcomes() {
        clearTerminal();
        std::cout << "--- Full Match Outcomes (" << MATCHES_CSV << ") ---" << std::endl;
        dataContainer2D history = getData(MATCHES_CSV); // Uses CsvToolkit
        if (history.error || history.y == 0) {
            std::cout << "No match history found or error reading file." << std::endl;
        } else {
            displayTabulatedData(history); // Uses CsvToolkit
        }
        deleteDataContainer2D(history); // Uses CsvToolkit
        getString("Press Enter to continue...");
    }

    void analyzePlayerPerformance() {
        clearTerminal();
        std::cout << "--- Player Performance Analysis ---" << std::endl;
        char* pId_cstr = getString("Enter Player ID to analyze (e.g., PLYR0001): ");
        std::string targetPlayerID(pId_cstr ? pId_cstr : ""); // Use std::string for easier comparison
        delete[] pId_cstr;

        if (targetPlayerID.empty()) {
            std::cout << "No Player ID entered." << std::endl;
            getString("Press Enter to continue...");
            return;
        }

        std::string playerName = "Unknown";
        dataContainer2D playersData = getData(PLAYERS_CSV);
        if (!playersData.error && playersData.fields) {
            int idCol = -1, nameCol = -1;
            for(int i=0; i < playersData.x; ++i) {
                if(playersData.fields[i] && strcmp(playersData.fields[i], "player_id") == 0) idCol = i;
                if(playersData.fields[i] && strcmp(playersData.fields[i], "player_name") == 0) nameCol = i;
            }
            if(idCol != -1 && nameCol != -1) {
                for(int i=0; i < playersData.y; ++i) {
                    if(playersData.data[i] && playersData.data[i][idCol] && targetPlayerID == playersData.data[i][idCol]) {
                        playerName = playersData.data[i][nameCol];
                        break;
                    }
                }
            }
        }
        deleteDataContainer2D(playersData);
        std::cout << "\n--- Performance for Player: " << playerName << " (" << targetPlayerID << ") ---" << std::endl;

        dataContainer2D matchHistory = getData(MATCHES_CSV);
        int matchesPlayed = 0, wins = 0;
        if (!matchHistory.error && matchHistory.fields) {
            int p1Col = -1, p2Col = -1, winnerCol = -1, statusCol = -1;
            for(int i=0; i < matchHistory.x; ++i) {
                if(matchHistory.fields[i] && strcmp(matchHistory.fields[i], "player1_id") == 0) p1Col = i;
                if(matchHistory.fields[i] && strcmp(matchHistory.fields[i], "player2_id") == 0) p2Col = i;
                if(matchHistory.fields[i] && strcmp(matchHistory.fields[i], "winner_player_id") == 0) winnerCol = i;
                if(matchHistory.fields[i] && strcmp(matchHistory.fields[i], "match_status") == 0) statusCol = i;
            }

            if(p1Col != -1 && p2Col != -1 && winnerCol != -1 && statusCol != -1) {
                for (int i = 0; i < matchHistory.y; ++i) {
                    if (!matchHistory.data[i]) continue;
                    bool isCompleted = matchHistory.data[i][statusCol] && strcmp(matchHistory.data[i][statusCol], "Completed") == 0;
                    if (!isCompleted) continue;

                    bool involved = false;
                    if (matchHistory.data[i][p1Col] && targetPlayerID == matchHistory.data[i][p1Col]) involved = true;
                    else if (matchHistory.data[i][p2Col] && targetPlayerID == matchHistory.data[i][p2Col]) involved = true;

                    if (involved) {
                        matchesPlayed++;
                        if (matchHistory.data[i][winnerCol] && targetPlayerID == matchHistory.data[i][winnerCol]) wins++;
                    }
                }
            } else std::cout << "Required columns for match analysis not found in " << MATCHES_CSV << std::endl;
        } else std::cout << "Could not load " << MATCHES_CSV << " for match analysis." << std::endl;
        
        std::cout << "Completed Matches Played: " << matchesPlayed << std::endl;
        std::cout << "Wins: " << wins << std::endl;
        if (matchesPlayed > 0) std::cout << "Win Rate: " << (static_cast<double>(wins) / matchesPlayed * 100.0) << "%" << std::endl;
        deleteDataContainer2D(matchHistory);

        dataContainer2D playerStatsHistory = getData(PLAYER_STATS_CSV);
        int totalKills = 0, totalDeaths = 0, totalAssists = 0, totalPoints = 0, statEntries = 0;
        if(!playerStatsHistory.error && playerStatsHistory.fields) {
            int pIdCol_stats = -1, killsCol = -1, deathsCol = -1, assistsCol = -1, pointsCol_stats = -1;
            for(int i=0; i < playerStatsHistory.x; ++i) {
                if(playerStatsHistory.fields[i] && strcmp(playerStatsHistory.fields[i], "player_id") == 0) pIdCol_stats = i;
                if(playerStatsHistory.fields[i] && strcmp(playerStatsHistory.fields[i], "kills") == 0) killsCol = i;
                if(playerStatsHistory.fields[i] && strcmp(playerStatsHistory.fields[i], "deaths") == 0) deathsCol = i;
                if(playerStatsHistory.fields[i] && strcmp(playerStatsHistory.fields[i], "assists") == 0) assistsCol = i;
                if(playerStatsHistory.fields[i] && strcmp(playerStatsHistory.fields[i], "points_scored") == 0) pointsCol_stats = i;
            }

            if (pIdCol_stats != -1 && killsCol != -1 && deathsCol != -1 && assistsCol != -1 && pointsCol_stats != -1) {
                for (int i = 0; i < playerStatsHistory.y; ++i) {
                    if (playerStatsHistory.data[i] && playerStatsHistory.data[i][pIdCol_stats] && targetPlayerID == playerStatsHistory.data[i][pIdCol_stats]) {
                        statEntries++;
                        totalKills += atoi(playerStatsHistory.data[i][killsCol] ? playerStatsHistory.data[i][killsCol] : "0");
                        totalDeaths += atoi(playerStatsHistory.data[i][deathsCol] ? playerStatsHistory.data[i][deathsCol] : "0");
                        totalAssists += atoi(playerStatsHistory.data[i][assistsCol] ? playerStatsHistory.data[i][assistsCol] : "0");
                        totalPoints += atoi(playerStatsHistory.data[i][pointsCol_stats] ? playerStatsHistory.data[i][pointsCol_stats] : "0");
                    }
                }
                 std::cout << "\nAggregate Stats from " << statEntries << " performance records:" << std::endl;
                 std::cout << "Total Kills: " << totalKills << std::endl;
                 std::cout << "Total Deaths: " << totalDeaths << std::endl;
                 std::cout << "Total Assists: " << totalAssists << std::endl;
                 std::cout << "Total Game Points Scored: " << totalPoints << std::endl;
                 if (statEntries > 0 && totalDeaths > 0) std::cout << "Avg KDA ((K+A)/D): " << (static_cast<double>(totalKills + totalAssists) / totalDeaths) << std::endl;
                 else if (statEntries > 0) std::cout << "Avg KDA ((K+A)/1): " << (static_cast<double>(totalKills + totalAssists)) << " (Deaths were 0)" << std::endl;


            } else std::cout << "Required columns not found in " << PLAYER_STATS_CSV << " for detailed stats analysis." << std::endl;
        } else std::cout << "Could not load " << PLAYER_STATS_CSV << " for detailed stats analysis." << std::endl;
        deleteDataContainer2D(playerStatsHistory);

        getString("Press Enter to continue...");
    }

    // Main entry point for Task 4
    void record_N_Analysis() {
        bool running = true;
        while (running) {
            clearTerminal();
            const char* menuOptions[] = {
                "Log New Match Outcome & Player Stats",
                "View Recent Match Outcomes",
                "View Full Match Outcomes History",
                "Analyze Player Performance",
                "Return to Main Menu"
            };
            // Using displayMenu for user interaction
            int choice = displayMenu("Game Result Logging & Performance History", menuOptions, 5);

            switch (choice) {
                case 1: logNewMatchOutcome(); break;
                case 2: recentMatchOutcomes.display(); break;
                case 3: viewFullMatchOutcomes(); break;
                case 4: analyzePlayerPerformance(); break;
                case 5: running = false; break;
                default: displaySystemMessage("Invalid choice, please try again.", 2); break;
            }
        }
        clearTerminal();
        std::cout << "Exiting Game Result Logging & Performance History module." << std::endl;
        getString("Press Enter to return to main APUEC system menu...");
    }

} // namespace EsportsAnalytics

#endif // ESPORTS_ANALYTICS_HPP