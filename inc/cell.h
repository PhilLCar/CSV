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

#ifdef CSV_EXTERN
  static Cell* M(Cell, build)(Type = LITTERAL);
  void M(Cell, dispose)();
#endif

public:
  Cell(Type = LITTERAL);
  Cell(Cell const& cell);
  Cell(const char* content);
  Cell(int integer);
  Cell(double decimal);
  ~Cell();

  const char*  M(Cell, string)();
  const long   M(Cell, integer)();
  const double M(Cell, decimal)();

  void M(Cell, setContent)(const char *content);
  void M(Cell, setInteger)(long integer);
  void M(Cell, setDecimal)(double decimal);

  void M(Cell, setType)(Type type);

#ifndef CSV_EXTERN
  Cell& operator =(Cell const& cell);
  Cell& operator =(Cell&& cell) noexcept;
  friend inline std::ostream& operator <<(std::ostream& stream, CSV::Cell& cell)
  {
    stream << std::setw(cell.width) << cell.string();
    return stream;
  }
#endif
private:
  mutable Type type;
  int          width;
  char*        content;
};

#endif