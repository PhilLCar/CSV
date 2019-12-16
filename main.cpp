#include <csv.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    CSV csv = CSV("csv/train.csv");

    csv.addColumn("test");
    //csv.addColumn("test2");
    //csv.addRow();
    //csv.addRow();

    //for(int i = 0; i < csv.columns(); i++) {
     //   cout << csv.columnNames()[i];
    //}
    // cout << endl;
    // for (int j = 0; j < csv.rows(); j++) {
    //     for(int i = 0; i < csv.columns(); i++) {
    //         cout << csv[i][j];
    //     }
    //     cout << endl;
    // }
}