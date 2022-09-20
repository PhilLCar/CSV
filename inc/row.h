#ifndef CSV_ROW_HEADER
#define CSV_ROW_HEADER

class CSV_API Row {
  friend CSV;
public:
  Row(const CSV& csv, int index);
  Row(const ColumnSet& cs, int index);
  Row(Row const& row);
  ~Row();

#ifdef CSV_EXTERN
  void M(Column, dispose)();
#endif

public:
#ifndef CSV_EXTERN
  // Hide unsafe methods in C++ context
private:
#endif
  bool  M(Row, lt)(const Row *other);
  Cell* M(Row, at)(int index);

#ifndef CSV_EXTERN
  bool  operator <(const Row& other) { return lt(&other);    }
  Cell& operator [](int index) const { return *cells[index]; }
  Cell& operator [](int index)       { return *at(index);    }
#endif

  void M(Row, select)(int index) const;
  int  M(Row, size)() const;

private:
  mutable int  selected;
  int          dim_h;
  Cell**       cells;
};

#endif