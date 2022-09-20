#include <csv.h>

#include <cstring>

CSV_BEGIN

CSV::Column::Column(const CSV& csv, int index)
  : header(&csv.header[index])
  , dim_v(csv.dim_v)
{
  cells = new Cell * [dim_v];
  for (int i = 0; i < dim_v; i++) {
    cells[i] = &csv.content[index][i];
  }
}

CSV::Column::Column(const Selection& selection, int index)
  : CSV::Column(selection.cs.ref, index)
{ // TODO
}

CSV::Column::Column(Column const& col)
  : header(col.header)
  , dim_v(col.dim_v)
{
  cells = new Cell * [dim_v];
  for (int i = 0; i < dim_v; i++) {
    cells[i] = col.cells[i];
  }
}

CSV::Column::~Column()
{
  delete[] cells;
}

#ifdef CSV_EXTERN
void CSV::Column::M(Column, dispose)()
{
  delete this;
}
#endif

bool CSV::Column::M(Column, lt)(const CSV::Column* other)
{
  return strcmp(header->string(), other->header->string()) < 0;
}

CSV::Cell* CSV::Column::M(Column, at)(int index)
{
  return cells[index];
}

int CSV::Column::M(Column, size)() const
{
  return dim_v;
}

CSV::Cell* CSV::Column::M(Column, name)()
{
  return header;
}

CSV_END