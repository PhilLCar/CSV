#ifndef CSV_CSV_HEADER
#define CSV_CSV_HEADER

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include <macro.h>

#ifndef CSV_EXTERN
#include <ostream>
#include <iomanip>
#endif

/* NOTE:
 * This class is optimized for interoperability and not for readability or performance.
 * It's meant to be an easy tool for projects that require a simple data management API.
 */

CSV_BEGIN

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
  CSV(const char* filename, bool firstRowIsHeader = false, char separator = ',');
  ~CSV();

#ifdef CSV_EXTERN
  static CSV* M(CSV, build)(const char* filename, bool firstRowIsHeader = false, char separator = ',');
  void        M(CSV, dispose)();
#endif

public:
  void M(CSV, flush)();

  // Column related methods:
  // =======================
public:
  bool  M(CSV, addColumn)();
  bool  M(CSV, moveColumn)(int from, int to);
  bool  M(CSV, moveColumnRel)(int from, int rel);
  bool  M(CSV, swapColumn)(int col1, int col2);
  bool  M(CSV, insertColumn)(int index);
  bool  M(CSV, removeColumn)(int index);
  Cell* M(CSV, columnNames)() const;
  int   M(CSV, cols)() const;

#ifndef CSV_EXTERN
  // Hides the named methods and exposes the overloaded ones
private:
#endif
  bool M(CSV, addNColumn)(const char* colName);
  bool M(CSV, moveNColumn)(const char* fromName, const char* toName);
  bool M(CSV, moveNColumnRel)(const char *fromName, int rel);
  bool M(CSV, swapNColumn)(const char* col1Name, const char* col2Name);
  bool M(CSV, insertNColumn)(int index, const char* colName);
  bool M(CSV, removeNColumn)(const char* colName);

#ifndef CSV_EXTERN
  // Overloading when included in a C++ project
public:
  inline bool addColumn(const char* colName)                         { return addNColumn(colName);             }
  inline bool moveColumn(const char* fromName, const char* toName)   { return moveNColumn(fromName, toName);   }
  inline bool moveColumnRel(const char *fromName, int rel)           { return moveNColumnRel(fromName, rel);   }
  inline bool swapColumn(const char* col1Name, const char* col2Name) { return swapNColumn(col1Name, col2Name); }
  inline bool insertColumn(int index, const char* colName)           { return insertNColumn(index, colName);   }
  inline bool removeColumn(const char* colName)                      { return removeNColumn(colName);          }
#endif

  // Row related methods:
  // ====================
public:
  bool M(CSV, addRow)();
  bool M(CSV, moveRow)(int from, int to);
  bool M(CSV, swapRow)(int row1, int row2);
  bool M(CSV, insertRow)(int index);
  bool M(CSV, deleteRow)(int index);
  int  M(CSV, rows)() const;

  // Selection related methods:
  // ==========================
public:
#ifndef CSV_EXTERN
  // Hide unsafe methods in C++ context
private:
#endif
  ColumnSet* M(CSV, selectAll)();
  ColumnSet* M(CSV, selectRange)(int col1, int col2);
  ColumnSet* M(CSV, selectRangeRel)(int col, int rel);
  ColumnSet* M(CSV, selectNRange)(const char* col1Name, const char* col2Name);
  ColumnSet* M(CSV, selectNRangeRel)(const char* colName, int rel);
  ColumnSet* M(CSV, selectNames)(int n, ...);

#ifndef CSV_EXTERN
public:
  inline ColumnSet operator ()()                             { MOVE(ColumnSet, selectAll()                  );             }
  inline ColumnSet operator ()(int col, int rel)             { MOVE(ColumnSet, selectRangeRel(col, rel)     );             }
  inline ColumnSet operator ()(const char* colName, int rel) { MOVE(ColumnSet, selectNRangeRel(colName, rel));             }
  template <typename... args>
  inline ColumnSet operator ()(args... colNames)             { MOVE(ColumnSet, selectNames(sizeof...(args), colNames...)); }
#endif

  // Indexing related methods:
  // =========================
public:
#ifndef CSV_EXTERN
  // Hide unsafe methods in C++ context
private:
#endif
  Column* M(CSV, column)(const char* colName);
  Column* M(CSV, at)(int index);

#ifndef CSV_EXTERN
public:
  inline Column operator [](const char* colName) { MOVE(Column, column(colName)); }
  inline Column operator [](int index)           { MOVE(Column, at(index));       }
#endif

private:
  int  _ioc (const char* colName) const;
  void _resizeh(int ncap);
  void _resizev(int ncap);
  char _next(FILE* file, char* string);

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

CSV_END

#endif