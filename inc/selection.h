#ifndef CSV_SELECTION_HEADER
#define CSV_SELECTION_HEADER

class CSV_API Selection {
  friend ColumnSet;
  friend Column;
  friend CSV;
private:
  Selection(ColumnSet& cs);
public:
  ~Selection();

#ifdef CSV_EXTERN
  void M(Selection, dispose)();
#endif

public:
  void M(Selection, sortBy)(int index);
  void M(Selection, sort)(bool increasing = true);

public:
#ifndef CSV_EXTERN
  // Hide named method in C++ context
private:
#endif
  void M(Selection, sortByN)(const char* colName);

#ifndef CSV_EXTERN
public:
  inline void sortBy(const char* colName) { return sortByN(colName); }
#endif

public:
#ifndef CSV_EXTERN
  // Hide unsafe methods in C++ context
private:
#endif
  CSV* M(Selection, toCSV)(const char *filename, char separator = ',');
 
#ifndef CSV_EXTERN
public:
  const inline CSV csv(const char *filename, char separator = ',') { MOVE(CSV, toCSV(filename, separator)); }
#endif

  // Modification related methods:
  // =============================
public:
#ifndef CSV_EXTERN
  // Hide unsafe methods in C++ context
private:
#endif
  Selection* M(Selection, add)(int index);
  Selection* M(Selection, sub)(int index);
  Selection* M(Selection, xor)(int index);

#ifndef CSV_EXTERN
public:
  inline Selection& operator +=(int index) { return *add(index); }
  inline Selection& operator -=(int index) { return *sub(index); }
  inline Selection& operator ^=(int index) { return *xor(index); }
#endif

  // Indexation related methods:
  // ===========================
public:
#ifndef CSV_EXTERN
  // Hide unsafe methods in C++ context
private:
#endif
  Column* M(Selection, column)(const char* colName);
  Column* M(Selection, at)(int index);

#ifndef CSV_EXTERN
public:
  inline Column operator [](const char *colName) { MOVE(Column, column(colName)); }
  inline Column operator [](int index)           { MOVE(Column, at(index)); }
#endif

public:
  int M(Selection, rows)() const;
  int M(Selection, cols)() const;

private:
  void _make_matrix();

private:
  const ColumnSet& cs;
  bool*            inset;
  int              dim_h;
  int              dim_v;
  int              index_s;
  Cell**           header;
  Cell***          matrix;
};

#endif