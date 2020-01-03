#include <csv.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    CSV csv = CSV("csv/train.csv");

    CSV::Selection &&selection = csv("PassengerId", "Survived", "Pclass")(0, 23, 32);

    for (int i = 0; i < selection.columns(); i++)
    {
        cout << *selection.columnNames()[i];
    }
    cout << endl;
    for (int i = 0; i < selection.rows(); i++)
    {
        for (int j = 0; j < selection.columns(); j++)
        {
            cout << selection[j][i];
        }
        cout << endl;
    }

}