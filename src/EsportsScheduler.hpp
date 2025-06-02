#ifndef ESPORTS_SCHEDULER_HPP
#define ESPORTS_SCHEDULER_HPP

#include <iostream>
#include <string>
#include <algorithm> // For std::sort
#include <iomanip>   // For output formatting
#include <cstdio>    // For sprintf
#include <cstring>   // For strcmp, strlen etc.

#include "File_exe.hpp" // Your CsvToolkit library

using namespace CsvToolkit; // Use the CsvToolkit namespace for CSV operations

namespace EsportsScheduler {

    // --- Constants for CSV Files ---
    const std::string TEAMS_CSV = "teams.csv";
    const std::string MATCH_CSV = "match.csv";
    const std::string PLAYER_CSV = "player.csv";
    const std::string GAME_STAT_CSV = "gameStat.csv";
    const std::string TOURNAMENT_BRACKET_CSV = "tournament_bracket.csv";
    const int MAX_TEAMS_DEFAULT = 12; // Default capacity for dynamic arrays of teams
    const int MAX_MATCHES_DEFAULT = 200; // Default capacity for dynamic arrays of matches

    const char* HERO_NAMES[] = {
        "Aegis Knight", "Void Ranger", "Chronomancer", "Frost Lich", "Sun Guardian",
        "Storm Zealot", "Night Stalker", "Blood Witch", "Iron Golem", "Sky Templar",
        "Abyssal Fiend", "Grove Warden", "Star Fencer", "Dune Strider", "Shadow Weaver",
        "Stone Breaker", "Wind Dancer", "Magma Lord", "Tidal Mystic", "Spirit Walker",
        "Phantom Lancer", "Crystal Maiden", "Dragon Knight", "Invoker", "Anti-Mage" // Added a few more
    };
    const int NUM_HEROES = sizeof(HERO_NAMES) / sizeof(char*);

    const int MIN_KILLS = 0, MAX_KILLS = 20;
    const int MIN_DEATHS = 0, MAX_DEATHS = 15;
    const int MIN_ASSISTS = 0, MAX_ASSISTS = 25;
    const int MIN_GPM = 250, MAX_GPM = 850;
    const int MIN_XPM = 300, MAX_XPM = 900;
    const int PLAYERS_PER_TEAM_FOR_STATS = 5;

    // --- Data Structures ---

    struct PlayerStatInfo {
        char* inGameName;

        PlayerStatInfo(const char* ign = nullptr) {
            inGameName = duplicateString(ign);
        }
        PlayerStatInfo(const PlayerStatInfo& other) {
            inGameName = duplicateString(other.inGameName);
        }
        PlayerStatInfo& operator=(const PlayerStatInfo& other) {
            if (this == &other) return *this;
            delete[] inGameName;
            inGameName = duplicateString(other.inGameName);
            return *this;
        }
        ~PlayerStatInfo() {
            delete[] inGameName;
        }
    };

    struct Team {
        char* team_id;
        char* team_name;
        char* university;
        //char* country;
        int ranking_point;
        char* team_status; // e.g., "Upper Bracket", "Lower Bracket", "Disqualified"

        Team() : team_id(nullptr), team_name(nullptr), university(nullptr), ranking_point(0), team_status(nullptr) {}

        Team(const char* id, const char* name, const char* uni, int rank, const char* status = "Pending") {
            team_id = duplicateString(id);
            team_name = duplicateString(name);
            university = duplicateString(uni);
            //country = duplicateString(ctry);
            ranking_point = rank;
            team_status = duplicateString(status);
        }

        Team(const Team& other) {
            team_id = duplicateString(other.team_id);
            team_name = duplicateString(other.team_name);
            university = duplicateString(other.university);
            //country = duplicateString(other.country);
            ranking_point = other.ranking_point;
            team_status = duplicateString(other.team_status);
        }

        Team& operator=(const Team& other) {
            if (this == &other) return *this;
            delete[] team_id; delete[] team_name; delete[] university; delete[] team_status;
            team_id = duplicateString(other.team_id);
            team_name = duplicateString(other.team_name);
            university = duplicateString(other.university);
            //country = duplicateString(other.country);
            ranking_point = other.ranking_point;
            team_status = duplicateString(other.team_status);
            return *this;
        }

        ~Team() {
            delete[] team_id; delete[] team_name; delete[] university; delete[] team_status;
        }
    };

    bool compareTeams(const Team& a, const Team& b) {
        return a.ranking_point > b.ranking_point;
    }

    struct Match {
        char* match_id;
        char* scheduled_date;
        char* scheduled_time;
        // char* match_round_number; // REMOVE THIS LINE
        char* actual_start_time;
        char* actual_end_time;
        char* team1_id;
        char* team2_id;
        char* winner_team_id;
        int team1_score;
        int team2_score;
        char* match_status;
        char* match_level;

        // Default constructor
        Match() : match_id(nullptr), scheduled_date(nullptr), scheduled_time(nullptr), /* match_round_number removed */
                actual_start_time(nullptr), actual_end_time(nullptr), team1_id(nullptr), team2_id(nullptr),
                winner_team_id(nullptr), team1_score(0), team2_score(0), match_status(nullptr), match_level(nullptr) {}

        // Parameterized constructor (remove 'round' parameter)
        Match(const char* mid, const char* s_date, const char* s_time, /* const char* round REMOVED */
            const char* t1_id, const char* t2_id, const char* status, const char* level) {
            match_id = duplicateString(mid);
            scheduled_date = duplicateString(s_date);
            scheduled_time = duplicateString(s_time);
            // match_round_number = duplicateString(round); // REMOVE THIS LINE
            actual_start_time = duplicateString(""); // Init empty
            actual_end_time = duplicateString("");   // Init empty
            team1_id = duplicateString(t1_id);
            team2_id = duplicateString(t2_id);
            winner_team_id = duplicateString(""); // Init empty
            team1_score = 0;
            team2_score = 0;
            match_status = duplicateString(status);
            match_level = duplicateString(level);
        }
        
        // Copy Constructor (remove match_round_number)
        Match(const Match& other){
            match_id = duplicateString(other.match_id);
            scheduled_date = duplicateString(other.scheduled_date);
            scheduled_time = duplicateString(other.scheduled_time);
            // match_round_number removed
            actual_start_time = duplicateString(other.actual_start_time);
            actual_end_time = duplicateString(other.actual_end_time);
            team1_id = duplicateString(other.team1_id);
            team2_id = duplicateString(other.team2_id);
            winner_team_id = duplicateString(other.winner_team_id);
            team1_score = other.team1_score;
            team2_score = other.team2_score;
            match_status = duplicateString(other.match_status);
            match_level = duplicateString(other.match_level);
        }

        // Assignment Operator (remove match_round_number)
        Match& operator=(const Match& other) {
            if (this == &other) return *this;
            delete[] match_id; delete[] scheduled_date; delete[] scheduled_time; /* delete[] match_round_number; REMOVED */
            delete[] actual_start_time; delete[] actual_end_time; delete[] team1_id; delete[] team2_id;
            delete[] winner_team_id; delete[] match_status; delete[] match_level;

            match_id = duplicateString(other.match_id);
            scheduled_date = duplicateString(other.scheduled_date);
            scheduled_time = duplicateString(other.scheduled_time);
            // match_round_number removed
            actual_start_time = duplicateString(other.actual_start_time);
            actual_end_time = duplicateString(other.actual_end_time);
            team1_id = duplicateString(other.team1_id);
            team2_id = duplicateString(other.team2_id);
            winner_team_id = duplicateString(other.winner_team_id);
            team1_score = other.team1_score;
            team2_score = other.team2_score;
            match_status = duplicateString(other.match_status);
            match_level = duplicateString(other.match_level);
            return *this;
        }

        // Destructor (remove match_round_number)
        ~Match() {
            delete[] match_id; delete[] scheduled_date; delete[] scheduled_time; /* delete[] match_round_number; REMOVED */
            delete[] actual_start_time; delete[] actual_end_time; delete[] team1_id; delete[] team2_id;
            delete[] winner_team_id; delete[] match_status; delete[] match_level;
        }

        // getAsCsvRow method (adjust for 12 fields and new order)
        void getAsCsvRow(const char** rowBuffer, int bufferSize) const {
            if (bufferSize < 12) return; // Now 12 fields
            
            static std::string t1_s, t2_s; // For int to string conversion
            t1_s = std::to_string(team1_score);
            t2_s = std::to_string(team2_score);

            rowBuffer[0] = match_id ? match_id : "";
            rowBuffer[1] = scheduled_date ? scheduled_date : "";
            rowBuffer[2] = scheduled_time ? scheduled_time : "";
            rowBuffer[3] = actual_start_time ? actual_start_time : "";   // Moved up, was index 4
            rowBuffer[4] = actual_end_time ? actual_end_time : "";     // Moved up, was index 5
            rowBuffer[5] = team1_id ? team1_id : "";                   // Moved up, was index 6
            rowBuffer[6] = team2_id ? team2_id : "";                   // Moved up, was index 7
            rowBuffer[7] = winner_team_id ? winner_team_id : "";       // Moved up, was index 8
            rowBuffer[8] = t1_s.c_str();                               // Moved up, was index 9
            rowBuffer[9] = t2_s.c_str();                               // Moved up, was index 10
            rowBuffer[10] = match_status ? match_status : "";          // Moved up, was index 11
            rowBuffer[11] = match_level ? match_level : "";            // Moved up, was index 12
        }
    };

    // --- Array-based Circular Queue for Team objects ---
    struct TeamQueue {
        Team* arr;
        int capacity;
        int front_idx;
        int rear_idx;
        int count;

        TeamQueue(int size = MAX_TEAMS_DEFAULT) : capacity(size), front_idx(0), rear_idx(-1), count(0) {
            arr = new Team[capacity];
        }
        ~TeamQueue() {
            delete[] arr; // Team destructor will be called for each element
        }
        bool isEmpty() const { return count == 0; }
        bool isFull() const { return count == capacity; }

        void enqueue(const Team& team) {
            if (isFull()) {
                std::cerr << "Error: TeamQueue is full. Cannot enqueue team " << team.team_id << std::endl;
                // In a real scenario, might resize or handle error differently
                return;
            }
            rear_idx = (rear_idx + 1) % capacity;
            arr[rear_idx] = team; // Uses Team's assignment operator
            count++;
        }

        Team dequeue() {
            if (isEmpty()) {
                std::cerr << "Error: TeamQueue is empty. Cannot dequeue." << std::endl;
                return Team(); // Return a default-constructed (empty) Team
            }
            Team dequeuedTeam = arr[front_idx]; // Uses Team's copy constructor
            front_idx = (front_idx + 1) % capacity;
            count--;
            return dequeuedTeam;
        }
         Team front() const {
            if (isEmpty()) {
                std::cerr << "Error: TeamQueue is empty. Cannot peek." << std::endl;
                return Team();
            }
            return arr[front_idx]; // Uses Team's copy constructor
        }
    };

    // --- Array-based Stack for Team objects ---
    struct TeamStack {
        Team* arr;
        int capacity;
        int top_idx;

        TeamStack(int size = MAX_TEAMS_DEFAULT) : capacity(size), top_idx(-1) {
            arr = new Team[capacity];
        }
        ~TeamStack() {
            delete[] arr;
        }
        bool isEmpty() const { return top_idx == -1; }
        bool isFull() const { return top_idx == capacity - 1; }

        void push(const Team& team) {
            if (isFull()) {
                std::cerr << "Error: TeamStack is full. Cannot push team " << team.team_id << std::endl;
                return;
            }
            arr[++top_idx] = team; // Uses Team's assignment operator
        }

        Team pop() {
            if (isEmpty()) {
                std::cerr << "Error: TeamStack is empty. Cannot pop." << std::endl;
                return Team();
            }
            // The object at arr[top_idx] will be copied, then top_idx decremented.
            // The original object in the array remains until overwritten or stack is destroyed.
            return arr[top_idx--]; // Uses Team's copy constructor
        }

        Team top() const {
            if (isEmpty()) {
                std::cerr << "Error: TeamStack is empty. Cannot peek." << std::endl;
                return Team();
            }
            return arr[top_idx]; // Uses Team's copy constructor
        }
    };

    struct BracketEntry {
        char* team_id;
        char* position;
        char* bracket_status;
        int sort_priority;

        BracketEntry(const char* tid = nullptr, const char* pos = nullptr, const char* br_status = nullptr) 
            : sort_priority(999) { // Default for unknown statuses, will appear last
            team_id = duplicateString(tid);
            position = duplicateString(pos);
            bracket_status = duplicateString(br_status);
            setSortPriority();
        }
        
        BracketEntry(char** rowData, int numCols, int teamIdColIdx, int posColIdx, int bracketColIdx)
            : sort_priority(999) {
            team_id = (teamIdColIdx != -1 && rowData[teamIdColIdx]) ? duplicateString(rowData[teamIdColIdx]) : duplicateString("");
            position = (posColIdx != -1 && rowData[posColIdx]) ? duplicateString(rowData[posColIdx]) : duplicateString("");
            bracket_status = (bracketColIdx != -1 && rowData[bracketColIdx]) ? duplicateString(rowData[bracketColIdx]) : duplicateString("");
            setSortPriority();
        }

