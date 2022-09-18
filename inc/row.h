#ifndef CSV_ROW_HEADER
#define CSV_ROW_HEADER

class CSV_API Row {
  friend CSV;
public:
  Row(CSV& csv, int index);
  Row(ColumnSet& cs, int index);
  Row(Row const& row);
  ~Row();

  void select(int index) const;

  bool         operator <(const Row& other);
  Cell&        operator [](int index) const;
  Cell&        operator [](int index);
  inline int   size() { return dim_h; }
private:
  mutable int  selected;
  int          dim_h;
  Cell**       cells;
};

#endif