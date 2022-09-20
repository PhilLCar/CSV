#include <csv.h>

#include <cstring>

namespace csv {

  CSV::Selection::Selection(ColumnSet& cs)
    : cs(cs)
    , inset(new bool [cs.ref.dim_v] {false})
    , dim_h(0)
    , dim_v(0)
    , header(nullptr)
    , matrix(nullptr)
  {
  }

  CSV::Selection::~Selection()
  {
    delete[] inset;
  }

  void CSV::Selection::sortBy(int index)
  {
    index_s = index;
  }

  void CSV::Selection::sortBy(const char* colName)
  {
    index_s = cs.ref.ioc(colName);
  }

  void CSV::Selection::sort(bool increasing)
  {
    // TODO
  }

  CSV CSV::Selection::csv(const char* filename, char separator) const
  {
    // TODO
    return CSV("");
  }

  CSV::Selection& CSV::Selection::operator +=(int index)
  {
    inset[index] = true;
    make_matrix();
    return *this;
  }

  CSV::Selection& CSV::Selection::operator -=(int index)
  {
    inset[index] = false;
    make_matrix();
    return *this;
  }

  CSV::Selection& CSV::Selection::operator ^=(int index)
  {
    inset[index] = !inset[index];
    make_matrix();
    return *this;
  }

  CSV::Column CSV::Selection::operator [](const char* colName)
  {
    return Column(*this, cs.ref.ioc(colName));
  }

  CSV::Column CSV::Selection::operator [](int index)
  {
    return Column(*this, index);
  }

  void CSV::Selection::make_matrix()
  {
    dim_h = cs.size();
    dim_v = 0;
    for (int i = 0; i < cs.ref.dim_v; i++) if (inset[i]) dim_v++;
  
    if (header) delete[] header;
    if (matrix) {
      for (int col = 0; col < dim_h; col++) delete[] matrix[col];
      delete[] matrix;
    }

    header = new Cell* [dim_h] {nullptr};
    matrix = new Cell**[dim_h] {nullptr};
    for (int j = 0, col = 0; col < dim_h; j++) {
      if (!cs.inset[j]) continue;

      header[col] = &cs.ref.header[j];
      matrix[col] = new Cell*[dim_v] {nullptr};
      for (int i = 0, row = 0; row < dim_v; i++) {
        if (!inset[i]) continue;

        matrix[col][row++] = &cs.ref.content[j][i];
      }
      col++;
    }
  }
}
