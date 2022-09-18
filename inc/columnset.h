#ifndef CSV_COLUMNSET_HEADER
#define CSV_COLUMNSET_HEADER

class CSV_API ColumnSet {
  friend Selection;
  friend Row;
  friend CSV;
private:
  ColumnSet(CSV& csv, int* indices);
public:
  ~ColumnSet();

  Selection  operator ()();
  Selection  operator ()(bool inrange, int row1, int row2);
#ifndef CSV_EXPORT
  template <typename... args>
  Selection  operator ()(args... rowIndices) {
    std::set<int> rows;
    int exp[sizeof...(args)] = { (addtoset(nullptr, rows, rowIndices), 0)... };
    return Selection(*this, rows);
  }
#endif

  Row        operator [](int index);
  inline int size() { return dim_h; }
  inline int rows() { return dim_v; }
private:
  int          dim_h;
  int          dim_v;
  PointerArray cols;
};

#endif