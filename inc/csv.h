#ifndef INC_CSV_H
#define INC_CSV_H

class CSV {
public:
    class CSVColumn {

    };

    class CSVRow {

    };

public:
    CSV(char *filename, bool firstRowIsHeader= true);
    ~CSV();

    bool addColumn();
    bool addColumn(const char *columnName);
    bool moveColumn(int from, int to);
    bool moveColumn(const char *fromName, int rel);
    bool swapColumn(int col1, int col2);
    bool swapColumn(const char *col1Name, const char *col2Name);
    bool insertColumn(int index);
    bool insertColumn(int index, const char *columnName);
    bool deleteColumn(int index);
    bool deleteColumn(const char *columnName);

    bool addRow();
    bool moveRow(int from, int to);
    bool swapRow(int row1, int row2);
    bool insertRow(int index);
    bool deleteRow(int index);


private:
    int column(const char *columnName);

    bool    hasHeader;
    char  **header;
    char ***content;
    char   *filename;

    int dim_h;
    int dim_v;
    int cap_h;
    int cap_v;
};

#endif