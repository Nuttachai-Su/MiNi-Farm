#include "MiNifarm.h"

//---ฟังก์ชั่นเก็บเกี่ยวพืช---//
void harvesting() {
    if (AP <= 0) return; //เช็คจำนวน AP 
    int r; cout << "Row: "; cin >> r; r--; //รับค่าตำแหน่งแถว
    if (r >= 0 && r < (int)rows.size() && rows[r].isPlanted) {
        int ch = baseSuccess[rows[r].plantTypeIndex] + (rows[r].isWatered ? 20 : 0) + (rows[r].hasFertilizer ? 15 : 0); //คำนวณเปอร์เซ็นการเก็บเกี่ยวสำเร็จ
        if (rows[r].age < 2) ch -= 40;
        if (!rows[r].isRotten && (rand() % 100) < ch) {
            int s = plantSells[rows[r].plantTypeIndex] + (rows[r].age * 5); M += s;
            cout << "[Success] Sold for $" << s;
        } else cout << "[Fail] Crop lost!";
        rows[r].isPlanted = false; rows[r].hasFertilizer = false; AP--; waitEnter();
    }
}

int main(){
    harvesting();
    return 0;
}