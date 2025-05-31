#include "spectator_management.hpp" // Your header file with declarations
#include <cstring> // For strcpy, strlen
#include <iostream> // For std::cout, std::endl (mainly for messages, UI through CsvToolkit)

// Constants are already defined in spectator_management.hpp

namespace SpectatorSystem {

    // --- 1. NormalSpectatorQueue Implementations ---
    NormalSpectatorQueue::NormalSpectatorQueue() : frontNode(nullptr), rearNode(nullptr), count(0) {
        // Constructor body
    }

    NormalSpectatorQueue::~NormalSpectatorQueue() {
        while (!isEmpty()) {
            SpectatorNode* temp = dequeue(); // Dequeue handles pointer updates and count
            delete temp; // Delete the node itself
        }
    }

    void NormalSpectatorQueue::enqueue(const char* id, const char* name, const char* arrival) {
        // For normal spectators, type is implicitly "Normal"
        SpectatorNode* newNode = new SpectatorNode(id, name, "Normal", arrival);
        if (isEmpty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        count++;
    }

    SpectatorNode* NormalSpectatorQueue::dequeue() {
        if (isEmpty()) {
            CsvToolkit::displaySystemMessage("Normal Spectator Queue is empty. Cannot dequeue.", 2);
            return nullptr;
        }
        SpectatorNode* temp = frontNode;
        frontNode = frontNode->next;
        if (frontNode == nullptr) { // Queue became empty
            rearNode = nullptr;
        }
        count--;
        temp->next = nullptr; // Decouple the node
        return temp; // Caller is responsible for deleting this node after use
    }

    bool NormalSpectatorQueue::isEmpty() const {
        return count == 0;
    }

    void NormalSpectatorQueue::displayQueue() const {
        std::cout << "\n--- Normal Spectator Queue (" << count << " waiting) ---" << std::endl;
        if (isEmpty()) {
            std::cout << "Queue is empty." << std::endl;
            return;
        }
        SpectatorNode* current = frontNode;
        int pos = 1;
        while (current != nullptr) {
            std::cout << pos++ << ". ID: " << current->spectatorID 
                      << ", Name: " << current->spectatorName 
                      << ", Arrival: " << current->arrivalTime << std::endl;
            current = current->next;
        }
        std::cout << "------------------------------------" << std::endl;
    }
    
    int NormalSpectatorQueue::getCount() const {
        return count;
    }

    // --- 2. VipSpectatorQueue Implementations ---
    VipSpectatorQueue::VipSpectatorQueue() : frontNode(nullptr), rearNode(nullptr), count(0) {
        // Constructor
    }

    VipSpectatorQueue::~VipSpectatorQueue() {
        while (!isEmpty()) {
            SpectatorNode* temp = dequeue();
            delete temp;
        }
    }

    void VipSpectatorQueue::enqueue(const char* id, const char* name, const char* arrival) {
        // For VIPs, type is "VIP". Priority is handled by processing this queue before NormalQueue.
        SpectatorNode* newNode = new SpectatorNode(id, name, "VIP", arrival);
        if (isEmpty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode; // Simple FIFO for VIPs among themselves
        }
        count++;
    }

    SpectatorNode* VipSpectatorQueue::dequeue() {
        if (isEmpty()) {
            CsvToolkit::displaySystemMessage("VIP Spectator Queue is empty. Cannot dequeue.", 2);
            return nullptr;
        }
        SpectatorNode* temp = frontNode;
        frontNode = frontNode->next;
        if (frontNode == nullptr) {
            rearNode = nullptr;
        }
        count--;
        temp->next = nullptr;
        return temp; // Caller responsible for deleting node
    }

    bool VipSpectatorQueue::isEmpty() const {
        return count == 0;
    }

    void VipSpectatorQueue::displayQueue() const {
        std::cout << "\n--- VIP Spectator Queue (" << count << " waiting) ---" << std::endl;
        if (isEmpty()) {
            std::cout << "Queue is empty." << std::endl;
            return;
        }
        SpectatorNode* current = frontNode;
        int pos = 1;
        while (current != nullptr) {
            std::cout << pos++ << ". ID: " << current->spectatorID 
                      << ", Name: " << current->spectatorName
                      << ", Arrival: " << current->arrivalTime << std::endl;
            current = current->next;
        }
        std::cout << "------------------------------------" << std::endl;
    }

    int VipSpectatorQueue::getCount() const {
        return count;
    }

    // --- 3. StreamerCircularQueue Implementations ---
    StreamerCircularQueue::StreamerCircularQueue(int size) : capacity(size), frontIndex(-1), rearIndex(-1), count(0) {
        queueArray = new Streamer[capacity];
    }

    StreamerCircularQueue::~StreamerCircularQueue() {
        delete[] queueArray; // Streamer destructor handles its char*
    }

    bool StreamerCircularQueue::enqueue(const char* id, const char* name, const char* arrival) {
        if (isFull()) {
            CsvToolkit::displaySystemMessage("Streamer Queue is full. Cannot enqueue.", 2);
            return false;
        }
        rearIndex = (rearIndex + 1) % capacity;
        // Clean up existing Streamer object at rearIndex before overwriting
        // This is important if Streamer objects are reused in the circular array.
        // The Streamer destructor will free its old char* members.
        // Then the placement new or assignment operator will allocate new memory.
        queueArray[rearIndex] = Streamer(id, name, arrival); // Uses Streamer constructor then assignment op
                                                              // or direct construction if array stores objects

        if (isEmpty()) { // If queue was empty, front also moves to this new element
            frontIndex = rearIndex;
        }
        count++;
        return true;
    }

    Streamer StreamerCircularQueue::dequeue() {
        if (isEmpty()) {
            CsvToolkit::displaySystemMessage("Streamer Queue is empty. Cannot dequeue.", 2);
            return Streamer(); // Return an empty/default Streamer object
        }
        Streamer frontItem = queueArray[frontIndex]; // Makes a copy (uses Streamer copy constructor)
        
        // To "clear" the dequeued slot, assign a default Streamer.
        // This ensures its destructor is called, freeing old char* memory.
        queueArray[frontIndex] = Streamer(); 

        if (frontIndex == rearIndex) { // Queue becomes empty
            frontIndex = -1;
            rearIndex = -1;
        } else {
            frontIndex = (frontIndex + 1) % capacity;
        }
        count--;
        return frontItem; // Return the copy
    }

    bool StreamerCircularQueue::isEmpty() const {
        return count == 0;
    }

    bool StreamerCircularQueue::isFull() const {
        return count == capacity;
    }

    void StreamerCircularQueue::displayQueue() const {
        std::cout << "\n--- Streamer Queue (" << count << " waiting, Capacity: " << capacity << ") ---" << std::endl;
        if (isEmpty()) {
            std::cout << "Queue is empty." << std::endl;
            return;
        }
        int current = frontIndex;
        for (int i = 0; i < count; ++i) {
            std::cout << (i + 1) << ". ID: " << queueArray[current].streamerID
                      << ", Name: " << queueArray[current].streamerName
                      << ", Arrival: " << queueArray[current].arrivalTime << std::endl;
            current = (current + 1) % capacity;
        }
        std::cout << "------------------------------------" << std::endl;
    }

    int StreamerCircularQueue::getCount() const {
        return count;
    }


    // --- SpectatorManager Implementations ---
    SpectatorManager::SpectatorManager() : vipSeatCount(0), normalSeatCount(0), streamerSlotCount(0) {
        // Initialize queues (their constructors are called automatically)
        // Initialize seating arrays to be empty
        initializeSeating();
        // loadData(); // You might call this here or explicitly from runMainLoop
    }

    SpectatorManager::~SpectatorManager() {
        // Free memory allocated for names in seat/slot arrays
        for (int i = 0; i < VIP_SEAT_CAPACITY; ++i) delete[] vipSeats[i];
        for (int i = 0; i < NORMAL_SEAT_CAPACITY; ++i) delete[] normalSeats[i];
        for (int i = 0; i < STREAMER_SLOT_CAPACITY; ++i) delete[] streamerSlots[i];
    }

    void SpectatorManager::initializeSeating() {
        for (int i = 0; i < VIP_SEAT_CAPACITY; ++i) vipSeats[i] = nullptr;
        for (int i = 0; i < NORMAL_SEAT_CAPACITY; ++i) normalSeats[i] = nullptr;
        for (int i = 0; i < STREAMER_SLOT_CAPACITY; ++i) streamerSlots[i] = nullptr;
        vipSeatCount = 0;
        normalSeatCount = 0;
        streamerSlotCount = 0;
    }

    void SpectatorManager::assignVipSeat(SpectatorNode* vip) {
        if (vipSeatCount < VIP_SEAT_CAPACITY) {
            // Find first empty slot
            for(int i=0; i < VIP_SEAT_CAPACITY; ++i) {
                if(vipSeats[i] == nullptr) {
                    vipSeats[i] = CsvToolkit::duplicateString(vip->spectatorID); // Or name
                    vipSeatCount++;
                    std::cout << "Assigned: " << vip->spectatorName << " (VIP) to VIP Seat #" << (i + 1) << std::endl;
                    return;
                }
            }
        } else {
            std::cout << "VIP seats are full. Cannot seat " << vip->spectatorName << "." << std::endl;
        }
    }

    void SpectatorManager::assignNormalSeat(SpectatorNode* normalSpectator) {
        if (normalSeatCount < NORMAL_SEAT_CAPACITY) {
            for(int i=0; i < NORMAL_SEAT_CAPACITY; ++i) {
                if(normalSeats[i] == nullptr) {
                    normalSeats[i] = CsvToolkit::duplicateString(normalSpectator->spectatorID); // Or name
                    normalSeatCount++;
                    std::cout << "Assigned: " << normalSpectator->spectatorName << " (Normal) to Normal Seat #" << (i + 1) << std::endl;
                    return;
                }
            }
        } else {
            std::cout << "Normal seats are full. Cannot seat " << normalSpectator->spectatorName << "." << std::endl;
        }
    }

    void SpectatorManager::assignStreamerSlot(Streamer streamer) { // streamer is a copy
        if (streamerSlotCount < STREAMER_SLOT_CAPACITY) {
             for(int i=0; i < STREAMER_SLOT_CAPACITY; ++i) {
                if(streamerSlots[i] == nullptr) {
                    streamerSlots[i] = CsvToolkit::duplicateString(streamer.streamerID); // Or name
                    streamerSlotCount++;
                    std::cout << "Assigned: " << streamer.streamerName << " (Streamer) to Slot #" << (i + 1) << std::endl;
                    // The 'streamer' copy will be destructed at the end of this function,
                    // freeing its internal char* members. streamerSlots[i] now holds a new copy.
                    return;
                }
            }
        } else {
            std::cout << "Streamer slots are full. Cannot assign " << streamer.streamerName << "." << std::endl;
        }
    }


    void SpectatorManager::loadData() {
        CsvToolkit::clearTerminal();
        std::cout << "Loading data from CSV files..." << std::endl;

        // Load spectators (VIPs and Normals)
        CsvToolkit::dataContainer2D spectatorData = CsvToolkit::getData("all_spectators.csv");
        if (!spectatorData.error && spectatorData.y > 0) {
            for (int i = 0; i < spectatorData.y; ++i) {
                // Assuming columns: 0:ID, 1:Name, 2:Type, 3:ArrivalTime
                // Add error checking for spectatorData.data[i] and number of columns
                if (spectatorData.data[i] && spectatorData.x >= 4) {
                    char* id = spectatorData.data[i][0];
                    char* name = spectatorData.data[i][1];
                    char* type = spectatorData.data[i][2];
                    char* arrival = spectatorData.data[i][3];

                    if (strcmp(type, "VIP") == 0) {
                        vipQueue.enqueue(id, name, arrival);
                    } else if (strcmp(type, "Normal") == 0) {
                        normalQueue.enqueue(id, name, arrival);
                    }
                }
            }
            std::cout << "Loaded " << spectatorData.y << " spectators." << std::endl;
        } else {
            std::cout << "Warning: Could not load 'all_spectators.csv' or it's empty." << std::endl;
        }
        CsvToolkit::deleteDataContainer2D(spectatorData);

        // Load streamers
        CsvToolkit::dataContainer2D streamerDataContainer = CsvToolkit::getData("streamers.csv");
        if (!streamerDataContainer.error && streamerDataContainer.y > 0) {
            for (int i = 0; i < streamerDataContainer.y; ++i) {
                // Assuming columns: 0:ID, 1:Name, 2:ArrivalTime
                if (streamerDataContainer.data[i] && streamerDataContainer.x >= 3) {
                    char* id = streamerDataContainer.data[i][0];
                    char* name = streamerDataContainer.data[i][1];
                    char* arrival = streamerDataContainer.data[i][2];
                    if(!streamerQueue.enqueue(id, name, arrival)) {
                        std::cout << "Streamer queue full. Could not add: " << name << std::endl;
                    }
                }
            }
            std::cout << "Loaded " << streamerDataContainer.y << " streamers." << std::endl;
        } else {
            std::cout << "Warning: Could not load 'streamers.csv' or it's empty." << std::endl;
        }
        CsvToolkit::deleteDataContainer2D(streamerDataContainer);
        CsvToolkit::displaySystemMessage("Data loading complete.", 2);
    }

    void SpectatorManager::addNewSpectatorMenu() {
        CsvToolkit::clearTerminal();
        std::cout << "--- Add New Spectator/Streamer ---" << std::endl;
        char* id = CsvToolkit::getString("Enter ID: ");
        char* name = CsvToolkit::getString("Enter Name: ");
        // Simple timestamp for now
        char timestamp[20];
        time_t now = time(0);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));


