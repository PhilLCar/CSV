#ifndef INC_CSV_H
#define INC_CSV_H

class CSV {
public:
    class CSVColumn {

    };

    class CSVRow {

    };

public:
    CSV(char *, bool = true);
    ~CSV();

    void addColumn();
    void addColumn(const char *columnName);
    void moveColumn(int from, int to);
    void moveColumn(const char *fromName, int to);
    void swapColumn(int col1, int col2);
    void swapColumn(const char *col1Name, const char *col2Name);
    void insertColumn(int index);
    void insertColumn(const char *columnName);
    void deleteColumn(int index);
    void deleteColumn(const char *columnName);

    void addRow();
    void moveRow(int from, int to);
    void swapRow(int row1, int row2);
    void insertRow(int index);
    void deleteRow(int index);


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