#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>

using namespace std;

// --- โครงสร้างข้อมูล ---
struct Row {
    bool isPlanted = false;
    string plantName = "";
    int plantTypeIndex = 0;
    int age = 0; 
    bool isWatered = false;
    bool isRotten = false;
    bool hasFertilizer = false; 
    int sprinklerDays = 0;      
};

struct Quest {
    string targetPlant = "Carrot";
    int targetAge = 1;
    int rewardMoney = 150;
    bool isActive = false;
};

// --- ตัวแปรหลัก ---
string playerName = "Farmer";
int D = 1; 
int M = 100; 
int G = 0;      // <--- เริ่มต้นเป็น 0 ตามที่แจ้งครับ
int AP = 5;
const int MAX_AP = 5;
const int RENT_AMOUNT = 500; 
const int RENT_CYCLE = 7;
int debt = 0; 
int rentStrikes = 0; 
int expansionCost = 5;

// ไอเทมพิเศษ
bool hasCCTV = false;
bool hasScarecrow = false;
bool hasLottery = false;

string plantNames[] = {"Carrot", "Corn", "Pumpkin"};
int plantCosts[] = {20, 50, 100};   
int plantSells[] = {40, 120, 300};  
int baseSuccess[] = {80, 60, 40}; 

vector<Row> rows(3); 
Quest currentQuest;

// --- ฟังก์ชันช่วยเหลือ ---
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void waitEnter() {
    cout << "\n> Press Enter to continue...";
    cin.ignore(1000, '\n'); cin.get();    
}