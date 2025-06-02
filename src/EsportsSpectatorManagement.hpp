#ifndef ESPORTS_SPECTATOR_MANAGEMENT_HPP
#define ESPORTS_SPECTATOR_MANAGEMENT_HPP

#include <iostream>
#include <cstring>
#include <cstdlib>
#include "File_exe.hpp"  

using namespace CsvToolkit;

// --- Constants ---
static const int MAX_SPECTATORS        = 200;
static const int VIP_SEATED_CAP        = 5;
static const int VIP_WAITING_CAP       = 20;
static const int STREAMER_SEATED_CAP   = 3;
static const int STREAMER_WAITING_CAP  = 15;
static const int GENERAL_SEATED_CAP    = 50;
static const int GENERAL_WAITING_CAP   = 100;
static const int ACTION_STACK_CAPACITY = 200;

// --- Types & Enums ---
enum ZoneType {
    VIP_ZONE,
    STREAMER_ZONE,
    GENERAL_ZONE
};

struct Spectator {
    char*       id;          
    char*       name;        
    ZoneType    zone;        
    bool        checkedIn;   
    bool        alreadySeated;

    Spectator()
        : id(nullptr), name(nullptr),
          zone(GENERAL_ZONE),
          checkedIn(false),
          alreadySeated(false)
    {}

    void init(const char* _id, const char* _name, ZoneType _zone) {
        id = new char[std::strlen(_id) + 1];
        std::strcpy(id, _id);
        name = new char[std::strlen(_name) + 1];
        std::strcpy(name, _name);
        zone = _zone;
        checkedIn = false;
        alreadySeated = false;
    }

    ~Spectator() {
        delete[] id;
        delete[] name;
    }
};

struct RegistrationAction {
    Spectator*  spec;
    bool        wasSeated;
    ZoneType    zone;
};

struct SpectatorQueue {
    Spectator** arr;
    int capacity;
    int front_idx;
    int rear_idx;
    int count;

    SpectatorQueue(int cap = 10)
        : capacity(cap), front_idx(0), rear_idx(-1), count(0)
    {
        arr = new Spectator*[capacity];
    }
    ~SpectatorQueue() {
        delete[] arr;
    }
    bool isEmpty() const { return count == 0; }
    bool isFull()  const { return count == capacity; }

    bool enqueue(Spectator* s) {
        if (isFull()) return false;
        rear_idx = (rear_idx + 1) % capacity; // wrap around if needed
        arr[rear_idx] = s;                    // place at rear
        count++;
        return true;
    }
    Spectator* dequeue() {
        if (isEmpty()) return nullptr;
        Spectator* s = arr[front_idx];              // grab front item
        front_idx = (front_idx + 1) % capacity;      // advance front
        count--;
        return s;
    }
    int size() const { return count; }

    Spectator* at(int i) const {
        if (i < 0 || i >= count) return nullptr;
        int idx = (front_idx + i) % capacity; // compute circular index
        return arr[idx];
    }
};

struct SpectatorStack {
    RegistrationAction* arr;
    int capacity;
    int top_idx;

    SpectatorStack(int cap = ACTION_STACK_CAPACITY)
        : capacity(cap), top_idx(-1)
    {
        arr = new RegistrationAction[capacity];
    }
    ~SpectatorStack() {
        delete[] arr;
    }
    bool isEmpty() const { return top_idx < 0; }
    bool isFull()  const { return top_idx + 1 == capacity; }

    bool push(const RegistrationAction& act) {
        if (isFull()) return false;
        arr[++top_idx] = act;
        return true;
    }
    RegistrationAction pop() {
        if (isEmpty()) {
            RegistrationAction dummy{nullptr,false,GENERAL_ZONE};
            return dummy;
        }
        return arr[top_idx--];
    }
};

class ZoneManager {
public:
    SpectatorQueue* vipSeated;
    SpectatorQueue* vipWaiting;
    SpectatorQueue* streamSeated;
    SpectatorQueue* streamWaiting;
    SpectatorQueue* genSeated;
    SpectatorQueue* genWaiting;
    SpectatorStack* undoStack;