        void setSortPriority() {
            sort_priority = 999; // Default: last

            if (bracket_status) {
                if (strcmp(bracket_status, "upper_bracket") == 0) {
                    sort_priority = 10; // Active UB teams
                    // Sub-prioritize within UB if needed, e.g., based on position string
                    if (position && strstr(position, "Winner") != nullptr) sort_priority += 1; // e.g. UB-R1-Winner
                    else if (position && strstr(position, "Finalist") != nullptr) sort_priority += 0; // e.g. UB-Final-Winner comes first
                    else if (position && strstr(position, "Advanced") != nullptr) sort_priority += 2;
                    else if (position && (strstr(position, "Slot") != nullptr || strstr(position, "-M") !=nullptr )) sort_priority +=3; // in a specific match
                    else sort_priority +=5; // Generic UB status

                } else if (strcmp(bracket_status, "lower_bracket") == 0) {
                    sort_priority = 20; // Active LB teams
                    if (position && strstr(position, "Winner") != nullptr) sort_priority += 1;
                    else if (position && strstr(position, "Finalist") != nullptr) sort_priority += 0; 
                    else if (position && strstr(position, "Advanced") != nullptr) sort_priority += 2;
                    else if (position && strstr(position, "FromUB") != nullptr) sort_priority += 3; // Recently dropped
                    else if (position && (strstr(position, "Slot") != nullptr || strstr(position, "-M") !=nullptr )) sort_priority +=4;
                    else sort_priority +=5;

                } else if (strcmp(bracket_status, "finished_placing") == 0) {
                    sort_priority = 30; // Base for placed teams
                    if (position) {
                        if (strcmp(position, "Champion") == 0) sort_priority = 31; // Champion highest among placed
                        else if (strcmp(position, "1st Runner Up") == 0) sort_priority = 32;
                        else if (strcmp(position, "2nd Runner Up") == 0) sort_priority = 33;
                        else if (strcmp(position, "3rd Runner Up") == 0) sort_priority = 34;
                        else sort_priority = 39; // Other placings
                    }
                } else if (strcmp(bracket_status, "eliminated") == 0) {
                    sort_priority = 100; // Base for eliminated teams
                    if (position) { // Sub-sort eliminated types
                        if (strcmp(position, "Eliminated-LB-R1") == 0) sort_priority = 101;
                        else if (strcmp(position, "Eliminated") == 0) sort_priority = 102; // Generic
                        else sort_priority = 105; // Other eliminated types
                    }
                } else if (strcmp(bracket_status, "disqualified") == 0) {
                    sort_priority = 200;
                }
            }
        }

        // Copy Constructor
        BracketEntry(const BracketEntry& other) {
            team_id = duplicateString(other.team_id);
            position = duplicateString(other.position);
            bracket_status = duplicateString(other.bracket_status);
            sort_priority = other.sort_priority;
        }

        // Assignment Operator
        BracketEntry& operator=(const BracketEntry& other) {
            if (this == &other) return *this;
            delete[] team_id; delete[] position; delete[] bracket_status;
            team_id = duplicateString(other.team_id);
            position = duplicateString(other.position);
            bracket_status = duplicateString(other.bracket_status);
            sort_priority = other.sort_priority;
            return *this;
        }
        
        ~BracketEntry() {
            delete[] team_id;
            delete[] position;
            delete[] bracket_status;
        }
    };

    bool compareBracketEntries(const BracketEntry& a, const BracketEntry& b) {
        if (a.sort_priority != b.sort_priority) {
            return a.sort_priority < b.sort_priority; // Lower sort_priority value means higher in the list
        }
        // Secondary sort: by position string alphabetically if priorities are the same
        if (a.position && b.position) {
            int pos_cmp = strcmp(a.position, b.position);
            if (pos_cmp != 0) return pos_cmp < 0;
        } else if (a.position) return true; // a not null, b null
        else if (b.position) return false; // b not null, a null

        // Tertiary sort: by team_id alphabetically if priorities and positions are same (or one position is null)
        if (a.team_id && b.team_id) {
            return strcmp(a.team_id, b.team_id) < 0;
        } else if (a.team_id) {
            return true;
        } else if (b.team_id) {
            return false;
        }
        return false;
    }


    // Helper to get players for a specific team, prioritizing "Main" role
    // Returns a dynamically allocated array of PlayerStatInfo objects. Caller must delete it and its contents.
    PlayerStatInfo* getPlayersForTeamStats(const char* targetTeamId, 
                                        const dataContainer2D& allPlayerData,
                                        int p_teamIdCol, int p_ignCol, int p_roleCol,
                                        int& selectedPlayerCount_out) {
        selectedPlayerCount_out = 0;
        if (!targetTeamId || allPlayerData.error || allPlayerData.y == 0 || p_teamIdCol == -1 || p_ignCol == -1 || p_roleCol == -1) {
            return nullptr;
        }

        PlayerStatInfo* teamPlayers = new PlayerStatInfo[PLAYERS_PER_TEAM_FOR_STATS];
        int mainPlayerCount = 0;
        int otherPlayerCount = 0;

        // First pass: Get "Main" players
        for (int i = 0; i < allPlayerData.y && mainPlayerCount < PLAYERS_PER_TEAM_FOR_STATS; ++i) {
            if (allPlayerData.data[i][p_teamIdCol] && strcmp(allPlayerData.data[i][p_teamIdCol], targetTeamId) == 0) {
                if (allPlayerData.data[i][p_roleCol] && strcmp(allPlayerData.data[i][p_roleCol], "Main") == 0) {
                    if (allPlayerData.data[i][p_ignCol]) {
                        teamPlayers[mainPlayerCount++] = PlayerStatInfo(allPlayerData.data[i][p_ignCol]);
                    }
                }
            }
        }

        selectedPlayerCount_out = mainPlayerCount;

        // Second pass: Fill remaining slots with other players from the team if needed
        if (selectedPlayerCount_out < PLAYERS_PER_TEAM_FOR_STATS) {
            for (int i = 0; i < allPlayerData.y && selectedPlayerCount_out < PLAYERS_PER_TEAM_FOR_STATS; ++i) {
                if (allPlayerData.data[i][p_teamIdCol] && strcmp(allPlayerData.data[i][p_teamIdCol], targetTeamId) == 0) {
                    // Avoid adding "Main" players again if they were already picked
                    bool alreadyAdded = false;
                    if (allPlayerData.data[i][p_roleCol] && strcmp(allPlayerData.data[i][p_roleCol], "Main") == 0) {
                        // This check is simplified; a more robust check would compare InGameNames
                        // to ensure the exact same player isn't added twice if logic allows.
                        // For now, this assumes if it's "Main", it was caught in the first pass.
                        // A better way: collect all players of the team, then prioritize.
                        // For simplicity now: add non-Main players.
                        continue; 
                    }
                    
                    // Check if this non-Main player was already added (if list was just main players)
                    // This check is basic. A more robust system would prevent adding the exact same InGameName twice.
                    // For now, we assume the two passes are distinct enough.
                    // Actually, it's simpler: add players until 5, prioritizing main.

                    // Let's refine player selection: collect all team players, then pick.
                    // The current two-pass logic for Main then others is okay for this scope.
                    // We just need to ensure we don't re-add.
                    // The easiest is just to take any player from the team if not "Main".
                    if (allPlayerData.data[i][p_ignCol]) {
                        // To avoid adding a main player picked in the first loop again if we remove the continue above
                        bool isDuplicate = false;
                        for(int k=0; k < mainPlayerCount; ++k) {
                            if(teamPlayers[k].inGameName && allPlayerData.data[i][p_ignCol] && strcmp(teamPlayers[k].inGameName, allPlayerData.data[i][p_ignCol]) == 0) {
                                isDuplicate = true;
                                break;
                            }
                        }
                        if (!isDuplicate) {
                            teamPlayers[selectedPlayerCount_out++] = PlayerStatInfo(allPlayerData.data[i][p_ignCol]);
                        }
                    }
                }
            }
        }
        
        if (selectedPlayerCount_out == 0) {
            delete[] teamPlayers;
            return nullptr;
        }
        return teamPlayers;
    }

    void generateAndSaveGameStats(const Match& completedMatch) {
        std::cout << "\nGenerating game stats for Match ID: " << completedMatch.match_id << "..." << std::endl;

        dataContainer2D allPlayerData = getData(PLAYER_CSV);
        if (allPlayerData.error || allPlayerData.y == 0) {
            std::cerr << "Error loading player data from " << PLAYER_CSV << " or file is empty. Cannot generate game stats." << std::endl;
            deleteDataContainer2D(allPlayerData);
            return;
        }

        // Find column indices in playerData
        int p_teamIdCol = -1, p_ignCol = -1, p_roleCol = -1;
        if(allPlayerData.fields && allPlayerData.x > 0){
            for (int k = 0; k < allPlayerData.x; ++k) {
                if (strcmp(allPlayerData.fields[k], "TeamID") == 0) p_teamIdCol = k;
                else if (strcmp(allPlayerData.fields[k], "InGameName") == 0) p_ignCol = k;
                else if (strcmp(allPlayerData.fields[k], "Role") == 0) p_roleCol = k;
            }
        }

        if (p_teamIdCol == -1 || p_ignCol == -1 || p_roleCol == -1) {
            std::cerr << "Error: Required columns (TeamID, InGameName, Role) not found in " << PLAYER_CSV 
                    << ". Cannot generate game stats." << std::endl;
            deleteDataContainer2D(allPlayerData);
            return;
        }

        // Ensure gameStat.csv header exists
        const char* gameStatHeaders[] = {"match_id", "InGameName", "hero_played", "kills", "deaths", "assists", "gpm", "xpm"};
        ensureCsvHeader(GAME_STAT_CSV, gameStatHeaders, 8);

        // Seed random number generator (once per program run is better, e.g. in main, but here for self-containment)
        // srand(time(0)); // Moved to main.cpp for better practice

        // Get players for Team 1
        int team1PlayerCount = 0;
        PlayerStatInfo* team1Players = getPlayersForTeamStats(completedMatch.team1_id, allPlayerData, p_teamIdCol, p_ignCol, p_roleCol, team1PlayerCount);
        
        // Get players for Team 2
        int team2PlayerCount = 0;
        PlayerStatInfo* team2Players = getPlayersForTeamStats(completedMatch.team2_id, allPlayerData, p_teamIdCol, p_ignCol, p_roleCol, team2PlayerCount);

        char kills_s[10], deaths_s[10], assists_s[10], gpm_s[10], xpm_s[10];

        // Generate and write stats for Team 1
        if (team1Players) {
            for (int i = 0; i < team1PlayerCount; ++i) {
                
                const char* hero = HERO_NAMES[rand() % NUM_HEROES];

                int kills = MIN_KILLS + (rand() % (MAX_KILLS - MIN_KILLS + 1));
                int deaths = MIN_DEATHS + (rand() % (MAX_DEATHS - MIN_DEATHS + 1));
                int assists = MIN_ASSISTS + (rand() % (MAX_ASSISTS - MIN_ASSISTS + 1));
                int gpm = MIN_GPM + (rand() % (MAX_GPM - MIN_GPM + 1));
                int xpm = MIN_XPM + (rand() % (MAX_XPM - MIN_XPM + 1));

                sprintf(kills_s, "%d", kills);
                sprintf(deaths_s, "%d", deaths);
                sprintf(assists_s, "%d", assists);
                sprintf(gpm_s, "%d", gpm);
                sprintf(xpm_s, "%d", xpm);

                const char* statRow[] = {
                    completedMatch.match_id,
                    team1Players[i].inGameName,
                    hero,
                    kills_s, deaths_s, assists_s, gpm_s, xpm_s
                };
                writeNewDataRow(GAME_STAT_CSV, 8, statRow);
            }
            // Cleanup team1Players array (destructors called by delete[])
            delete[] team1Players; // This will correctly call ~PlayerStatInfo() for each element.
            team1Players = nullptr;
        }

        // Generate and write stats for Team 2
        if (team2Players) {
            for (int i = 0; i < team2PlayerCount; ++i) {
                const char* hero = HERO_NAMES[rand() % NUM_HEROES];
                int kills = MIN_KILLS + (rand() % (MAX_KILLS - MIN_KILLS + 1));
                int deaths = MIN_DEATHS + (rand() % (MAX_DEATHS - MIN_DEATHS + 1));
                int assists = MIN_ASSISTS + (rand() % (MAX_ASSISTS - MIN_ASSISTS + 1));
                int gpm = MIN_GPM + (rand() % (MAX_GPM - MIN_GPM + 1));
                int xpm = MIN_XPM + (rand() % (MAX_XPM - MIN_XPM + 1));

                sprintf(kills_s, "%d", kills);
                sprintf(deaths_s, "%d", deaths);
                sprintf(assists_s, "%d", assists);
                sprintf(gpm_s, "%d", gpm);
                sprintf(xpm_s, "%d", xpm);

                const char* statRow[] = {
                    completedMatch.match_id,
                    team2Players[i].inGameName,
                    hero,
                    kills_s, deaths_s, assists_s, gpm_s, xpm_s
                };
                writeNewDataRow(GAME_STAT_CSV, 8, statRow);
            }
            // Cleanup team2Players array
            delete[] team2Players; // This will correctly call ~PlayerStatInfo() for each element.
            team1Players = nullptr;
        }
        
        if (team1PlayerCount > 0 || team2PlayerCount > 0) { // Only print if some stats were attempted
            std::cout << "Game stats generated and saved to " << GAME_STAT_CSV << std::endl;
        } else if (!team1Players && !team2Players && completedMatch.team1_id && completedMatch.team2_id) {
            // This case might happen if getPlayersForTeamStats returned nullptr for both
            std::cout << "No players found for one or both teams. No game stats generated." << std::endl;
        }

        deleteDataContainer2D(allPlayerData);
    }


