#ifndef INC_CSV_H
#define INC_CSV_H

#include <stdlib.h>
#include <stdio.h>
#include <ostream>
#include <set>

class CSV {
public:
    class Cell {
    public:
        Cell();
        Cell(Cell const &cell);
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

        Cell &operator =(Cell const &cell);
        Cell &operator =(Cell &&cell);
        friend std::ostream& operator <<(std::ostream &stream, Cell &cell);
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
        inline int size() { return dim_v; }
    private:
        int    dim_v;
        Cell  *header;
        Cell **cells;
    };

    class ColumnSet {
    public:
        ColumnSet(CSV &csv, std::set<int> &cols);
        ~ColumnSet();

        inline int size() { return dim_h; }
        inline int rows() { return dim_v; }
    private:
        int              dim_h;
        int              dim_v;
        std::set<Column> cols;
    };

    class Row {
        friend CSV;
    public:
        Row(CSV csv, int index);
        ~Row();

        Cell &operator [](int index);
        inline int size() { return dim_h; }
    private:
        int    dim_h;
        Cell **cells;
    };

public:
    CSV(const char *filename, bool firstRowIsHeader= true);
    ~CSV();

    bool addColumn();
    bool addColumn(const char *colName);
    bool moveColumn(int from, int to);
    bool moveColumn(const char *fromName, int rel);
    bool swapColumn(int col1, int col2);
    bool swapColumn(const char *col1Name, const char *col2Name);
    bool insertColumn(int index);
    bool insertColumn(int index, const char *colName);
    bool deleteColumn(int index);
    bool deleteColumn(const char *colName);
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

    ColumnSet operator ()();
    ColumnSet operator ()(const char *col1Name, const char *col2Name);
    ColumnSet operator ()(int col1, int col2);
    ColumnSet operator ()(std::set<const char *> &colNames);
    ColumnSet operator ()(std::set<int> &cols);
    Column    operator [](const char *colName);
    Column    operator [](int index);

private:
    int column(const char *colName);
    void resizeh(int ncap);
    void resizev(int ncap);

    char next(FILE *file, char *string);

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