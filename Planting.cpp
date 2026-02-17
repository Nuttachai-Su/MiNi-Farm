#include <iostream>
#include <vector>
#include <string>
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

//---ตัวแปรหลัก---//
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

//----ข้อมูลพืช---//
string plantNames[] = {"Carrot", "Corn", "Pumpkin"};
int plantCosts[] = {20, 50, 100};   
int plantSells[] = {40, 120, 300};  
int baseSuccess[] = {80, 60, 40}; 

vector<Row> rows(3); // สร้างเวกเตอร์เก็บแถว 3 ตำแหน่ง //

//---ฟังก์ชั่นปลูกพืช---//
void planting() {
    if (AP <= 0) return; //เช็คค่า AP ไม่น้อยกว่า 0
    int r; cout << "Row: "; cin >> r; r--;
    if (r >= 0 && r < (int)rows.size() && !rows[r].isPlanted) { // เช็คค่า r ต้องมากกว่าหรือเท่ากับ 0 และน้อยกว่าขนาดของแถวและต้องไม่มีพืชปลูกไว้อยู่แล้ว
        cout << "1.Carrot($20) 2.Corn($50) 3.Pumpkin($100): ";
        int p; cin >> p; p--; // รับค่า p และลบออกด้วย 1 เพื่อไม่ให้เกินขนาดของ array
        // เช็ค p มากกว่าหรือเท่ากับ 0 และน้อยกว่าหรือเท่ากับ 2 และเช็คเงินของผู้เล่นว่ามากกว่าหรือเท่ากับราคาของพืชหรือไม่
        if (p >= 0 && p <= 2 && M >= plantCosts[p]) { 
            M -= plantCosts[p]; rows[r].isPlanted = true; rows[r].plantName = plantNames[p];
            rows[r].plantTypeIndex = p; rows[r].age = 0; rows[r].isRotten = false; AP--;
        } /* ถ้าตรงถามเงื่อนไข เงินของผู้เล่นก็จะถูกลบด้วยราคาพืช แถว r ก็จะเปลี่ยนสถานะเป็นปลูกพืชแล้ว แถว r เปลี่ยนชื่อและข้อมูลเป็นพืชที่เลือก 
        ปรับอายุเป็น 0 และการเน่าเสียเป็น false และลบค่า AP 1 หน่วย */
    }
}

int main(){
    planting();
    return 0;
}