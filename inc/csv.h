#ifndef INC_CSV_H
#define INC_CSV_H

#include <stdlib.h>
#include <stdio.h>
#include <ostream>
#include <set>
#include <cstdarg>

class CSV {
public:
    class Cell;
    class Column;
    class Row;
    class ColumnSet;
    class Selection;

    class Cell {
    public:
        Cell();
        Cell(Cell const &cell);
        Cell(const char *content);
        Cell(int integer);
        Cell(double decimal);
        ~Cell();

        const inline char *string() {
            return content;
        }
        const inline int integer() {
            return atoi(content);
        }
        const inline double decimal() {
            return atof(content);
        }

        Cell &operator =(Cell const &cell);
        Cell &operator =(Cell &&cell) noexcept;
        friend std::ostream& operator <<(std::ostream &stream, Cell &cell);
    private:
        int   width;
        char *content;
    };

    class Column {
        friend CSV;
    public:
        Column(CSV &csv, int index);
        Column(Selection &selection, int index);
        Column(Column const &col);
        ~Column();

        friend bool operator <(const Column &a, const Column &b);
        Cell       &operator [](int index) const;
        Cell       &operator [](int index);
        inline int   size() { return dim_v; }
        inline Cell &name() { return *header; }
    private:
        int    dim_v;
        Cell  *header;
        Cell **cells;
    };

    class Row {
        friend CSV;
    public:
        Row(CSV &csv, int index);
        Row(ColumnSet &cs, int index);
        Row(Row const &row);
        ~Row();

        void select(int index) const;

        friend bool operator <(const Row &a, const Row &b);
        Cell &operator [](int index) const;
        Cell &operator [](int index);
        inline int size() { return dim_h; }
    private:
        mutable int selected;
        int         dim_h;
        Cell      **cells;
    };

    class ColumnSet {
        friend Selection;
        friend Row;
    public:
        ColumnSet(CSV &csv, std::set<int> &cols);
        ~ColumnSet();

        Selection  operator ()();
        Selection  operator ()(bool inrange, int row1, int row2);
        template <typename... args>
        Selection  operator ()(args... rowIndices) {
            std::set<int> rows;
            int exp[sizeof...(args)] = { (addtoset(nullptr, rows, rowIndices), 0)... };
            return Selection(*this, rows);
        }
        Row        operator [](int index);
        inline int size() { return dim_h; }
        inline int rows() { return dim_v; }
    private:
        int              dim_h;
        int              dim_v;
        std::set<Column> colset;
    };

    class Selection {
      friend Column;
    public:
        Selection(CSV &csv, std::set<int> &rows);
        Selection(ColumnSet &cs, std::set<int> &rows);
        ~Selection();

        void select(int index);
        CSV  tocsv();

        Column operator [](const char *colName);
        Column operator [](int index);
        inline Cell **columnNames() { return headers; }
        inline int    columns()     { return dim_h; }
        inline int    rows()        { return dim_v; }
    private:
        int column(const char *colName);
    private:
        int           dim_h;
        int           dim_v;
        Cell        **headers;
        std::set<Row> rowset;
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
    ColumnSet operator ()(bool inrange, const char *col1Name, const char *col2Name);
    ColumnSet operator ()(bool inrange, int col1, int col2);
    template <typename... args>
    ColumnSet operator ()(args... colNames) {
        std::set<int> cols;
        int exp[sizeof...(args)] = { (addtoset(this, cols, colNames), 0)... };
        return ColumnSet(*this, cols);
    }
    Column    operator [](const char *colName);
    Column    operator [](int index);

private:
    static void addtoset(CSV *csv, std::set<int> &set, const char *colName);
    static void addtoset(CSV *csv, std::set<int> &set, int index);
    int  column(const char *colName);
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