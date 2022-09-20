#ifndef CSV_SELECTION_HEADER
#define CSV_SELECTION_HEADER

class CSV_API Selection {
  friend ColumnSet;
  friend Column;
  friend CSV;
private:
  Selection(ColumnSet& cs);
public:
  ~Selection();

public:
  void sortBy(int index);
  void sortBy(const char* name);
  void sort(bool increasing = true);

  CSV csv(const char *filename, char separator = ',') const;

  // Modification related methods:
  // =============================
public:
  Selection& operator +=(int index);
  Selection& operator -=(int index);
  Selection& operator ^=(int index);

  // Indexation related methods:
  // ===========================
public:
  Column operator [](const char *colName);
  Column operator [](int index);

public:
  inline int rows() const { return dim_v; }
  inline int cols() const { return dim_h; }

private:
  void make_matrix();

private:
  const ColumnSet& cs;
  bool*            inset;
  int              dim_h;
  int              dim_v;
  int              index_s;
  Cell**           header;
  Cell***          matrix;
};

#endif