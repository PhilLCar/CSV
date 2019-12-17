#include <csv.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    CSV csv = CSV("csv/train.csv");

    for(int i = 0; i < csv.columns(); i++) {
        cout << csv.columnNames()[i];
    }
    cout << endl;
    for(int i = 0; i < csv.columns(); i++) {
        cout << csv[i][698];
    }
    cout << endl;
}