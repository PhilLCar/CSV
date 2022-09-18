#include <csv.h>

#include <cstring>
#include <stdexcept>

namespace csv {
  CSV::Selection::Selection(CSV& csv, int* rows)
    : dim_h(csv.columns())
    , dim_v(0)
  {
    header = new Cell*[dim_h];
    for (int i = 0; i < dim_h; i++)
    {
      header[i] = &csv.header[i];
    }
    for (; rows[dim_v] > 0; dim_v++)
    {
      this->rowset += new Row(csv, rows[dim_v]);
    }
    delete[] rows;
  }

  CSV::Selection::Selection(ColumnSet& cs, int* rows)
    : dim_h((int)cs.size())
    , dim_v(0)
  {
    header = new Cell * [dim_h];
    for (int i = 0; i < cs.dim_h; i++)
    {
      header[i] = (*(Column**)cs.cols[i])->header;
    }
    for (; rows[dim_v] > 0; dim_v++)
    {
      this->rowset += new Row(cs, rows[dim_v]);
    }
    delete[] rows;
  }

  CSV::Selection::~Selection()
  {
    delete[] header;
    for (int i = 0; i < rowset.size(); i++) delete *(Row**)rowset[i];
  }

  //CSV&& CSV::Selection::toCSV()
  //{
  //  // TODO
  //}

  void CSV::Selection::select(int index)
  {
    for (int i = 0; i < rowset.size(); i++)
    {
      (*(Row**)rowset[i])->select(i);
    }
  }

  CSV::Column CSV::Selection::operator [](const char* colName)
  {
    return Column(*this, column(colName));
  }

  CSV::Column CSV::Selection::operator [](int index)
  {
    if (index < 0) index += dim_h;
    return Column(*this, index);
  }

  int CSV::Selection::column(const char* colName)
  {
    int index = -1;
    for (int i = 0; i < dim_h; i++) {
      if (!strcmp(colName, header[i]->string())) {
        index = i;
        break;
      }
    }
    if (index == -1) {
      char error[64] = { 0 };
      sprintf(error, "Column '%s' doesn't exist", colName);
      throw std::out_of_range(error);
    }
    return index;
  }
}