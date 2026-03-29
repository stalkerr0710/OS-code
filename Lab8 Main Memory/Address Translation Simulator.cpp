#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <bitset>

using namespace std;

const int NUM_PAGES = 64;
const int NUM_FRAMES = 32;
const int PAGE_SIZE = 1024;
const int OFFSET_BITS = 10;  // log2(1024)
const int PAGE_BITS = 6;     // log2(64)

class PageTable {
private:
    int pageTable[NUM_PAGES];
    bool valid[NUM_PAGES];

public:
    PageTable() {
        srand(time(0));
        bool usedFrames[NUM_FRAMES] = {false};
        int framesUsed = 0;
        
        // สุ่มกำหนดค่าใน Page Table [cite: 823]
        for (int i = 0; i < NUM_PAGES; i++) {
            if (framesUsed < NUM_FRAMES && rand() % 100 < 75) { // กำหนดให้ 75% เป็น Valid
                int frame;
                do {
                    frame = rand() % NUM_FRAMES;
                } while (usedFrames[frame]);
                pageTable[i] = frame;
                usedFrames[frame] = true;
                valid[i] = true;
                framesUsed++;
            } else {
                pageTable[i] = -1;
                valid[i] = false;
            }
        }
    }

    int translateAddress(int logicalAddress) {
        int pageNumber = logicalAddress / PAGE_SIZE; [cite: 827]
        int offset = logicalAddress % PAGE_SIZE;      [cite: 828]

        if (pageNumber < 0 || pageNumber >= NUM_PAGES) {
            cout << "Error: Invalid page number " << pageNumber << endl; [cite: 829]
            return -1;
        }

        if (!valid[pageNumber]) {
            cout << "Page Fault: Page " << pageNumber << " is not in memory" << endl; [cite: 830]
            return -1;
        }

        int frameNumber = pageTable[pageNumber]; [cite: 831]
        int physicalAddress = (frameNumber * PAGE_SIZE) + offset; [cite: 832]

        cout << "Logical Address: " << logicalAddress << endl; [cite: 833]
        cout << "  Page Number: " << pageNumber << endl;      [cite: 834]
        cout << "  Offset: " << offset << endl;               [cite: 835]
        cout << "  Frame Number: " << frameNumber << endl;    [cite: 836]
        cout << "Physical Address: " << physicalAddress << endl;
        return physicalAddress;
    }

    void displayPageTable() {
        cout << "\n=== PAGE TABLE ===" << endl;
        cout << setw(10) << "Page#" << setw(10) << "Frame#" << setw(10) << "Valid" << endl;
        for (int i = 0; i < NUM_PAGES; i++) {
            cout << setw(10) << i 
                 << setw(10) << (valid[i] ? to_string(pageTable[i]) : "N/A") 
                 << setw(10) << (valid[i] ? "Yes" : "No") << endl; [cite: 839]
        }
    }
};

int main() {
    PageTable pt;
    int testAddresses[] = {0, 1024, 2048, 5120, 65535}; [cite: 844]
    
    for (int addr : testAddresses) {
        cout << "\n--- Translation Test ---" << endl;
        pt.translateAddress(addr); [cite: 846]
    }
    
    pt.displayPageTable(); [cite: 846]
    return 0;
}