    int getNextMatchCounterFileBased() {
        dataContainer2D matches = getData(MATCH_CSV);
        int maxIdNum = 0;
        if (!matches.error && matches.y > 0) {
            int matchIdCol = -1;
            for(int i=0; i < matches.x; ++i) {
                if(matches.fields[i] && strcmp(matches.fields[i], "match_id") == 0) {
                    matchIdCol = i;
                    break;
                }
            }
            if (matchIdCol != -1) {
                for (int i = 0; i < matches.y; ++i) {
                    if (matches.data[i][matchIdCol] && strncmp(matches.data[i][matchIdCol], "MATCH", 5) == 0) {
                        int id = atoi(matches.data[i][matchIdCol] + 5);
                        if (id > maxIdNum) {
                            maxIdNum = id;
                        }
                    }
                }
            }
        }
        deleteDataContainer2D(matches);
        return maxIdNum + 1;
    }

    // In namespace EsportsScheduler
    Team* loadTeamsFromCSV(int& teamCount_out) 
    {
        teamCount_out = 0;
        dataContainer2D teamData = getData(TEAMS_CSV); // TEAMS_CSV should be "team.csv"
        
        dataContainer2D checkedInTeams = filterDataContainer(teamData, "CheckInStatus", "CheckedIn");
        deleteDataContainer2D(teamData);
        
        if (checkedInTeams.error) {
            std::cerr << "Error loading teams from " << TEAMS_CSV << std::endl;
            deleteDataContainer2D(checkedInTeams);
            return nullptr;
        }

        // Adjusted column lookups
        int idCol = -1, nameCol = -1, uniCol = -1, /* countryCol = -1, REMOVED */ rankCol = -1;
        for(int i=0; i < checkedInTeams.x; ++i) {
            if(strcmp(checkedInTeams.fields[i], "TeamID") == 0) idCol = i;
            else if(strcmp(checkedInTeams.fields[i], "TeamName") == 0) nameCol = i;
            else if(strcmp(checkedInTeams.fields[i], "University") == 0) uniCol = i;
            // else if(strcmp(checkedInTeams.fields[i], "Country") == 0) countryCol = i; // REMOVED
            else if(strcmp(checkedInTeams.fields[i], "RankingPoint") == 0) rankCol = i; // Corrected from "RankingPoint" previously
        }

        // Update condition to check for required columns
        if(idCol == -1 || nameCol == -1 || uniCol == -1 || /* countryCol == -1 || REMOVED */ rankCol == -1) {
            std::cerr << "Error: Required columns (TeamID, TeamName, University, ranking_points) not found in " << TEAMS_CSV << std::endl;
            deleteDataContainer2D(checkedInTeams);
            return nullptr;
        }

        if (checkedInTeams.y == 0) {
            deleteDataContainer2D(checkedInTeams);
            return nullptr;
        }

        // Allocate raw memory using global operator new[]
        void* rawMemory = ::operator new[](static_cast<size_t>(checkedInTeams.y) * sizeof(Team));
        if (!rawMemory) {
            std::cerr << "Error: Memory allocation failed for teams array." << std::endl;
            deleteDataContainer2D(checkedInTeams);
            return nullptr;
        }
        Team* loadedTeams = static_cast<Team*>(rawMemory);

        for (int i = 0; i < checkedInTeams.y; ++i) {
            // Construct objects in allocated memory using placement new
            new (&loadedTeams[i]) Team(
                checkedInTeams.data[i][idCol],
                checkedInTeams.data[i][nameCol],
                checkedInTeams.data[i][uniCol],
                //teamData.data[i][countryCol],
                atoi(checkedInTeams.data[i][rankCol] ? checkedInTeams.data[i][rankCol] : "0")
            );
        }
        
        teamCount_out = checkedInTeams.y;
        deleteDataContainer2D(checkedInTeams); // CSV data is now copied or used, so free it

        // Sort the constructed teams
        if (teamCount_out > 0) {
            std::sort(loadedTeams, loadedTeams + teamCount_out, compareTeams);
        }
        
        return loadedTeams; // Caller is responsible for destruction and deallocation
    }

        void displayProposedMatches(const Match matches[], int count) {
        if (count == 0) {
            std::cout << "No matches to display." << std::endl;
            return;
        }
        dataContainer2D displayData;
        displayData.x = 5; // Index, MatchID, Team1, Team2, DateTime
        displayData.fields = new char*[displayData.x];
        displayData.fields[0] = duplicateString("No.");
        displayData.fields[1] = duplicateString("Match ID");
        displayData.fields[2] = duplicateString("Team 1 ID");
        displayData.fields[3] = duplicateString("Team 2 ID");
        displayData.fields[4] = duplicateString("Scheduled Date & Time");

        displayData.y = count;
        displayData.data = new char**[displayData.y];

        for (int i = 0; i < count; ++i) {
            displayData.data[i] = new char*[displayData.x];
            std::string indexStr = std::to_string(i + 1);
            std::string dateTimeStr = std::string(matches[i].scheduled_date ? matches[i].scheduled_date : "") + " " + std::string(matches[i].scheduled_time ? matches[i].scheduled_time : "");

            displayData.data[i][0] = duplicateString(indexStr.c_str());
            displayData.data[i][1] = duplicateString(matches[i].match_id ? matches[i].match_id : "N/A");
            displayData.data[i][2] = duplicateString(matches[i].team1_id ? matches[i].team1_id : "N/A");
            displayData.data[i][3] = duplicateString(matches[i].team2_id ? matches[i].team2_id : "N/A");
            displayData.data[i][4] = duplicateString(dateTimeStr.c_str());
        }
        displayTabulatedData(displayData);
        deleteDataContainer2D(displayData);
    }

    // Function to calculate ranking points for all teams based on match results
    void calculateAndUpdateTeamRankingPoints(Team* teams, int teamCount, const std::string& matchCsvFile) {
        if (!teams || teamCount == 0) {
            return;
        }

        // Initialize all team points to 0 before processing,
        // so teams not in matches or if matches file is bad, they have 0 points from this calculation.
        for (int i = 0; i < teamCount; ++i) {
            if (teams[i].team_id) { // Check if team_id is not null before trying to access ranking_point
                teams[i].ranking_point = 0;
            } else {
                // Handle null team_id if necessary, maybe skip or log error
            }
        }

        dataContainer2D matchData = getData(matchCsvFile); // From CsvToolkit
        if (matchData.error) {
            std::cerr << "Warning: Error loading match data from " << matchCsvFile << " for ranking calculation. Points remain 0." << std::endl;
            deleteDataContainer2D(matchData); // From CsvToolkit
            // Points already defaulted to 0 or their previous values if not reset above
            return;
        }
        if (matchData.y == 0) {
            std::cout << "Info: No match data found in " << matchCsvFile << ". Calculated points will be 0 for new calculations." << std::endl;
            // Points already defaulted to 0
            deleteDataContainer2D(matchData); // From CsvToolkit
            return;
        }

        int m_team1IdCol = -1, m_team2IdCol = -1, m_team1ScoreCol = -1, m_team2ScoreCol = -1, m_statusCol = -1;
        // These field names "team1_id", "team2_id" etc. MUST match the headers in your matches.csv
        for (int k = 0; k < matchData.x; ++k) {
            if (strcmp(matchData.fields[k], "team1_id") == 0) m_team1IdCol = k; // Or "Player1ID", "Team1ID" from your actual matches.csv
            else if (strcmp(matchData.fields[k], "team2_id") == 0) m_team2IdCol = k; // Or "Player2ID", "Team2ID"
            else if (strcmp(matchData.fields[k], "team1_score") == 0) m_team1ScoreCol = k; // Or "Player1Score"
            else if (strcmp(matchData.fields[k], "team2_score") == 0) m_team2ScoreCol = k; // Or "Player2Score"
            else if (strcmp(matchData.fields[k], "match_status") == 0) m_statusCol = k; // Or "Status"
        }

        if (m_team1IdCol == -1 || m_team2IdCol == -1 || m_team1ScoreCol == -1 || m_team2ScoreCol == -1 || m_statusCol == -1) {
            std::cerr << "Warning: Could not find all required columns (e.g., team1_id, team2_id, team1_score, team2_score, match_status) in "
                    << matchCsvFile << ". Column names are case-sensitive. Calculated points remain 0." << std::endl;
            // Points already defaulted to 0
            deleteDataContainer2D(matchData); // From CsvToolkit
            return;
        }

        for (int i = 0; i < teamCount; ++i) { // For each team in your `teams` array
            int calculated_points = 0;
            // Ensure team_id is valid, starts with 'T', and has digits following.
            if (!teams[i].team_id || teams[i].team_id[0] != 'T' || strlen(teams[i].team_id) <= 1) {
                teams[i].ranking_point = 0; // Default or preserve existing, based on desired logic
                continue;
            }
            const char* currentTeamNumericPart = teams[i].team_id + 1; // Skips 'T', e.g., "001" from "T001"

            for (int j = 0; j < matchData.y; ++j) { // For each match in the CSV
                if (matchData.data[j][m_statusCol] && strcmp(matchData.data[j][m_statusCol], "Completed") == 0) {
                    int score = 0;
                    bool participated = false;

                    // Check Team 1 in the match
                    const char* match_team1_id_str = matchData.data[j][m_team1IdCol];
                    // Expecting "TXXX" format in CSV, e.g., T001, T010
                    if (match_team1_id_str && match_team1_id_str[0] == 'T' && strlen(match_team1_id_str) > 1) {
                        const char* match_team1_numeric_part = match_team1_id_str + 1; // Skips 'T'
                        if (atoi(currentTeamNumericPart) == atoi(match_team1_numeric_part)) {
                            score = atoi(matchData.data[j][m_team1ScoreCol] ? matchData.data[j][m_team1ScoreCol] : "0");
                            participated = true;
                        }
                    }

                    // Check Team 2 in the match, if not already found as Team 1
                    if (!participated) {
                        const char* match_team2_id_str = matchData.data[j][m_team2IdCol];
                        if (match_team2_id_str && match_team2_id_str[0] == 'T' && strlen(match_team2_id_str) > 1) {
                            const char* match_team2_numeric_part = match_team2_id_str + 1; // Skips 'T'
                            if (atoi(currentTeamNumericPart) == atoi(match_team2_numeric_part)) {
                                score = atoi(matchData.data[j][m_team2ScoreCol] ? matchData.data[j][m_team2ScoreCol] : "0");
                                participated = true;
                            }
                        }
                    }

                    if (participated) 
                    {
                        calculated_points += (score * score);
                    }
                }
            }
            teams[i].ranking_point = calculated_points;
        }
        deleteDataContainer2D(matchData); // From CsvToolkit
    }


    // Modified displayTeamRankings function
    void displayTeamRankings() {
        clearTerminal();
        std::cout << "--- Team Rankings (Calculated from Completed Matches) ---" << std::endl;
        int teamCount = 0;
        Team* teams = loadTeamsFromCSV(teamCount); // Loads teams with initial data

        if (teamCount == 0 || !teams) {
            std::cout << "No teams found or error loading base team data." << std::endl;
        } else {
            // Calculate and update ranking points based on match results
            calculateAndUpdateTeamRankingPoints(teams, teamCount, MATCH_CSV);

            // Re-sort teams based on newly calculated points
            if (teamCount > 0) {
                std::sort(teams, teams + teamCount, compareTeams);
            }

            // --- Display Logic (same as before) ---
            dataContainer2D displayData;
            displayData.x = 5; // Rank, ID, Name, University, Calculated Points
            displayData.fields = new char*[displayData.x];
            displayData.fields[0] = duplicateString("Rank");
            displayData.fields[1] = duplicateString("TeamID");
            displayData.fields[2] = duplicateString("TeamName");
            displayData.fields[3] = duplicateString("University");
            displayData.fields[4] = duplicateString("Calc. Points"); // Changed header

            displayData.y = teamCount;
            displayData.data = new char**[displayData.y];
            for (int i = 0; i < teamCount; ++i) {
                displayData.data[i] = new char*[displayData.x];
                std::string rankStr = std::to_string(i + 1);
                std::string pointsStr = std::to_string(teams[i].ranking_point); // This is now the calculated point

                displayData.data[i][0] = duplicateString(rankStr.c_str());
                displayData.data[i][1] = duplicateString(teams[i].team_id ? teams[i].team_id : "N/A");
                displayData.data[i][2] = duplicateString(teams[i].team_name ? teams[i].team_name : "N/A");
                displayData.data[i][3] = duplicateString(teams[i].university ? teams[i].university : "N/A");
                displayData.data[i][4] = duplicateString(pointsStr.c_str());
            }
            displayTabulatedData(displayData);
            deleteDataContainer2D(displayData); 
        }

        // Correct cleanup for memory allocated by loadTeamsFromCSV
        if (teams) {
            for (int i = 0; i < teamCount; ++i) {
                teams[i].~Team(); // Explicitly call destructor
            }
            ::operator delete[](teams); // Deallocate the raw memory
        }
        
        getString("Press Enter to continue...");
    }

