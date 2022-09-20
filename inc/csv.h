#ifndef CSV_CSV_HEADER
#define CSV_CSV_HEADER

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include <ostream>
#include <iomanip>

#include <macro.h>

namespace csv {
  class CSV_API CSV {
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
    CSV(const char* filename, bool firstRowIsHeader = false, char separator = ',');
    ~CSV();

  public:
    void flush();

    // Column related methods:
    // =======================
  public:
    bool addColumn();
    bool moveColumn(int from, int to);
    bool moveColumnRel(int from, int rel);
    bool swapColumn(int col1, int col2);
    bool insertColumn(int index);
    bool removeColumn(int index);

    bool addColumn(const char* colName);
    bool moveColumn(const char* fromName, const char* toName);
    bool moveColumnRel(const char *fromName, int rel);
    bool swapColumn(const char* col1Name, const char* col2Name);
    bool insertColumn(int index, const char* colName);
    bool removeColumn(const char* colName);

    inline Cell* columnNames() const { return header; }
    inline int   cols()        const { return dim_h;  }

    // Row related methods:
    // ====================
  public:
    bool addRow();
    bool moveRow(int from, int to);
    bool swapRow(int row1, int row2);
    bool insertRow(int index);
    bool deleteRow(int index);
  
    inline int rows() const { return dim_v; }

    // Selection related methods:
    // ==========================
    ColumnSet selectNames(int n, ...);

  public:
    ColumnSet operator ()();
    ColumnSet operator ()(int col, int rel);
    ColumnSet operator ()(const char* colName, int rel);
  #ifndef CSV_EXPORT
    template <typename... args>
    inline ColumnSet operator ()(args... colNames) { selectNames(sizeof...(args), colNames...); }
  #endif


    // Indexing related methods:
    // =========================
  public:
  public:
    inline Column operator [](const char* colName);
    inline Column operator [](int index);

  private:
    int  ioc (const char* colName) const;
    void resizeh(int ncap);
    void resizev(int ncap);
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