#include <csv.h>

#include <string.h>
#include <stdio.h>
#include <iomanip>

// Cell
//===================================================================================
CSV::Cell::Cell() : Cell("")
{
}

CSV::Cell::Cell(const char *content)
    : width(20)
{
    this->content = new char[strlen(content) + 1]{0};
    sprintf(this->content, "%s", content);
}

CSV::Cell::~Cell()
{
    delete content;
}

std::ostream operator << (std::ostream &stream, CSV::Cell &cell) {
    stream << std::setw(cell.width) << cell.string();
}

// Column
//===================================================================================
CSV::Column::Column(CSV &csv, int index)
    : header(&csv.columnNames()[index])
{
    cells = new Cell*[csv.dim_v];
    for (int i = 0; i < csv.dim_v; i++) {
        cells[i] = &csv.content[index][i];
    }
}

CSV::Column::~Column()
{
    delete cells;
}

CSV::Cell& CSV::Column::operator [](int index)
{
    return *cells[index];
}

// Row
//===================================================================================

// CSV
//===================================================================================
CSV::CSV(const char *filename, bool firstRowIsHeader)
    : filename(filename)
    , hasHeader(firstRowIsHeader)
    , header(nullptr)
    , content(nullptr)
    , dim_h(0)
    , dim_v(0)
    , cap_h(0)
    , cap_v(0)
{
}

CSV::~CSV()
{
    for (int j = 0; j < dim_h; j++) {
        if (content[j] != nullptr) delete[] content[j];
    }
    if (header  != nullptr) delete[] header;
    if (content != nullptr) delete[] content;
}

// Columns
//===================================================================================
bool CSV::addColumn()
{
    return addColumn(nullptr);
}

bool CSV::addColumn(const char *columnName)
{
    bool success = true;
    try {
    if (dim_h + 1 > cap_h) {
        int ncap;
        if (cap_h == 0) ncap = 1;
        else            ncap = 2 * cap_h;
        resizeh(ncap);
    }
    header[dim_h] = Cell(columnName);
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
                for (int i = from; i > to; i++) {
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

bool CSV::moveColumn(const char *columnName, int rel)
{
    int from = column(columnName);
    return moveColumn(from, from + rel);
}

bool CSV::swapColumn(int col1, int col2)
{
    bool success = false;
    if (col1 >= 0 && col1 < dim_h && col2 >= 0 && col2 < dim_h) {
        if (col1 != col2) {
            Cell  t_columnName = header[col1];
            Cell *t_column     = content[col1];
            header[col1]  = header[col2];
            content[col1] = content[col2];
            header[col2]  = t_columnName;
            content[col2] = t_column;
        }
        success = true;
    }
    return success;
}

bool CSV::swapColumn(const char *col1Name, const char *col2Name)
{
    return swapColumn(column(col1Name), column(col2Name));
}

bool CSV::insertColumn(int index)
{
    return insertColumn(index, nullptr);
}

bool CSV::insertColumn(int index, const char *columnName)
{
    bool success = false;
    if (index >= 0 && index < dim_h) {
        success |= addColumn(columnName);
        success |= moveColumn(dim_h - 1, index);
    }
    return success;
}

bool CSV::deleteColumn(int index)
{
    bool success = true;
    try {
        moveColumn(index, --dim_h);
        if (content[dim_h] != nullptr) delete[] content[dim_h];
        if (dim_h < cap_h / 4) {
            int ncap = cap_h / 2;
            resizeh(ncap);
        }
    } catch (...) {
        success = false;
    }
    return success;
}

bool CSV::deleteColumn(const char *columnName)
{
    deleteColumn(column(columnName));
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
        else            ncap = 2 * cap_v;
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
                    for (int i = from; i > to; i++) {
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
        moveRow(index, --dim_v);
        if (dim_v < cap_v / 4) {
            int ncap = cap_v / 2;
            resizev(ncap);
        }
    } catch (...) {
        success = false;
    }
    return success;
}

// Operators
//===================================================================================
CSV::Column CSV::operator [](int index)
{
    return Column(*this, index);
}

// Private
//===================================================================================
int CSV::column(const char *columnName) 
{
    int index = -1;
    for (int i = 0; i < dim_h; i++) {
        if (!strcmp(columnName, header[i].string())) {
            index = i;
            break;
        }
    }
    return index;
}

void CSV::resizeh(int ncap)
{
    Cell  *t_header  = new Cell[ncap];
    Cell **t_content = new Cell*[ncap];
    for (int i = 0;     i < dim_h; i++) {
        t_header[i]  = header[i];
        t_content[i] = content[i];
    }
    for (int i = dim_h; i < ncap;  i++) {
        t_header[i]  = nullptr;
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
        Cell *t_column = new Cell[ncap];
        for (int i = 0;     i < dim_v; i++) {
            t_column[i] = content[j][i];
        }
        for (int i = dim_v; i < ncap;  i++) {
            t_column[i] = nullptr;
        }
        delete[] content[j];
        content[j] = t_column;
    }
    cap_v = ncap;
}