#include <csv.h>

#include <memory>
#include <stdexcept>

namespace csv {
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
          c = next(file, string);
          if (c == '\n' || c == EOF) break;
          addColumn(string);
        }
      }
      for (int row = 0; row == 0 || (c != '\n' && c != EOF); row++) {
        for (int col = 0;; col++) {
          c = next(file, string);
          if (c == '\n' || c == EOF) break;
          if (col == 0) addRow();
          if (row >= dim_h) resizeh(cap_h << 1);
          (*this)[col][row] = string;
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

  void CSV::flush()
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
  bool CSV::addColumn()
  {
    return addColumn(nullptr);
  }

  bool CSV::addColumn(const char* colName)
  {
    bool success = true;
    try {
    if (dim_h + 1 > cap_h) {
      int ncap;
      if (cap_h == 0) ncap = 1;
      else            ncap = cap_h << 1;
      resizeh(ncap);
    }
    header[dim_h]  = colName;
    content[dim_h] = new Cell[dim_v];
    dim_h++;
    } catch (...) {
      success = false;
    }
    return success;
  }

  bool CSV::moveColumn(int from, int to)
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

  bool CSV::moveColumn(const char *colName, int rel)
  {
    int from = column(colName);
    return moveColumn(from, from + rel);
  }

  bool CSV::swapColumn(int col1, int col2)
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

  bool CSV::swapColumn(const char* col1Name, const char* col2Name)
  {
    return swapColumn(column(col1Name), column(col2Name));
  }

  bool CSV::insertColumn(int index)
  {
    return insertColumn(index, nullptr);
  }

  bool CSV::insertColumn(int index, const char* colName)
  {
    bool success = false;
    if (index >= 0 && index < dim_h) {
      success |= addColumn(colName);
      success |= moveColumn(dim_h - 1, index);
    }
    return success;
  }

  bool CSV::deleteColumn(int index)
  {
    bool success = true;
    try {
      moveColumn(index, dim_h - 1);
      delete[] content[dim_h--];
      if (dim_h < cap_h >> 2) {
        int ncap = cap_h >> 1;
        resizeh(ncap);
      }
    } catch (...) {
      success = false;
    }
    return success;
  }

  bool CSV::deleteColumn(const char* colName)
  {
      return deleteColumn(column(colName));
  }

  // Rows
  //===================================================================================
  bool CSV::addRow()
  {
    bool success = true;
    try {
    if (dim_v + 1 > cap_v) {
      int ncap;
      if (cap_v == 0) ncap = 1;
      else            ncap = cap_v << 1;
      resizev(ncap);
    }
    dim_v++;
    } catch (...) {
      success = false;
    }
    return success;
  }

  bool CSV::moveRow(int from, int to)
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

  bool CSV::swapRow(int row1, int row2)
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

  bool CSV::insertRow(int index)
  {
    bool success = false;
    if (index >= 0 && index < dim_v) {
      success |= addRow();
      success |= moveRow(dim_v - 1, index);
    }
    return success;
  }

  bool CSV::deleteRow(int index)
  {
    bool success = true;
    try {
      moveRow(index, dim_v - 1);
      dim_v--;
      if (dim_v < cap_v >> 2) {
        int ncap = cap_v >> 1;
        resizev(ncap);
      }
    } catch (...) {
      success = false;
    }
    return success;
  }

  // Operators
  //===================================================================================
  CSV::ColumnSet CSV::operator ()()
  {
    return (*this)(true, 0, -1);
  }

  CSV::ColumnSet CSV::operator ()(bool inrange, const char* col1Name, const char* col2Name)
  {
    return (*this)(inrange, column(col1Name), column(col2Name));
  }

  CSV::ColumnSet CSV::operator ()(bool inrange, int col1, int col2)
  {
    int* indices = new int[dim_h]{0};
    int  s    = 0;

    if (col1 < 0) col1 += dim_h;
    if (col2 < 0) col2 += dim_h;
    if (col1 > col2) std::swap(col1, col2);
    if (inrange) {
      for (int i = col1; i <= col2; i++) indices[s++] = i;
    } else {
      for (int i = 0; i < col1; i++)         indices[s++] = i;
      for (int i = col2 + 1; i < dim_h; i++) indices[s++] = i;
    }
    indices[s] = -1;

    return ColumnSet(*this, indices);
  }

  CSV::Column CSV::operator [](const char *colName)
  {
    return Column(*this, column(colName));
  }

  CSV::Column CSV::operator [](int index)
  {
    if (index < 0) index += dim_h;
    return Column(*this, index);
  }

  // Private
  //===================================================================================
  void CSV::addToSet(CSV* csv, int* set, const char* colName)
  {
    addToSet(csv, set, csv->column(colName));
  }

  void CSV::addToSet(CSV* csv, int* set, int index)
  {
    int i;
    for (i = 0; set[i] > 0; i++) {
      if (set[i] == index) return;
    }
    set[i++] = index;
    set[i] = -1;
  }

  int CSV::column(const char* colName) 
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

  void CSV::resizeh(int ncap)
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

  void CSV::resizev(int ncap)
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
  char CSV::next(FILE* file, char* string) {
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
}