    void writeMatchToCSV(const Match& match) {
        const int NUM_MATCH_FIELDS = 12; // Corrected number of fields
        const char* matchCsvRow[NUM_MATCH_FIELDS];
        match.getAsCsvRow(matchCsvRow, NUM_MATCH_FIELDS);

        // Corrected headers array to match the 12-field structure
        const char* headers[] = {
            "match_id", "scheduled_date", "scheduled_time", 
            "actual_start_time", "actual_end_time", 
            "team1_id", "team2_id", "winner_team_id", 
            "team1_score", "team2_score", "match_status", "match_level"
        };
        ensureCsvHeader(MATCH_CSV, headers, NUM_MATCH_FIELDS);
        
        if (writeNewDataRow(MATCH_CSV, NUM_MATCH_FIELDS, matchCsvRow) != 0) {
            std::cerr << "Error writing match " << (match.match_id ? match.match_id : "UNKNOWN_ID") << " to " << MATCH_CSV << std::endl;
        }
    }

    // EsportsScheduler.hpp

// ... (Team, Match, TeamQueue, TeamStack, other functions like loadTeamsFromCSV, calculateAndUpdateTeamRankingPoints, writeMatchToCSV etc. remain as previously defined) ...

// --- Date and Time Validation Helpers ---
bool isValidDate(const char* dateStr) {
    if (!dateStr || strlen(dateStr) != 10) return false;
    if (dateStr[4] != '-' || dateStr[7] != '-') return false;
    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(dateStr[i])) return false;
    }
    // Basic parsing for year, month, day
    char year_s[5] = {0}, month_s[3] = {0}, day_s[3] = {0};
    strncpy(year_s, dateStr, 4);
    strncpy(month_s, dateStr + 5, 2);
    strncpy(day_s, dateStr + 8, 2);

    int year = atoi(year_s);
    int month = atoi(month_s);
    int day = atoi(day_s);

    if (year < 2024 || year > 2050) return false; // Sensible year range
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false; // Basic day check

    // More specific day checks
    if (month == 2) {
        bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (leap ? 29 : 28)) return false;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return false;
    }
    return true;
}

char* getDateFromUser(const char* prompt) {
    char* input_cstr;
    while (true) {
        input_cstr = getString(prompt); // Returns heap-allocated string
        if (isValidDate(input_cstr)) {
            return input_cstr; // Caller is responsible for delete[]
        } else {
            std::cout << "Invalid date format. Please use YYYY-MM-DD with valid values." << std::endl;
            delete[] input_cstr; // Clean up invalid input
        }
    }
}

bool isValidTime(const char* timeStr) {
    if (!timeStr || strlen(timeStr) != 5) return false;
    if (timeStr[2] != ':') return false;
    for (int i = 0; i < 5; ++i) {
        if (i == 2) continue;
        if (!isdigit(timeStr[i])) return false;
    }
    char hour_s[3] = {0}, min_s[3] = {0};
    strncpy(hour_s, timeStr, 2);
    strncpy(min_s, timeStr + 3, 2);

    int hour = atoi(hour_s);
    int minute = atoi(min_s);

    if (hour < 0 || hour > 23) return false;
    if (minute < 0 || minute > 59) return false;
    return true;
}

