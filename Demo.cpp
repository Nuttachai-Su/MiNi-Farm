#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>

using namespace std;

const int PLANT_TYPES = 3;
string plantNames[] = {"Carrot", "Corn", "Pumpkin"};
int plantCosts[] = {20, 50, 100};   
int plantSells[] = {40, 120, 300};  
int baseSuccess[] = {80, 60, 40}; 

const int ITEM_TYPES = 5;
string itemNames[] = {"Fertilizer", "Sprinkler", "CCTV", "Scarecrow", "Lottery"};
int itemCosts[] = {50, 150, 200, 150, 50};

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

string playerName = "Farmer";
int D = 1; 
int M = 100; 
int G = 0;
int AP = 5;
const int MAX_AP = 5;
const int RENT_AMOUNT = 500; 
const int RENT_CYCLE = 7;
int debt = 0; 
int rentStrikes = 0; 
int expansionCost = 5;

bool hasCCTV = false;
bool hasScarecrow = false;
bool hasLottery = false;

vector<Row> rows(3); 
Quest currentQuest;

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

void generateQuest() {
    int pIdx = rand() % 3;
    currentQuest.targetPlant = plantNames[pIdx];
    currentQuest.targetAge = (rand() % 3) + 1;
    currentQuest.rewardMoney = (pIdx * 60) + 150;
    currentQuest.isActive = true;
}

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

void loadGame(string code) {
    try {
        replace(code.begin(), code.end(), '_', ' ');
        stringstream ss(code); 
        int rowCount;
        ss >> playerName >> D >> M >> G >> debt >> rentStrikes >> rowCount;
        
        string qD; ss >> qD; replace(qD.begin(), qD.end(), '.', ' ');
        stringstream qss(qD); qss >> currentQuest.targetPlant >> currentQuest.targetAge >> currentQuest.rewardMoney;

        string iD; ss >> iD; replace(iD.begin(), iD.end(), '.', ' ');
        stringstream iss(iD); iss >> hasCCTV >> hasScarecrow >> hasLottery;

        rows.clear();

        for(int i = 0; i < rowCount; i++) {
            string rD; ss >> rD; replace(rD.begin(), rD.end(), '.', ' ');
            stringstream rss(rD); Row r; int pIdx;
            rss >> pIdx >> r.age >> r.isWatered >> r.isRotten >> r.hasFertilizer >> r.sprinklerDays;
            if(pIdx != -1) { 
                r.isPlanted = true; 
                r.plantTypeIndex = pIdx; 
                r.plantName = plantNames[pIdx]; 
            }
            rows.push_back(r);
        } 
        expansionCost = 5 + ((rows.size() - 3) * 5);
        cout << "\n[Success] Welcome back, " << playerName << "!"; waitEnter();
    } catch (...) { 
        cout << "\n[Error] Invalid Code! Starting new game."; 
        waitEnter(); 
    }
}

void processNightEvents() {
    clearScreen();
    cout << "________________________________________________________________" << endl;
    cout << " [ NIGHT REPORT - DAY " << D << " ]" << endl;
    cout << "________________________________________________________________" << endl;
    
    int event = rand() % 100;
    if (event < 15) {
        cout << " > Luck: It rained! All plants are watered (+20% Success tomorrow)." << endl;
        for(auto &r : rows) if(r.isPlanted) r.isWatered = true;
    } else if (event < 25) {
        if (hasScarecrow) cout << " > Defense: Wild animals came, but the Scarecrow scared them off!" << endl;
        else {
            int target = rand() % rows.size();
            if (rows[target].isPlanted) { rows[target].isPlanted = false; cout << " > Danger: Animals ate your crop in Row " << target+1 << "!" << endl; }
            else cout << " > Info: Animals visited but there was nothing to eat." << endl;
        }
    } else if (event < 35) {
        if (hasCCTV) cout << " > Defense: A thief spotted your CCTV and fled!" << endl;
        else {
            int loss = (rand() % 151) + 50; M = max(0, M - loss);
            cout << " > Danger: A thief stole $" << loss << " from your wallet!" << endl;
        }
    } else cout << " > Peace: A quiet night at the farm." << endl;

    if (hasLottery) {
        if (rand() % 10 == 0) { M += 1000; cout << " > !!! WINNER: You won the Lottery jackpot $1000! !!!" << endl; }
        else cout << " > Lottery: Better luck next time!" << endl;
        hasLottery = false;
    }
    cout << "________________________________________________________________" << endl;
}