    ZoneManager() {
        vipSeated      = new SpectatorQueue(VIP_SEATED_CAP);
        vipWaiting     = new SpectatorQueue(VIP_WAITING_CAP);
        streamSeated   = new SpectatorQueue(STREAMER_SEATED_CAP);
        streamWaiting  = new SpectatorQueue(STREAMER_WAITING_CAP);
        genSeated      = new SpectatorQueue(GENERAL_SEATED_CAP);
        genWaiting     = new SpectatorQueue(GENERAL_WAITING_CAP);
        undoStack      = new SpectatorStack(ACTION_STACK_CAPACITY);
    }
    ~ZoneManager() {
        delete vipSeated;    delete vipWaiting;
        delete streamSeated; delete streamWaiting;
        delete genSeated;    delete genWaiting;
        delete undoStack;
    }

    void preAssignToSeat(Spectator* s) {
        bool seated = false;
        switch (s->zone) {
            case VIP_ZONE:
                if (!vipSeated->isFull()) {
                    vipSeated->enqueue(s);
                    s->checkedIn = true;
                    s->alreadySeated = true;
                    seated = true;
                }
                break;
            case STREAMER_ZONE:
                if (!streamSeated->isFull()) {
                    streamSeated->enqueue(s);
                    s->checkedIn = true;
                    s->alreadySeated = true;
                    seated = true;
                }
                break;
            case GENERAL_ZONE:
                if (!genSeated->isFull()) {
                    genSeated->enqueue(s);
                    s->checkedIn = true;
                    s->alreadySeated = true;
                    seated = true;
                }
                break;
        }
        if (!seated) {
            switch (s->zone) {
                case VIP_ZONE:      vipWaiting->enqueue(s);    break;
                case STREAMER_ZONE: streamWaiting->enqueue(s); break;
                case GENERAL_ZONE:  genWaiting->enqueue(s);    break;
            }
            s->checkedIn = false;
            s->alreadySeated = true;
        }
    }

    bool registerSpectator(Spectator* s) {
        if (s->checkedIn || s->alreadySeated) return false;
        bool seated = false;
        switch (s->zone) {
            case VIP_ZONE:
                // Try to seat into VIP first
                if (!vipSeated->isFull()) {
                    vipSeated->enqueue(s);
                    seated = true;
                }
                // Otherwise go to VIP waiting
                else if (!vipWaiting->isFull()) {
                    vipWaiting->enqueue(s);
                    seated = false;
                } else {
                    return false; // both full
                }
                break;
            case STREAMER_ZONE:
                if (!streamSeated->isFull()) {
                    streamSeated->enqueue(s);
                    seated = true;
                } else if (!streamWaiting->isFull()) {
                    streamWaiting->enqueue(s);
                    seated = false;
                } else {
                    return false;
                }
                break;
            case GENERAL_ZONE:
                if (!genSeated->isFull()) {
                    genSeated->enqueue(s);
                    seated = true;
                } else if (!genWaiting->isFull()) {
                    genWaiting->enqueue(s);
                    seated = false;
                } else {
                    return false;
                }
                break;
        }

        s->checkedIn = seated;
        s->alreadySeated = true;

        // PUSH EXACTLY ONE ACTION FOR EVERY REGISTER CALL.
        // (We do NOT push promotions or any other change.)
        RegistrationAction act{ s, seated, s->zone };
        undoStack->push(act);

        return true;
    }

    bool undoRegistration() {
        if (undoStack->isEmpty()) return false;
        RegistrationAction act = undoStack->pop();
        Spectator* s = act.spec;
        ZoneType z = act.zone;

        if (act.wasSeated) {
            // If they were seated, remove them from seated queue,
            // then promote the next waiting (if any).
            switch (z) {
                case VIP_ZONE:
                    removeFromQueue(vipSeated, s);
                    promoteFromWaiting(vipWaiting, vipSeated);
                    break;
                case STREAMER_ZONE:
                    removeFromQueue(streamSeated, s);
                    promoteFromWaiting(streamWaiting, streamSeated);
                    break;
                case GENERAL_ZONE:
                    removeFromQueue(genSeated, s);
                    promoteFromWaiting(genWaiting, genSeated);
                    break;
            }
        } else {
            // If they were in waiting, just remove from waiting.
            switch (z) {
                case VIP_ZONE:
                    removeFromQueue(vipWaiting, s);
                    break;
                case STREAMER_ZONE:
                    removeFromQueue(streamWaiting, s);
                    break;
                case GENERAL_ZONE:
                    removeFromQueue(genWaiting, s);
                    break;
            }
        }

        s->checkedIn = false;
        s->alreadySeated = false;
        return true;
    }

