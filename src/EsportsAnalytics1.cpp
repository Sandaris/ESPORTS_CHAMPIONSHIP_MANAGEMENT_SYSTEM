#include "EsportsAnalytics1.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstring>    // for std::strcmp

namespace EsportsAnalytics {

    // globals for storing all match outcomes in memory
    // allMatches[i] holds the i-th match read from match.csv
    MatchOutcome allMatches[MAX_TOTAL_MATCHES];
    int           totalMatchesLoaded = 0;

    // stack (lifo) for recently logged matches
    // loggedStack[0] is the most recent, stackTopIndex tracks count
    MatchOutcome loggedStack[MAX_LOG_STACK];
    int           stackTopIndex     = 0;

    // globals for storing all game stats in memory
    // allGameStats[i] holds the i-th row from player_match_stats.csv
    GameStat allGameStats[MAX_TOTAL_STATS];
    int      totalGameStatsLoaded = 0;

    // a single global to allow calling recentMatchOutcomes.display()
    RecentMatches recentMatchOutcomes;


    // loadAllMatchOutcomes: read every line from 'filename' (match.csv)
    // skip header, parse 12 comma-separated fields per line,
    // convert scores to ints, then store in allMatches array
    int loadAllMatchOutcomes(const char* filename) {
        std::ifstream inFile(filename);
        if (!inFile.is_open()) {
            std::cerr << "ERROR: Cannot open " << filename << "\n";
            return -1;
        }

        // skip header row
        std::string line;
        if (!std::getline(inFile, line)) {
            std::cerr << "ERROR: " << filename << " appears empty.\n";
            return -1;
        }

        int idx = 0;
        while (idx < MAX_TOTAL_MATCHES && std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string tokens[12];

            // split line into exactly 12 tokens
            for (int i = 0; i < 12; ++i) {
                if (!std::getline(ss, tokens[i], ',')) {
                    tokens[i].clear();
                }
            }

            // convert the two score fields to integers
            int t1s = std::stoi(tokens[8]);
            int t2s = std::stoi(tokens[9]);

            // construct a MatchOutcome with deep-copied strings
            MatchOutcome mo(
                tokens[0].c_str(),  // match_id
                tokens[1].c_str(),  // scheduled_date
                tokens[2].c_str(),  // scheduled_time
                tokens[3].c_str(),  // actual_start_time
                tokens[4].c_str(),  // actual_end_time
                tokens[5].c_str(),  // team1_id
                tokens[6].c_str(),  // team2_id
                tokens[7].c_str(),  // winner_team_id
                t1s,                // team1_score
                t2s,                // team2_score
                tokens[10].c_str(), // match_status
                tokens[11].c_str()  // match_level
            );

            allMatches[idx] = mo;  // store in global array
            ++idx;
        }

        totalMatchesLoaded = idx;
        inFile.close();
        return totalMatchesLoaded;
    }


    // printAllMatchOutcomes: iterate over allMatches[] and display each one
    // shows id, date, time, teams with scores, winner, status, and level
    void printAllMatchOutcomes() {
        std::cout << "loaded " << totalMatchesLoaded << " matches:\n";
        for (int i = 0; i < totalMatchesLoaded; ++i) {
            const MatchOutcome& m = allMatches[i];
            std::cout
                << "[" << (i+1) << "] "
                << "id="   << (m.match_id          ? m.match_id          : "") << "  "
                << "date=" << (m.scheduled_date    ? m.scheduled_date    : "") << "  "
                << "time=" << (m.scheduled_time    ? m.scheduled_time    : "") << "  "
                << "teams="<< (m.team1_id          ? m.team1_id          : "") << "(" << m.team1_score << ") vs "
                          << (m.team2_id          ? m.team2_id          : "") << "(" << m.team2_score << ")  "
                << "winner=" << (m.winner_team_id  ? m.winner_team_id   : "") << "  "
                << "status=" << (m.match_status    ? m.match_status      : "") << "  "
                << "level="  << (m.match_level     ? m.match_level       : "")
                << "\n";
        }
    }


    // clearLoggedStack: explicitly destroy each MatchOutcome in loggedStack[]
    // and reset the stackTopIndex to 0 (empty stack)
    void clearLoggedStack() {
        for (int i = 0; i < stackTopIndex; ++i) {
            loggedStack[i].~MatchOutcome();
        }
        stackTopIndex = 0;
    }


