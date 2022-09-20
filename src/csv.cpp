#include <csv.h>

#include <memory>
#include <stdexcept>

CSV_BEGIN

CSV::CSV(const char* filename, bool firstRowIsHeader, char separator)
  : filename(filename)
  , hasHeader(firstRowIsHeader)
  , separator(separator)
  , header(nullptr)
  , content(nullptr)
  , dim_h(0)
  , dim_v(0)
  , cap_h(0)
  , cap_v(0)
{
  FILE* file = fopen(filename, "r");
  char string[512] = "";

  if (file != nullptr) {
    char c = '\0';

    if (firstRowIsHeader) {
      while (true) {
        c = _next(file, string);
        if (c == '\n' || c == EOF) break;
        M(CSV, addNColumn)(string);
      }
    }
    for (int row = 0; row == 0 || (c != '\n' && c != EOF); row++) {
      for (int col = 0;; col++) {
        c = _next(file, string);
        if (c == '\n' || c == EOF) break;
        if (col == 0) M(CSV, addRow)();
        if (row >= dim_h) _resizeh(cap_h << 1);
        content[col][row] = string;
      }
    }
    fclose(file);
  }
}

CSV::~CSV()
{
  for (int j = 0; j < dim_h; j++) {
    delete[] content[j];
  }
  delete[] header;
  delete[] content;
}

#ifdef CSV_EXTERN
static CSV* CSV::M(CSV, build)(const char* filename, bool firstRowIsHeader, char separator)
{
  return new CSV(filename, firstRowIsHeader, separator);
}

void CSV::M(CSV, dispose)()
{
  delete this;
}
#endif

void CSV::M(CSV, flush)()
{
  FILE *file = fopen(filename, "w+");

  if (hasHeader) {
    for (int i = 0; i < dim_h; i++) {
      fprintf(file, "%s", header[i].string());
      fprintf(file, "%c", i < dim_h - 1 ? separator : '\n');
    }
  }

  for (int j = 0; j < dim_v; j++) {
    for (int i = 0; i < dim_h; i++) {
      fprintf(file, "%s", content[j][i].string());
      fprintf(file, "%c", i < dim_h - 1 ? separator : '\n');
    }
  }

  fclose(file);
}

// Columns
//===================================================================================
bool CSV::M(CSV, addColumn)()
{
  return M(CSV, addNColumn)(nullptr);
}

bool CSV::M(CSV, addNColumn)(const char* colName)
{
  bool success = true;
  try {
    if (dim_h + 1 > cap_h) {
      int ncap;
      if (cap_h == 0) ncap = 1;
      else            ncap = cap_h << 1;
      _resizeh(ncap);
    }
    if (colName != nullptr) header[dim_h]  = colName;
    content[dim_h++] = new Cell[dim_v];
  } catch (...) {
    success = false;
  }
  return success;
}

bool CSV::M(CSV, moveColumn)(int from, int to)
{
  bool success = false;
  if (from >= 0 && from < dim_h && to >= 0 && to < dim_h) {
    if (from != to) {
      Cell  t_columnName = header[from];
      Cell *t_column     = content[from];
      if (from < to) {
        for (int i = from; i < to; i++) {
          header[i]  = header[i + 1];
          content[i] = content[i + 1];
        }
      } else {
        for (int i = from; i > to; i--) {
          header[i]  = header[i - 1];
          content[i] = content[i - 1];
        }
      }
      header[to]  = t_columnName;
      content[to] = t_column;
    }
    success = true;
  }
  return success;
}

bool CSV::M(CSV, moveNColumn)(const char* fromName, const char *toName)
{
  int from = _ioc(fromName);
  int to   = _ioc(toName);

  if (from >= 0 && to >= 0) return M(CSV, moveColumn)(from, to);
  return false;
}

bool CSV::M(CSV, moveColumnRel)(int from, int rel)
{
  return M(CSV, moveColumn)(from, from + rel);
}

bool CSV::M(CSV, moveNColumnRel)(const char *colName, int rel)
{
  int col = _ioc(colName);

  if (col >= 0) return M(CSV, moveColumnRel)(col, rel);
  return false;
}