    void printStatus() const {
        std::cout << "\n=== Zone Status & Name Lists ===\n";
        printZoneList("VIP", vipSeated, VIP_SEATED_CAP, vipWaiting, VIP_WAITING_CAP);
        printZoneList("Streamer", streamSeated, STREAMER_SEATED_CAP, streamWaiting, STREAMER_WAITING_CAP);
        printZoneList("General", genSeated, GENERAL_SEATED_CAP, genWaiting, GENERAL_WAITING_CAP);
    }

    // --- New Function: Remove first from seated, then promote from waiting ---
    void checkOutAndPromote(ZoneType z) {
        Spectator* removed = nullptr;
        switch (z) {
            case VIP_ZONE:
                if (!vipSeated->isEmpty()) {
                    removed = vipSeated->dequeue(); // FIFO dequeue
                }
                if (removed) removed->checkedIn = false;
                if (!vipWaiting->isEmpty() && !vipSeated->isFull()) {
                    Spectator* nxt = vipWaiting->dequeue(); // FIFO dequeue from waiting
                    vipSeated->enqueue(nxt);                 // FIFO enqueue to seated
                    nxt->checkedIn = true;
                    // **DO NOT** push this promotion onto undoStack — we only undo original registrations.
                }
                break;
            case STREAMER_ZONE:
                if (!streamSeated->isEmpty()) {
                    removed = streamSeated->dequeue();
                }
                if (removed) removed->checkedIn = false;
                if (!streamWaiting->isEmpty() && !streamSeated->isFull()) {
                    Spectator* nxt = streamWaiting->dequeue();
                    streamSeated->enqueue(nxt);
                    nxt->checkedIn = true;
                    // no push to undoStack
                }
                break;
            case GENERAL_ZONE:
                if (!genSeated->isEmpty()) {
                    removed = genSeated->dequeue();
                }
                if (removed) removed->checkedIn = false;
                if (!genWaiting->isEmpty() && !genSeated->isFull()) {
                    Spectator* nxt = genWaiting->dequeue();
                    genSeated->enqueue(nxt);
                    nxt->checkedIn = true;
                    // no push to undoStack
                }
                break;
        }

        std::cout << "\n";
        if (removed) {
            std::cout << "Checked out: " << removed->name 
                      << " (" << removed->id << ") from ";
            if (z == VIP_ZONE)          std::cout << "VIP.\n";
            else if (z == STREAMER_ZONE) std::cout << "Streamer.\n";
            else                        std::cout << "General.\n";
        } else {
            std::cout << "Nothing to check out in that zone (no one seated).\n";
        }
    }

private:
    void removeFromQueue(SpectatorQueue* q, Spectator* s) {
        if (!q || q->isEmpty()) return;
        int n = q->capacity;
        Spectator** temp = new Spectator*[n];
        int cnt = 0;
        // Drain queue, skipping the spectator we want to remove
        while (!q->isEmpty()) {
            Spectator* cur = q->dequeue();
            if (cur != s) {
                temp[cnt++] = cur;
            }
        }
        // Re‐enqueue everyone else in original order
        for (int i = 0; i < cnt; i++) {
            q->enqueue(temp[i]);
        }
        delete[] temp;
    }

    void promoteFromWaiting(SpectatorQueue* waiting, SpectatorQueue* seated) {
        if (!waiting || !seated) return;
        if (!waiting->isEmpty() && !seated->isFull()) {
            Spectator* nxt = waiting->dequeue();
            seated->enqueue(nxt);
            nxt->checkedIn = true;
            // **Do not push** onto undoStack here
        }
    }

