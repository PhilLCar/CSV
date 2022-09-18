#include <csv.h>

#include <cstring>

namespace csv {
  CSV::Row::Row(CSV& csv, int index)
    : dim_h(csv.dim_h)
    , selected(0)
  {
    cells = new Cell * [dim_h];
    for (int i = 0; i < dim_h; i++) {
      cells[i] = &csv.content[index][i];
    }
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

  CSV::Row::Row(ColumnSet& cs, int index)
    : dim_h(cs.size())
    , selected(0)
  {
    cells = new Cell * [dim_h];
    for (int i = 0; i < cs.cols.size(); i++)
    {
      cells[i] = &(**(Column**)cs.cols[i])[index];
    }
  }

  CSV::Row::~Row()
  {
    delete[] cells;
  }

  void CSV::Row::select(int index) const
  {
    selected = index;
  }

  bool CSV::Row::operator <(const CSV::Row& other)
  {
    switch (cells[selected]->type) {
    case Cell::INTEGER:
      return cells[selected]->integer() < other.cells[selected]->integer();
    case Cell::DECIMAL:
      return cells[selected]->decimal() < other.cells[selected]->decimal();
    case Cell::LITTERAL:
    default:
      return strcmp(cells[selected]->string(), other.cells[other.selected]->string()) < 0;
    }
  }

  CSV::Cell& CSV::Row::operator [](int index) const
  {
    return *cells[index];
  }

  CSV::Cell& CSV::Row::operator [](int index)
  {
    return *cells[index];
  }
}