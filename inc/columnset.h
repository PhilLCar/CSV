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

  // Modification related methods:
  // =============================
public:
  ColumnSet& operator +=(int index);
  ColumnSet& operator -=(int index);
  ColumnSet& operator ^=(int index);

  ColumnSet& operator +=(const char* colName);
  ColumnSet& operator -=(const char* colName);
  ColumnSet& operator ^=(const char* colName);

  // Selection related methods:
  // ==========================
public:
  Selection operator ()();
  Selection operator ()(int row, int rel);

  // Indexation related methods:
  // ===========================
public:
  Row operator [](int index);

public:
  int size() const;

private:
  const CSV& ref;
  bool*      inset;
};

#endif