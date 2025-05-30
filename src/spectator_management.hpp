// spectator_management.hpp
#ifndef SPECTATOR_MANAGEMENT_HPP
#define SPECTATOR_MANAGEMENT_HPP

#include <iostream> // For basic cout for debugging if needed
#include "File_exe.hpp" // Your friend's CSV toolkit

// Define capacities based on your previous decision
const int VIP_SEAT_CAPACITY = 10;
const int NORMAL_SEAT_CAPACITY = 50;
const int STREAMER_SLOT_CAPACITY = 10;

namespace SpectatorSystem {

    // --- Node Structure for Linked List Queues (VIPs and Normal) ---
    struct SpectatorNode {
        char* spectatorID;
        char* spectatorName;
        char* spectatorType; // "VIP" or "Normal"
        char* arrivalTime;
        // int priority; // Could be used if multiple VIP levels, for now "VIP" type implies priority
        SpectatorNode* next;

        // Constructor for easy node creation
        SpectatorNode(const char* id, const char* name, const char* type, const char* arrival) {
            spectatorID = CsvToolkit::duplicateString(id);
            spectatorName = CsvToolkit::duplicateString(name);
            spectatorType = CsvToolkit::duplicateString(type);
            arrivalTime = CsvToolkit::duplicateString(arrival);
            next = nullptr;
        }

        // Destructor to free allocated memory
        ~SpectatorNode() {
            delete[] spectatorID;
            delete[] spectatorName;
            delete[] spectatorType;
            delete[] arrivalTime;
        }
    };

    // --- Data Structure for Streamer Circular Queue ---
    struct Streamer {
        char* streamerID;
        char* streamerName;
        char* arrivalTime;

        // Default constructor
        Streamer() : streamerID(nullptr), streamerName(nullptr), arrivalTime(nullptr) {}
        
        // Parameterized constructor
        Streamer(const char* id, const char* name, const char* arrival) {
            streamerID = CsvToolkit::duplicateString(id);
            streamerName = CsvToolkit::duplicateString(name);
            arrivalTime = CsvToolkit::duplicateString(arrival);
        }

        // Copy constructor for returning by value from dequeue
        Streamer(const Streamer& other) {
            streamerID = CsvToolkit::duplicateString(other.streamerID);
            streamerName = CsvToolkit::duplicateString(other.streamerName);
            arrivalTime = CsvToolkit::duplicateString(other.arrivalTime);
        }

        // Assignment operator
        Streamer& operator=(const Streamer& other) {
            if (this == &other) return *this;
            delete[] streamerID;
            delete[] streamerName;
            delete[] arrivalTime;
            streamerID = CsvToolkit::duplicateString(other.streamerID);
            streamerName = CsvToolkit::duplicateString(other.streamerName);
            arrivalTime = CsvToolkit::duplicateString(other.arrivalTime);
            return *this;
        }
        
        // Destructor
        ~Streamer() {
            delete[] streamerID;
            delete[] streamerName;
            delete[] arrivalTime;
        }
    };

    // --- 1. Standard Queue for Normal Spectators (Linked List) ---
    class NormalSpectatorQueue {
    private:
        SpectatorNode* frontNode;
        SpectatorNode* rearNode;
        int count;

    public:
        NormalSpectatorQueue();
        ~NormalSpectatorQueue();
        void enqueue(const char* id, const char* name, const char* arrival);
        SpectatorNode* dequeue(); // Returns node to get data, caller must delete node if dynamic
        bool isEmpty() const;
        void displayQueue() const; // You might use CsvToolkit::displayTabulatedData indirectly
        int getCount() const;
    };

    // --- 2. Priority Queue for VIPs (Linked List, simple version) ---
    // For this task, all VIPs have higher priority than Normal.
    // Among VIPs, it's FIFO. So, it's like a separate VIP_FIFO_Queue.
    class VipSpectatorQueue {
    private:
        SpectatorNode* frontNode;
        SpectatorNode* rearNode; // VIPs are added here, dequeued from front
        int count;

    public:
        VipSpectatorQueue();
        ~VipSpectatorQueue();
        void enqueue(const char* id, const char* name, const char* arrival);
        SpectatorNode* dequeue();
        bool isEmpty() const;
        void displayQueue() const;
        int getCount() const;
    };

    // --- 3. Circular Queue for Streamers (Array) ---
    class StreamerCircularQueue {
    private:
        Streamer* queueArray;
        int capacity;
        int frontIndex;
        int rearIndex;
        int count;

    public:
        StreamerCircularQueue(int size = STREAMER_SLOT_CAPACITY);
        ~StreamerCircularQueue();
        bool enqueue(const char* id, const char* name, const char* arrival);
        Streamer dequeue(); // Returns a copy of the Streamer object
        bool isEmpty() const;
        bool isFull() const;
        void displayQueue() const;
        int getCount() const;
    };

    // --- Main Management Class for Task 3 ---
    class SpectatorManager {
    private:
        NormalSpectatorQueue normalQueue;
        VipSpectatorQueue vipQueue;
        StreamerCircularQueue streamerQueue;

        // Arrays to represent seats/slots (pointers to hold names/IDs)
        char* vipSeats[VIP_SEAT_CAPACITY];
        int vipSeatCount;
        char* normalSeats[NORMAL_SEAT_CAPACITY];
        int normalSeatCount;
        char* streamerSlots[STREAMER_SLOT_CAPACITY];
        int streamerSlotCount;

        void initializeSeating(); // Helper to set seat arrays to empty
        void assignVipSeat(SpectatorNode* vip);
        void assignNormalSeat(SpectatorNode* normalSpectator);
        void assignStreamerSlot(Streamer streamer);


    public:
        SpectatorManager();
        ~SpectatorManager(); // To free seat/slot name memory

        void loadData(); // Will call CsvToolkit::getData and populate queues
        void addNewSpectatorMenu(); // For user to add a new spectator interactively
        void viewCurrentQueues();   // Displays all three queues
        void processAndSeatAll();   // Main logic: dequeues and assigns seats/slots in order
        
        void runMainLoop(); // Handles the main menu
    };

} // namespace SpectatorSystem

#endif // SPECTATOR_MANAGEMENT_HPP