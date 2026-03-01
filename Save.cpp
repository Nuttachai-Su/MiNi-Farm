#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

// ---  ข้อมูลพื้นฐาน  ---
const int PLANT_TYPES = 3;
string plantNames[] = {"Carrot", "Corn", "Pumpkin"};
int plantCosts[]    = {20, 50, 100};   
int plantSells[]    = {40, 120, 300};  
int baseSuccess[]   = {80, 60, 40}; 

const int ITEM_TYPES = 5;
string itemNames[] = {"Fertilizer", "Sprinkler", "CCTV", "Scarecrow", "Lottery"};
int itemCosts[]    = {50, 150, 200, 150, 50};


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
int D = 1, M = 100, G = 0, AP = 5;
const int MAX_AP = 5, RENT_AMOUNT = 500, RENT_CYCLE = 7;
int debt = 0, rentStrikes = 0, expansionCost = 5;
bool hasCCTV = false, hasScarecrow = false, hasLottery = false;

vector<Row> rows(3); 
Quest currentQuest;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// --- ระบบ Save ---
void stopGame() {
    stringstream ss;
    ss << playerName << "_" << D << "_" << M << "_" << G << "_" << debt << "_" << rentStrikes << "_" << rows.size();
    ss << "_" << currentQuest.targetPlant << "." << currentQuest.targetAge << "." << currentQuest.rewardMoney;
    ss << "_" << hasCCTV << "." << hasScarecrow << "." << hasLottery;
    for(const auto& r : rows) {
        ss << "_" << (r.isPlanted ? r.plantTypeIndex : -1) << "." << r.age << "." 
           << r.isWatered << "." << r.isRotten << "." << r.hasFertilizer << "." << r.sprinklerDays;
    }
    clearScreen();
    cout << "________________________________________________________________\n [ GAME STOPPED ]\n Code: " << ss.str() << "\n________________________________________________________________\n";
    exit(0);
}

int main(){

    // --test code : nemo_4_380_2_0_0_3_Pumpkin.3.2700.0.0-1.2.0.0.0.0_0.2.0.0.0.0_0.2.0.0.0.0--
    cout << " Enter Save Code: "; string code; cin >> code;
    
    stopGame();

    cout <<  playerName << " "<< D <<" "<< M <<" "<< G <<" "<< debt <<" "<< rentStrikes ;
}