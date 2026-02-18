#include "MiNifarm.h"

//---ฟังก์ชั่นโชว์คู่มือผู้เล่น---//
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

int main(){
    showIndex();
    return 0;
}