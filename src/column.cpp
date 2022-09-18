#include <csv.h>

#include <cstring>

namespace csv {
  CSV::Column::Column(CSV& csv, int index)
    : header(&csv.header[index])
    , dim_v(csv.dim_v)
  {
    cells = new Cell * [dim_v];
    for (int i = 0; i < dim_v; i++) {
      cells[i] = &csv.content[index][i];
    }
  }

  CSV::Column::Column(Selection& selection, int index)
    : header(selection.header[index])
    , dim_v(selection.dim_v)
  {
    int i = 0;

    cells = new Cell * [dim_v];
    for (int i = 0; i < selection.rowset.size(); i++)
    {
      cells[i] = &(**(Row**)selection.rowset[i])[index];
    }
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

  bool CSV::Column::operator <(const CSV::Column& other)
  {
    return strcmp(header->string(), other.header->string()) < 0;
  }

  CSV::Cell& CSV::Column::operator [](int index) const
  {
    return *cells[index];
  }

  CSV::Cell& CSV::Column::operator [](int index)
  {
    return *cells[index];
  }
}