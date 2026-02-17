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