    // logRecentMatches: push the N most recent matches from allMatches[] into loggedStack[]
    // N should be <= totalMatchesLoaded; if larger, take all matches
    // the newest matches come from the end of allMatches[], pushing in order
    void logRecentMatches(int N) {
        clearLoggedStack();
        if (totalMatchesLoaded == 0 || N <= 0) {
            return;
        }

        int toTake = (N < totalMatchesLoaded ? N : totalMatchesLoaded);
        for (int i = 0; i < toTake; ++i) {
            // allMatches[totalMatchesLoaded - 1] is the newest, then -2, etc.
            const MatchOutcome& src = allMatches[totalMatchesLoaded - 1 - i];
            new (&loggedStack[i]) MatchOutcome(src);
        }
        stackTopIndex = toTake;
    }


    // printLoggedMatches: if the stack is empty, show a message; otherwise, print each entry
    // shows in newest-first order because logRecentMatches stored them that way
    void printLoggedMatches() {
        if (stackTopIndex == 0) {
            std::cout << "no matches have been logged yet.\n";
            return;
        }

        std::cout << "=== recently logged matches (newest first) ===\n";
        for (int i = 0; i < stackTopIndex; ++i) {
            const MatchOutcome& m = loggedStack[i];
            std::cout
                << "[" << (i+1) << "] "
                << "id="   << (m.match_id          ? m.match_id          : "") << "  "
                << "date=" << (m.scheduled_date    ? m.scheduled_date    : "") << "  "
                << "time=" << (m.scheduled_time    ? m.scheduled_time    : "") << "  "
                << "teams="<< (m.team1_id          ? m.team1_id          : "") << "(" << m.team1_score << ") vs "
                          << (m.team2_id          ? m.team2_id          : "") << "(" << m.team2_score << ")  "
                << "winner=" << (m.winner_team_id  ? m.winner_team_id   : "") << "  "
                << "status=" << (m.match_status    ? m.match_status      : "") << "  "
                << "level="  << (m.match_level     ? m.match_level       : "")
                << "\n";
        }
    }


    // loadAllGameStats: read every line from 'filename' (player_match_stats.csv)
    // skip header, parse 8 comma-separated fields, convert numbers, store in allGameStats[]
    int loadAllGameStats(const char* filename) {
        std::ifstream inFile(filename);
        if (!inFile.is_open()) {
            std::cerr << "ERROR: Cannot open " << filename << "\n";
            return -1;
        }

        // skip header row
        std::string line;
        if (!std::getline(inFile, line)) {
            std::cerr << "ERROR: " << filename << " appears empty.\n";
            return -1;
        }

        int idx = 0;
        while (idx < MAX_TOTAL_STATS && std::getline(inFile, line)) {
            std::stringstream ss(line);
            std::string tokens[8];

            // split into exactly 8 fields:
            // match_id, in-game-name, hero_played, kills, deaths, assists, gpm, xpm
            for (int i = 0; i < 8; ++i) {
                if (!std::getline(ss, tokens[i], ',')) {
                    tokens[i].clear();
                }
            }

            // convert numeric fields
            int kills   = std::stoi(tokens[3]);
            int deaths  = std::stoi(tokens[4]);
            int assists = std::stoi(tokens[5]);
            int gpm     = std::stoi(tokens[6]);
            int xpm     = std::stoi(tokens[7]);

            // construct a GameStat with deep-copied strings
            GameStat gs(
                tokens[0].c_str(),  // match_id
                tokens[1].c_str(),  // in-game-name
                tokens[2].c_str(),  // hero_played
                kills,
                deaths,
                assists,
                gpm,
                xpm
            );

            allGameStats[idx] = gs;  // store in global array
            ++idx;
        }

        totalGameStatsLoaded = idx;
        inFile.close();
        return totalGameStatsLoaded;
    }


