#ifndef CSV_CELL_HEADER
#define CSV_CELL_HEADER

class CSV_API Cell {
  friend Row;
private:
  enum Type {
    LITTERAL,
    INTEGER,
    DECIMAL
  };

public:
  Cell(Type = LITTERAL);
  Cell(Cell const& cell);
  Cell(const char* content);
  Cell(int integer);
  Cell(double decimal);
  ~Cell();

  inline const char*  string()  { return content; }
  inline const long   integer() { return atol(content); }
  inline const double decimal() { return atof(content); }

  Cell& operator =(Cell const& cell);
  Cell& operator =(Cell&& cell) noexcept;
  /*friend inline std::ostream& operator <<(std::ostream& stream, CSV::Cell& cell)
  {
    stream << std::setw(cell.width) << cell.string();
    return stream;
  }*/

private:
  mutable Type type;
  int          width;
  char*        content;
};

#endif