    void printZoneList(const char* zoneName,
                       SpectatorQueue* seatedQ, int seatedCap,
                       SpectatorQueue* waitingQ, int waitingCap) const
    {
        std::cout << "\n" << zoneName << ":\n";
        std::cout << "  Seated (" << seatedQ->size() << "/" << seatedCap << "):\n";
        if (seatedQ->size() == 0) {
            std::cout << "    (none)\n";
        } else {
            for (int i = 0; i < seatedQ->size(); ++i) {
                Spectator* s = seatedQ->at(i);
                std::cout << "    - " << s->name << " (" << s->id << ")\n";
            }
        }
        std::cout << "  Waiting (" << waitingQ->size() << "/" << waitingCap << "):\n";
        if (waitingQ->size() == 0) {
            std::cout << "    (none)\n";
        } else {
            for (int i = 0; i < waitingQ->size(); ++i) {
                Spectator* s = waitingQ->at(i);
                std::cout << "    - " << s->name << " (" << s->id << ")\n";
            }
        }
    }
};

static int getColumnIndex(const dataContainer2D& dc, const char* name) {
    if (dc.error || !dc.fields) return -1;
    for (int i = 0; i < dc.x; ++i) {
        if (dc.fields[i] && std::strcmp(dc.fields[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

static int loadSpectatorsCSV(const char* filename, Spectator allSpecs[], int& outCount) {
    dataContainer2D dc = getData(filename);
    if (dc.error) return -1;
    int idCol   = getColumnIndex(dc, "spectator_id");
    int nameCol = getColumnIndex(dc, "spectator_name");
    int typeCol = getColumnIndex(dc, "spectator_type");
    int checkCol= getColumnIndex(dc, "check_in");
    if (idCol < 0 || nameCol < 0 || typeCol < 0 || checkCol < 0) {
        deleteDataContainer2D(dc);
        return -1;
    }
    int count = 0;
    for (int i = 0; i < dc.y && count < MAX_SPECTATORS; ++i) {
        char* idv    = dc.data[i][idCol];
        char* namev  = dc.data[i][nameCol];
        char* typev  = dc.data[i][typeCol];
        char* checkv = dc.data[i][checkCol];
        ZoneType z = GENERAL_ZONE;
        if (std::strcmp(typev, "VIP") == 0)            z = VIP_ZONE;
        else if (std::strcmp(typev, "Influencer") == 0) z = STREAMER_ZONE;
        else                                           z = GENERAL_ZONE;
        allSpecs[count].init(idv, namev, z);
        allSpecs[count].checkedIn = (std::strcmp(checkv, "1") == 0);
        allSpecs[count].alreadySeated = false;
        count++;
    }
    outCount = count;
    deleteDataContainer2D(dc);
    return 0;
}

static int loadSeatAssignmentsCSV(const char* filename, Spectator allSpecs[], int allCount, ZoneManager& zm) {
    dataContainer2D dc = getData(filename);
    if (dc.error) return -1;
    int specIdCol = getColumnIndex(dc, "spectator_id");
    int sectionCol= getColumnIndex(dc, "section_name");
    if (specIdCol < 0 || sectionCol < 0) {
        deleteDataContainer2D(dc);
        return -1;
    }
    for (int i = 0; i < dc.y; ++i) {
        char* sid = dc.data[i][specIdCol];
        char* sec = dc.data[i][sectionCol];
        ZoneType z = GENERAL_ZONE;
        if (std::strstr(sec, "VIP"))            z = VIP_ZONE;
        else if (std::strstr(sec, "Influencer")) z = STREAMER_ZONE;
        else                                      z = GENERAL_ZONE;
        for (int j = 0; j < allCount; ++j) {
            if (std::strcmp(allSpecs[j].id, sid) == 0) {
                zm.preAssignToSeat(&allSpecs[j]);
                break;
            }
        }
    }
    deleteDataContainer2D(dc);
    return 0;
}

static void populateRemainingSpectators(Spectator allSpecs[], int allCount, ZoneManager& zm) {
    for (int i = 0; i < allCount; ++i) {
        Spectator* s = &allSpecs[i];
        if (s->alreadySeated) continue;
        bool seated = false;
        switch (s->zone) {
            case VIP_ZONE:
                if (!zm.vipSeated->isFull()) {
                    zm.vipSeated->enqueue(s);
                    s->checkedIn = true;
                    seated = true;
                }
                break;
            case STREAMER_ZONE:
                if (!zm.streamSeated->isFull()) {
                    zm.streamSeated->enqueue(s);
                    s->checkedIn = true;
                    seated = true;
                }
                break;
            case GENERAL_ZONE:
                if (!zm.genSeated->isFull()) {
                    zm.genSeated->enqueue(s);
                    s->checkedIn = true;
                    seated = true;
                }
                break;
        }
        if (!seated) {
            switch (s->zone) {
                case VIP_ZONE:      zm.vipWaiting->enqueue(s);     break;
                case STREAMER_ZONE: zm.streamWaiting->enqueue(s);  break;
                case GENERAL_ZONE:  zm.genWaiting->enqueue(s);     break;
            }
            s->checkedIn = false;
        }
        s->alreadySeated = true;
    }
}

static void manageSpectatorManagement() {
    ZoneManager zm;
    static Spectator allSpecs[MAX_SPECTATORS];
    int allCount = 0;

    if (loadSpectatorsCSV("spectators.csv", allSpecs, allCount) < 0) {
        std::cerr << "Failed to load spectators.csv\n";
        return;
    }
    loadSeatAssignmentsCSV("seatAssignment.csv", allSpecs, allCount, zm);
    populateRemainingSpectators(allSpecs, allCount, zm);

    bool inMenu = true;
    while (inMenu) {
        clearTerminal();
        std::printf("\n=== Task 3: Live Stream & Spectator Management ===\n");
        std::printf("1) Add a new spectator (register)\n");
        std::printf("2) Check out & promote (VIP)\n");
        std::printf("3) Check out & promote (Streamer)\n");
        std::printf("4) Check out & promote (General)\n");
        std::printf("5) Show current status\n");
        std::printf("6) Undo last registration\n");
        std::printf("7) Exit Task 3\n");
        std::printf("Choose (1-7): ");

        int ch = getInt(nullptr);
        switch (ch) {
            case 1: {
                if (allCount >= MAX_SPECTATORS) {
                    std::printf("Cannot register more—max %d reached.\n", MAX_SPECTATORS);
                    getString("Press Enter to continue...");
                    break;
                }
                char* idv   = getString("Enter Spectator ID: ");
                char* namev = getString("Enter Spectator Name: ");
                std::printf("Select zone (1=VIP, 2=Streamer, 3=General): ");
                int zsel = getInt(nullptr);
                ZoneType z = (zsel == 1 ? VIP_ZONE : (zsel == 2 ? STREAMER_ZONE : GENERAL_ZONE));

                Spectator* s = &allSpecs[allCount++];
                s->init(idv, namev, z);
                s->checkedIn = false;
                s->alreadySeated = false;
                bool ok = zm.registerSpectator(s);
                if (!ok) {
                    std::printf("Zone and waiting full—registration failed.\n");
                }
                delete[] idv; delete[] namev;
                getString("Press Enter to continue...");
                break;
            }
            case 2:
                zm.checkOutAndPromote(VIP_ZONE);
                getString("Press Enter to continue...");
                break;
            case 3:
                zm.checkOutAndPromote(STREAMER_ZONE);
                getString("Press Enter to continue...");
                break;
            case 4:
                zm.checkOutAndPromote(GENERAL_ZONE);
                getString("Press Enter to continue...");
                break;
            case 5:
                zm.printStatus();
                getString("Press Enter to continue...");
                break;
            case 6:
                if (!zm.undoRegistration()) {
                    std::printf("Nothing to undo.\n");
                }
                getString("Press Enter to continue...");
                break;
            case 7:
                inMenu = false;
                break;
            default:
                std::printf("Invalid choice.\n");
                getString("Press Enter to continue...");
        }
    }
}

#endif // ESPORTS_SPECTATOR_MANAGEMENT_HPP