    // printStatsForPlayer: look through allGameStats[] for entries matching playerName
    // for each match, find the corresponding MatchOutcome by match_id and print combined info
    void printStatsForPlayer(const char* playerName) {
        if (!playerName || totalGameStatsLoaded == 0) {
            std::cout << "no player stats loaded or invalid name.\n";
            return;
        }

        bool foundAny = false;
        for (int i = 0; i < totalGameStatsLoaded; ++i) {
            const GameStat &gs = allGameStats[i];
            if (gs.InGameName && std::strcmp(gs.InGameName, playerName) == 0) {
                // found a matching stat; now find match details by match_id
                const char* mid = gs.match_id;
                const MatchOutcome* moPtr = nullptr;
                for (int j = 0; j < totalMatchesLoaded; ++j) {
                    if (allMatches[j].match_id &&
                        std::strcmp(allMatches[j].match_id, mid) == 0)
                    {
                        moPtr = &allMatches[j];
                        break;
                    }
                }

                // print the joined information
                std::cout << "match=" << (mid ? mid : "") << "  ";
                if (moPtr) {
                    std::cout
                        << "teams=" << (moPtr->team1_id ? moPtr->team1_id : "") << "(" << moPtr->team1_score << ") vs "
                                  << (moPtr->team2_id ? moPtr->team2_id : "") << "(" << moPtr->team2_score << ")  "
                                  << "winner=" << (moPtr->winner_team_id ? moPtr->winner_team_id : "") << "  ";
                }
                std::cout
                    << "hero=" << (gs.hero_played ? gs.hero_played : "") << "  "
                    << "k/d/a=" << gs.kills << "/" << gs.deaths << "/" << gs.assists << "  "
                    << "gpm=" << gs.gpm << "  "
                    << "xpm=" << gs.xpm
                    << "\n";

                foundAny = true;
            }
        }

        if (!foundAny) {
            std::cout << "no records found for player \"" << playerName << "\".\n";
        }
    }


    // isHigher: helper to compare two GameStat entries by chosen metric
    // returns true if 'a' has a larger value than 'b' for kills/gpm/xpm
    bool isHigher(const GameStat &a, const GameStat &b, int metricChoice) {
        switch (metricChoice) {
            case 1: // kills
                return (a.kills > b.kills);
            case 2: // gpm
                return (a.gpm > b.gpm);
            case 3: // xpm
                return (a.xpm > b.xpm);
            default:
                return false;
        }
    }

    // heapifyDown: maintain max-heap property at index i within heapArr[0..size-1]
    // if a child is larger than the current node (by metricChoice), swap downwards
    void heapifyDown(GameStat heapArr[], int size, int i, int metricChoice) {
        while (true) {
            int left  = 2 * i + 1;  // left child index
            int right = 2 * i + 2;  // right child index
            int largest = i;

            // compare with left child
            if (left < size && isHigher(heapArr[left], heapArr[largest], metricChoice)) {
                largest = left;
            }
            // compare with right child
            if (right < size && isHigher(heapArr[right], heapArr[largest], metricChoice)) {
                largest = right;
            }

            if (largest != i) {
                // swap current node with larger child
                GameStat tmp = heapArr[i];
                heapArr[i] = heapArr[largest];
                heapArr[largest] = tmp;
                i = largest;  // continue heapifying at new position
            } else {
                break;  // subtree is now a valid max-heap
            }
        }
    }

    // buildMaxHeap: convert an unsorted array heapArr[0..n-1] into a max-heap
    // by calling heapifyDown on each non-leaf node, bottom-up
    void buildMaxHeap(GameStat heapArr[], int n, int metricChoice) {
        for (int i = (n / 2) - 1; i >= 0; --i) {
            heapifyDown(heapArr, n, i, metricChoice);
        }
    }

    // printAllStatsSortedBy: use array-based max-heap to sort by kills/gpm/xpm
    // only prints player name plus the chosen metric
    void printAllStatsSortedBy(int metricChoice) {
        if (totalGameStatsLoaded == 0) {
            std::cout << "no game stats loaded.\n";
            return;
        }

        int n = totalGameStatsLoaded;
        // make a local copy so the original allGameStats[] stays intact
        static GameStat heapArr[MAX_TOTAL_STATS];
        for (int i = 0; i < n; ++i) {
            heapArr[i] = allGameStats[i];
        }

        // build a max-heap over heapArr
        buildMaxHeap(heapArr, n, metricChoice);

        // print header depending on metricChoice
        if (metricChoice == 1)
            std::cout << "=== player kills (descending) ===\n";
        else if (metricChoice == 2)
            std::cout << "=== player gpm (descending) ===\n";
        else
            std::cout << "=== player xpm (descending) ===\n";

        // repeatedly extract the top element (largest) and print just name+metric
        int heapSize = n;
        int rank = 1;
        while (heapSize > 0) {
            GameStat gs = heapArr[0];  // root of heap = max element

            // print only the in-game name and chosen metric
            if (metricChoice == 1) {
                std::cout << "[" << rank++ << "] "
                          << (gs.InGameName ? gs.InGameName : "")
                          << "  kills=" << gs.kills << "\n";
            }
            else if (metricChoice == 2) {
                std::cout << "[" << rank++ << "] "
                          << (gs.InGameName ? gs.InGameName : "")
                          << "  gpm=" << gs.gpm << "\n";
            }
            else { // metricChoice == 3
                std::cout << "[" << rank++ << "] "
                          << (gs.InGameName ? gs.InGameName : "")
                          << "  xpm=" << gs.xpm << "\n";
            }

            // remove the root by replacing it with the last element
            heapArr[0] = heapArr[heapSize - 1];
            --heapSize;

            // re-heapify downward from root
            if (heapSize > 0) {
                heapifyDown(heapArr, heapSize, 0, metricChoice);
            }
        }
    }


