#include <csv.h>

#include <memory>

namespace csv {

  CSV::ColumnSet::ColumnSet(CSV& csv)
    : ref(csv)
    , inset(new bool [csv.dim_h] {false})
  {
  }

  CSV::ColumnSet::~ColumnSet()
  {
    delete [] inset;
  }

  CSV::ColumnSet& CSV::ColumnSet::operator +=(int index)
  {
    inset[index] = true;
    return *this;
  }

  CSV::ColumnSet& CSV::ColumnSet::operator -=(int index)
  {
    inset[index] = false;
    return *this;
  }

  CSV::ColumnSet& CSV::ColumnSet::operator ^=(int index)
  {
    inset[index] = !inset[index];
    return *this;
  }

  CSV::ColumnSet& CSV::ColumnSet::operator +=(const char* colName)
  {
    int index = ref.ioc(colName);
    if (index >= 0) inset[index] = true;
    return *this;
  }

  CSV::ColumnSet& CSV::ColumnSet::operator -=(const char* colName)
  {
    int index = ref.ioc(colName);
    if (index >= 0) inset[index] = false;
    return *this;
  }

  CSV::ColumnSet& CSV::ColumnSet::operator ^=(const char* colName)
  {
    int index = ref.ioc(colName);
    if (index >= 0) inset[index] = !inset[index];
    return *this;
  }

  CSV::Selection CSV::ColumnSet::operator ()()
  {
    return (*this)(0, ref.dim_v - 1);
  }

  CSV::Selection CSV::ColumnSet::operator ()(int row, int rel)
  {
    Selection sel(*this);

    if (row < 0) row += ref.dim_v;
    for (int i = row; i <= row + rel; i++) sel += i;

    return sel;
  }

  CSV::Row CSV::ColumnSet::operator [](int index)
  {
    return Row(*this, index);
  }

  int CSV::ColumnSet::size() const
  {
    int s = 0;

    for (int i = 0; i < ref.dim_h; i++) if (inset[i]) s++;

    return s;
  }
}
