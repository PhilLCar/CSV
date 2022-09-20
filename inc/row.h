#ifndef CSV_ROW_HEADER
#define CSV_ROW_HEADER

class CSV_API Row {
  friend CSV;
public:
  Row(const CSV& csv, int index);
  Row(const ColumnSet& cs, int index);
  Row(Row const& row);
  ~Row();

public:
  bool  operator <(const Row& other);
  Cell& operator [](int index) const;
  Cell& operator [](int index);

  inline void select(int index) const { selected = index; }
  inline int  size()            const { return dim_h; }

private:
  mutable int  selected;
  int          dim_h;
  Cell**       cells;
};

#endif