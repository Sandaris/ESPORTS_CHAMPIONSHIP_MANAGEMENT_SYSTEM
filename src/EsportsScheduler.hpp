#ifndef ESPORTS_SCHEDULER_HPP
#define ESPORTS_SCHEDULER_HPP

#include <iostream>
#include <string>
#include <algorithm> // For std::sort
#include <iomanip>   // For output formatting
#include <cstdio>    // For sprintf
#include <cstring>   // For strcmp, strlen etc.

#include "File_exe.hpp" // Your CsvToolkit library

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
            team_id = CsvToolkit::duplicateString(id);
            team_name = CsvToolkit::duplicateString(name);
            university = CsvToolkit::duplicateString(uni);
            //country = CsvToolkit::duplicateString(ctry);
            ranking_point = rank;
            team_status = CsvToolkit::duplicateString(status);
        }

        Team(const Team& other) {
            team_id = CsvToolkit::duplicateString(other.team_id);
            team_name = CsvToolkit::duplicateString(other.team_name);
            university = CsvToolkit::duplicateString(other.university);
            //country = CsvToolkit::duplicateString(other.country);
            ranking_point = other.ranking_point;
            team_status = CsvToolkit::duplicateString(other.team_status);
        }

        Team& operator=(const Team& other) {
            if (this == &other) return *this;
            delete[] team_id; delete[] team_name; delete[] university; delete[] team_status;
            team_id = CsvToolkit::duplicateString(other.team_id);
            team_name = CsvToolkit::duplicateString(other.team_name);
            university = CsvToolkit::duplicateString(other.university);
            //country = CsvToolkit::duplicateString(other.country);
            ranking_point = other.ranking_point;
            team_status = CsvToolkit::duplicateString(other.team_status);
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
        char* match_round_number;
        char* actual_start_time;
        char* actual_end_time;
        char* team1_id;
        char* team2_id;
        char* winner_team_id;
        int team1_score;
        int team2_score;
        char* match_status;
        char* match_level;

        Match() : match_id(nullptr), scheduled_date(nullptr), scheduled_time(nullptr), match_round_number(nullptr),
                  actual_start_time(nullptr), actual_end_time(nullptr), team1_id(nullptr), team2_id(nullptr),
                  winner_team_id(nullptr), team1_score(0), team2_score(0), match_status(nullptr), match_level(nullptr) {}

        Match(const char* mid, const char* s_date, const char* s_time, const char* round,
              const char* t1_id, const char* t2_id, const char* status, const char* level) {
            match_id = CsvToolkit::duplicateString(mid);
            scheduled_date = CsvToolkit::duplicateString(s_date);
            scheduled_time = CsvToolkit::duplicateString(s_time);
            match_round_number = CsvToolkit::duplicateString(round);
            actual_start_time = CsvToolkit::duplicateString("");
            actual_end_time = CsvToolkit::duplicateString("");
            team1_id = CsvToolkit::duplicateString(t1_id);
            team2_id = CsvToolkit::duplicateString(t2_id);
            winner_team_id = CsvToolkit::duplicateString("");
            team1_score = 0;
            team2_score = 0;
            match_status = CsvToolkit::duplicateString(status);
            match_level = CsvToolkit::duplicateString(level);
        }

        Match(const Match& other) {
            match_id = CsvToolkit::duplicateString(other.match_id);
            scheduled_date = CsvToolkit::duplicateString(other.scheduled_date);
            scheduled_time = CsvToolkit::duplicateString(other.scheduled_time);
            match_round_number = CsvToolkit::duplicateString(other.match_round_number);
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

        Match& operator=(const Match& other) {
            if (this == &other) return *this;
            delete[] match_id; delete[] scheduled_date; delete[] scheduled_time; delete[] match_round_number;
            delete[] actual_start_time; delete[] actual_end_time; delete[] team1_id; delete[] team2_id;
            delete[] winner_team_id; delete[] match_status; delete[] match_level;
            match_id = CsvToolkit::duplicateString(other.match_id);
            scheduled_date = CsvToolkit::duplicateString(other.scheduled_date);
            scheduled_time = CsvToolkit::duplicateString(other.scheduled_time);
            match_round_number = CsvToolkit::duplicateString(other.match_round_number);
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

        ~Match() {
            delete[] match_id; delete[] scheduled_date; delete[] scheduled_time; delete[] match_round_number;
            delete[] actual_start_time; delete[] actual_end_time; delete[] team1_id; delete[] team2_id;
            delete[] winner_team_id; delete[] match_status; delete[] match_level;
        }

        void getAsCsvRow(const char** rowBuffer, int bufferSize) const {
            if (bufferSize < 13) return;
            static std::string t1_s, t2_s;
            t1_s = std::to_string(team1_score);
            t2_s = std::to_string(team2_score);
            rowBuffer[0] = match_id ? match_id : ""; rowBuffer[1] = scheduled_date ? scheduled_date : "";
            rowBuffer[2] = scheduled_time ? scheduled_time : ""; rowBuffer[3] = match_round_number ? match_round_number : "";
            rowBuffer[4] = actual_start_time ? actual_start_time : ""; rowBuffer[5] = actual_end_time ? actual_end_time : "";
            rowBuffer[6] = team1_id ? team1_id : ""; rowBuffer[7] = team2_id ? team2_id : "";
            rowBuffer[8] = winner_team_id ? winner_team_id : ""; rowBuffer[9] = t1_s.c_str();
            rowBuffer[10] = t2_s.c_str(); rowBuffer[11] = match_status ? match_status : "";
            rowBuffer[12] = match_level ? match_level : "";
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
        CsvToolkit::dataContainer2D matches = CsvToolkit::getData(MATCH_CSV);
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
        CsvToolkit::deleteDataContainer2D(matches);
        return maxIdNum + 1;
    }

    // In namespace EsportsScheduler
    Team* loadTeamsFromCSV(int& teamCount_out) 
    {
        teamCount_out = 0;
        CsvToolkit::dataContainer2D teamData = CsvToolkit::getData(TEAMS_CSV); // TEAMS_CSV should be "team.csv"
        if (teamData.error) {
            std::cerr << "Error loading teams from " << TEAMS_CSV << std::endl;
            CsvToolkit::deleteDataContainer2D(teamData);
            return nullptr;
        }

        // Adjusted column lookups
        int idCol = -1, nameCol = -1, uniCol = -1, /* countryCol = -1, REMOVED */ rankCol = -1;
        for(int i=0; i < teamData.x; ++i) {
            if(strcmp(teamData.fields[i], "team_id") == 0) idCol = i;
            else if(strcmp(teamData.fields[i], "team_name") == 0) nameCol = i;
            else if(strcmp(teamData.fields[i], "university_name") == 0) uniCol = i;
            // else if(strcmp(teamData.fields[i], "Country") == 0) countryCol = i; // REMOVED
            else if(strcmp(teamData.fields[i], "ranking_points") == 0) rankCol = i; // Corrected from "RankingPoint" previously
        }

        // Update condition to check for required columns
        if(idCol == -1 || nameCol == -1 || uniCol == -1 || /* countryCol == -1 || REMOVED */ rankCol == -1) {
            std::cerr << "Error: Required columns (TeamID, TeamName, University, ranking_points) not found in " << TEAMS_CSV << std::endl;
            CsvToolkit::deleteDataContainer2D(teamData);
            return nullptr;
        }

        if (teamData.y == 0) {
            CsvToolkit::deleteDataContainer2D(teamData);
            return nullptr;
        }

        Team* loadedTeams = new Team[teamData.y];
        for (int i = 0; i < teamData.y; ++i) {
            new (&loadedTeams[i]) Team( // Invokes constructor without country
                teamData.data[i][idCol],
                teamData.data[i][nameCol],
                teamData.data[i][uniCol],
                // teamData.data[i][countryCol], // REMOVED
                atoi(teamData.data[i][rankCol] ? teamData.data[i][rankCol] : "0")
            );
        }
        teamCount_out = teamData.y;
        CsvToolkit::deleteDataContainer2D(teamData);
        std::sort(loadedTeams, loadedTeams + teamCount_out, compareTeams);
        return loadedTeams;
    }

    void displayTeamRankings() {
        CsvToolkit::clearTerminal();
        std::cout << "--- Team Rankings ---" << std::endl;
        int teamCount = 0;
        Team* teams = loadTeamsFromCSV(teamCount);

        if (teamCount == 0 || !teams) {
            std::cout << "No teams found or error loading teams." << std::endl;
        } else {
            CsvToolkit::dataContainer2D displayData;
            displayData.x = 5;
            displayData.fields = new char*[displayData.x];
            displayData.fields[0] = CsvToolkit::duplicateString("Rank");
            displayData.fields[1] = CsvToolkit::duplicateString("TeamID");
            displayData.fields[2] = CsvToolkit::duplicateString("TeamName");
            displayData.fields[3] = CsvToolkit::duplicateString("University");
            displayData.fields[4] = CsvToolkit::duplicateString("Ranking Points");
            displayData.y = teamCount;
            displayData.data = new char**[displayData.y];
            for (int i = 0; i < teamCount; ++i) {
                displayData.data[i] = new char*[displayData.x];
                std::string rankStr = std::to_string(i + 1);
                std::string pointsStr = std::to_string(teams[i].ranking_point);
                displayData.data[i][0] = CsvToolkit::duplicateString(rankStr.c_str());
                displayData.data[i][1] = CsvToolkit::duplicateString(teams[i].team_id);
                displayData.data[i][2] = CsvToolkit::duplicateString(teams[i].team_name);
                displayData.data[i][3] = CsvToolkit::duplicateString(teams[i].university);
                displayData.data[i][4] = CsvToolkit::duplicateString(pointsStr.c_str());
            }
            CsvToolkit::displayTabulatedData(displayData);
            CsvToolkit::deleteDataContainer2D(displayData);
        }
        for(int i=0; i<teamCount; ++i) teams[i].~Team(); // Explicitly call destructor for objects in array
        delete[] teams; // Deallocate the array memory
        CsvToolkit::getString("Press Enter to continue...");
    }

    void writeMatchToCSV(const Match& match) {
        const int NUM_MATCH_FIELDS = 13;
        const char* matchCsvRow[NUM_MATCH_FIELDS];
        match.getAsCsvRow(matchCsvRow, NUM_MATCH_FIELDS);
        const char* headers[] = {"match_id","scheduled_date","scheduled_time","match_round_number","actual_start_time","actual_end_time","team1_id","team2_id","winner_team_id","team1_score","team2_score","match_status","match_level"};
        CsvToolkit::ensureCsvHeader(MATCH_CSV, headers, NUM_MATCH_FIELDS);
        if (CsvToolkit::writeNewDataRow(MATCH_CSV, NUM_MATCH_FIELDS, matchCsvRow) != 0) {
            std::cerr << "Error writing match " << match.match_id << " to " << MATCH_CSV << std::endl;
        }
    }

    void generateInitialSchedule() {
        CsvToolkit::clearTerminal();
        std::cout << "--- Generating Initial Schedule ---" << std::endl;
        int totalTeamCount = 0;
        Team* allTeamsSorted = loadTeamsFromCSV(totalTeamCount);

        if (!allTeamsSorted || totalTeamCount < 8) {
            std::cout << "Not enough teams to form standard brackets (need at least 8)." << std::endl;
            if(allTeamsSorted) {
                for(int i=0; i<totalTeamCount; ++i) allTeamsSorted[i].~Team();
                delete[] allTeamsSorted;
            }
            CsvToolkit::getString("Press Enter to continue...");
            return;
        }

        Team upperBracketTeams[4]; int ubCount = 0;
        Team lowerBracketTeams[4]; int lbCount = 0;
        // Disqualified teams are not explicitly stored in a separate array here for simplicity,
        // but their status would be updated in tournament_bracket.csv.

        CsvToolkit::dataContainer2D bracketData; // For writing to tournament_bracket.csv
        bracketData.x = 3;
        bracketData.fields = new char*[bracketData.x];
        bracketData.fields[0] = CsvToolkit::duplicateString("team_id");
        bracketData.fields[1] = CsvToolkit::duplicateString("position");
        bracketData.fields[2] = CsvToolkit::duplicateString("bracket");
        
        // Allocate data rows: 4 UB + 4 LB + potentially disqualified
        int maxBracketEntries = totalTeamCount;
        bracketData.data = new char**[maxBracketEntries];
        bracketData.y = 0; // Actual number of rows

        auto addTeamToBracketCSV = [&](const Team& team, const char* pos, const char* bracket_name) {
            if (bracketData.y < maxBracketEntries) {
                bracketData.data[bracketData.y] = new char*[bracketData.x];
                bracketData.data[bracketData.y][0] = CsvToolkit::duplicateString(team.team_id);
                bracketData.data[bracketData.y][1] = CsvToolkit::duplicateString(pos);
                bracketData.data[bracketData.y][2] = CsvToolkit::duplicateString(bracket_name);
                bracketData.y++;
            }
        };

        for (int i = 0; i < totalTeamCount; ++i) {
            if (i < 4) { // Top 4 to UB
                upperBracketTeams[ubCount++] = allTeamsSorted[i]; // Copy assignment
            } else if (i < 8) { // Next 4 to LB
                lowerBracketTeams[lbCount++] = allTeamsSorted[i]; // Copy assignment
            } else { // Disqualified
                 addTeamToBracketCSV(allTeamsSorted[i], "DQ", "disqualified");
            }
        }
        
        // UB: Seed 1 vs 4, Seed 2 vs 3
        if (ubCount == 4) {
            addTeamToBracketCSV(upperBracketTeams[0], "UB-R1-M1-S1", "upper_bracket");
            addTeamToBracketCSV(upperBracketTeams[3], "UB-R1-M1-S2", "upper_bracket");
            addTeamToBracketCSV(upperBracketTeams[1], "UB-R1-M2-S1", "upper_bracket");
            addTeamToBracketCSV(upperBracketTeams[2], "UB-R1-M2-S2", "upper_bracket");
        }
        // LB: Seed 5 vs 8, Seed 6 vs 7
        if (lbCount == 4) {
            addTeamToBracketCSV(lowerBracketTeams[0], "LB-R1-M1-S1", "lower_bracket");
            addTeamToBracketCSV(lowerBracketTeams[3], "LB-R1-M1-S2", "lower_bracket");
            addTeamToBracketCSV(lowerBracketTeams[1], "LB-R1-M2-S1", "lower_bracket");
            addTeamToBracketCSV(lowerBracketTeams[2], "LB-R1-M2-S2", "lower_bracket");
        }

        CsvToolkit::writeData(TOURNAMENT_BRACKET_CSV, bracketData);
        CsvToolkit::deleteDataContainer2D(bracketData); // This handles fields and data rows
        std::cout << "Initial bracket positions written to " << TOURNAMENT_BRACKET_CSV << std::endl;

        const char* defaultDate = "2025-07-01"; 
        const char* defaultTimeUB = "10:00:00";
        const char* defaultTimeLB = "14:00:00";
        char matchIdBuffer[20];
        int initialMatchCount = 0;
        Match initialMatches[4]; // Max 4 initial matches (2 UB, 2 LB)

        if (ubCount == 4) {
            sprintf(matchIdBuffer, "MATCH%03d", getNextMatchCounterFileBased());
            initialMatches[initialMatchCount++] = Match(matchIdBuffer, defaultDate, defaultTimeUB, "1", upperBracketTeams[0].team_id, upperBracketTeams[3].team_id, "Scheduled", "Upper Bracket R1");
            sprintf(matchIdBuffer, "MATCH%03d", getNextMatchCounterFileBased());
            initialMatches[initialMatchCount++] = Match(matchIdBuffer, defaultDate, defaultTimeUB, "1", upperBracketTeams[1].team_id, upperBracketTeams[2].team_id, "Scheduled", "Upper Bracket R1");
        }
        if (lbCount == 4) {
            sprintf(matchIdBuffer, "MATCH%03d", getNextMatchCounterFileBased());
            initialMatches[initialMatchCount++] = Match(matchIdBuffer, defaultDate, defaultTimeLB, "1", lowerBracketTeams[0].team_id, lowerBracketTeams[3].team_id, "Scheduled", "Lower Bracket R1");
            sprintf(matchIdBuffer, "MATCH%03d", getNextMatchCounterFileBased());
            initialMatches[initialMatchCount++] = Match(matchIdBuffer, defaultDate, defaultTimeLB, "1", lowerBracketTeams[1].team_id, lowerBracketTeams[2].team_id, "Scheduled", "Lower Bracket R1");
        }

        for (int i = 0; i < initialMatchCount; ++i) {
            writeMatchToCSV(initialMatches[i]);
        }
        std::cout << initialMatchCount << " initial matches generated and written to " << MATCH_CSV << std::endl;

        for(int i=0; i<totalTeamCount; ++i) allTeamsSorted[i].~Team(); // Destruct teams from load
        delete[] allTeamsSorted;
        // upperBracketTeams and lowerBracketTeams are stack arrays of Team objects,
        // their destructors will be called when they go out of scope.
        // initialMatches is also a stack array of Match objects.
        std::cout << "Initial schedule generated." << std::endl;
        CsvToolkit::getString("Press Enter to continue...");
    }

    void displayMatchSchedule() {
        CsvToolkit::clearTerminal();
        std::cout << "--- Current Match Schedule ---" << std::endl;
        CsvToolkit::dataContainer2D scheduleData = CsvToolkit::getData(MATCH_CSV);
        if (scheduleData.error || scheduleData.y == 0) {
            std::cout << "No match schedule found or error loading data." << std::endl;
        } else {
            CsvToolkit::displayTabulatedData(scheduleData);
        }
        CsvToolkit::deleteDataContainer2D(scheduleData);
        CsvToolkit::getString("Press Enter to continue...");
    }

    void displayBracketProgress() {
        CsvToolkit::clearTerminal();
        std::cout << "--- Bracket Progress ---" << std::endl;
        std::cout << "Team Positions/Status (from " << TOURNAMENT_BRACKET_CSV << "):" << std::endl;
        CsvToolkit::dataContainer2D bracketInfo = CsvToolkit::getData(TOURNAMENT_BRACKET_CSV);
        if (!bracketInfo.error && bracketInfo.y > 0) {
            CsvToolkit::displayTabulatedData(bracketInfo);
        } else {
            std::cout << "No bracket position data found." << std::endl;
        }
        CsvToolkit::deleteDataContainer2D(bracketInfo);

        std::cout << "\nCompleted Matches (from " << MATCH_CSV << "):" << std::endl;
        CsvToolkit::dataContainer2D allMatches = CsvToolkit::getData(MATCH_CSV);
        CsvToolkit::dataContainer2D completedMatches = CsvToolkit::filterDataContainer(allMatches, "match_status", "Completed");
        if (!completedMatches.error && completedMatches.y > 0) {
            CsvToolkit::displayTabulatedData(completedMatches);
        } else {
            std::cout << "No completed matches found." << std::endl;
        }
        CsvToolkit::deleteDataContainer2D(allMatches);
        CsvToolkit::deleteDataContainer2D(completedMatches);
        std::cout << "\nNote: Full dynamic progression requires implementing logic in 'Update Result'." << std::endl;
        CsvToolkit::getString("Press Enter to continue...");
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
        CsvToolkit::dataContainer2D bracketData = CsvToolkit::getData(TOURNAMENT_BRACKET_CSV);
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
                            bracketData.data[i][bracketNameCol] = CsvToolkit::duplicateString("lower_bracket"); // Dropped to LB
                            bracketData.data[i][positionCol] = CsvToolkit::duplicateString("LB-FromUB"); // New position
                            std::cout << "Team " << loserId << " drops to Lower Bracket." << std::endl;
                            // For actual queuing: Need to load Team object for loserId and push to ubLosersStack
                        } else { // Was in Lower Bracket
                            bracketData.data[i][bracketNameCol] = CsvToolkit::duplicateString("eliminated");
                            bracketData.data[i][positionCol] = CsvToolkit::duplicateString("Eliminated");
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
                        bracketData.data[i][positionCol] = CsvToolkit::duplicateString(nextPos.c_str());
                         std::cout << "Team " << winnerId << " advances." << std::endl;
                         // For actual queuing: Need to load Team object for winnerId and enqueue to appropriate next round queue
                        bracketUpdated = true;
                        break;
                    }
                 }
            }
            if(bracketUpdated) CsvToolkit::writeData(TOURNAMENT_BRACKET_CSV, bracketData);
        }
        CsvToolkit::deleteDataContainer2D(bracketData);
        
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
        CsvToolkit::clearTerminal();
        std::cout << "--- Update Match Result ---" << std::endl;
        displayMatchSchedule(); // Show current matches to help user pick

        char* matchIdToUpdate_cstr = CsvToolkit::getString("Enter Match ID to update (e.g., MATCH001): ");
        if (!matchIdToUpdate_cstr || strlen(matchIdToUpdate_cstr) == 0) {
            std::cout << "No Match ID entered." << std::endl;
            delete[] matchIdToUpdate_cstr;
            CsvToolkit::getString("Press Enter to continue...");
            return;
        }
        std::string matchIdToUpdate_str(matchIdToUpdate_cstr);
        delete[] matchIdToUpdate_cstr;

        CsvToolkit::dataContainer2D allMatches = CsvToolkit::getData(MATCH_CSV);
        if (allMatches.error) {
            std::cout << "Error loading matches from " << MATCH_CSV << std::endl;
            CsvToolkit::deleteDataContainer2D(allMatches);
            CsvToolkit::getString("Press Enter to continue...");
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
            CsvToolkit::deleteDataContainer2D(allMatches); CsvToolkit::getString("Press Enter..."); return;
        }

        Match currentMatch; // To hold the data of the match being updated

        for (int i = 0; i < allMatches.y; ++i) {
            if (allMatches.data[i][idCol] && strcmp(allMatches.data[i][idCol], matchIdToUpdate_str.c_str()) == 0) {
                matchRowIdx = i;
                // Populate currentMatch from CSV data
                currentMatch = Match(allMatches.data[i][idCol], "", "", "", // Simplified, need full constructor or setters
                                     allMatches.data[i][t1IdCol], allMatches.data[i][t2IdCol],
                                     allMatches.data[i][statusCol], allMatches.data[i][levelCol]);
                // Actual date/time/round would also be copied if needed by progression
                break;
            }
        }

        if (matchRowIdx != -1) {
            std::cout << "Updating Match: " << currentMatch.match_id << " (" << currentMatch.match_level << ")" << std::endl;
            std::cout << "Team 1: " << currentMatch.team1_id << " vs Team 2: " << currentMatch.team2_id << std::endl;
            
            int score1 = CsvToolkit::getInt("Enter Team 1 Score: ");
            int score2 = CsvToolkit::getInt("Enter Team 2 Score: ");
            char* winnerId_cstr = CsvToolkit::getString("Enter Winner Team ID (or type DRAW if applicable): ");

            std::string s1_str = std::to_string(score1);
            std::string s2_str = std::to_string(score2);

            delete[] allMatches.data[matchRowIdx][t1ScoreCol]; allMatches.data[matchRowIdx][t1ScoreCol] = CsvToolkit::duplicateString(s1_str.c_str());
            delete[] allMatches.data[matchRowIdx][t2ScoreCol]; allMatches.data[matchRowIdx][t2ScoreCol] = CsvToolkit::duplicateString(s2_str.c_str());
            delete[] allMatches.data[matchRowIdx][winnerCol];  allMatches.data[matchRowIdx][winnerCol] = CsvToolkit::duplicateString(winnerId_cstr);
            delete[] allMatches.data[matchRowIdx][statusCol];  allMatches.data[matchRowIdx][statusCol] = CsvToolkit::duplicateString("Completed");
            // Update actual_end_time (optional)
            // char* endTime = CsvToolkit::getString("Enter actual end time (YYYY-MM-DD HH:MM, or blank):");
            // if(strlen(endTime)>0) { /* update allMatches.data[matchRowIdx][actualEndCol] */ } delete[] endTime;

            currentMatch.team1_score = score1; currentMatch.team2_score = score2;
            delete[] currentMatch.winner_team_id; currentMatch.winner_team_id = CsvToolkit::duplicateString(winnerId_cstr);
            delete[] currentMatch.match_status;   currentMatch.match_status   = CsvToolkit::duplicateString("Completed");

            if (CsvToolkit::writeData(MATCH_CSV, allMatches) == 0) {
                std::cout << "Match result updated successfully in " << MATCH_CSV << std::endl;
                handleTeamProgression(currentMatch, ubLosersStack, nextRoundUBQueue, nextRoundLBQueue);
            } else {
                std::cout << "Error writing updated match data to " << MATCH_CSV << std::endl;
            }
            delete[] winnerId_cstr;
        } else {
            std::cout << "Match ID '" << matchIdToUpdate_str << "' not found." << std::endl;
        }
        CsvToolkit::deleteDataContainer2D(allMatches);
        CsvToolkit::getString("Press Enter to continue...");
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
            CsvToolkit::clearTerminal();
            const char* menuOptions[] = {"View Tournament", "Generate Initial Schedule", "Update Match Result", "Return to Main Menu"};
            int choice = CsvToolkit::displayMenu("Match Scheduling & Player Progression", menuOptions, 4);
            switch (choice) {
                case 1: {
                    bool viewRunning = true;
                    while(viewRunning){
                        CsvToolkit::clearTerminal();
                        const char* viewOptions[] = {"View Team Ranking", "View Match Schedule", "View Bracket Progress", "Back"};
                        int viewChoice = CsvToolkit::displayMenu("View Tournament Information", viewOptions, 4);
                        switch(viewChoice){
                            case 1: displayTeamRankings(); break;
                            case 2: displayMatchSchedule(); break;
                            case 3: displayBracketProgress(); break;
                            case 4: viewRunning = false; break;
                            default: CsvToolkit::displaySystemMessage("Invalid choice.", 2); break;
                        }
                    }
                    break;
                }
                case 2: generateInitialSchedule(); break;
                case 3: updateMatchResult(ubLosersStack, nextRoundUBQueue, nextRoundLBQueue); break;
                case 4: running = false; break;
                default: CsvToolkit::displaySystemMessage("Invalid choice.", 2); break;
            }
        }
    }

} 
#endif