void drawInterface() {
    stringstream ss;
    int dRent = RENT_CYCLE - (D % RENT_CYCLE);
    if (D % RENT_CYCLE == 0) dRent = 0;

    ss << "________________________________________________________________\n";
    ss << " [ PLAYER: " << left << setw(12) << playerName << " ]  DAY: " << setw(4) << D << " | MONEY: " << setw(6) << M << "\n";
    ss << " [ STATUS ]  GEMS: " << left << setw(4) << G << " | DEBT: $" << setw(5) << debt << " | STRIKES: " << rentStrikes << "/3\n";
    ss << " [ GEAR ]    CCTV: " << (hasCCTV?"ON":"OFF") << " | Scarecrow: " << (hasScarecrow?"ON":"OFF") << " | Lottery: " << (hasLottery?"Owned":"-") << "\n";
    
    if (dRent == 0) ss << " !!! ALERT: RENT DUE TODAY: $" << RENT_AMOUNT + debt << "\n";
    else ss << " [ INFO ]    Rent collection in: " << dRent << " days\n";
    ss << "________________________________________________________________\n\n";

    for (int i = 0; i < (int)rows.size(); i++) {
        string vis = rows[i].isPlanted ? (rows[i].isRotten ? "[X] [X] [X]" : "{@} {@} {@}") : "[ ] [ ] [ ]";
        string info = rows[i].isPlanted ? rows[i].plantName : "Empty";
        if (rows[i].isRotten) info = "!!! ROTTEN !!!";
        else {
            if (rows[i].isWatered) info += " (Wet)";
            if (rows[i].hasFertilizer) info += " [Fert]";
            if (rows[i].sprinklerDays > 0) info += " [Spr:" + to_string(rows[i].sprinklerDays) + "]";
        }
        ss << "  " << i+1 << ")  " << vis << "    Plant: " << left << setw(20) << info << " | Age: " << (rows[i].isPlanted ? to_string(rows[i].age) : "-") << "\n";
        ss << "      ------------------------------------------------------\n";
    }

    ss << "\n [ QUEST ] Need: " << currentQuest.targetPlant << " (Age:" << currentQuest.targetAge << ") Reward: $" << currentQuest.rewardMoney << "\n";
    ss << "________________________________________________________________\n\n";
    ss << " [ ACTIONS ]  AP: " << AP << "/" << MAX_AP << " | Expand: " << expansionCost << " Gems\n";
    
    string menus[] = {"Plant", "Harvest", "Water", "Shop", "Index", "Quest", "End Day", "Stop"};
    for(int i=0; i<8; i++) {
        ss << "  " << i+1 << ") " << left << setw(11) << menus[i];
        if((i+1)%2==0) ss << "\n";
    }
    ss << "________________________________________________________________\n Command: ";

    clearScreen();
    cout << ss.str();
}

void showIndex() {
    clearScreen();
    cout << "________________________________________________________________" << endl;
    cout << " [ FARMER'S HANDBOOK ]" << endl;
    cout << "________________________________________________________________" << endl;
    cout << "  PLANTS INFO:" << endl;
    for(int i=0; i<3; i++) {
        cout << "  - " << left << setw(8) << plantNames[i] << " Cost: $" << setw(4) << plantCosts[i] 
             << " | Base Sell: $" << setw(4) << plantSells[i] << " | Success: " << baseSuccess[i] << "%" << endl;
    }
    cout << "\n  SUCCESS BONUSES:" << endl;
    cout << "  - Watering: +20% | Fertilizer: +15% | Young Crop: -40%" << endl;
    cout << "\n  HOW TO GET GEMS:" << endl;
    cout << "  - Complete Quests (Option 6) to receive 2 Gems per quest." << endl;
    cout << "________________________________________________________________" << endl;
    waitEnter();
}

