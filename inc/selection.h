#ifndef CSV_SELECTION_HEADER
#define CSV_SELECTION_HEADER

class CSV_API Selection {
  friend Column;
  friend CSV;
private:
  Selection(CSV& csv, int* indices);
  Selection(ColumnSet& cs, int* indices);
public:
  ~Selection();

  void  select(int index);
  //CSV&& toCSV();

  Column        operator [](const char* colName);
  Column        operator [](int index);
  inline Cell** columnNames() { return header; }
  inline int    columns()     { return dim_h;  }
  inline int    rows()        { return dim_v;  }
private:
  int column(const char* colName);
private:
  int          dim_h;
  int          dim_v;
  Cell**       header;
  PointerArray rowset;
};

#endif