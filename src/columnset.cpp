#include <csv.h>
#include <memory>

namespace csv {
  CSV::ColumnSet::ColumnSet(CSV& csv, int* cols)
    : dim_h(0)
    , dim_v(csv.rows())
  {
    for (; cols[dim_h] > 0; dim_h++)
    {
      this->cols += new Column(csv, cols[dim_h]);
    }
    delete[] cols;
  }

  CSV::ColumnSet::~ColumnSet()
  {
    for (int i = 0; i < dim_h; i++) delete *(Column**)cols[i];
  }

  CSV::Selection CSV::ColumnSet::operator ()()
  {
    return (*this)(true, 0, -1);
  }

  CSV::Selection CSV::ColumnSet::operator ()(bool inrange, int row1, int row2)
  {
    int* indices = new int[dim_h]{0};
    int  s       = 0;

    if (row1 < 0) row1 += dim_h;
    if (row2 < 0) row2 += dim_h;
    if (row1 > row2) std::swap(row1, row2);
    if (inrange) {
      for (int i = row1; i <= row2; i++) indices[s++] = i;
    }
    else {
      for (int i = 0; i < row1; i++)         indices[s++] = i;
      for (int i = row2 + 1; i < dim_h; i++) indices[s++] = i;
    }
    indices[s] = -1;

    return Selection(*this, indices);
  }

  CSV::Row CSV::ColumnSet::operator [](int index)
  {
    return Row(*this, index);
  }
}