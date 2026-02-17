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




// --- ฟังก์ชันช่วย ---
void waitEnter() {
    cout << "\n> Press Enter to continue...";
    cin.ignore(1000, '\n'); cin.get();    
}

// --- ฟังก์ชันหลัก ---
void loadGame(string code) {
    try {
        // 1. เตรียมข้อความรหัสเซฟ (Pre-processing):
        // ใช้ฟังก์ชัน replace เพื่อเปลี่ยนเครื่องหมาย '_' (ขีดล่าง) ทั้งหมดให้เป็นช่องว่าง ' '
        // เนื่องจากเครื่องหมาย >> ของ stringstream จะใช้ "ช่องว่าง" เป็นตัวคั่นในการแยกข้อมูล
        replace(code.begin(), code.end(), '_', ' ');

        // 2. สร้างสายธารข้อมูลหลัก (Main Stream):
        // นำตัวแปร code ที่ทำความสะอาดแล้วมาใส่ใน stringstream ชื่อ ss 
        // เพื่อให้สามารถ "ดึง" ข้อมูลออกมาทีละส่วนได้เหมือนการใช้ cin
        stringstream ss(code); 
        int rowCount;

        // 3. ดึงข้อมูลพื้นฐานของตัวผู้เล่น:
        // ข้อมูลจะถูกดึงออกมาตามลำดับที่เก็บไว้: ชื่อ -> วัน -> เงิน -> เพชร -> หนี้ -> การผิดนัด -> จำนวนแถว
        // การใช้ ss >> จะแปลงจากข้อความ (string) เป็นตัวเลข (int) ให้โดยอัตโนมัติ
        ss >> playerName >> D >> M >> G >> debt >> rentStrikes >> rowCount;
        
        // 4. โหลดข้อมูลภารกิจ (Quest Data):
        // ข้อมูลเควสถูกเก็บแยกเป็นกลุ่มเล็กโดยใช้ '.' คั่น เช่น "Carrot.1.150"
        // เราดึงก้อนนั้นออกมาใส่ qD แล้วเปลี่ยน '.' เป็นช่องว่าง 
        // จากนั้นใช้ qss (stringstream ย่อย) แยกค่าเข้าตัวแปรเป้าหมาย, อายุ และรางวัล
        string qD; ss >> qD; replace(qD.begin(), qD.end(), '.', ' ');
        stringstream qss(qD); qss >> currentQuest.targetPlant >> currentQuest.targetAge >> currentQuest.rewardMoney;

        // 5. โหลดข้อมูลอุปกรณ์ (Gear/Item Data):
        // ใช้วิธีเดียวกับเควส คือแยกข้อมูลกลุ่มอุปกรณ์ออกมา แล้วใช้ iss (stringstream ย่อย)
        // เพื่อแยกค่าสถานะว่าผู้เล่นมี CCTV, Scarecrow หรือ Lottery หรือไม่ (0=ไม่มี, 1=มี)
        string iD; ss >> iD; replace(iD.begin(), iD.end(), '.', ' ');
        stringstream iss(iD); iss >> hasCCTV >> hasScarecrow >> hasLottery;

        // 6. ล้างสถานะฟาร์มเก่า (Reset Farm):
        // ลบข้อมูลใน vector rows ทั้งหมด เพื่อเตรียมสร้างแถวใหม่ตามจำนวน rowCount ที่โหลดมา
        rows.clear();

        // ลูปเพื่อสร้างและตั้งค่าแถวผักทีละแถว
        for(int i = 0; i < rowCount; i++) {
            // 7. สกัดข้อมูลรายแถว (Row Extraction):
            // ดึงรหัสสถานะของแถวนั้นๆ ออกมา (เช่น "0.2.1.0.1.0") ใส่ใน rD
            // เปลี่ยน '.' เป็นช่องว่าง เพื่อให้ตัวแปร rss (Row String Stream) แยกข้อมูลได้
            string rD; ss >> rD; replace(rD.begin(), rD.end(), '.', ' ');

            // 8. การทำงานของ rss (หัวใจสำคัญของการแยกข้อมูลรายแถว):
            // สร้าง stringstream ชื่อ rss เพื่อรับผิดชอบการกระจายข้อมูลภายในแถวผัก "หนึ่งแถว" เท่านั้น
            stringstream rss(rD); Row r; int pIdx;

            // ดึงข้อมูลจาก rss เข้าสู่โครงสร้างข้อมูล Row (r) ตามลำดับ:
            // ดัชนีพืช >> อายุ >> สถานะรดน้ำ >> สถานะเน่า >> สถานะปุ๋ย >> วันสปริงเกอร์
            rss >> pIdx >> r.age >> r.isWatered >> r.isRotten >> r.hasFertilizer >> r.sprinklerDays;

            // 9. ตรวจสอบการปลูกพืช (Plant Validation):
            // ถ้า pIdx ไม่เท่ากับ -1 แสดงว่าแถวนั้นมีการปลูกพืชอยู่
            // โปรแกรมจะตั้งค่า isPlanted เป็นจริง และไปดึงชื่อพืชจาก Array plantNames มาแสดงผล
            if(pIdx != -1) { 
                r.isPlanted = true; 
                r.plantTypeIndex = pIdx; 
                r.plantName = plantNames[pIdx]; 
            }

            // 10. เก็บแถวที่ตั้งค่าเสร็จแล้ว:
            // นำ Object แถว (r) ที่ได้จากการแยกข้อมูลผ่าน rss ใส่กลับเข้าไปในรายการแถวของเกม
            rows.push_back(r);
        } 

        // 11. อัปเดตราคาสถานที่ (Price Calculation):
        // คำนวณราคาขยายฟาร์มครั้งต่อไปใหม่ โดยอิงจากจำนวนแถวที่มีอยู่จริงในเซฟ
        expansionCost = 5 + ((rows.size() - 3) * 5);

        // 12. แจ้งสถานะสำเร็จ:
        cout << "\n[Success] Welcome back, " << playerName << "!"; waitEnter();

    } catch (...) { 
        // 13. การจัดการข้อผิดพลาด (Error Handling):
        // หากรหัสเซฟผิดรูปแบบ หรือมีข้อมูลขาดหายไประหว่างที่ stringstream กำลังดึงค่า
        // โปรแกรมจะเด้งมาทำงานในส่วนนี้ เพื่อไม่ให้เกมค้าง (Crash)
        cout << "\n[Error] Invalid Code! Starting new game."; 
        waitEnter(); 
    }
}

int main(){
    cout << " Enter Save Code: "; string code; cin >> code;
    
    loadGame(code);

    cout <<  playerName << " "<< D <<" "<< M <<" "<< G <<" "<< debt <<" "<< rentStrikes ;
}