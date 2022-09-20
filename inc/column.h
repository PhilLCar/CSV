#ifndef CSV_COLUMN_HEADER
#define CSV_COLUMN_HEADER

class CSV_API Column {
  friend CSV;
  friend Selection;
public:
  Column(const CSV& csv, int index);
  Column(const Selection& selection, int index);
  Column(Column const& col);
  ~Column();

#ifdef CSV_EXTERN
  void M(Column, dispose)();
#endif

public:
#ifndef CSV_EXTERN
  // Hide unsafe methods in C++ context
private:
#endif
  bool  M(Column, lt)(const Column *other);
  Cell* M(Column, at)(int index);

#ifndef CSV_EXTERN
public:
  inline bool  operator <(const Column& other) { return lt(&other);    }
  inline Cell& operator [](int index) const    { return *cells[index]; }
  inline Cell& operator [](int index)          { return *at(index);    }
#endif

public:
  int   M(Column, size)() const;
  Cell* M(Column, name)();

private:
  int    dim_v;
  Cell*  header;
  Cell** cells;
};

#endif