char* getTimeFromUser(const char* prompt) {
    char* input_cstr;
    while (true) {
        input_cstr = getString(prompt);
        if (isValidTime(input_cstr)) {
            return input_cstr; // Caller is responsible for delete[]
        } else {
            std::cout << "Invalid time format. Please use HH:MM with valid values." << std::endl;
            delete[] input_cstr;
        }
    }
}

    void generateInitialSchedule() {
        clearTerminal();
        std::cout << "--- Generate Single Initial Match (based on calculated rankings) ---" << std::endl;
        
        int totalTeamCount = 0;
        Team* allTeams = loadTeamsFromCSV(totalTeamCount); // Sorted by original points initially

        if (!allTeams || totalTeamCount < 8) {
            std::cout << "Not enough teams for full initial bracket (need at least 8)." << std::endl;
            if (allTeams) {
                for (int i = 0; i < totalTeamCount; ++i) allTeams[i].~Team();
                ::operator delete[](allTeams);
            }
            getString("Press Enter to continue...");
            return;
        }

        // Calculate dynamic ranking points and re-sort
        calculateAndUpdateTeamRankingPoints(allTeams, totalTeamCount, MATCH_CSV);
        if (totalTeamCount > 0) { // Re-sort based on calculated points
            std::sort(allTeams, allTeams + totalTeamCount, compareTeams);
        }
        std::cout << "Teams ranked based on completed match performance." << std::endl;
        std::cout << "Top 8 teams will be used for initial UB/LB matches." << std::endl;
        getString("Press Enter to see available matches to schedule...");

        // Track which of the 4 standard initial matches have been scheduled in this session
        // Indices: 0 = UB R1vR4, 1 = UB R2vR3, 2 = LB R5vR8, 3 = LB R6vR7
        bool initialMatchesScheduled[4] = {false, false, false, false};
        const int NUM_INITIAL_SLOTS = 4;

        Team* teamForSlot[8]; // Pointers to the top 8 teams from allTeams
        for(int i=0; i<8; ++i) teamForSlot[i] = &allTeams[i];

        bool generating = true;
        while(generating) {
            clearTerminal();
            std::cout << "--- Select an Initial Match to Schedule ---" << std::endl;
            std::cout << "Using dynamically ranked teams (Top 8 for these slots):\n";
            for(int i=0; i<8 && i < totalTeamCount; ++i) {
                std::cout << "  Rank " << (i+1) << ": " << allTeams[i].team_id << " (" << allTeams[i].team_name << ")" << std::endl;
            }
            std::cout << "\nAvailable Matches to Create:" << std::endl;

            const char* matchOptions[NUM_INITIAL_SLOTS + 1];
            int currentOption = 0;

            if (!initialMatchesScheduled[0]) 
                matchOptions[currentOption++] = "UB Match 1 (Rank 1 vs Rank 4)";
            else 
                matchOptions[currentOption++] = "UB Match 1 (Rank 1 vs Rank 4) - SCHEDULED";
            
            if (!initialMatchesScheduled[1])
                matchOptions[currentOption++] = "UB Match 2 (Rank 2 vs Rank 3)";
            else
                matchOptions[currentOption++] = "UB Match 2 (Rank 2 vs Rank 3) - SCHEDULED";

            if (!initialMatchesScheduled[2])
                matchOptions[currentOption++] = "LB Match 1 (Rank 5 vs Rank 8)";
            else
                matchOptions[currentOption++] = "LB Match 1 (Rank 5 vs Rank 8) - SCHEDULED";
                
            if (!initialMatchesScheduled[3])
                matchOptions[currentOption++] = "LB Match 2 (Rank 6 vs Rank 7)";
            else
                matchOptions[currentOption++] = "LB Match 2 (Rank 6 vs Rank 7) - SCHEDULED";
            
            matchOptions[currentOption++] = "Done Scheduling Initial Matches";

            int choice = displayMenu("Initial Match Scheduler", matchOptions, currentOption);

            Team *teamA = nullptr, *teamB = nullptr;
            const char* matchLevelStr = nullptr;
            const char* bracketPosBase = nullptr;
            int scheduleSlotIndex = -1; // To mark which of initialMatchesScheduled to set

            // Determine teams based on choice, only if not already scheduled
            if (choice == 1) { // UB Rank 1 vs Rank 4
                if (!initialMatchesScheduled[0]) {
                    teamA = teamForSlot[0]; teamB = teamForSlot[3];
                    matchLevelStr = "Upper Bracket R1"; bracketPosBase = "UB-R1-M1"; scheduleSlotIndex = 0;
                }
            } else if (choice == 2) { // UB Rank 2 vs Rank 3
                if (!initialMatchesScheduled[1]) {
                    teamA = teamForSlot[1]; teamB = teamForSlot[2];
                    matchLevelStr = "Upper Bracket R1"; bracketPosBase = "UB-R1-M2"; scheduleSlotIndex = 1;
                }
            } else if (choice == 3) { // LB Rank 5 vs Rank 8
                if (!initialMatchesScheduled[2]) {
                    teamA = teamForSlot[4]; teamB = teamForSlot[7];
                    matchLevelStr = "Lower Bracket R1"; bracketPosBase = "LB-R1-M1"; scheduleSlotIndex = 2;
                }
            } else if (choice == 4) { // LB Rank 6 vs Rank 7
                if (!initialMatchesScheduled[3]) {
                    teamA = teamForSlot[5]; teamB = teamForSlot[6];
                    matchLevelStr = "Lower Bracket R1"; bracketPosBase = "LB-R1-M2"; scheduleSlotIndex = 3;
                }
            } else if (choice == 5) { // Done
                generating = false;
                continue;
            }

                if (teamA && teamB && matchLevelStr) 
                {
                    std::cout << "\nScheduling: " << teamA->team_name << " vs " << teamB->team_name << " for " << matchLevelStr << std::endl;
                    
                    char* newDate = getDateFromUser("Enter Scheduled Date (YYYY-MM-DD): ");
                    char* newTime = getTimeFromUser("Enter Scheduled Time (HH:MM): ");
                    
                    char matchIdBuffer[20];
                    sprintf(matchIdBuffer, "MATCH%03d", getNextMatchCounterFileBased());

                    Match newMatch(matchIdBuffer, newDate, newTime, teamA->team_id, teamB->team_id, "Scheduled", matchLevelStr);

                    clearTerminal();
                    std::cout << "--- Proposed New Match ---" << std::endl;
                    // Display single match (can adapt displayProposedMatches or inline)
                    std::cout << "Match ID: " << newMatch.match_id << std::endl;

                    std::cout << "Team 1: " << newMatch.team1_id << " (" << teamA->team_name << ")" << std::endl;
                    std::cout << "Team 2: " << newMatch.team2_id << " (" << teamB->team_name << ")" << std::endl;
                    std::cout << "Date: " << newMatch.scheduled_date << std::endl;
                    std::cout << "Time: " << newMatch.scheduled_time << std::endl;

                    char* confirm_cstr = getString("Save this match? (yes/no): ");
                    std::string confirm_str(confirm_cstr ? confirm_cstr : "");
                    delete[] confirm_cstr;
                    for(char &c : confirm_str) c = std::tolower(c);

                                if (confirm_str == "yes" || confirm_str == "y") {
                    writeMatchToCSV(newMatch);

                    // --- Correctly handle headers for tournament_bracket.csv ---
                    // Define the specific headers expected for TOURNAMENT_BRACKET_CSV
                    const char* bracket_csv_headers[] = {"team_id", "position", "bracket"};
                   ensureCsvHeader(TOURNAMENT_BRACKET_CSV, bracket_csv_headers, 3);
                    
                    // Prepare the data rows for teamA and teamB to write into tournament_bracket.csv
                    // These strings for position and bracket type are dynamically created and need cleanup
                    char* teamA_position_dynamic = duplicateString((std::string(bracketPosBase) + "-S1").c_str());
                    char* teamA_bracket_dynamic = duplicateString(strstr(matchLevelStr,"Upper") ? "upper_bracket" : "lower_bracket");
                    const char* teamA_pos_for_csv[3] = {teamA->team_id, teamA_position_dynamic, teamA_bracket_dynamic};
                   writeNewDataRow(TOURNAMENT_BRACKET_CSV, 3, teamA_pos_for_csv);

                    char* teamB_position_dynamic = duplicateString((std::string(bracketPosBase) + "-S2").c_str());
                    char* teamB_bracket_dynamic = duplicateString(strstr(matchLevelStr,"Upper") ? "upper_bracket" : "lower_bracket");
                    const char* teamB_pos_for_csv[3] = {teamB->team_id, teamB_position_dynamic, teamB_bracket_dynamic};
                   writeNewDataRow(TOURNAMENT_BRACKET_CSV, 3, teamB_pos_for_csv);

                    // Clean up the dynamically allocated strings for this row
                    delete[] teamA_position_dynamic;
                    delete[] teamA_bracket_dynamic;
                    delete[] teamB_position_dynamic;
                    delete[] teamB_bracket_dynamic;
                    //--- End of tournament_bracket.csv update ---

                    initialMatchesScheduled[scheduleSlotIndex] = true;
                    std::cout << "Match saved. Bracket positions for " << teamA->team_id << " and " << teamB->team_id 
                            << " noted in " << TOURNAMENT_BRACKET_CSV << std::endl;
                } else {
                    std::cout << "Match not saved." << std::endl;
                }
                // Clean up date/time strings obtained from user input, as Match constructor duplicates them.
                delete[] newDate;
                delete[] newTime;

            } else if (choice != 5 && choice >=1 && choice <=currentOption-1) { // User chose an option that was already scheduled
               displaySystemMessage("This match slot has already been scheduled in this session.", 2);
            } else if (choice != currentOption ) { // Invalid choice from displayMenu, currentOption is "Done"
               displaySystemMessage("Invalid selection.", 2);
            }
        }

        // Cleanup allTeams
        if (allTeams) {
            for (int i = 0; i < totalTeamCount; ++i) allTeams[i].~Team();
            ::operator delete[](allTeams);
        }
        std::cout << "Finished generating initial matches." << std::endl;
        getString("Press Enter to continue...");
    }

    // Helper function to find a team name given a team ID
    // Returns a new dynamically allocated string for the name, or for "N/A"/"Unknown". Caller must delete it.
    char* findTeamName(const char* teamId, const dataContainer2D& teamDataContainer, 
                    int teamDataIdCol, int teamDataNameCol) {
        if (!teamId || teamId[0] == '\0' || teamDataContainer.error || teamDataIdCol == -1 || teamDataNameCol == -1) {
            return duplicateString("N/A");
        }
        for (int i = 0; i < teamDataContainer.y; ++i) {
            if (teamDataContainer.data[i][teamDataIdCol] && strcmp(teamDataContainer.data[i][teamDataIdCol], teamId) == 0) {
                return duplicateString(teamDataContainer.data[i][teamDataNameCol] ? teamDataContainer.data[i][teamDataNameCol] : "Unknown Name");
            }
        }
        return duplicateString("ID Not Found"); // Or "N/A" if preferred for non-matches
    }

    // Comparison function for sorting Match objects by date and time (ascending)
    bool compareMatchesByDateTime(const Match& a, const Match& b) {
        // Ensure date and time strings are not null for comparison; use empty string if null.
        const char* date_a_str = a.scheduled_date ? a.scheduled_date : "";
        const char* date_b_str = b.scheduled_date ? b.scheduled_date : "";
        const char* time_a_str = a.scheduled_time ? a.scheduled_time : "";
        const char* time_b_str = b.scheduled_time ? b.scheduled_time : "";

        int date_cmp = strcmp(date_a_str, date_b_str);
        if (date_cmp != 0) {
            return date_cmp < 0; // Ascending date
        }
        // Dates are equal (or both empty/null), compare times
        return strcmp(time_a_str, time_b_str) < 0; // Ascending time
    }

    void displayMatchSchedule() {
        clearTerminal();
        std::cout << "--- Current Match Schedule (Sorted by Date/Time) ---" << std::endl;
        
        dataContainer2D originalMatchData = getData(MATCH_CSV);
        dataContainer2D teamData = getData(TEAMS_CSV); // TEAMS_CSV is "team.csv"

        if (originalMatchData.error || originalMatchData.y == 0) {
            std::cout << "No match schedule found or error loading match data." << std::endl;
        } else {
            // Find column indices in originalMatchData for all 12 fields
            int m_idCol = -1, m_schedDateCol = -1, m_schedTimeCol = -1, 
                m_actualStartCol = -1, m_actualEndCol = -1,
                m_team1IdCol = -1, m_team2IdCol = -1, m_winnerIdCol = -1,
                m_t1ScoreCol = -1, m_t2ScoreCol = -1, m_statusCol = -1, m_levelCol = -1;

            if (originalMatchData.fields && originalMatchData.x > 0) {
                for(int k=0; k < originalMatchData.x; ++k) {
                    if(!originalMatchData.fields[k]) continue; // Safety check for null field names
                    if(strcmp(originalMatchData.fields[k], "match_id") == 0) m_idCol = k;
                    else if(strcmp(originalMatchData.fields[k], "scheduled_date") == 0) m_schedDateCol = k;
                    else if(strcmp(originalMatchData.fields[k], "scheduled_time") == 0) m_schedTimeCol = k;
                    else if(strcmp(originalMatchData.fields[k], "actual_start_time") == 0) m_actualStartCol = k;
                    else if(strcmp(originalMatchData.fields[k], "actual_end_time") == 0) m_actualEndCol = k;
                    else if(strcmp(originalMatchData.fields[k], "team1_id") == 0) m_team1IdCol = k;
                    else if(strcmp(originalMatchData.fields[k], "team2_id") == 0) m_team2IdCol = k;
                    else if(strcmp(originalMatchData.fields[k], "winner_team_id") == 0) m_winnerIdCol = k;
                    else if(strcmp(originalMatchData.fields[k], "team1_score") == 0) m_t1ScoreCol = k;
                    else if(strcmp(originalMatchData.fields[k], "team2_score") == 0) m_t2ScoreCol = k;
                    else if(strcmp(originalMatchData.fields[k], "match_status") == 0) m_statusCol = k;
                    else if(strcmp(originalMatchData.fields[k], "match_level") == 0) m_levelCol = k;
                }
            }
            
            bool essentialColsFound = m_idCol != -1 && m_schedDateCol != -1 && m_schedTimeCol != -1 && 
                                    m_actualStartCol != -1 && m_actualEndCol != -1 &&
                                    m_team1IdCol != -1 && m_team2IdCol != -1 && m_winnerIdCol != -1 &&
                                    m_t1ScoreCol != -1 && m_t2ScoreCol != -1 && m_statusCol != -1 && m_levelCol != -1;

            if (!essentialColsFound) {
                std::cerr << "Error: Not all required columns found in " << MATCH_CSV << ". Displaying raw data if possible." << std::endl;
                displayTabulatedData(originalMatchData);
            } else {
                // Convert originalMatchData to an array of Match objects
                // Allocate raw memory for Match objects
                size_t numMatches = originalMatchData.y > 0 ? static_cast<size_t>(originalMatchData.y) : 1;
                void* rawMatchMemory = ::operator new[](numMatches * sizeof(Match));
                Match* matchesArray = static_cast<Match*>(rawMatchMemory);

                for (int i = 0; i < originalMatchData.y; ++i) {
                    // Use placement new to construct Match object using the main constructor
                    new (&matchesArray[i]) Match( 
                        originalMatchData.data[i][m_idCol],
                        originalMatchData.data[i][m_schedDateCol],
                        originalMatchData.data[i][m_schedTimeCol],
                        originalMatchData.data[i][m_team1IdCol],
                        originalMatchData.data[i][m_team2IdCol],
                        originalMatchData.data[i][m_statusCol],
                        originalMatchData.data[i][m_levelCol]
                    );
                    // Manually populate remaining fields not set by that specific constructor
                    delete[] matchesArray[i].actual_start_time; // Free string from Match default part of constructor
                    matchesArray[i].actual_start_time = duplicateString(originalMatchData.data[i][m_actualStartCol] ? originalMatchData.data[i][m_actualStartCol] : "");
                    delete[] matchesArray[i].actual_end_time;
                    matchesArray[i].actual_end_time   = duplicateString(originalMatchData.data[i][m_actualEndCol] ? originalMatchData.data[i][m_actualEndCol] : "");
                    delete[] matchesArray[i].winner_team_id;
                    matchesArray[i].winner_team_id    = duplicateString(originalMatchData.data[i][m_winnerIdCol] ? originalMatchData.data[i][m_winnerIdCol] : "");
                    matchesArray[i].team1_score       = atoi(originalMatchData.data[i][m_t1ScoreCol] ? originalMatchData.data[i][m_t1ScoreCol] : "0");
                    matchesArray[i].team2_score       = atoi(originalMatchData.data[i][m_t2ScoreCol] ? originalMatchData.data[i][m_t2ScoreCol] : "0");
                }

                // Sort the array of Match objects
                if (originalMatchData.y > 0) {
                    std::sort(matchesArray, matchesArray + originalMatchData.y, compareMatchesByDateTime);
                }

                // Create augmented displayData from the sorted matchesArray
                dataContainer2D displayData;
                displayData.x = originalMatchData.x; 
                displayData.y = originalMatchData.y;
                displayData.error = originalMatchData.error; 
                
                displayData.fields = new char*[displayData.x > 0 ? displayData.x : 1];
                for(int k=0; k < displayData.x; ++k) { // Copy original headers
                    displayData.fields[k] = duplicateString(originalMatchData.fields[k]);
                }
                
                displayData.data = new char**[displayData.y > 0 ? displayData.y : 1];

                int t_teamIdCol = -1, t_teamNameCol = -1;
                bool canDisplayNames = false;
                if (!teamData.error && teamData.fields && teamData.x > 0) {
                    for(int k=0; k < teamData.x; ++k) {
                        if(strcmp(teamData.fields[k], "TeamID") == 0) t_teamIdCol = k;
                        else if(strcmp(teamData.fields[k], "TeamName") == 0) t_teamNameCol = k;
                    }
                    if (t_teamIdCol != -1 && t_teamNameCol != -1) canDisplayNames = true;
                    else std::cerr << "Warning: TeamID or TeamName column not found in " << TEAMS_CSV << ". Names may not be shown correctly." << std::endl;
                } else std::cerr << "Warning: Could not load " << TEAMS_CSV << " or bad format. Names may not be shown correctly." << std::endl;

                char combinedBuffer[256]; 

                for(int i=0; i < displayData.y; ++i) {
                    displayData.data[i] = new char*[displayData.x > 0 ? displayData.x : 1];
                    const Match& currentMatch = matchesArray[i];

                    // Populate displayData row from currentMatch, augmenting team IDs
                    displayData.data[i][m_idCol] = duplicateString(currentMatch.match_id);
                    displayData.data[i][m_schedDateCol] = duplicateString(currentMatch.scheduled_date);
                    displayData.data[i][m_schedTimeCol] = duplicateString(currentMatch.scheduled_time);
                    displayData.data[i][m_actualStartCol] = duplicateString(currentMatch.actual_start_time);
                    displayData.data[i][m_actualEndCol] = duplicateString(currentMatch.actual_end_time);
                    
                    if (canDisplayNames) {
                        char* name1 = findTeamName(currentMatch.team1_id, teamData, t_teamIdCol, t_teamNameCol);
                        snprintf(combinedBuffer, sizeof(combinedBuffer), "%s (%s)", currentMatch.team1_id ? currentMatch.team1_id : "N/A", name1);
                        displayData.data[i][m_team1IdCol] = duplicateString(combinedBuffer);
                        delete[] name1;

                        char* name2 = findTeamName(currentMatch.team2_id, teamData, t_teamIdCol, t_teamNameCol);
                        snprintf(combinedBuffer, sizeof(combinedBuffer), "%s (%s)", currentMatch.team2_id ? currentMatch.team2_id : "N/A", name2);
                        displayData.data[i][m_team2IdCol] = duplicateString(combinedBuffer);
                        delete[] name2;

                        if (currentMatch.winner_team_id && strlen(currentMatch.winner_team_id) > 0) {
                            char* winner_name = findTeamName(currentMatch.winner_team_id, teamData, t_teamIdCol, t_teamNameCol);
                            snprintf(combinedBuffer, sizeof(combinedBuffer), "%s (%s)", currentMatch.winner_team_id, winner_name);
                            displayData.data[i][m_winnerIdCol] = duplicateString(combinedBuffer);
                            delete[] winner_name;
                        } else {
                            displayData.data[i][m_winnerIdCol] = duplicateString(""); // Empty if no winner
                        }
                    } else { // Fallback if names can't be displayed
                        displayData.data[i][m_team1IdCol] = duplicateString(currentMatch.team1_id ? currentMatch.team1_id : "");
                        displayData.data[i][m_team2IdCol] = duplicateString(currentMatch.team2_id ? currentMatch.team2_id : "");
                        displayData.data[i][m_winnerIdCol] = duplicateString(currentMatch.winner_team_id ? currentMatch.winner_team_id : "");
                    }
                    
                    std::string s1_str = std::to_string(currentMatch.team1_score);
                    std::string s2_str = std::to_string(currentMatch.team2_score);
                    displayData.data[i][m_t1ScoreCol] = duplicateString(s1_str.c_str());
                    displayData.data[i][m_t2ScoreCol] = duplicateString(s2_str.c_str());
                    displayData.data[i][m_statusCol] = duplicateString(currentMatch.match_status);
                    displayData.data[i][m_levelCol] = duplicateString(currentMatch.match_level);
                }
                
                displayTabulatedData(displayData);
                deleteDataContainer2D(displayData);

                // Cleanup matchesArray
                if (matchesArray && originalMatchData.y > 0) {
                    for(int i=0; i < originalMatchData.y; ++i) {
                        matchesArray[i].~Match(); 
                    }
                    ::operator delete[](rawMatchMemory); 
                } else if (rawMatchMemory) { // if y was 0 but memory was allocated
                    ::operator delete[](rawMatchMemory);
                }
            } // end if essentialColsFound
        } // end if originalMatchData is valid
        
        deleteDataContainer2D(originalMatchData);
        deleteDataContainer2D(teamData);
        getString("Press Enter to continue...");
    }

    void displayBracketProgress() {
        clearTerminal();
        std::cout << "--- Bracket Progress (Sorted by Bracket/Placing) ---" << std::endl; // Updated title
        
        dataContainer2D bracketInfo = getData(TOURNAMENT_BRACKET_CSV);

        if (!bracketInfo.error && bracketInfo.y > 0) {
            int teamIdCol = -1, posCol = -1, bracketCol = -1;
            if(bracketInfo.fields && bracketInfo.x > 0) { 
                for (int k = 0; k < bracketInfo.x; ++k) {
                    if (strcmp(bracketInfo.fields[k], "team_id") == 0) teamIdCol = k;
                    else if (strcmp(bracketInfo.fields[k], "position") == 0) posCol = k;
                    else if (strcmp(bracketInfo.fields[k], "bracket") == 0) bracketCol = k;
                }
            }

            if (teamIdCol == -1 || posCol == -1 || bracketCol == -1) {
                std::cerr << "Error: Required columns ('team_id', 'position', 'bracket') not found in " << TOURNAMENT_BRACKET_CSV << "." << std::endl;
                std::cout << "Displaying data unsorted." << std::endl;
                displayTabulatedData(bracketInfo);
            } else {
                BracketEntry* entries = new BracketEntry[bracketInfo.y > 0 ? bracketInfo.y : 1];
                for (int i = 0; i < bracketInfo.y; ++i) {
                    entries[i] = BracketEntry(bracketInfo.data[i], bracketInfo.x, teamIdCol, posCol, bracketCol);
                }

                std::sort(entries, entries + bracketInfo.y, compareBracketEntries);

                dataContainer2D sortedDisplayData;
                sortedDisplayData.x = 3; // We will display these 3 columns
                sortedDisplayData.fields = new char*[sortedDisplayData.x];
                sortedDisplayData.fields[0] = duplicateString("team_id");
                sortedDisplayData.fields[1] = duplicateString("position");
                sortedDisplayData.fields[2] = duplicateString("bracket");
                
                sortedDisplayData.y = bracketInfo.y;
                sortedDisplayData.data = new char**[sortedDisplayData.y > 0 ? sortedDisplayData.y : 1];
                for (int i = 0; i < sortedDisplayData.y; ++i) {
                    sortedDisplayData.data[i] = new char*[sortedDisplayData.x];
                    sortedDisplayData.data[i][0] = duplicateString(entries[i].team_id);
                    sortedDisplayData.data[i][1] = duplicateString(entries[i].position);
                    sortedDisplayData.data[i][2] = duplicateString(entries[i].bracket_status);
                }
                
                displayTabulatedData(sortedDisplayData);
                deleteDataContainer2D(sortedDisplayData);

                delete[] entries; 
            }
        } else {
            std::cout << "No bracket position data found or error loading from " << TOURNAMENT_BRACKET_CSV << "." << std::endl;
        }
        deleteDataContainer2D(bracketInfo);
        getString("Press Enter to continue...");
    }
    
    // Modified handleTeamProgression
    void handleTeamProgression(const Match& updatedMatch) { // Removed queue/stack params for now
        std::cout << "Processing progression for Match ID: " << updatedMatch.match_id << std::endl;
        if (!updatedMatch.winner_team_id || strlen(updatedMatch.winner_team_id) == 0 || 
            strcmp(updatedMatch.winner_team_id, "DRAW") == 0 || // Handle draws if necessary
            strcmp(updatedMatch.match_status, "Completed") != 0) {
            std::cout << "No clear winner or match not completed. Progression not processed." << std::endl;
            return;
        }

        const char* winnerId = updatedMatch.winner_team_id;
        const char* loserId = nullptr;

        if (strcmp(updatedMatch.team1_id, winnerId) == 0) {
            loserId = updatedMatch.team2_id;
        } else if (strcmp(updatedMatch.team2_id, winnerId) == 0) {
            loserId = updatedMatch.team1_id;
        } else {
            std::cerr << "Error: Winner ID does not match either team in match " << updatedMatch.match_id << std::endl;
            return;
        }

        // Helper to update team status in tournament_bracket.csv
        auto updateTeamStatusInBracketFile = [&](const char* teamId, const char* newPosition, const char* newBracketStatus) {
            dataContainer2D bracketData = getData(TOURNAMENT_BRACKET_CSV);
            bool updated = false;
            if (!bracketData.error && bracketData.y > 0) {
                int teamIdCol = -1, posCol = -1, bracketCol = -1;
                for(int k=0; k<bracketData.x; ++k) {
                    if(strcmp(bracketData.fields[k], "team_id")==0) teamIdCol=k;
                    else if(strcmp(bracketData.fields[k], "position")==0) posCol=k;
                    else if(strcmp(bracketData.fields[k], "bracket")==0) bracketCol=k;
                }

                if (teamIdCol != -1 && posCol != -1 && bracketCol != -1) {
                    for (int i = 0; i < bracketData.y; ++i) {
                        if (bracketData.data[i][teamIdCol] && strcmp(bracketData.data[i][teamIdCol], teamId) == 0) {
                            delete[] bracketData.data[i][posCol];
                            bracketData.data[i][posCol] = duplicateString(newPosition);
                            delete[] bracketData.data[i][bracketCol];
                            bracketData.data[i][bracketCol] = duplicateString(newBracketStatus);
                            updated = true;
                            break;
                        }
                    }
                    if (updated) {
                        writeData(TOURNAMENT_BRACKET_CSV, bracketData);
                        std::cout << "Team " << teamId << " status updated in " << TOURNAMENT_BRACKET_CSV << " to position: " << newPosition << ", bracket: " << newBracketStatus << std::endl;
                    } else {
                        std::cout << "Warning: Team " << teamId << " not found in " << TOURNAMENT_BRACKET_CSV << " to update status." << std::endl;
                    }
                } else {
                    std::cerr << "Warning: Required columns missing in " << TOURNAMENT_BRACKET_CSV << " for status update." << std::endl;
                }
            } else {
                std::cerr << "Warning: Could not load or empty " << TOURNAMENT_BRACKET_CSV << " for status update." << std::endl;
            }
            deleteDataContainer2D(bracketData);
        };

        const char* matchLevel = updatedMatch.match_level ? updatedMatch.match_level : "";

        if (strstr(matchLevel, "Upper Bracket Final") != nullptr) { 
            updateTeamStatusInBracketFile(winnerId, "UB-Final-Winner", "upper_bracket"); // Advances to Grand Final as UB Rep
            updateTeamStatusInBracketFile(loserId, "To LB Final", "lower_bracket"); // Drops to LB Final (or a match to decide LB rep)
        }
        else if (strstr(matchLevel, "Upper Bracket Semi-Final") != nullptr) { 
            updateTeamStatusInBracketFile(winnerId, "UB-To-Final", "upper_bracket"); // Winner advances to UB Final
            updateTeamStatusInBracketFile(loserId, "LB-FromUB-Semi", "lower_bracket"); // Loser drops to a later LB stage
        }
        else if (strstr(matchLevel, "Upper Bracket R1") != nullptr) { // Initial UB round
            updateTeamStatusInBracketFile(winnerId, "UB-R1-Winner", "upper_bracket"); 
            updateTeamStatusInBracketFile(loserId, "LB-FromUB-R1", "lower_bracket"); 
        }
        else if (strstr(matchLevel, "Upper Bracket") != nullptr) { // Other generic UB rounds (less specific than above)
            updateTeamStatusInBracketFile(winnerId, "UB-Advanced", "upper_bracket");
            updateTeamStatusInBracketFile(loserId, "LB-FromUB-Generic", "lower_bracket"); 
        }
        // --- Lower Bracket Progression ---
        else if (strstr(matchLevel, "Lower Bracket Final") != nullptr) {
            updateTeamStatusInBracketFile(winnerId, "LB-Final-Winner", "lower_bracket"); // Advances to Grand Final as LB Rep
            updateTeamStatusInBracketFile(loserId, "2nd Runner Up", "finished_placing"); 
        }
        else if (strstr(matchLevel, "Lower Bracket Semi-Final") != nullptr) { // YOUR REQUESTED CHANGE
            updateTeamStatusInBracketFile(winnerId, "LB-Advanced-To-Final", "lower_bracket"); // Winner advances to LB Final
            updateTeamStatusInBracketFile(loserId, "3rd Runner Up", "finished_placing");    // Loser is 3rd Runner Up
        }
        else if (strstr(matchLevel, "Lower Bracket R1") != nullptr) { // Initial LB round
            updateTeamStatusInBracketFile(winnerId, "LB-R1-Winner", "lower_bracket"); 
            updateTeamStatusInBracketFile(loserId, "Eliminated-LB-R1", "eliminated");
        }
        else if (strstr(matchLevel, "Lower Bracket") != nullptr) { // Other generic LB rounds (e.g., Round 2, Quarterfinals)
            updateTeamStatusInBracketFile(winnerId, "LB-Advanced", "lower_bracket");
            updateTeamStatusInBracketFile(loserId, "Eliminated", "eliminated"); 
        }
        // --- Fallback for unknown levels ---
        else {
               
            updateTeamStatusInBracketFile(winnerId, "Champion", "finished_placing");
            updateTeamStatusInBracketFile(loserId, "1st Runner Up", "finished_placing");
        }
    }


    void updateMatchResult(TeamStack& ubLosersStack, TeamQueue& nextRoundUBQueue, TeamQueue& nextRoundLBQueue) { // Pass by ref if they are modified
        clearTerminal();
        std::cout << "--- Update Match Result ---" << std::endl;
        displayMatchSchedule(); 

        char* matchIdToUpdate_cstr = getString("Enter Match ID to update (e.g., MATCH001): ");
        if (!matchIdToUpdate_cstr || strlen(matchIdToUpdate_cstr) == 0) {
            std::cout << "No Match ID entered." << std::endl;
            delete[] matchIdToUpdate_cstr;
            getString("Press Enter to continue...");
            return;
        }
        std::string matchIdToUpdate_str(matchIdToUpdate_cstr);
        delete[] matchIdToUpdate_cstr;

        dataContainer2D allMatches = getData(MATCH_CSV);
        if (allMatches.error) {
            std::cout << "Error loading matches from " << MATCH_CSV << std::endl;
            deleteDataContainer2D(allMatches);
            getString("Press Enter to continue...");
            return;
        }

        int matchRowIdx = -1;
        // Column indices for match.csv
        int idCol=-1, schedDateCol=-1, schedTimeCol=-1, 
            actualStartTimeCol=-1, actualEndTimeCol=-1, // New
            t1IdCol=-1, t2IdCol=-1, winnerCol=-1, 
            t1ScoreCol=-1, t2ScoreCol=-1, statusCol=-1, levelCol=-1;

        if (allMatches.x > 0 && allMatches.fields) {
            for(int i=0; i < allMatches.x; ++i) {
                if(strcmp(allMatches.fields[i], "match_id") == 0) idCol = i;
                else if(strcmp(allMatches.fields[i], "scheduled_date") == 0) schedDateCol = i;
                else if(strcmp(allMatches.fields[i], "scheduled_time") == 0) schedTimeCol = i;
                else if(strcmp(allMatches.fields[i], "actual_start_time") == 0) actualStartTimeCol = i; // Find index
                else if(strcmp(allMatches.fields[i], "actual_end_time") == 0) actualEndTimeCol = i;   // Find index
                else if(strcmp(allMatches.fields[i], "team1_id") == 0) t1IdCol = i;
                else if(strcmp(allMatches.fields[i], "team2_id") == 0) t2IdCol = i;
                else if(strcmp(allMatches.fields[i], "winner_team_id") == 0) winnerCol = i;
                else if(strcmp(allMatches.fields[i], "team1_score") == 0) t1ScoreCol = i;
                else if(strcmp(allMatches.fields[i], "team2_score") == 0) t2ScoreCol = i;
                else if(strcmp(allMatches.fields[i], "match_status") == 0) statusCol = i;
                else if(strcmp(allMatches.fields[i], "match_level") == 0) levelCol = i;
            }
        }

        // Check if all necessary columns were found
        if(idCol == -1 || schedDateCol == -1 || schedTimeCol == -1 || actualStartTimeCol == -1 || actualEndTimeCol == -1 || 
        t1IdCol == -1 || t2IdCol == -1 || winnerCol == -1 || t1ScoreCol == -1 || t2ScoreCol == -1 || 
        statusCol == -1 || levelCol == -1){
            std::cerr << "Error: One or more critical columns not found in " << MATCH_CSV 
                    << ". Cannot update match." << std::endl;
            deleteDataContainer2D(allMatches); 
            getString("Press Enter..."); 
            return;
        }
        
        Match foundMatchForProgression; 
        bool matchDataFound = false;

        for (int i = 0; i < allMatches.y; ++i) {
            if (allMatches.data[i][idCol] && strcmp(allMatches.data[i][idCol], matchIdToUpdate_str.c_str()) == 0) {
                matchRowIdx = i;
                // Populate foundMatchForProgression with data needed by handleTeamProgression
                // Constructor: Match(mid, s_date, s_time, t1_id, t2_id, status, level)
                foundMatchForProgression = Match(
                    allMatches.data[i][idCol],
                    allMatches.data[i][schedDateCol],
                    allMatches.data[i][schedTimeCol],
                    allMatches.data[i][t1IdCol],
                    allMatches.data[i][t2IdCol],
                    allMatches.data[i][statusCol], // This will be updated to "Completed"
                    allMatches.data[i][levelCol]
                );
                // Copy existing actual times, they will be overwritten by user input if provided
                delete[] foundMatchForProgression.actual_start_time; // Delete default ""
                foundMatchForProgression.actual_start_time = duplicateString(allMatches.data[i][actualStartTimeCol] ? allMatches.data[i][actualStartTimeCol] : "");
                delete[] foundMatchForProgression.actual_end_time; // Delete default ""
                foundMatchForProgression.actual_end_time = duplicateString(allMatches.data[i][actualEndTimeCol] ? allMatches.data[i][actualEndTimeCol] : "");
                matchDataFound = true;
                break;
            }
        }

        if (matchRowIdx != -1) {
            std::cout << "\nUpdating Match ID: " << matchIdToUpdate_str 
                    << " (Level: " << (foundMatchForProgression.match_level ? foundMatchForProgression.match_level : "N/A") << ")" << std::endl;
            std::cout << "Team 1: " << (foundMatchForProgression.team1_id ? foundMatchForProgression.team1_id : "N/A") 
                    << " vs Team 2: " << (foundMatchForProgression.team2_id ? foundMatchForProgression.team2_id : "N/A") << std::endl;
            
            char* actual_start_time_input = getTimeFromUser("Enter Actual Start Time (HH:MM): ");
            char* actual_end_time_input = getTimeFromUser("Enter Actual End Time (HH:MM): ");
            int score1 = getInt("Enter Team 1 Score: ");
            int score2 = getInt("Enter Team 2 Score: ");
            char* winnerId_cstr = getString("Enter Winner Team ID: ");

            // Update dataContainer2D allMatches in memory
            std::string s1_str = std::to_string(score1);
            std::string s2_str = std::to_string(score2);

            delete[] allMatches.data[matchRowIdx][actualStartTimeCol]; // Delete old string
            allMatches.data[matchRowIdx][actualStartTimeCol] = duplicateString(actual_start_time_input);
            
            delete[] allMatches.data[matchRowIdx][actualEndTimeCol];   // Delete old string
            allMatches.data[matchRowIdx][actualEndTimeCol] = duplicateString(actual_end_time_input);

            delete[] allMatches.data[matchRowIdx][t1ScoreCol];
            allMatches.data[matchRowIdx][t1ScoreCol] = duplicateString(s1_str.c_str());
            delete[] allMatches.data[matchRowIdx][t2ScoreCol];
            allMatches.data[matchRowIdx][t2ScoreCol] = duplicateString(s2_str.c_str());
            delete[] allMatches.data[matchRowIdx][winnerCol];
            allMatches.data[matchRowIdx][winnerCol] = duplicateString(winnerId_cstr);
            delete[] allMatches.data[matchRowIdx][statusCol];
            allMatches.data[matchRowIdx][statusCol] = duplicateString("Completed");
            
            // Update foundMatchForProgression object for handleTeamProgression if it was found
            if (matchDataFound) {
                delete[] foundMatchForProgression.actual_start_time; // Delete previously copied/default value
                foundMatchForProgression.actual_start_time = duplicateString(actual_start_time_input);
                delete[] foundMatchForProgression.actual_end_time;   // Delete previously copied/default value
                foundMatchForProgression.actual_end_time = duplicateString(actual_end_time_input);
                foundMatchForProgression.team1_score = score1; 
                foundMatchForProgression.team2_score = score2;
                delete[] foundMatchForProgression.winner_team_id; 
                foundMatchForProgression.winner_team_id = duplicateString(winnerId_cstr);
                delete[] foundMatchForProgression.match_status;   
                foundMatchForProgression.match_status   = duplicateString("Completed");
            }

            if (writeData(MATCH_CSV, allMatches) == 0) {
                std::cout << "Match result (including times) updated successfully in " << MATCH_CSV << std::endl;
                if (matchDataFound) {
                    // Pass the updated foundMatchForProgression object
                    handleTeamProgression(foundMatchForProgression); 
                    generateAndSaveGameStats(foundMatchForProgression);
                }
            } else {
                std::cout << "Error writing updated match data to " << MATCH_CSV << std::endl;
            }
            
            // Clean up user inputs
            delete[] actual_start_time_input; 
            delete[] actual_end_time_input;
            delete[] winnerId_cstr;

        } else {
            std::cout << "Match ID '" << matchIdToUpdate_str << "' not found." << std::endl;
        }

        deleteDataContainer2D(allMatches);
        // foundMatchForProgression is a local object, its destructor will be called when it goes out of scope,
        // cleaning up its duplicated char* members.
        getString("Press Enter to continue...");
    }


