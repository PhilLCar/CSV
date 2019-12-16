#ifndef INC_CSV_H
#define INC_CSV_H

#include <stdlib.h>
#include <ostream>

class CSV {
public:
    class Cell {
    public:
        Cell();
        Cell(const char *content);
        ~Cell();

        inline char *string() {
            return content;
        }
        inline int integer() {
            return atoi(content);
        }
        inline double decimal() {
            return atof(content);
        }

        friend std::ostream operator <<(std::ostream &stream, Cell &cell);
    private:
        int   width;
        char *content;
    };

    class Column {
        friend CSV;
    public:
        Column(CSV &csv, int index);
        ~Column();

        Cell &operator [](int index);
    private:
        Cell  *header;
        Cell **cells;
    };

    class Row {
        friend CSV;
    public:
        Row(CSV &csv, int index);
        ~Row();

        Cell &operator [](int index);
    private:
        Cell **cells;
    };

public:
    CSV(const char *filename, bool firstRowIsHeader= true);
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
    inline Cell *columnNames() {
        return header;
    }
    inline int columns() {
        return dim_h;
    }

    bool addRow();
    bool moveRow(int from, int to);
    bool swapRow(int row1, int row2);
    bool insertRow(int index);
    bool deleteRow(int index);
    inline int rows() {
        return dim_v;
    }

    Column operator [](int index);

private:
    int column(const char *columnName);
    void resizeh(int ncap);
    void resizev(int ncap);

    bool         hasHeader;
    Cell        *header;
    Cell       **content;
    const char  *filename;

    int dim_h;
    int dim_v;
    int cap_h;
    int cap_v;
};

#endif