bool CSV::M(CSV, swapColumn)(int col1, int col2)
{
  bool success = false;
  if (col1 >= 0 && col1 < dim_h && col2 >= 0 && col2 < dim_h) {
    if (col1 != col2) {
      Cell  t_columnName = header[col1];
      Cell* t_column     = content[col1];
      header[col1]  = header[col2];
      content[col1] = content[col2];
      header[col2]  = t_columnName;
      content[col2] = t_column;
    }
    success = true;
  }
  return success;
}

bool CSV::M(CSV, swapNColumn)(const char* col1Name, const char* col2Name)
{
  int col1 = _ioc(col1Name);
  int col2 = _ioc(col2Name);

  if (col1 >= 0 && col1 >= 0) return M(CSV,swapColumn)(col1, col2);
  return false;
}

bool CSV::M(CSV, insertColumn)(int index)
{
  return M(CSV, insertNColumn)(index, nullptr);
}

bool CSV::M(CSV, insertNColumn)(int index, const char* colName)
{
  bool success = false;
  if (index >= 0 && index < dim_h) {
    if (M(CSV, addNColumn)(colName)) success = M(CSV, moveColumn)(dim_h - 1, index);
  }
  return success;
}

bool CSV::M(CSV, removeColumn)(int index)
{
  bool success = true;
  try {
    M(CSV, moveColumn)(index, dim_h - 1);
    delete[] content[dim_h--];
    if (dim_h < cap_h >> 2) {
      int ncap = cap_h >> 1;
      _resizeh(ncap);
    }
  } catch (...) {
    success = false;
  }
  return success;
}

bool CSV::M(CSV, removeNColumn)(const char* colName)
{
  int col = _ioc(colName);

  if (col >= 0) return M(CSV, removeColumn)(col);
  return false;
}

CSV::Cell* CSV::M(CSV, columnNames)() const
{
  return header;
}

int CSV::M(CSV, cols)() const
{
  return dim_h;
}

// Rows
//===================================================================================
bool CSV::M(CSV, addRow)()
{
  bool success = true;
  try {
  if (dim_v + 1 > cap_v) {
    int ncap;
    if (cap_v == 0) ncap = 1;
    else            ncap = cap_v << 1;
    _resizev(ncap);
  }
  dim_v++;
  } catch (...) {
    success = false;
  }
  return success;
}

bool CSV::M(CSV, moveRow)(int from, int to)
{
  bool success = false;
  if (from >= 0 && from < dim_v && to >= 0 && to < dim_v) {
    if (from != to) {
      for (int j = 0; j < dim_h; j++)
      {
        Cell t_cell = content[j][from];
        if (from < to) {
          for (int i = from; i < to; i++) {
            content[j][i] = content[j][i + 1];
          }
        } else {
          for (int i = from; i > to; i--) {
            content[j][i] = content[j][i - 1];
          }
        }
        content[j][to] = t_cell;
      }
    }
    success = true;
  }
  return success;
}

bool CSV::M(CSV, swapRow)(int row1, int row2)
{
  bool success = false;
  if (row1 >= 0 && row1 < dim_v && row2 >= 0 && row2 < dim_v) {
    if (row1 != row2) {
      for (int i = 0; i < dim_h; i++)
      {
        Cell t_cell = content[i][row1];
        content[i][row1] = content[i][row2];
        content[i][row2] = t_cell;
      }
    }
    success = true;
  }
  return success;
}

bool CSV::M(CSV, insertRow)(int index)
{
  bool success = false;
  if (index >= 0 && index < dim_v) {
    if (M(CSV, addRow)()) success = M(CSV, moveRow)(dim_v - 1, index);
  }
  return success;
}

bool CSV::M(CSV, deleteRow)(int index)
{
  bool success = true;
  try {
    M(CSV, moveRow)(index, dim_v - 1);
    dim_v--;
    if (dim_v < cap_v >> 2) {
      int ncap = cap_v >> 1;
      _resizev(ncap);
    }
  } catch (...) {
    success = false;
  }
  return success;
}