// Helper to get a Team object by ID from the master list
Team getTeamDetails(const char* teamId, Team* masterList, int masterListCount) {
    if (teamId && masterList) {
        for (int i = 0; i < masterListCount; ++i) {
            if (masterList[i].team_id && strcmp(masterList[i].team_id, teamId) == 0) {
                return masterList[i]; // Returns a copy
            }
        }
    }
    return Team(); // Return empty/default team if not found
}

    // EsportsScheduler.hpp
// ... (inside namespace EsportsScheduler) ...

Team selectTeamForProgressionMatch(const char* prompt, const dataContainer2D& bracketStates, 
                                   const char* desiredBracketStatus,
                                   Team* allTeamsMasterList, int totalMasterTeamCount,
                                   const char* excludeTeamId1 = nullptr, 
                                   const char* excludeTeamId2 = nullptr) { // excludeTeamId2 is not used in current logic but kept for signature
    std::cout << prompt << std::endl;
    
    Team* eligibleTeams = new Team[bracketStates.y > 0 ? bracketStates.y : 1];
    int eligibleCount = 0;

    int teamIdCol_bs = -1, bracketCol_bs = -1, positionCol_bs = -1;
     if (bracketStates.x > 0 && bracketStates.fields) {
        for (int k = 0; k < bracketStates.x; ++k) {
            if (strcmp(bracketStates.fields[k], "team_id") == 0) teamIdCol_bs = k;
            else if (strcmp(bracketStates.fields[k], "bracket") == 0) bracketCol_bs = k;
            else if (strcmp(bracketStates.fields[k], "position") == 0) positionCol_bs = k;
        }
    }

    if (teamIdCol_bs == -1 || bracketCol_bs == -1 || positionCol_bs == -1) {
        std::cerr << "Error: Required columns (team_id, bracket, position) missing in tournament_bracket.csv for team selection." << std::endl;
        delete[] eligibleTeams;
        return Team();
    }

    std::cout << "Eligible teams in '" << desiredBracketStatus << "':" << std::endl;
    int displayIndex = 1;
    for (int i = 0; i < bracketStates.y; ++i) {
        const char* currentTeamId = bracketStates.data[i][teamIdCol_bs];
        const char* currentBracket = bracketStates.data[i][bracketCol_bs];
        const char* currentPosition = bracketStates.data[i][positionCol_bs];

        if (currentTeamId && currentBracket && strcmp(currentBracket, desiredBracketStatus) == 0) {
            if (currentPosition && (strstr(currentPosition, "Eliminated") != nullptr || strstr(currentPosition, "eliminated") != nullptr) ) {
                continue;
            }
            if (excludeTeamId1 && strcmp(currentTeamId, excludeTeamId1) == 0) continue;
            if (excludeTeamId2 && strcmp(currentTeamId, excludeTeamId2) == 0) continue;

            Team fullTeamDetails = getTeamDetails(currentTeamId, allTeamsMasterList, totalMasterTeamCount);
            if (fullTeamDetails.team_id) {
                 if (eligibleCount < (bracketStates.y > 0 ? bracketStates.y : 1) ) {
                    eligibleTeams[eligibleCount++] = fullTeamDetails;
                    std::cout << "  " << displayIndex++ << ". " << fullTeamDetails.team_id 
                              << " (" << (fullTeamDetails.team_name ? fullTeamDetails.team_name : "N/A") 
                              << ") - Pos: " << (currentPosition ? currentPosition : "N/A") << std::endl;
                }
            }
        }
    }

    Team selectedTeam; // Will hold the chosen team (as a copy)
    if (eligibleCount == 0) {
        std::cout << "No eligible teams found in '" << desiredBracketStatus << "' for selection." << std::endl;
    } else {
        bool validSelectionMade = false;
        while(!validSelectionMade) {
            char* choice_cstr = getString("Select team by number (or 0 to cancel selection): ");
            if (choice_cstr) {
                int choice_num = atoi(choice_cstr);
                if (choice_num >= 1 && choice_num <= eligibleCount) {
                    selectedTeam = eligibleTeams[choice_num - 1]; // Team assignment operator makes a deep copy
                    validSelectionMade = true;
                } else if (choice_num == 0) {
                    std::cout << "Selection cancelled." << std::endl;
                    // selectedTeam remains default/empty (team_id will be nullptr)
                    validSelectionMade = true; // Exit loop
                } else {
                    std::cout << "Invalid selection. Please pick a number between 1 and " << eligibleCount << ", or 0 to cancel." << std::endl;
                }
                delete[] choice_cstr;
            } else { // Should ideally not happen with getString
                std::cout << "Input error during selection." << std::endl;
                validSelectionMade = true; // Break loop to prevent infinite loop
            }
        }
    }
    
    delete[] eligibleTeams; // This calls destructors for all Team objects in the array
    return selectedTeam; 
}


   // This is a helper that can be called after a new match is scheduled.
    void updateTeamPositionInBracketFile(const char* teamId, const char* newMatchId, const char* newMatchLevel, bool isTeam1) {
        dataContainer2D bracketData = getData(TOURNAMENT_BRACKET_CSV);
        bool updated = false;
        if (!bracketData.error && bracketData.y > 0) {
            int teamIdCol = -1, posCol = -1;
            for(int k=0; k<bracketData.x; ++k) {
                if(strcmp(bracketData.fields[k], "team_id")==0) teamIdCol=k;
                else if(strcmp(bracketData.fields[k], "position")==0) posCol=k;
            }

            if (teamIdCol != -1 && posCol != -1) {
                for (int i = 0; i < bracketData.y; ++i) {
                    if (bracketData.data[i][teamIdCol] && strcmp(bracketData.data[i][teamIdCol], teamId) == 0) {
                        delete[] bracketData.data[i][posCol];
                        std::string positionStr = std::string(newMatchLevel) + " vs " + newMatchId + (isTeam1 ? "-P1" : "-P2");
                        bracketData.data[i][posCol] = duplicateString(positionStr.c_str());
                        updated = true;
                        break;
                    }
                }
                if (updated) {
                    writeData(TOURNAMENT_BRACKET_CSV, bracketData);
                    std::cout << "Team " << teamId << " position updated in " << TOURNAMENT_BRACKET_CSV << std::endl;
                } else {
                    std::cout << "Warning: Team " << teamId << " not found in " << TOURNAMENT_BRACKET_CSV << " to update position for new match." << std::endl;
                }
            } else {
                std::cerr << "Warning: Required columns (team_id, position) missing in " << TOURNAMENT_BRACKET_CSV << " for position update." << std::endl;
            }
        } else {
            std::cerr << "Warning: Could not load or empty " << TOURNAMENT_BRACKET_CSV << " for position update." << std::endl;
        }
        deleteDataContainer2D(bracketData);
    }

        Team* findTeamsByStatus(const dataContainer2D& bracketData, const char* statusNeedle, const char* statusColumnName, int& count_out, Team* allLoadedTeams, int totalLoadedTeamCount) {
        count_out = 0;
        if (bracketData.error || bracketData.y == 0) return nullptr;

        int teamIdCol = -1, statusCol = -1;
        for (int k = 0; k < bracketData.x; ++k) {
            if (strcmp(bracketData.fields[k], "team_id") == 0) teamIdCol = k;
            else if (strcmp(bracketData.fields[k], statusColumnName) == 0) statusCol = k; // "position" or "bracket"
        }
        if (teamIdCol == -1 || statusCol == -1) return nullptr;

        Team* foundTeamsArr = new Team[bracketData.y]; // Max possible
        int foundCount = 0;

        for (int i = 0; i < bracketData.y; ++i) {
            if (bracketData.data[i][statusCol] && strcmp(bracketData.data[i][statusCol], statusNeedle) == 0) {
                // Find the full Team object from allLoadedTeams to get name etc.
                for(int tdx = 0; tdx < totalLoadedTeamCount; ++tdx) {
                    if (allLoadedTeams[tdx].team_id && strcmp(allLoadedTeams[tdx].team_id, bracketData.data[i][teamIdCol]) == 0) {
                        if (foundCount < bracketData.y) { // Boundary check
                            new (&foundTeamsArr[foundCount++]) Team(allLoadedTeams[tdx]); // Copy construct
                        }
                        break;
                    }
                }
            }
        }
        count_out = foundCount;
        if (foundCount == 0) {
            delete[] foundTeamsArr;
            return nullptr;
        }
        // Trim array if needed (or just return with foundCount)
        // For simplicity, returning potentially oversized array with correct count_out
        return foundTeamsArr;
    }

    void scheduleProgressionMatches() {
        bool schedulingMoreOuter = true;

        int totalMasterTeamCount = 0;
        Team* allTeamsMasterList = loadTeamsFromCSV(totalMasterTeamCount);
        if (!allTeamsMasterList || totalMasterTeamCount == 0) {
            std::cout << "Cannot load base team data. Aborting progression scheduling." << std::endl;
            if(allTeamsMasterList) {
                for(int i=0; i<totalMasterTeamCount; ++i) allTeamsMasterList[i].~Team();
                ::operator delete[](allTeamsMasterList);
            }
            getString("Press Enter...");
            return;
        }

        while(schedulingMoreOuter) {
            clearTerminal();
            std::cout << "--- Schedule Progression Match ---" << std::endl;
            
            const char* menuOptions[] = {
                "Schedule Next Upper Bracket Match",
                "Schedule Next Lower Bracket Match",
                "Schedule Grand Final",
                "Done Scheduling Progression Matches"
            };
            int choice = displayMenu("Progression Match Type", menuOptions, 4);

            if (choice == 4) { // Done
                schedulingMoreOuter = false;
                continue;
            }

            dataContainer2D bracketStates = getData(TOURNAMENT_BRACKET_CSV);
            if (bracketStates.error || bracketStates.y == 0) {
                std::cout << "Cannot load tournament bracket states from " << TOURNAMENT_BRACKET_CSV << ". Cannot proceed." << std::endl;
                deleteDataContainer2D(bracketStates);
                getString("Press Enter...");
                continue; 
            }

            Team team1, team2; // To store selected teams
            const char* desiredBracketStatusTeam1 = nullptr;
            const char* desiredBracketStatusTeam2 = nullptr;
            bool isGrandFinal = false;
            char* defaultMatchLevel = nullptr; // For Grand Final

            if (choice == 1) { // Schedule Next Upper Bracket Match
                desiredBracketStatusTeam1 = "upper_bracket";
                desiredBracketStatusTeam2 = "upper_bracket";
                std::cout << "\n--- Scheduling Next Upper Bracket Match ---" << std::endl;
                team1 = selectTeamForProgressionMatch("Select Team 1 for Upper Bracket match:", bracketStates, desiredBracketStatusTeam1, allTeamsMasterList, totalMasterTeamCount);
                if (team1.team_id) {
                    team2 = selectTeamForProgressionMatch("Select Team 2 for Upper Bracket match:", bracketStates, desiredBracketStatusTeam2, allTeamsMasterList, totalMasterTeamCount, team1.team_id);
                }
            } else if (choice == 2) { // Schedule Next Lower Bracket Match
                desiredBracketStatusTeam1 = "lower_bracket";
                desiredBracketStatusTeam2 = "lower_bracket";
                std::cout << "\n--- Scheduling Next Lower Bracket Match ---" << std::endl;
                team1 = selectTeamForProgressionMatch("Select Team 1 for Lower Bracket match:", bracketStates, desiredBracketStatusTeam1, allTeamsMasterList, totalMasterTeamCount);
                if (team1.team_id) {
                    team2 = selectTeamForProgressionMatch("Select Team 2 for Lower Bracket match:", bracketStates, desiredBracketStatusTeam2, allTeamsMasterList, totalMasterTeamCount, team1.team_id);
                }
            } else if (choice == 3) { // Schedule Grand Final
                isGrandFinal = true;
                defaultMatchLevel = duplicateString("Grand Final");
                std::cout << "\n--- Scheduling Grand Final ---" << std::endl;
                
                desiredBracketStatusTeam1 = "upper_bracket"; // UB Finalist
                std::cout << "Select the Upper Bracket Finalist:" << std::endl;
                team1 = selectTeamForProgressionMatch("Select Upper Bracket Finalist:", bracketStates, desiredBracketStatusTeam1, allTeamsMasterList, totalMasterTeamCount);
                
                if (team1.team_id) {
                    desiredBracketStatusTeam2 = "lower_bracket"; // LB Finalist
                    std::cout << "\nSelect the Lower Bracket Finalist:" << std::endl;
                    // Pass team1.team_id as excludeTeamId1, though not strictly necessary if brackets are distinct
                    // and selectTeamForProgressionMatch filters by desiredBracketStatus.
                    team2 = selectTeamForProgressionMatch("Select Lower Bracket Finalist:", bracketStates, desiredBracketStatusTeam2, allTeamsMasterList, totalMasterTeamCount, team1.team_id);
                }
                
                if (!team1.team_id || !team2.team_id) {
                    std::cout << "Could not select both finalists for the Grand Final." << std::endl;
                } else {
                    std::cout << "Grand Finalists proposed:" << std::endl;
                    std::cout << "  From Upper Bracket: " << team1.team_id << " (" << (team1.team_name ? team1.team_name : "N/A") << ")" << std::endl;
                    std::cout << "  From Lower Bracket: " << team2.team_id << " (" << (team2.team_name ? team2.team_name : "N/A") << ")" << std::endl;
                }
            }
            
            deleteDataContainer2D(bracketStates); // Done with reading bracketStates for this selection phase

            if (team1.team_id && team2.team_id) { // Both teams selected/identified
                std::cout << "\nConfirming details for match between:" << std::endl;
                std::cout << "Team 1: " << (team1.team_id ? team1.team_id : "N/A") << " (" << (team1.team_name ? team1.team_name : "N/A") << ")" << std::endl;
                std::cout << "Team 2: " << (team2.team_id ? team2.team_id : "N/A") << " (" << (team2.team_name ? team2.team_name : "N/A") << ")" << std::endl;

                char* date_str = getDateFromUser("Enter Scheduled Date (YYYY-MM-DD): ");
                char* time_str = getTimeFromUser("Enter Scheduled Time (HH:MM): ");
                char* level_str_input;

                if (isGrandFinal) {
                    level_str_input = duplicateString(defaultMatchLevel ? defaultMatchLevel : "Grand Final"); // Use earlier duplicated string or default
                } else {
                    level_str_input = getString("Enter Match Level/Description (e.g., UB Semifinal, LB Round 3): ");
                }

                char matchIdBuffer[20];
                sprintf(matchIdBuffer, "MATCH%03d", getNextMatchCounterFileBased());
                Match newMatch(matchIdBuffer, date_str, time_str, team1.team_id, team2.team_id, "Scheduled", level_str_input);

                clearTerminal();
                std::cout << "--- Proposed New Match ---" << std::endl;
                std::cout << "Match ID: " << newMatch.match_id << std::endl;
                std::cout << "Level: " << newMatch.match_level << std::endl;
                std::cout << "Team 1: " << newMatch.team1_id << " (" << (team1.team_name ? team1.team_name : "N/A") << ")" << std::endl;
                std::cout << "Team 2: " << newMatch.team2_id << " (" << (team2.team_name ? team2.team_name : "N/A") << ")" << std::endl;
                std::cout << "Date: " << newMatch.scheduled_date << std::endl;
                std::cout << "Time: " << newMatch.scheduled_time << std::endl;

                char* confirmSave_cstr = getString("Save this match? (yes/no): ");
                std::string confirmSave_str(confirmSave_cstr ? confirmSave_cstr : "");
                delete[] confirmSave_cstr;

                if (confirmSave_str == "yes" || confirmSave_str == "y") {
                    writeMatchToCSV(newMatch);
                    // Update team positions in tournament_bracket.csv
                    // The updateTeamPositionInBracketFile should use the team's current bracket (upper/lower)
                    // and then set a new position string.
                    // For Grand Final, both teams are now "In Grand Final".
                    const char* team1_current_bracket = choice == 1 || (isGrandFinal && desiredBracketStatusTeam1 == "upper_bracket") ? "upper_bracket" : "lower_bracket";
                    const char* team2_current_bracket = choice == 2 || (isGrandFinal && desiredBracketStatusTeam2 == "lower_bracket") ? "lower_bracket" : "upper_bracket";
                    if (isGrandFinal) { // For GF, both effectively move to a "Finals" stage/position
                        team1_current_bracket = "finals_contender"; // Or some other descriptive bracket status
                        team2_current_bracket = "finals_contender";
                    }

                    updateTeamPositionInBracketFile(team1.team_id, newMatch.match_id, newMatch.match_level, true);
                    updateTeamPositionInBracketFile(team2.team_id, newMatch.match_id, newMatch.match_level, false);
                    // The updateTeamPositionInBracketFile might also need to update the 'bracket' column if it changes (e.g. for Grand Finalists)
                    std::cout << "Match " << newMatch.match_id << " saved." << std::endl;
                } else {
                    std::cout << "Match not saved." << std::endl;
                }
                delete[] date_str; 
                delete[] time_str; 
                delete[] level_str_input;
            } else if (choice != 4) { 
                std::cout << "Could not set up the match. One or both teams were not selected/identified." << std::endl;
            }
            
            delete[] defaultMatchLevel; // Was duplicated if GrandFinal, safe to delete if nullptr
            defaultMatchLevel = nullptr; 

            // Destructors for local Team copies team1 and team2 are called automatically when they go out of scope
            
            if (choice != 4) { // If not "Done"
                getString("Press Enter to continue scheduling or choose 'Done' from the menu...");
            }

        } // end while(schedulingMoreOuter)

        if (allTeamsMasterList) {
            for(int i=0; i<totalMasterTeamCount; ++i) allTeamsMasterList[i].~Team();
            ::operator delete[](allTeamsMasterList);
        }
        std::cout << "Exited progression match scheduling." << std::endl;
        getString("Press Enter to return to main scheduling menu...");
    }


    void deleteScheduledMatch() {
       clearTerminal();
        std::cout << "--- Delete a Scheduled Match ---" << std::endl;
        displayMatchSchedule(); // Show current matches to help user choose

        char* matchIdToDelete_cstr = getString("Enter Match ID to delete (or press Enter to skip): ");

        if (!matchIdToDelete_cstr || strlen(matchIdToDelete_cstr) == 0) {
            delete[] matchIdToDelete_cstr; // Still delete if it was allocated but empty
            std::cout << "\nNo Match ID entered. No schedule deleted." << std::endl;
        } else {
            std::string matchIdToDelete_str(matchIdToDelete_cstr);
            delete[] matchIdToDelete_cstr;

            // Confirm deletion
            // Print prompt separately, then call getString(nullptr) or getString("") if it handles that.
            std::cout << "Are you sure you want to delete Match ID '" << matchIdToDelete_str << "'? (yes/no): ";
            char* confirm_cstr =getString(nullptr); // Assuming getString(nullptr) waits for input without re-prompting
            std::string confirm_str;
            if (confirm_cstr) {
                confirm_str = confirm_cstr;
                delete[] confirm_cstr;
                for(char &c : confirm_str) c = std::tolower(c);
            }

            if (confirm_str == "yes" || confirm_str == "y") {
                // Find the column index of 'match_id' in match.csv to pass to deleteByKey
               dataContainer2D tempMatchData =getData(MATCH_CSV);
                int matchIdColIdx = -1;
                if (!tempMatchData.error && tempMatchData.fields && tempMatchData.x > 0) {
                    for (int k = 0; k < tempMatchData.x; ++k) {
                        if (tempMatchData.fields[k] && strcmp(tempMatchData.fields[k], "match_id") == 0) {
                            matchIdColIdx = k;
                            break;
                        }
                    }
                }
               deleteDataContainer2D(tempMatchData);

                if (matchIdColIdx == -1) {
                    std::cerr << "\nError: Could not determine 'match_id' column in " << MATCH_CSV 
                            << ". Deletion aborted." << std::endl;
                } else {
                    int result =deleteByKey(MATCH_CSV, matchIdToDelete_str.c_str(), matchIdColIdx);
                    if (result == 0) {
                        std::cout << "\nMatch ID '" << matchIdToDelete_str << "' deleted successfully." << std::endl;
                    } else if (result == 2) {
                        std::cout << "\nMatch ID '" << matchIdToDelete_str << "' not found. No schedule deleted." << std::endl;
                    } else { // result == 1 or other error
                        std::cout << "\nError occurred while trying to delete Match ID '" << matchIdToDelete_str << "'." << std::endl;
                    }
                }
            } else {
                std::cout << "\nDeletion cancelled for Match ID '" << matchIdToDelete_str << "'." << std::endl;
            }
        }
       getString("Press Enter to continue...");
    }

    void manageMatchSchedulingAndProgression() {
        // These queues/stacks would manage teams between rounds.
        // For simplicity in this example, their full usage in generating next-round matches is not detailed.
        // Their size should be appropriate for the number of teams.
        TeamQueue nextRoundUBQueue(8); 
        TeamQueue nextRoundLBQueue(8);
        TeamStack ubLosersStack(4);   

        bool running = true;
        while (running) {
            clearTerminal();
            const char* menuOptions[] = {
                "View Tournament",
                "Generate Initial Schedule",
                "Update Match Result",
                "Schedule Progression Matches",
                "Delete a Schedule",
                "Return to Main Menu"
            };
            int choice = displayMenu("Match Scheduling & Player Progression", menuOptions, 6); // Now 5 options

            switch (choice) {
                case 1: {
                    bool viewRunning = true;
                    while(viewRunning){
                        clearTerminal();
                        const char* viewOptions[] = {"View Team Ranking", "View Match Schedule", "View Bracket Progress", "Back"};
                        int viewChoice = displayMenu("View Tournament Information", viewOptions, 4);
                        switch(viewChoice){
                            case 1: displayTeamRankings(); break;
                            case 2: displayMatchSchedule(); break;
                            case 3: displayBracketProgress(); break;
                            case 4: viewRunning = false; break;
                            default: displaySystemMessage("Invalid choice.", 2); break;
                        }
                    }
                    break;
                }
                case 2: generateInitialSchedule(); break;
                case 3: updateMatchResult(ubLosersStack, nextRoundUBQueue, nextRoundLBQueue); break;
                case 4: scheduleProgressionMatches(); break;
                case 5: deleteScheduledMatch(); break;
                case 6: running = false; break;
                default: displaySystemMessage("Invalid choice.", 2); break;
            }
        }
    }

} 
#endif