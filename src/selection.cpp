#include <csv.h>

#include <cstring>

CSV_BEGIN

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

#ifdef CSV_EXTERN
void CSV::Selection::M(Selection, dispose)()
{
  delete this;
}
#endif

void CSV::Selection::M(Selection, sortBy)(int index)
{
  index_s = index;
}

void CSV::Selection::M(Selection, sortByN)(const char* colName)
{
  index_s = cs.ref._ioc(colName);
}

void CSV::Selection::M(Selection, sort)(bool increasing)
{
  // TODO
}

CSV* CSV::Selection::M(Selection, toCSV)(const char* filename, char separator)
{
  // TODO
  return nullptr;
}

CSV::Selection* CSV::Selection::M(Selection, add)(int index)
{
  inset[index] = true;
  _make_matrix();
  return this;
}

CSV::Selection* CSV::Selection::M(Selection, sub)(int index)
{
  inset[index] = false;
  _make_matrix();
  return this;
}

CSV::Selection* CSV::Selection::M(Selection, xor)(int index)
{
  inset[index] = !inset[index];
  _make_matrix();
  return this;
}

CSV::Column* CSV::Selection::M(Selection, column)(const char* colName)
{
  return new Column(*this, cs.ref._ioc(colName));
}

CSV::Column* CSV::Selection::M(Selection, at)(int index)
{
  return new Column(*this, index);
}

int CSV::Selection::M(Selection, rows)() const
{
  return dim_v;
}

int CSV::Selection::M(Selection, cols)() const
{
  return dim_h;
}

void CSV::Selection::_make_matrix()
{
  dim_h = cs.M(ColumnSet, size)();
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

CSV_END