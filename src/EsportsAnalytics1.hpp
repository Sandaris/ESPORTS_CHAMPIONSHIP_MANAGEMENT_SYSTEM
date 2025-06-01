#ifndef ESPORTS_ANALYTICS_HPP
#define ESPORTS_ANALYTICS_HPP

#include "File_exe.hpp"
using namespace CsvToolkit;

#include <cstring>
#include <string>

namespace EsportsAnalytics {

    // helper: deep-copy a null-terminated c-string
    // inline char* duplicateString(const char* src) {
    //     if (!src) return nullptr;
    //     size_t len = std::strlen(src);
    //     char* dst = new char[len + 1];
    //     std::strcpy(dst, src);
    //     return dst;
    // }

    // struct to hold one line from match.csv
    // fields correspond to each column:
    //   match_id, scheduled_date, scheduled_time,
    //   actual_start_time, actual_end_time,
    //   team1_id, team2_id, winner_team_id,
    //   team1_score, team2_score, match_status, match_level
    struct MatchOutcome {
        char* match_id;           // unique id for this match
        char* scheduled_date;     // date when match was scheduled
        char* scheduled_time;     // time when match was scheduled
        char* actual_start_time;  // time match actually started
        char* actual_end_time;    // time match ended
        char* team1_id;           // identifier for team1
        char* team2_id;           // identifier for team2
        char* winner_team_id;     // id of the winning team
        int   team1_score;        // score of team1
        int   team2_score;        // score of team2
        char* match_status;       // e.g. "Completed", "Cancelled"
        char* match_level;        // e.g. "Group Stage", "Quarterfinal"

        // default constructor: initialize pointers to nullptr, ints to zero
        MatchOutcome()
            : match_id(nullptr),
              scheduled_date(nullptr),
              scheduled_time(nullptr),
              actual_start_time(nullptr),
              actual_end_time(nullptr),
              team1_id(nullptr),
              team2_id(nullptr),
              winner_team_id(nullptr),
              team1_score(0),
              team2_score(0),
              match_status(nullptr),
              match_level(nullptr)
        {}

        // parameterized constructor: deep-copy each string and set scores
        MatchOutcome(const char* mid,
                     const char* schDate,
                     const char* schTime,
                     const char* asTime,
                     const char* aeTime,
                     const char* t1id,
                     const char* t2id,
                     const char* wtid,
                     int         t1s,
                     int         t2s,
                     const char* mStatus,
                     const char* mLevel)
            : match_id(duplicateString(mid)),
              scheduled_date(duplicateString(schDate)),
              scheduled_time(duplicateString(schTime)),
              actual_start_time(duplicateString(asTime)),
              actual_end_time(duplicateString(aeTime)),
              team1_id(duplicateString(t1id)),
              team2_id(duplicateString(t2id)),
              winner_team_id(duplicateString(wtid)),
              team1_score(t1s),
              team2_score(t2s),
              match_status(duplicateString(mStatus)),
              match_level(duplicateString(mLevel))
        {}

        // copy constructor: deep-copy all fields from another object
        MatchOutcome(const MatchOutcome& other) {
            match_id           = duplicateString(other.match_id);
            scheduled_date     = duplicateString(other.scheduled_date);
            scheduled_time     = duplicateString(other.scheduled_time);
            actual_start_time  = duplicateString(other.actual_start_time);
            actual_end_time    = duplicateString(other.actual_end_time);
            team1_id           = duplicateString(other.team1_id);
            team2_id           = duplicateString(other.team2_id);
            winner_team_id     = duplicateString(other.winner_team_id);
            team1_score        = other.team1_score;
            team2_score        = other.team2_score;
            match_status       = duplicateString(other.match_status);
            match_level        = duplicateString(other.match_level);
        }

        // assignment operator: free existing memory, then deep-copy
        MatchOutcome& operator=(const MatchOutcome& other) {
            if (this == &other) return *this;
            delete[] match_id;
            delete[] scheduled_date;
            delete[] scheduled_time;
            delete[] actual_start_time;
            delete[] actual_end_time;
            delete[] team1_id;
            delete[] team2_id;
            delete[] winner_team_id;
            delete[] match_status;
            delete[] match_level;

            match_id           = duplicateString(other.match_id);
            scheduled_date     = duplicateString(other.scheduled_date);
            scheduled_time     = duplicateString(other.scheduled_time);
            actual_start_time  = duplicateString(other.actual_start_time);
            actual_end_time    = duplicateString(other.actual_end_time);
            team1_id           = duplicateString(other.team1_id);
            team2_id           = duplicateString(other.team2_id);
            winner_team_id     = duplicateString(other.winner_team_id);
            team1_score        = other.team1_score;
            team2_score        = other.team2_score;
            match_status       = duplicateString(other.match_status);
            match_level        = duplicateString(other.match_level);

            return *this;
        }

