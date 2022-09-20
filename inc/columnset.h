#ifndef CSV_COLUMNSET_HEADER
#define CSV_COLUMNSET_HEADER

class CSV_API ColumnSet {
  friend Selection;
  friend Row;
  friend CSV;
private:
  ColumnSet(CSV& csv);
public:
  ~ColumnSet();

#ifdef CSV_EXTERN
  void M(ColumnSet, dispose)();
#endif

  // Modification related methods:
  // =============================
public:
#ifndef CSV_EXTERN
  // Hide unsafe methods in C++ context
private:
#endif
  ColumnSet* M(ColumnSet, add)(int index);
  ColumnSet* M(ColumnSet, sub)(int index);
  ColumnSet* M(ColumnSet, xor)(int index);

  ColumnSet* M(ColumnSet, addN)(const char* colName);
  ColumnSet* M(ColumnSet, subN)(const char* colName);
  ColumnSet* M(ColumnSet, xorN)(const char* colName);

#ifndef CSV_EXTERN
public:
  inline ColumnSet& operator +=(int index) { return *add(index); }
  inline ColumnSet& operator -=(int index) { return *sub(index); }
  inline ColumnSet& operator ^=(int index) { return *xor(index); }

  inline ColumnSet& operator +=(const char* colName) { return *addN(colName); }
  inline ColumnSet& operator -=(const char* colName) { return *subN(colName); }
  inline ColumnSet& operator ^=(const char* colName) { return *xorN(colName); }
#endif

  // Selection related methods:
  // ==========================
public:
#ifndef CSV_EXTERN
  // Hide unsafe methods in C++ context
private:
#endif
  Selection* M(ColumnSet, selectAll)();
  Selection* M(ColumnSet, selectRange)(int row1, int row2);
  Selection* M(ColumnSet, selectRangeRel)(int row, int rel);

#ifndef CSV_EXTERN
public:
  inline Selection operator ()()                 { MOVE(Selection, selectAll()             ); }
  inline Selection operator ()(int row, int rel) { MOVE(Selection, selectRangeRel(row, rel)); }
#endif

  // Indexation related methods:
  // ===========================
public:
#ifndef CSV_EXTERN
  // Hide unsafe methods in C++ context
private:
#endif
  Row* M(ColumnSet, at)(int index);

#ifndef CSV_EXTERN
public:
  inline Row operator [](int index) { MOVE(Row, at(index)); }
#endif

public:
  int M(ColumnSet, size)() const;

private:
  const CSV& ref;
  bool*      inset;
};

#endif