int CSV::M(CSV, rows)() const
{
  return dim_v;
}

// Operators
//===================================================================================
CSV::ColumnSet* CSV::M(CSV, selectAll)()
{
  return M(CSV, selectRangeRel)(0, dim_h - 1);
}

CSV::ColumnSet* CSV::M(CSV, selectRange)(int col1, int col2)
{
  ColumnSet* cs = new ColumnSet(*this);

  if (col1 < 0) col1 += dim_h;
  if (col2 < 0) col2 += dim_h;
  if (col1 > col2) std::swap(col1, col2);
  for (int i = col1; i <= col2; i++) cs->M(ColumnSet, add)(i);

  return cs;
}

CSV::ColumnSet* CSV::M(CSV, selectNRange)(const char* col1Name, const char* col2Name)
{
  int col1 = _ioc(col1Name);
  int col2 = _ioc(col2Name);

  if (col1 >= 0 && col1 >= 0) return M(CSV, selectRange)(col1, col2);
  return new ColumnSet(*this);
}

CSV::ColumnSet* CSV::M(CSV, selectRangeRel)(int col, int rel)
{
  return M(CSV, selectRange)(col, col + rel);
}

CSV::ColumnSet* CSV::M(CSV, selectNRangeRel)(const char* colName, int rel)
{
  int col = _ioc(colName);

  if (col >= 0) return M(CSV, selectRangeRel)(col, rel);
  return new ColumnSet(*this);
}

CSV::ColumnSet* CSV::M(CSV, selectNames)(int n, ...)
{
  ColumnSet* cs = new ColumnSet(*this);
  va_list    args;

  va_start(args, n);
  for (int i = 0; i < n; ++i) {
    int col = _ioc(va_arg(args, char*));
    if (col >= 0) cs->M(ColumnSet, sub)(col);
  }
  va_end(args);

  return cs;
}

CSV::Column* CSV::M(CSV, column)(const char* colName)
{
  int col = _ioc(colName);

  if (col >= 0) return new Column(*this, col);
  // This will fail on purpose vv
  return new Column(*this, dim_h);
}

CSV::Column* CSV::M(CSV, at)(int index)
{
  if (index < 0) index += dim_h;
  return new Column(*this, index);
}

// Private
//===================================================================================
int CSV::_ioc(const char* colName) const
{
  int index = -1;
  for (int i = 0; i < dim_h; i++) {
    if (!strcmp(colName, header[i].string())) {
      index = i;
      break;
    }
  }
  if (index == -1) {
    char error[64] = {0};
    sprintf(error, "Column '%s' doesn't exist", colName);
    throw std::out_of_range(error);
  }
  return index;
}

void CSV::_resizeh(int ncap)
{
  Cell*  t_header  = new Cell[ncap];
  Cell** t_content = new Cell*[ncap];
  for (int i = 0;     i < dim_h; i++) {
    t_header[i]  = header[i];
    t_content[i] = content[i];
  }
  for (int i = dim_h; i < ncap;  i++) {
    t_content[i] = nullptr;
  }
  delete[] header;
  delete[] content;
  header  = t_header;
  content = t_content;
  cap_h = ncap;
}

void CSV::_resizev(int ncap)
{
  for (int j = 0; j < dim_h; j++) {
    Cell* t_column = new Cell[ncap];
    for (int i = 0;     i < dim_v; i++) {
      t_column[i] = content[j][i];
    }
    delete[] content[j];
    content[j] = t_column;
  }
  cap_v = ncap;
}

// TODO: Make a better parsing function
char CSV::_next(FILE* file, char* string) {
  bool quotes = false;
  char c;

  for (int i = 0;; i++) {
    do {
      c = getc(file);
    } while (c == '\r');
    if (c == '\\') c = getc(file);
    else if (c == '"') quotes = !quotes;
    if ((c == ',' && !quotes) || c == '\n' || c == EOF) {
      string[i] = 0;
      break;
    }
    string[i] = c;
  }

  return c;
}

CSV_END