        // destructor: free any allocated strings
        ~MatchOutcome() {
            delete[] match_id;
            delete[] scheduled_date;
            delete[] scheduled_time;
            delete[] actual_start_time;
            delete[] actual_end_time;
            delete[] team1_id;
            delete[] team2_id;
            delete[] winner_team_id;
            delete[] match_status;
            delete[] match_level;
        }
    };


    // struct to hold one line from player_match_stats.csv (gameStat.csv)
    // fields correspond to columns:
    //   match_id, in-game-name, hero_played, kills, deaths, assists, gpm, xpm
    struct GameStat {
        char* match_id;     // id of the match this stat belongs to
        char* InGameName;   // player's identifier (nickname)
        char* hero_played;  // name of hero or character used
        int   kills;        // number of kills in this match
        int   deaths;       // number of deaths in this match
        int   assists;      // number of assists in this match
        int   gpm;          // gold-per-minute stat
        int   xpm;          // xp-per-minute stat

        // default constructor: pointers to nullptr, ints to zero
        GameStat()
            : match_id(nullptr),
              InGameName(nullptr),
              hero_played(nullptr),
              kills(0),
              deaths(0),
              assists(0),
              gpm(0),
              xpm(0)
        {}

        // parameterized constructor: deep-copy strings, set numeric fields
        GameStat(const char* mid,
                 const char* igname,
                 const char* hero,
                 int         k,
                 int         d,
                 int         a,
                 int         gp,
                 int         xp)
            : match_id(duplicateString(mid)),
              InGameName(duplicateString(igname)),
              hero_played(duplicateString(hero)),
              kills(k),
              deaths(d),
              assists(a),
              gpm(gp),
              xpm(xp)
        {}

        // copy constructor: deep-copy all fields
        GameStat(const GameStat& other) {
            match_id    = duplicateString(other.match_id);
            InGameName  = duplicateString(other.InGameName);
            hero_played = duplicateString(other.hero_played);
            kills       = other.kills;
            deaths      = other.deaths;
            assists     = other.assists;
            gpm         = other.gpm;
            xpm         = other.xpm;
        }

        // assignment operator: free old, then deep-copy
        GameStat& operator=(const GameStat& other) {
            if (this == &other) return *this;
            delete[] match_id;
            delete[] InGameName;
            delete[] hero_played;

            match_id    = duplicateString(other.match_id);
            InGameName  = duplicateString(other.InGameName);
            hero_played = duplicateString(other.hero_played);
            kills       = other.kills;
            deaths      = other.deaths;
            assists     = other.assists;
            gpm         = other.gpm;
            xpm         = other.xpm;
            return *this;
        }

        // destructor: free any allocated strings
        ~GameStat() {
            delete[] match_id;
            delete[] InGameName;
            delete[] hero_played;
        }
    };


    // maximum sizes for our in-memory arrays
    const int MAX_TOTAL_MATCHES = 2000;     // max rows in match.csv
    const int MAX_LOG_STACK     = 2000;     // max “recent matches” to store
    static const int MAX_TOTAL_STATS = 10000; // max rows in player_match_stats.csv

    // arrays and counters for match outcomes
    extern MatchOutcome allMatches[MAX_TOTAL_MATCHES];
    extern int           totalMatchesLoaded;

    // stack (lifo) for recent matches
    extern MatchOutcome loggedStack[MAX_LOG_STACK];
    extern int           stackTopIndex;

    // array and counter for game stats
    extern GameStat allGameStats[MAX_TOTAL_STATS];
    extern int      totalGameStatsLoaded;

    // load match.csv into allMatches[], return number of rows read
    int  loadAllMatchOutcomes(const char* filename);

    // print every loaded match outcome
    void printAllMatchOutcomes();

    // clear out the loggedStack[]
    void clearLoggedStack();

    // push the N most recent matches from allMatches into loggedStack
    void logRecentMatches(int N);

    // print the contents of loggedStack[]
    void printLoggedMatches();

    // load player_match_stats.csv into allGameStats[], return row count
    int  loadAllGameStats(const char* filename);

    // find all entries for a given in-game name and print them
    void printStatsForPlayer(const char* playerName);

    // sort allGameStats[] by kills, gpm, or xpm and display player+metric
    void printAllStatsSortedBy(int metricChoice);

    // main entry point for the “analytics” submenu
    void record_N_Analysis();

    // option 1 in submenu: log new match outcome
    void logNewMatchOutcome();

    // helper struct for option 2 display
    struct RecentMatches {
        void display();
    };

    // option 3 in submenu: view full match history
    void viewFullMatchOutcomes();

    // option 4 in submenu: analyze player performance (4A + 4B)
    void analyzePlayerPerformance();

    // single global instance for calling display()
    extern RecentMatches recentMatchOutcomes;

} // namespace EsportsAnalytics

#endif // ESPORTS_ANALYTICS_HPP
