#ifndef CSV_COLUMN_HEADER
#define CSV_COLUMN_HEADER

class CSV_API Column {
  friend CSV;
public:
  Column(CSV& csv, int index);
  Column(Selection& selection, int index);
  Column(Column const& col);
  ~Column();

  bool         operator <(const Column& other);
  Cell&        operator [](int index) const;
  Cell&        operator [](int index);
  inline int   size() { return  dim_v; }
  inline Cell& name() { return *header; }
private:
  int          dim_v;
  Cell*        header;
  Cell**       cells;
};

#endif