void endDay() {
    if (D % RENT_CYCLE == 0) {
        int total = RENT_AMOUNT + debt;
        if (M >= total) { M -= total; debt = 0; rentStrikes = 0; }
        else { debt = total - M; M = 0; rentStrikes++; if (rentStrikes >= 3) { cout << "GAME OVER!"; exit(0); } }
    }
    processNightEvents();
    D++; AP = MAX_AP; M += 50;
    for(auto &r : rows) {
        if (r.isPlanted) {
            r.age++; r.isWatered = false;
            if (r.sprinklerDays > 0) { r.isWatered = true; r.sprinklerDays--; }
            if (rand() % 100 < 10) r.isRotten = true;
        } else if (r.sprinklerDays > 0) r.sprinklerDays--;
    }
    waitEnter();
}

void planting() {
    if (AP <= 0) return;
    int r; cout << "Row: "; cin >> r; r--;
    if (r >= 0 && r < (int)rows.size() && !rows[r].isPlanted) {
        cout << "1.Carrot($20) 2.Corn($50) 3.Pumpkin($100): ";
        int p; cin >> p; p--;
        if (p >= 0 && p <= 2 && M >= plantCosts[p]) { 
            M -= plantCosts[p]; rows[r].isPlanted = true; rows[r].plantName = plantNames[p];
            rows[r].plantTypeIndex = p; rows[r].age = 0; rows[r].isRotten = false; AP--;
        }
    }
}

void harvesting() {
    if (AP <= 0) return;
    int r; cout << "Row: "; cin >> r; r--;
    if (r >= 0 && r < (int)rows.size() && rows[r].isPlanted) {
        int ch = baseSuccess[rows[r].plantTypeIndex] + (rows[r].isWatered ? 20 : 0) + (rows[r].hasFertilizer ? 15 : 0);
        if (rows[r].age < 2) ch -= 40;
        if (!rows[r].isRotten && (rand() % 100) < ch) {
            int s = plantSells[rows[r].plantTypeIndex] + (rows[r].age * 5); M += s;
            cout << "[Success] Sold for $" << s;
        } else cout << "[Fail] Crop lost!";
        rows[r].isPlanted = false; rows[r].hasFertilizer = false; AP--; waitEnter();
    }
}

void openShop() {
    clearScreen();
    cout << "--- SHOP --- (Money: $" << M << " | Gems: " << G << ")" << endl;
    cout << " 1) Fertilizer($50)  2) Sprinkler($150)  3) Expand(" << expansionCost << "G)" << endl;
    cout << " 4) CCTV($200)       5) Scarecrow($150)  6) Lottery($50)" << endl;
    cout << " 0) Exit" << endl;
    int sc; cin >> sc;
    if(sc==1){ int r; cout<<"Row: "; cin>>r; if(M>=50){M-=50; rows[r-1].hasFertilizer=true;} }
    else if(sc==2){ int r; cout<<"Row: "; cin>>r; if(M>=150){M-=150; rows[r-1].sprinklerDays=3;} }
    else if(sc==3){ if(G >= expansionCost){G -= expansionCost; rows.push_back(Row()); expansionCost += 5;} }
    else if(sc==4){ if(M>=200){M-=200; hasCCTV=true;} }
    else if(sc==5){ if(M>=150){M-=150; hasScarecrow=true;} }
    else if(sc==6){ if(M>=50){M-=50; hasLottery=true;} }
}

