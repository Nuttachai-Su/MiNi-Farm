#include "MiNifarm.h"

//---ฟังก์ชั่นสุ่มอีเวนต์กลางคืน---//
void processNightEvents() {
    clearScreen();
    cout << "________________________________________________________________" << endl;
    cout << " [ NIGHT REPORT - DAY " << D << " ]" << endl;
    cout << "________________________________________________________________" << endl;
    
    int event = rand() % 100; //สุ่มเปอร์เซ็นการเกิดอีเวนต์
    if (event < 15) {
        cout << " > Luck: It rained! All plants are watered (+20% Success tomorrow)." << endl;
        for(auto &r : rows) if(r.isPlanted) r.isWatered = true;
        //ถ้าเลขที่สุ่มได้น้อยกว่า 15 พืชทุกแถวจะได้รับการรดน้ำ โอกาสเกิด 15%
    } else if (event < 25) {
        if (hasScarecrow) cout << " > Defense: Wild animals came, but the Scarecrow scared them off!" << endl;
        //ถ้าเลขที่สุ่มได้น้อยกว่า 25 และถ้ามีหุ่นไล่กาในฟาร์มจะไม่เกิดอะไรขึ้น โอกาสเกิด 10%
        else {
            int target = rand() % rows.size();
            if (rows[target].isPlanted) { rows[target].isPlanted = false; cout << " > Danger: Animals ate your crop in Row " << target+1 << "!" << endl; }
            //ถ้าไม่มีหุ่นไล่กาและแถวที่สุ่มได้มีพืชอยู่ พืชจะถูกทำลายทั้งแถว
            else cout << " > Info: Animals visited but there was nothing to eat." << endl;
            //ถ้าไม่มีหุ่นไล่กาและแถวที่สุ่มไม่มีพืชอยู่ จะไม่เกิดอะไรขึ้น
        }
    } else if (event < 35) {
        if (hasCCTV) cout << " > Defense: A thief spotted your CCTV and fled!" << endl;
        //ถ้าสุ่มได้น้อยกว่า 35 และถ้ามีกล้องวงจรปิดในฟาร์มจะไม่มีอะไรเกิดขึ้น โอกาสเกิด 10%
        else {
            int loss = (rand() % 151) + 50; M = max(0, M - loss);
            cout << " > Danger: A thief stole $" << loss << " from your wallet!" << endl;
            //ถ้าไม่มีกล้องวงจรปิด จะถูกโจรขโมยเงินเป็นจำนวนสุ่ม 1 ถึง 150 บวกกับ 50
        }
    } else cout << " > Peace: A quiet night at the farm." << endl;
        //ถ้าเลขที่สุ่มมากกว่าหรือเท่ากับ 35 จะไม่มีอะไรเกิดขึ้น โอกาสเกิด 65%

    if (hasLottery) {
        if (rand() % 10 == 0) { M += 1000; cout << " > !!! WINNER: You won the Lottery jackpot $1000! !!!" << endl; }
        //ถ้ามีลอตเตอรี่และถ้าถูกรางวัลจะได้เงิน 1000 มีโอกาสสุ่มได้ 10%
        else cout << " > Lottery: Better luck next time!" << endl;
        hasLottery = false;
        //ถ้าไม่ถูกลอตเตอรี่ก็จะไม่มีอะไรเกิดขึ้นและลอตเตอรี่ก็จะหายไป
    }
    cout << "________________________________________________________________" << endl;
}

//---ฟังก์ชั่นจบวัน
void endDay() {
    if (D % RENT_CYCLE == 0) {
        int total = RENT_AMOUNT + debt; //รวมค่าเช่ากับหนี้
        if (M >= total) { M -= total; debt = 0; rentStrikes = 0; } //ถ้าเงินมากกว่าหรือเท่ากับผลรวม จะหักเงินและปรับให้ค่าเช่ากับหนี้เป็น 0
        else { debt = total - M; M = 0; rentStrikes++; if (rentStrikes >= 3) { cout << "GAME OVER!"; exit(0); } }
        //ถ้าเงินน้อยกว่า จะเพิ่มค่าหนี้โดยเอาค่าจากผลรวมลบด้วยเงินผู้เล่น เงินผู้เล่นจะเหลือ 0 และจะเพิ่มจำนวนการท้วงค่าเช่า ถ้าจำนวนการท้วงค่าเช่าครบ 3 ครั้ง เกมจะจบ
    }
    processNightEvents();
    D++; AP = MAX_AP; M += 50; //เพิ่มจำนวนวัน ตั้งค่า AP ให้เท่ากับ Max_AP และเพิ่มเงินผู้เล่น 50
    for(auto &r : rows) { //เช็คทุกแถวในฟาร์ม
        if (r.isPlanted) { //ถ้ามีพืช
            r.age++; r.isWatered = false; //เพิ่มอายุพืช และปรับให้การรดน้ำเป็น false
            if (r.sprinklerDays > 0) { r.isWatered = true; r.sprinklerDays--; }
            //ถ้าสปิกเกอร์มีจำนวนใช้งานมากกว่า 0 การรดน้ำพืชจะถูกปรับเป็น true และจะลดจำนวนการใช้งานสปิกเกอร์ 1 ครั้ง
            if (rand() % 100 < 10) r.isRotten = true;
            //ถ้าสุ่มเลข 1 ถึง 100 แล้วน้อยกว่า 10 พืชแถวนั้นๆ จะเน่าเสีย
        } else if (r.sprinklerDays > 0) r.sprinklerDays--;
            //ถ้าไม่มีพืชในแถว และถ้าสปิกเกอร์มีจำนวนใช้งานมากกว่า 0 จะลดจำนวนการใช้งานสปิกเกอร์ 1 ครั้ง
    }
    waitEnter();
}

int main(){
    processNightEvents();
    return 0;
}