#include <csv.h>

#include <cstring>

CSV_BEGIN

CSV::Row::Row(const CSV& csv, int index)
  : dim_h(csv.dim_h)
  , selected(0)
{
  cells = new Cell * [dim_h];
  for (int i = 0; i < dim_h; i++) {
    cells[i] = &csv.content[index][i];
  }
}

CSV::Row::Row(const ColumnSet& cs, int index)
  : CSV::Row(cs.ref, index)
{ // TODO
}

CSV::Row::Row(Row const& row)
  : dim_h(row.dim_h)
  , selected(row.selected)
{
  cells = new Cell * [dim_h];
  for (int i = 0; i < dim_h; i++) {
    cells[i] = row.cells[i];
  }
}

CSV::Row::~Row()
{
  delete[] cells;
}

#ifdef CSV_EXTERN
void CSV::Row::M(Column, dispose)()
{
  delete this;
}
#endif

bool CSV::Row::M(Row, lt)(const CSV::Row* other)
{
  switch (cells[selected]->type) {
  case Cell::INTEGER:
    return cells[selected]->integer() < other->cells[selected]->integer();
  case Cell::DECIMAL:
    return cells[selected]->decimal() < other->cells[selected]->decimal();
  case Cell::LITTERAL:
  default:
    return strcmp(cells[selected]->string(), other->cells[selected]->string()) < 0;
  }
}

CSV::Cell* CSV::Row::M(Row, at)(int index)
{
  return cells[index];
}

void CSV::Row::select(int index) const
{
  selected = index;
}

int CSV::Row::M(Row, size)() const
{
  return dim_h;
}

CSV_END
