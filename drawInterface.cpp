#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

//--ตัวแปรที่น่าจะต้องมี--
string playerName = "Farmer";
int D = 1, M = 100, G = 0, AP = 5;
const int MAX_AP = 5, RENT_AMOUNT = 500, RENT_CYCLE = 7;
int debt = 0, rentStrikes = 0, expansionCost = 5;
bool hasCCTV = false, hasScarecrow = false, hasLottery = false;

//--สถานะของแต่ละแถว--
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
vector<Row> rows(3);

//--ฟังก์ชันล้างหน้าจอ--
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
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

    /*ss << "\n [ QUEST ] Need: " << currentQuest.targetPlant << " (Age:" << currentQuest.targetAge << ") Reward: $" << currentQuest.rewardMoney << "\n";*/ //--รอเอาเควสมาใส่--
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

int main(){
    clearScreen();
    drawInterface();
    return 0;
}