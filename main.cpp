#include <bits/stdc++.h>
#include "Btree.h"
using namespace std;

signed main(){

    Btree Tree (10, 5, 5);
    while (true) {
        int c;
        cout << "1-insert\n"
                "2-delete\n"
                "3-search\n"
                "4-display\n"
                "5-exit\n";
        cin >> c;
        if (c == 1) {
            int recID, ref;
            cout << "Enter record ID and reference: \n";
            cin >> recID >> ref;
            Tree.insert(recID, ref);
            cout << "*****************************************************\n";
            Tree.display();
            cout << "*****************************************************\n";
        } else break;
    }
    return 0;
}