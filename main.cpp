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

    CSV::Selection &&selection = csv("PassengerId", "Survived", "Pclass")(0, 23, 32);

    for (int i = 0; i < selection.columns(); i++)
    {
        cout << *selection.columnNames()[i];
    }

}