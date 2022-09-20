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

public:
  bool  operator <(const Column& other);
  Cell& operator [](int index) const;
  Cell& operator [](int index);

public:
  inline int   size() const { return dim_v;   }
  inline Cell& name()       { return *header; }

private:
  int    dim_v;
  Cell*  header;
  Cell** cells;
};

#endif