    // logNewMatchOutcome: option 1 in submenu
    // loads match.csv, shows all matches, prompts how many to log,
    // then builds the loggedStack via logRecentMatches()
    void logNewMatchOutcome() {
        int loaded = loadAllMatchOutcomes("../data/match.csv");
        if (loaded < 0) {
            displaySystemMessage("failed to load match.csv", 2);
            return;
        }

        clearTerminal();
        printAllMatchOutcomes();

        int N = getInt("how many of the latest results would you like to log?: ");
        logRecentMatches(N);

        displaySystemMessage("result successfully logged.", 2);
    }


    // RecentMatches::display: option 2 in submenu
    // simply prints whatever is currently in loggedStack[]
    void RecentMatches::display() {
        clearTerminal();
        printLoggedMatches();
        getString("press enter to return to analytics menu...");
    }


    // viewFullMatchOutcomes: option 3 in submenu
    // reloads match.csv and prints all matches in file order
    void viewFullMatchOutcomes() {
        int loaded = loadAllMatchOutcomes("../data/match.csv");
        if (loaded < 0) {
            displaySystemMessage("failed to load match.csv", 2);
            return;
        }

        clearTerminal();
        printAllMatchOutcomes();
        getString("press enter to return to analytics menu...");
    }


    // analyzePlayerPerformance: option 4 in submenu
    // first loads both match.csv and gameStat.csv,
    // then presents a small sub-menu: 4A = lookup by name, 4B = sort by metric
    void analyzePlayerPerformance() {
        int loadedMatches = loadAllMatchOutcomes("../data/match.csv");
        if (loadedMatches < 0) {
            displaySystemMessage("failed to load match.csv", 2);
            return;
        }
        int loadedStats = loadAllGameStats("../data/gameStat.csv");
        if (loadedStats < 0) {
            displaySystemMessage("failed to load gameStat.csv", 2);
            return;
        }

        bool subRunning = true;
        while (subRunning) {
            clearTerminal();
            const char* subOpts[] = {
                "lookup by player name",
                "sort player stats by metric (kills/gpm/xpm)",
                "return to analytics menu"
            };
            int subChoice = displayMenu("analyze player performance", subOpts, 3);

            switch (subChoice) {
                case 1: {
                    // 4a: prompt for in-game name, then print that player’s stats
                    clearTerminal();
                    char* nameBuf = getString("enter an in-game name: ");
                    clearTerminal();
                    printStatsForPlayer(nameBuf);
                    delete[] nameBuf;   // free the returned buffer
                    getString("press enter to return to analysis menu...");
                    break;
                }
                case 2: {
                    // 4b: prompt which metric to sort by, then print sorted list
                    clearTerminal();
                    const char* sortOpts[] = {
                        "by kills (descending)",
                        "by gpm   (descending)",
                        "by xpm   (descending)",
                        "back"
                    };
                    int metric = displayMenu("sort player stats by…", sortOpts, 4);
                    if (metric >= 1 && metric <= 3) {
                        clearTerminal();
                        printAllStatsSortedBy(metric);
                        getString("press enter to return to analysis menu...");
                    }
                    break;
                }
                default:
                    subRunning = false;
                    break;
            }
        }
    }


    // record_N_Analysis: main entry point for the analytics submenu (options 1–4)
    // loops until user chooses to return to main menu
    void record_N_Analysis() {
        bool running = true;
        while (running) {
            clearTerminal();
            const char* menuOptions[] = {
                "log new match outcome",           // option 1
                "view recent match outcomes",      // option 2
                "view full match outcomes history",// option 3
                "analyze player performance",      // option 4
                "return to main menu"              // option 5
            };
            int choice = displayMenu("game result logging & performance history", menuOptions, 5);

            switch (choice) {
                case 1: logNewMatchOutcome();         break;
                case 2: recentMatchOutcomes.display();break;
                case 3: viewFullMatchOutcomes();      break;
                case 4: analyzePlayerPerformance();   break;
                case 5: running = false;              break;
                default: displaySystemMessage("invalid choice, please try again.", 2); break;
            }
        }
        clearTerminal();
        std::cout << "exiting game result logging & performance history module." << std::endl;
        getString("press enter to return to main apuec system menu...");
    }

} // namespace EsportsAnalytics