void howToplay() {

    cout << "========================================\n";
    cout << "           MINI FARM - GUIDE            \n";
    cout << "========================================\n\n";

    cout << "GAME SUMMARY\n";
    cout << "Mini Farm is a farming simulation game where you:\n";
    cout << "- Plant crops (Carrot, Corn, Pumpkin)\n";
    cout << "- Take care of them (water, fertilize, protect)\n";
    cout << "- Harvest and sell crops to earn money\n";
    cout << "- Complete quests to earn gems\n";
    cout << "- Pay rent every 7 days\n";
    cout << "- Survive random night events\n\n";

    cout << "----------------------------------------\n";
    cout << "HOW TO PLAY (STEP BY STEP)\n";
    cout << "----------------------------------------\n\n";

    cout << "STEP 1: Plant Crops\n";
    cout << "Choose option 1 (Plant).\n";
    cout << "Select a row number.\n";
    cout << "Choose a plant:\n";
    cout << "1 = Carrot ($20)\n";
    cout << "2 = Corn ($50)\n";
    cout << "3 = Pumpkin ($100)\n\n";

    cout << "STEP 2: Take Care of Crops\n";
    cout << "- Option 3: Water crops (+20% success chance).\n";
    cout << "- Option 4: Shop (buy fertilizer, sprinkler, CCTV, etc).\n\n";

    cout << "STEP 3: Harvest Crops\n";
    cout << "Choose option 2 (Harvest).\n";
    cout << "Success depends on water, fertilizer, age, and luck.\n";
    cout << "If successful, you earn money.\n\n";

    cout << "STEP 4: Complete Quests\n";
    cout << "Choose option 6 (Quest).\n";
    cout << "Deliver the correct plant with required age.\n";
    cout << "Reward: Money + 2 Gems.\n\n";

    cout << "STEP 5: End the Day\n";
    cout << "Choose option 7 (End Day).\n";
    cout << "- Plants grow (age +1)\n";
    cout << "- Random night event occurs\n";
    cout << "- You earn $50 daily income\n";
    cout << "- AP resets to 5\n\n";

    cout << "STEP 6: Pay Rent\n";
    cout << "Every 7 days you must pay $500 + debt.\n";
    cout << "If you fail 3 times, Game Over.\n\n";

    cout << "STEP 7: Expand Farm\n";
    cout << "Use gems in the shop to add more rows.\n\n";

    cout << "STEP 8: Save Game\n";
    cout << "Choose option 8 (Stop) to get save code.\n\n";

    cout << "----------------------------------------\n";
    cout << "GOOD LUCK, FARMER!\n";
    cout << "----------------------------------------\n";
}


int main() {
    srand(time(0)); clearScreen();
    cout << "========================================" << endl;
    cout << "       WELCOME TO THE MINI FARM         " << endl;
    cout << "========================================" << endl;
    cout << " Have you played before? (y/n): "; char ch; cin >> ch;
    if (ch == 'y' || ch == 'Y') { cout << "Enter Save Code: "; string code; cin >> code; loadGame(code); }
    else { cout << "Enter Farmer Name: "; cin >> playerName; generateQuest(); 
        clearScreen();
        howToplay();
        waitEnter();
    }

    while (true) {
        drawInterface();
        int c; if (!(cin >> c)) { cin.clear(); cin.ignore(1000, '\n'); continue; }
        switch (c) {
            case 1: planting(); break;
            case 2: harvesting(); break;
            case 3: { int r; cout << "Row: "; cin >> r; r--; if(r>=0 && r<(int)rows.size()){rows[r].isWatered=true; AP--;} break; }
            case 4: openShop(); break;
            case 5: showIndex(); break;
            case 6: {
                bool found = false;
                for(auto &r : rows) if(r.isPlanted && !r.isRotten && r.plantName == currentQuest.targetPlant && r.age == currentQuest.targetAge){
                    M += currentQuest.rewardMoney; G += 2; r.isPlanted = false; 
                    cout << "\n[Success] Quest Done! Gem +2"; generateQuest(); found = true; waitEnter(); break;
                } 
                if(!found) { cout << "\n[!] No matching crop."; waitEnter(); }
                break;
            }
            case 7: endDay(); break;
            case 8: stopGame(); break;
        }
    }
    return 0;
}