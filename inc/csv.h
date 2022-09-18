#ifndef CSV_CSV_HEADER
#define CSV_CSV_HEADER

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include <osal.h>
#include <pointerarray.h>

namespace csv {
  CSV_LINK class CSV_API CSV {
  public:
    class Cell;
    class Column;
    class ColumnSet;
    class Row;
    class Selection;

#include <cell.h>
#include <column.h>
#include <columnset.h>
#include <row.h>
#include <selection.h>

  public:
    CSV(const char* filename, bool firstRowIsHeader = true, char separator = ',');
    ~CSV();

    void flush();

    bool         addColumn();
    bool         addColumn(const char* colName);
    bool         moveColumn(int from, int to);
    bool         moveColumn(const char* fromName, int rel);
    bool         swapColumn(int col1, int col2);
    bool         swapColumn(const char* col1Name, const char* col2Name);
    bool         insertColumn(int index);
    bool         insertColumn(int index, const char* colName);
    bool         deleteColumn(int index);
    bool         deleteColumn(const char* colName);
    inline Cell* columnNames() { return header; }
    inline int   columns()     { return dim_h;  }

    bool        addRow();
    bool        moveRow(int from, int to);
    bool        swapRow(int row1, int row2);
    bool        insertRow(int index);
    bool        deleteRow(int index);
    inline int  rows() { return dim_v; }

    ColumnSet operator ()();
    ColumnSet operator ()(bool inrange, const char* col1Name, const char* col2Name);
    ColumnSet operator ()(bool inrange, int col1, int col2);
#ifndef CSV_EXPORT
    template <typename... args>
    ColumnSet&& operator ()(args... colNames) {
      std::set<int> cols;
      int exp[sizeof...(args)] = { (addtoset(this, cols, colNames), 0)... };
      return ColumnSet(*this, cols);
    }
#endif

    Column operator [](const char* colName);
    Column operator [](int index);

  private:
    static void addToSet(CSV* csv, int* set, const char* colName);
    static void addToSet(CSV* csv, int* set, int index);
    int         column(const char* colName);
    void        resizeh(int ncap);
    void        resizev(int ncap);

    char next(FILE* file, char* string);

  private:
    bool        hasHeader;
    Cell*       header;
    Cell**      content;
    const char* filename;
    const char  separator;

    int dim_h;
    int dim_v;
    int cap_h;
    int cap_v;
  };
}

#endif