        const char* typeOptions[] = {"VIP", "Normal Spectator", "Streamer"};
        int typeChoice = CsvToolkit::displayMenu("Select Type", typeOptions, 3);

        switch(typeChoice) {
            case 1: // VIP
                vipQueue.enqueue(id, name, timestamp);
                CsvToolkit::displaySystemMessage("VIP added to queue.", 2);
                break;
            case 2: // Normal
                normalQueue.enqueue(id, name, timestamp);
                CsvToolkit::displaySystemMessage("Normal spectator added to queue.", 2);
                break;
            case 3: // Streamer
                if(!streamerQueue.enqueue(id, name, timestamp)) {
                     CsvToolkit::displaySystemMessage("Streamer queue is full. Could not add.", 2);
                } else {
                    CsvToolkit::displaySystemMessage("Streamer added to queue.", 2);
                }
                break;
            default:
                CsvToolkit::displaySystemMessage("Invalid type choice.", 2);
                break;
        }
        // Free memory allocated by CsvToolkit::getString
        delete[] id;
        delete[] name;
    }

    void SpectatorManager::viewCurrentQueues() {
        CsvToolkit::clearTerminal();
        std::cout << "--- Current Queue States ---" << std::endl;
        streamerQueue.displayQueue();
        vipQueue.displayQueue();
        normalQueue.displayQueue();
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // consume leftover newline
        std::cin.get(); // wait for enter
    }

    void SpectatorManager::processAndSeatAll() {
        CsvToolkit::clearTerminal();
        std::cout << "--- Processing Spectators & Assigning Seats/Slots ---" << std::endl;

        std::cout << "\nProcessing Streamers..." << std::endl;
        while(!streamerQueue.isEmpty() && streamerSlotCount < STREAMER_SLOT_CAPACITY) {
            Streamer currentStreamer = streamerQueue.dequeue(); // copy
            assignStreamerSlot(currentStreamer); 
            // 'currentStreamer' goes out of scope, its destructor handles its char*s
        }
        if(streamerQueue.isEmpty() && streamerSlotCount < STREAMER_SLOT_CAPACITY) std::cout << "All streamers in queue processed." << std::endl;
        if(streamerSlotCount == STREAMER_SLOT_CAPACITY) std::cout << "All streamer slots are now full." << std::endl;


        std::cout << "\nProcessing VIPs..." << std::endl;
        while(!vipQueue.isEmpty() && vipSeatCount < VIP_SEAT_CAPACITY) {
            SpectatorNode* currentVip = vipQueue.dequeue();
            assignVipSeat(currentVip);
            delete currentVip; // Free the node after processing
        }
        if(vipQueue.isEmpty() && vipSeatCount < VIP_SEAT_CAPACITY) std::cout << "All VIPs in queue processed." << std::endl;
        if(vipSeatCount == VIP_SEAT_CAPACITY) std::cout << "All VIP seats are now full." << std::endl;


        std::cout << "\nProcessing Normal Spectators..." << std::endl;
        while(!normalQueue.isEmpty() && normalSeatCount < NORMAL_SEAT_CAPACITY) {
            SpectatorNode* currentNormal = normalQueue.dequeue();
            assignNormalSeat(currentNormal);
            delete currentNormal; // Free the node after processing
        }
        if(normalQueue.isEmpty() && normalSeatCount < NORMAL_SEAT_CAPACITY) std::cout << "All normal spectators in queue processed." << std::endl;
        if(normalSeatCount == NORMAL_SEAT_CAPACITY) std::cout << "All normal seats are now full." << std::endl;

        std::cout << "\n--- Seating Process Complete ---" << std::endl;
        std::cout << "VIP Seats Occupied: " << vipSeatCount << "/" << VIP_SEAT_CAPACITY << std::endl;
        std::cout << "Normal Seats Occupied: " << normalSeatCount << "/" << NORMAL_SEAT_CAPACITY << std::endl;
        std::cout << "Streamer Slots Occupied: " << streamerSlotCount << "/" << STREAMER_SLOT_CAPACITY << std::endl;

        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    void SpectatorManager::runMainLoop() {
        loadData(); // Load initial data from CSVs

        bool taskRunning = true;
        while (taskRunning) {
            CsvToolkit::clearTerminal();
            const char* task3Options[] = {
                "Add New Spectator/Streamer",
                "View Current Waiting Queues",
                "Process & Seat All Waiting",
                "View Assigned Seats/Slots", // Added feature
                "Return to Main Menu"
            };
            int choice = CsvToolkit::displayMenu("Live Stream & Spectator Management Menu", task3Options, 5);

            switch (choice) {
                case 1:
                    addNewSpectatorMenu();
                    break;
                case 2:
                    viewCurrentQueues();
                    break;
                case 3:
                    processAndSeatAll();
                    break;
                case 4: // View Assigned Seats (Simple display)
                    {
                        CsvToolkit::clearTerminal();
                        std::cout << "--- Assigned Seats & Slots ---" << std::endl;
                        std::cout << "\nVIP Seats (" << vipSeatCount << "/" << VIP_SEAT_CAPACITY << "):" << std::endl;
                        for(int i=0; i < VIP_SEAT_CAPACITY; ++i) if(vipSeats[i]) std::cout << "Seat " << (i+1) << ": " << vipSeats[i] << std::endl; else std::cout << "Seat " << (i+1) << ": EMPTY" << std::endl;
                        
                        std::cout << "\nNormal Seats (" << normalSeatCount << "/" << NORMAL_SEAT_CAPACITY << "):" << std::endl;
                        for(int i=0; i < NORMAL_SEAT_CAPACITY; ++i) if(normalSeats[i]) std::cout << "Seat " << (i+1) << ": " << normalSeats[i] << std::endl; else std::cout << "Seat " << (i+1) << ": EMPTY" << std::endl;

                        std::cout << "\nStreamer Slots (" << streamerSlotCount << "/" << STREAMER_SLOT_CAPACITY << "):" << std::endl;
                        for(int i=0; i < STREAMER_SLOT_CAPACITY; ++i) if(streamerSlots[i]) std::cout << "Slot " << (i+1) << ": " << streamerSlots[i] << std::endl; else std::cout << "Slot " << (i+1) << ": EMPTY" << std::endl;
                        
                        std::cout << "\nPress Enter to continue...";
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cin.get();
                    }
                    break;
                case 5:
                    taskRunning = false;
                    break;
                default:
                    CsvToolkit::displaySystemMessage("Invalid choice, please try again.", 2);
                    break;
            }
        }
    }

} // namespace SpectatorSystem