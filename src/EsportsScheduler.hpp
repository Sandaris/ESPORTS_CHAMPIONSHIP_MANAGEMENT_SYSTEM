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
    const std::string TOURNAMENT_BRACKET_CSV = "tournament_bracket.csv";
    const int MAX_TEAMS_DEFAULT = 12; // Default capacity for dynamic arrays of teams
    const int MAX_MATCHES_DEFAULT = 200; // Default capacity for dynamic arrays of matches

    // --- Data Structures ---

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
            match_id = CsvToolkit::duplicateString(mid);
            scheduled_date = CsvToolkit::duplicateString(s_date);
            scheduled_time = CsvToolkit::duplicateString(s_time);
            // match_round_number = CsvToolkit::duplicateString(round); // REMOVE THIS LINE
            actual_start_time = CsvToolkit::duplicateString(""); // Init empty
            actual_end_time = CsvToolkit::duplicateString("");   // Init empty
            team1_id = CsvToolkit::duplicateString(t1_id);
            team2_id = CsvToolkit::duplicateString(t2_id);
            winner_team_id = CsvToolkit::duplicateString(""); // Init empty
            team1_score = 0;
            team2_score = 0;
            match_status = CsvToolkit::duplicateString(status);
            match_level = CsvToolkit::duplicateString(level);
        }
        
        // Copy Constructor (remove match_round_number)
        Match(const Match& other){
            match_id = CsvToolkit::duplicateString(other.match_id);
            scheduled_date = CsvToolkit::duplicateString(other.scheduled_date);
            scheduled_time = CsvToolkit::duplicateString(other.scheduled_time);
            // match_round_number removed
            actual_start_time = CsvToolkit::duplicateString(other.actual_start_time);
            actual_end_time = CsvToolkit::duplicateString(other.actual_end_time);
            team1_id = CsvToolkit::duplicateString(other.team1_id);
            team2_id = CsvToolkit::duplicateString(other.team2_id);
            winner_team_id = CsvToolkit::duplicateString(other.winner_team_id);
            team1_score = other.team1_score;
            team2_score = other.team2_score;
            match_status = CsvToolkit::duplicateString(other.match_status);
            match_level = CsvToolkit::duplicateString(other.match_level);
        }

        // Assignment Operator (remove match_round_number)
        Match& operator=(const Match& other) {
            if (this == &other) return *this;
            delete[] match_id; delete[] scheduled_date; delete[] scheduled_time; /* delete[] match_round_number; REMOVED */
            delete[] actual_start_time; delete[] actual_end_time; delete[] team1_id; delete[] team2_id;
            delete[] winner_team_id; delete[] match_status; delete[] match_level;

            match_id = CsvToolkit::duplicateString(other.match_id);
            scheduled_date = CsvToolkit::duplicateString(other.scheduled_date);
            scheduled_time = CsvToolkit::duplicateString(other.scheduled_time);
            // match_round_number removed
            actual_start_time = CsvToolkit::duplicateString(other.actual_start_time);
            actual_end_time = CsvToolkit::duplicateString(other.actual_end_time);
            team1_id = CsvToolkit::duplicateString(other.team1_id);
            team2_id = CsvToolkit::duplicateString(other.team2_id);
            winner_team_id = CsvToolkit::duplicateString(other.winner_team_id);
            team1_score = other.team1_score;
            team2_score = other.team2_score;
            match_status = CsvToolkit::duplicateString(other.match_status);
            match_level = CsvToolkit::duplicateString(other.match_level);
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

        dataContainer2D matchData = getData(matchCsvFile);
        if (matchData.error) {
            std::cerr << "Warning: Error loading match data from " << matchCsvFile << " for ranking calculation." << std::endl;
            deleteDataContainer2D(matchData);
            for (int i = 0; i < teamCount; ++i) teams[i].ranking_point = 0; // Default to 0
            return;
        }
        if (matchData.y == 0) {
            std::cout << "Info: No match data found in " << matchCsvFile << ". Calculated points will be 0." << std::endl;
            for (int i = 0; i < teamCount; ++i) teams[i].ranking_point = 0;
            deleteDataContainer2D(matchData);
            return;
        }

        int m_team1IdCol = -1, m_team2IdCol = -1, m_team1ScoreCol = -1, m_team2ScoreCol = -1, m_statusCol = -1;
        for (int k = 0; k < matchData.x; ++k) {
            if (strcmp(matchData.fields[k], "team1_id") == 0) m_team1IdCol = k;
            else if (strcmp(matchData.fields[k], "team2_id") == 0) m_team2IdCol = k;
            else if (strcmp(matchData.fields[k], "team1_score") == 0) m_team1ScoreCol = k;
            else if (strcmp(matchData.fields[k], "team2_score") == 0) m_team2ScoreCol = k;
            else if (strcmp(matchData.fields[k], "match_status") == 0) m_statusCol = k;
        }

        if (m_team1IdCol == -1 || m_team2IdCol == -1 || m_team1ScoreCol == -1 || m_team2ScoreCol == -1 || m_statusCol == -1) {
            std::cerr << "Warning: Could not find all required columns (team1_id, team2_id, team1_score, team2_score, match_status) in "
                    << matchCsvFile << ". Calculated points set to 0." << std::endl;
            for (int i = 0; i < teamCount; ++i) teams[i].ranking_point = 0;
            deleteDataContainer2D(matchData);
            return;
        }

        for (int i = 0; i < teamCount; ++i) { // For each team
            int calculated_points = 0;
            if (!teams[i].team_id || strlen(teams[i].team_id) <= 1) { // Ensure team_id is valid and has prefix + number
                teams[i].ranking_point = 0; // Or handle as error
                continue;
            }

            const char* team_csv_id_numeric_part = teams[i].team_id + 1; // Skips 'T' from "T001", giving "001"

            for (int j = 0; j < matchData.y; ++j) { // For each match
                if (matchData.data[j][m_statusCol] && strcmp(matchData.data[j][m_statusCol], "Completed") == 0) {
                    int score = 0;
                    bool participated = false;

                    const char* match_team1_id_str = matchData.data[j][m_team1IdCol];
                    if (match_team1_id_str && strncmp(match_team1_id_str, "TEAM", 4) == 0 && strlen(match_team1_id_str) > 4) {
                        const char* match_team1_numeric_part = match_team1_id_str + 4; // Skips "TEAM"
                        if (atoi(team_csv_id_numeric_part) == atoi(match_team1_numeric_part)) {
                            score = atoi(matchData.data[j][m_team1ScoreCol] ? matchData.data[j][m_team1ScoreCol] : "0");
                            participated = true;
                        }
                    }

                    if (!participated) {
                        const char* match_team2_id_str = matchData.data[j][m_team2IdCol];
                        if (match_team2_id_str && strncmp(match_team2_id_str, "TEAM", 4) == 0 && strlen(match_team2_id_str) > 4) {
                            const char* match_team2_numeric_part = match_team2_id_str + 4; // Skips "TEAM"
                            if (atoi(team_csv_id_numeric_part) == atoi(match_team2_numeric_part)) {
                                score = atoi(matchData.data[j][m_team2ScoreCol] ? matchData.data[j][m_team2ScoreCol] : "0");
                                participated = true;
                            }
                        }
                    }

                    if (participated) {
                        calculated_points += (score * score);
                    }
                }
            }
            teams[i].ranking_point = calculated_points;
        }
        deleteDataContainer2D(matchData);
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

    // void displayTeamRankings() {
    //     clearTerminal();
    //     std::cout << "--- Team Rankings ---" << std::endl;
    //     int teamCount = 0;
    //     Team* teams = loadTeamsFromCSV(teamCount);

    //     if (teamCount == 0 || !teams) {
    //         std::cout << "No teams found or error loading teams." << std::endl;
    //     } else {
    //         dataContainer2D displayData;
    //         displayData.x = 5; // Rank, ID, Name, University, Points
    //         displayData.fields = new char*[displayData.x];
    //         displayData.fields[0] = duplicateString("Rank");
    //         displayData.fields[1] = duplicateString("TeamID");
    //         displayData.fields[2] = duplicateString("TeamName");
    //         displayData.fields[3] = duplicateString("University");
    //         displayData.fields[4] = duplicateString("Ranking Points");

    //         displayData.y = teamCount;
    //         displayData.data = new char**[displayData.y];
    //         for (int i = 0; i < teamCount; ++i) {
    //             displayData.data[i] = new char*[displayData.x];
    //             std::string rankStr = std::to_string(i + 1);
    //             std::string pointsStr = std::to_string(teams[i].ranking_point);
    //             displayData.data[i][0] = duplicateString(rankStr.c_str());
    //             displayData.data[i][1] = duplicateString(teams[i].team_id);
    //             displayData.data[i][2] = duplicateString(teams[i].team_name);
    //             displayData.data[i][3] = duplicateString(teams[i].university);
    //             displayData.data[i][4] = duplicateString(pointsStr.c_str());
    //         }
    //         displayTabulatedData(displayData);
    //         deleteDataContainer2D(displayData); 
    //     }

    //     // Correct cleanup for memory allocated by loadTeamsFromCSV
    //     if (teams) {
    //         for (int i = 0; i < teamCount; ++i) {
    //             teams[i].~Team(); // Explicitly call destructor for objects constructed with placement new
    //         }
    //         ::operator delete[](teams); // Deallocate the raw memory using global operator delete[]
    //     }
        
    //     getString("Press Enter to continue..."); // This should now be reached
    // }

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
        CsvToolkit::ensureCsvHeader(MATCH_CSV, headers, NUM_MATCH_FIELDS);
        
        if (CsvToolkit::writeNewDataRow(MATCH_CSV, NUM_MATCH_FIELDS, matchCsvRow) != 0) {
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

    //     void generateInitialSchedule() 
    // {
    //     clearTerminal();
    //     std::cout << "--- Generating Initial Schedule (based on calculated rankings) ---" << std::endl;
        
    //     int totalTeamCount = 0;
    //     Team* allTeams = loadTeamsFromCSV(totalTeamCount);

    //     if (!allTeams || totalTeamCount < 8) {
    //         std::cout << "Not enough teams to form standard brackets (need at least 8)." << std::endl;
    //         if (allTeams) {
    //             for (int i = 0; i < totalTeamCount; ++i) allTeams[i].~Team();
    //             ::operator delete[](allTeams);
    //         }
    //         getString("Press Enter to continue...");
    //         return;
    //     }

    //     // Calculate dynamic ranking points and re-sort
    //     calculateAndUpdateTeamRankingPoints(allTeams, totalTeamCount, MATCH_CSV);
    //     if (totalTeamCount > 0) {
    //         std::sort(allTeams, allTeams + totalTeamCount, compareTeams);
    //     }
    //     std::cout << "Teams ranked based on completed match performance." << std::endl;

    //     Team upperBracketTeams[4]; int ubCount = 0;
    //     Team lowerBracketTeams[4]; int lbCount = 0;
    //     // Teams that won't be in the initial 8-team bracket (for tournament_bracket.csv status)
    //     Team* disqualifiedForThisBracket = new Team[totalTeamCount > 8 ? totalTeamCount - 8 : 0];
    //     int dqCount = 0;


    //     for (int i = 0; i < totalTeamCount; ++i) {
    //         if (i < 4) {
    //             upperBracketTeams[ubCount++] = allTeams[i]; // Copy assignment
    //         } else if (i < 8) {
    //             lowerBracketTeams[lbCount++] = allTeams[i]; // Copy assignment
    //         } else {
    //             if (dqCount < (totalTeamCount - 8)) { // Defensive check
    //                 disqualifiedForThisBracket[dqCount++] = allTeams[i];
    //             }
    //         }
    //     }

    //     // --- Generate Proposed Matches ---
    //     Match proposedMatches[4]; // 2 UB + 2 LB
    //     int currentProposedMatchCount = 0;
    //     char matchIdBuffer[20];
    //     const char* defaultDate = "YYYY-MM-DD"; // Placeholder
    //     const char* defaultTimeUB = "10:00";
    //     const char* defaultTimeLB = "14:00";

    //     // UB Matches: Seed 1 vs 4, Seed 2 vs 3 (after dynamic ranking)
    //     if (ubCount == 4) {
    //         sprintf(matchIdBuffer, "MATCH%03d", getNextMatchCounterFileBased()); // Ensure this counter is robust
    //         proposedMatches[currentProposedMatchCount++] = Match(matchIdBuffer, defaultDate, defaultTimeUB, "1", upperBracketTeams[0].team_id, upperBracketTeams[3].team_id, "Scheduled", "Upper Bracket R1");
            
    //         sprintf(matchIdBuffer, "MATCH%03d", getNextMatchCounterFileBased());
    //         proposedMatches[currentProposedMatchCount++] = Match(matchIdBuffer, defaultDate, defaultTimeUB, "1", upperBracketTeams[1].team_id, upperBracketTeams[2].team_id, "Scheduled", "Upper Bracket R1");
    //     }
    //     // LB Matches: Seed 5 vs 8, Seed 6 vs 7 (after dynamic ranking)
    //     if (lbCount == 4) {
    //         sprintf(matchIdBuffer, "MATCH%03d", getNextMatchCounterFileBased());
    //         proposedMatches[currentProposedMatchCount++] = Match(matchIdBuffer, defaultDate, defaultTimeLB, "1", lowerBracketTeams[0].team_id, lowerBracketTeams[3].team_id, "Scheduled", "Lower Bracket R1");

    //         sprintf(matchIdBuffer, "MATCH%03d", getNextMatchCounterFileBased());
    //         proposedMatches[currentProposedMatchCount++] = Match(matchIdBuffer, defaultDate, defaultTimeLB, "1", lowerBracketTeams[1].team_id, lowerBracketTeams[2].team_id, "Scheduled", "Lower Bracket R1");
    //     }

    //     bool saveSchedule = false; // Flag to determine if we should save

    //     if (currentProposedMatchCount > 0) {
    //         bool modifying = true;
    //         while (modifying) {
    //             clearTerminal();
    //             std::cout << "--- Proposed Initial Match Schedule ---" << std::endl;
    //             displayProposedMatches(proposedMatches, currentProposedMatchCount);
                
    //             char* choice_cstr = getString("\nModify date/time for any match? (yes/no/cancel): ");
    //             std::string choice_str = choice_cstr;
    //             delete[] choice_cstr;
    //             for(char &c : choice_str) c = std::tolower(c);

    //             if (choice_str == "yes" || choice_str == "y") {
    //                 int matchNum = getInt("Enter match number (1-4) to modify, or 0 to finish: ");
    //                 if (matchNum >= 1 && matchNum <= currentProposedMatchCount) {
    //                     int matchIdx = matchNum - 1;
    //                     std::cout << "Modifying Match ID: " << proposedMatches[matchIdx].match_id << std::endl;
                        
    //                     delete[] proposedMatches[matchIdx].scheduled_date; // free old date
    //                     char* newDate = getString("Enter new Scheduled Date (YYYY-MM-DD): ");
    //                     proposedMatches[matchIdx].scheduled_date = newDate; // duplicateString(newDate); newDate is already heap
                        
    //                     delete[] proposedMatches[matchIdx].scheduled_time; // free old time
    //                     char* newTime = getString("Enter new Scheduled Time (HH:MM): ");
    //                     proposedMatches[matchIdx].scheduled_time = newTime; // duplicateString(newTime);
    //                 } else if (matchNum == 0) {
    //                     modifying = false; // Finished modifying
    //                 } else {
    //                     displaySystemMessage("Invalid match number.", 2);
    //                 }
    //             } else if (choice_str == "no" || choice_str == "n") {
    //                 modifying = false; // No modifications desired
    //                 saveSchedule = true; // Proceed to confirm save
    //             } else if (choice_str == "cancel" || choice_str == "c") {
    //                 modifying = false;
    //                 saveSchedule = false; // Cancelled generation
    //                 std::cout << "Schedule generation cancelled." << std::endl;
    //             } else {
    //                 displaySystemMessage("Invalid input. Please type 'yes', 'no', or 'cancel'.", 2);
    //             }
    //         }

    //         if (saveSchedule) { // If not cancelled and user finished modification phase or said no to modifying
    //             clearTerminal();
    //             std::cout << "--- Final Proposed Schedule ---" << std::endl;
    //             displayProposedMatches(proposedMatches, currentProposedMatchCount);
    //             char* confirm_cstr = getString("Save this schedule to CSVs? (yes/no): ");
    //             std::string confirm_str = confirm_cstr;
    //             delete[] confirm_cstr;
    //             for(char &c : confirm_str) c = std::tolower(c);

    //             if (confirm_str == "yes" || confirm_str == "y") {
    //                 // --- Populate tournament_bracket.csv ---
    //                 dataContainer2D bracketData;
    //                 bracketData.x = 3;
    //                 bracketData.fields = new char*[bracketData.x];
    //                 bracketData.fields[0] = duplicateString("team_id");
    //                 bracketData.fields[1] = duplicateString("position");
    //                 bracketData.fields[2] = duplicateString("bracket");
                    
    //                 int bracketEntriesCount = ubCount + lbCount + dqCount;
    //                 bracketData.data = new char**[bracketEntriesCount > 0 ? bracketEntriesCount : 1]; // Avoid new char**[0]
    //                 bracketData.y = 0;

    //                 auto addTeamToBracketCSV = [&](const Team& team, const char* pos, const char* bracket_name) {
    //                     if (bracketData.y < bracketEntriesCount) {
    //                         bracketData.data[bracketData.y] = new char*[bracketData.x];
    //                         bracketData.data[bracketData.y][0] = duplicateString(team.team_id);
    //                         bracketData.data[bracketData.y][1] = duplicateString(pos);
    //                         bracketData.data[bracketData.y][2] = duplicateString(bracket_name);
    //                         bracketData.y++;
    //                     }
    //                 };
    //                 if (ubCount == 4) {
    //                     addTeamToBracketCSV(upperBracketTeams[0], "UB-R1-M1-S1", "upper_bracket");
    //                     addTeamToBracketCSV(upperBracketTeams[3], "UB-R1-M1-S2", "upper_bracket");
    //                     addTeamToBracketCSV(upperBracketTeams[1], "UB-R1-M2-S1", "upper_bracket");
    //                     addTeamToBracketCSV(upperBracketTeams[2], "UB-R1-M2-S2", "upper_bracket");
    //                 }
    //                 if (lbCount == 4) {
    //                     addTeamToBracketCSV(lowerBracketTeams[0], "LB-R1-M1-S1", "lower_bracket");
    //                     addTeamToBracketCSV(lowerBracketTeams[3], "LB-R1-M1-S2", "lower_bracket");
    //                     addTeamToBracketCSV(lowerBracketTeams[1], "LB-R1-M2-S1", "lower_bracket");
    //                     addTeamToBracketCSV(lowerBracketTeams[2], "LB-R1-M2-S2", "lower_bracket");
    //                 }
    //                 for(int i=0; i < dqCount; ++i) {
    //                     addTeamToBracketCSV(disqualifiedForThisBracket[i], "DQ-Initial", "disqualified");
    //                 }

    //                 writeData(TOURNAMENT_BRACKET_CSV, bracketData);
    //                 deleteDataContainer2D(bracketData);
    //                 std::cout << "Initial bracket positions written to " << TOURNAMENT_BRACKET_CSV << std::endl;

    //                 // --- Write matches to match.csv ---
    //                 for (int i = 0; i < currentProposedMatchCount; ++i) {
    //                     writeMatchToCSV(proposedMatches[i]);
    //                 }
    //                 std::cout << currentProposedMatchCount << " initial matches saved to " << MATCH_CSV << std::endl;
    //                 saveSchedule = true; // Confirm it was saved
    //             } else {
    //                 std::cout << "Schedule saving cancelled." << std::endl;
    //                 saveSchedule = false;
    //             }
    //         }


    //     } else {
    //         std::cout << "Could not generate any proposed matches (likely due to insufficient teams)." << std::endl;
    //     }

    //     // Cleanup dynamically allocated allTeams and disqualifiedForThisBracket
    //     if (allTeams) {
    //         for (int i = 0; i < totalTeamCount; ++i) allTeams[i].~Team();
    //         ::operator delete[](allTeams);
    //     }
    //     if (totalTeamCount > 8) { // Only delete if it was allocated with a size > 0
    //         for(int i=0; i < dqCount; ++i) disqualifiedForThisBracket[i].~Team(); // Call destructors
    //         delete[] disqualifiedForThisBracket; // delete array itself
    //     } else {
    //         delete[] disqualifiedForThisBracket; // delete even if size was 0 (new Team[0] is valid, delete[] on it is valid)
    //     }
    //     // proposedMatches is a stack array, its Match destructors will be called automatically when it goes out of scope.
    //     // upperBracketTeams and lowerBracketTeams are also stack arrays of Team objects, destructors called.

    //     getString("Press Enter to continue...");
    // }

    void displayMatchSchedule() {
        clearTerminal();
        std::cout << "--- Current Match Schedule ---" << std::endl;
        dataContainer2D scheduleData = getData(MATCH_CSV);
        if (scheduleData.error || scheduleData.y == 0) {
            std::cout << "No match schedule found or error loading data." << std::endl;
        } else {
            displayTabulatedData(scheduleData);
        }
        deleteDataContainer2D(scheduleData);
        getString("Press Enter to continue...");
    }

    void displayBracketProgress() {
        clearTerminal();
        std::cout << "--- Bracket Progress ---" << std::endl;
        std::cout << "Team Positions/Status (from " << TOURNAMENT_BRACKET_CSV << "):" << std::endl;
        dataContainer2D bracketInfo = getData(TOURNAMENT_BRACKET_CSV);
        if (!bracketInfo.error && bracketInfo.y > 0) {
            displayTabulatedData(bracketInfo);
        } else {
            std::cout << "No bracket position data found." << std::endl;
        }
        deleteDataContainer2D(bracketInfo);

        std::cout << "\nCompleted Matches (from " << MATCH_CSV << "):" << std::endl;
        dataContainer2D allMatches = getData(MATCH_CSV);
        dataContainer2D completedMatches = filterDataContainer(allMatches, "match_status", "Completed");
        if (!completedMatches.error && completedMatches.y > 0) {
            displayTabulatedData(completedMatches);
        } else {
            std::cout << "No completed matches found." << std::endl;
        }
        deleteDataContainer2D(allMatches);
        deleteDataContainer2D(completedMatches);
        std::cout << "\nNote: Full dynamic progression requires implementing logic in 'Update Result'." << std::endl;
        getString("Press Enter to continue...");
    }
    
    void handleTeamProgression(const Match& updatedMatch, TeamStack& ubLosersStack, TeamQueue& nextRoundUBQueue, TeamQueue& nextRoundLBQueue) {
        std::cout << "Processing progression for Match ID: " << updatedMatch.match_id << std::endl;
        if (!updatedMatch.winner_team_id || strlen(updatedMatch.winner_team_id) == 0) {
            std::cout << "No winner declared. Cannot process progression." << std::endl;
            return;
        }

        const char* winnerId = updatedMatch.winner_team_id;
        const char* loserId = nullptr;
        Team winnerTeam, loserTeam; // Need to fetch full Team objects if more details are needed for queue/stack

        // Identify loser and find Team objects (simplified: just IDs for now)
        if (strcmp(updatedMatch.team1_id, winnerId) == 0) {
            loserId = updatedMatch.team2_id;
        } else {
            loserId = updatedMatch.team1_id;
        }
        
        // --- Update tournament_bracket.csv with new status ---
        dataContainer2D bracketData = getData(TOURNAMENT_BRACKET_CSV);
        bool bracketUpdated = false;
        if (!bracketData.error) {
            int teamIdCol = -1, bracketNameCol = -1, positionCol = -1;
            for(int i=0; i < bracketData.x; ++i) {
                if(strcmp(bracketData.fields[i], "team_id") == 0) teamIdCol = i;
                else if(strcmp(bracketData.fields[i], "bracket") == 0) bracketNameCol = i;
                else if(strcmp(bracketData.fields[i], "position") == 0) positionCol = i;
            }

            if (teamIdCol != -1 && bracketNameCol != -1 && positionCol !=-1) {
                 for (int i = 0; i < bracketData.y; ++i) {
                    if (bracketData.data[i][teamIdCol] && strcmp(bracketData.data[i][teamIdCol], loserId) == 0) {
                        std::string currentBracketName = bracketData.data[i][bracketNameCol];
                        delete[] bracketData.data[i][bracketNameCol]; // existing bracket name
                        delete[] bracketData.data[i][positionCol]; // existing position

                        if (strstr(updatedMatch.match_level, "Upper Bracket") != nullptr) {
                            bracketData.data[i][bracketNameCol] = duplicateString("lower_bracket"); // Dropped to LB
                            bracketData.data[i][positionCol] = duplicateString("LB-FromUB"); // New position
                            std::cout << "Team " << loserId << " drops to Lower Bracket." << std::endl;
                            // For actual queuing: Need to load Team object for loserId and push to ubLosersStack
                        } else { // Was in Lower Bracket
                            bracketData.data[i][bracketNameCol] = duplicateString("eliminated");
                            bracketData.data[i][positionCol] = duplicateString("Eliminated");
                            std::cout << "Team " << loserId << " is eliminated." << std::endl;
                        }
                        bracketUpdated = true;
                        break; 
                    }
                }
                // Update winner's position (simplified, a real system would map to next match slot)
                 for (int i = 0; i < bracketData.y; ++i) {
                    if (bracketData.data[i][teamIdCol] && strcmp(bracketData.data[i][teamIdCol], winnerId) == 0) {
                        delete[] bracketData.data[i][positionCol];
                        std::string nextPos = std::string(updatedMatch.match_level) + "-Winner";
                        bracketData.data[i][positionCol] = duplicateString(nextPos.c_str());
                         std::cout << "Team " << winnerId << " advances." << std::endl;
                         // For actual queuing: Need to load Team object for winnerId and enqueue to appropriate next round queue
                        bracketUpdated = true;
                        break;
                    }
                 }
            }
            if(bracketUpdated) writeData(TOURNAMENT_BRACKET_CSV, bracketData);
        }
        deleteDataContainer2D(bracketData);
        
        // TODO: Implement actual queuing/stacking of Team objects and generating next round matches
        // This part requires loading the full Team objects for winner and loser,
        // then pushing/enqueuing them.
        // Example (conceptual, needs Team objects not just IDs):
        // if (strstr(updatedMatch.match_level, "Upper Bracket")) {
        //     nextRoundUBQueue.enqueue(winnerTeamObject);
        //     ubLosersStack.push(loserTeamObject);
        // } else { // Lower Bracket
        //     nextRoundLBQueue.enqueue(winnerTeamObject); // Loser is eliminated
        // }
        // Then, a function would check these queues/stacks to form new matches.
        std::cout << "Further progression logic (scheduling next matches using queues/stacks) is pending." << std::endl;
    }


    void updateMatchResult(TeamStack& ubLosersStack, TeamQueue& nextRoundUBQueue, TeamQueue& nextRoundLBQueue) {
        clearTerminal();
        std::cout << "--- Update Match Result ---" << std::endl;
        displayMatchSchedule(); // Show current matches to help user pick

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
        int idCol=-1, t1IdCol=-1, t2IdCol=-1, t1ScoreCol=-1, t2ScoreCol=-1, winnerCol=-1, statusCol=-1, levelCol=-1;
        for(int i=0; i < allMatches.x; ++i) {
            if(strcmp(allMatches.fields[i], "match_id") == 0) idCol = i;
            else if(strcmp(allMatches.fields[i], "team1_id") == 0) t1IdCol = i;
            else if(strcmp(allMatches.fields[i], "team2_id") == 0) t2IdCol = i;
            else if(strcmp(allMatches.fields[i], "team1_score") == 0) t1ScoreCol = i;
            else if(strcmp(allMatches.fields[i], "team2_score") == 0) t2ScoreCol = i;
            else if(strcmp(allMatches.fields[i], "winner_team_id") == 0) winnerCol = i;
            else if(strcmp(allMatches.fields[i], "match_status") == 0) statusCol = i;
            else if(strcmp(allMatches.fields[i], "match_level") == 0) levelCol = i;
        }
        if(idCol == -1 || t1IdCol == -1 || t2IdCol == -1 || t1ScoreCol == -1 || t2ScoreCol == -1 || winnerCol == -1 || statusCol == -1 || levelCol == -1){
            std::cerr << "Error: Critical columns missing in " << MATCH_CSV << std::endl;
            deleteDataContainer2D(allMatches); getString("Press Enter..."); return;
        }

        Match currentMatch; // To hold the data of the match being updated

        for (int i = 0; i < allMatches.y; ++i) {
            if (allMatches.data[i][idCol] && strcmp(allMatches.data[i][idCol], matchIdToUpdate_str.c_str()) == 0) {
                matchRowIdx = i;
                // Populate currentMatch from CSV data
                currentMatch = Match(allMatches.data[i][idCol], "", "", // Simplified, need full constructor or setters
                                     allMatches.data[i][t1IdCol], allMatches.data[i][t2IdCol],
                                     allMatches.data[i][statusCol], allMatches.data[i][levelCol]);
                // Actual date/time/round would also be copied if needed by progression
                break;
            }
        }

        if (matchRowIdx != -1) {
            std::cout << "Updating Match: " << currentMatch.match_id << " (" << currentMatch.match_level << ")" << std::endl;
            std::cout << "Team 1: " << currentMatch.team1_id << " vs Team 2: " << currentMatch.team2_id << std::endl;
            
            int score1 = getInt("Enter Team 1 Score: ");
            int score2 = getInt("Enter Team 2 Score: ");
            char* winnerId_cstr = getString("Enter Winner Team ID (or type DRAW if applicable): ");

            std::string s1_str = std::to_string(score1);
            std::string s2_str = std::to_string(score2);

            delete[] allMatches.data[matchRowIdx][t1ScoreCol]; allMatches.data[matchRowIdx][t1ScoreCol] = duplicateString(s1_str.c_str());
            delete[] allMatches.data[matchRowIdx][t2ScoreCol]; allMatches.data[matchRowIdx][t2ScoreCol] = duplicateString(s2_str.c_str());
            delete[] allMatches.data[matchRowIdx][winnerCol];  allMatches.data[matchRowIdx][winnerCol] = duplicateString(winnerId_cstr);
            delete[] allMatches.data[matchRowIdx][statusCol];  allMatches.data[matchRowIdx][statusCol] = duplicateString("Completed");
            // Update actual_end_time (optional)
            // char* endTime = getString("Enter actual end time (YYYY-MM-DD HH:MM, or blank):");
            // if(strlen(endTime)>0) { /* update allMatches.data[matchRowIdx][actualEndCol] */ } delete[] endTime;

            currentMatch.team1_score = score1; currentMatch.team2_score = score2;
            delete[] currentMatch.winner_team_id; currentMatch.winner_team_id = duplicateString(winnerId_cstr);
            delete[] currentMatch.match_status;   currentMatch.match_status   = duplicateString("Completed");

            if (writeData(MATCH_CSV, allMatches) == 0) {
                std::cout << "Match result updated successfully in " << MATCH_CSV << std::endl;
                handleTeamProgression(currentMatch, ubLosersStack, nextRoundUBQueue, nextRoundLBQueue);
            } else {
                std::cout << "Error writing updated match data to " << MATCH_CSV << std::endl;
            }
            delete[] winnerId_cstr;
        } else {
            std::cout << "Match ID '" << matchIdToUpdate_str << "' not found." << std::endl;
        }
        deleteDataContainer2D(allMatches);
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
            const char* menuOptions[] = {"View Tournament", "Generate Initial Schedule", "Update Match Result", "Return to Main Menu"};
            int choice = displayMenu("Match Scheduling & Player Progression", menuOptions, 4);
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
                case 4: running = false; break;
                default: displaySystemMessage("Invalid choice.", 2); break;
            }
        }
    }

} 
#endif