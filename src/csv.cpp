#include <csv.h>

#include <string.h>
#include <stdio.h>
#include <iomanip>

// Cell
//===================================================================================
CSV::Cell::Cell() : Cell("")
{
}

CSV::Cell::Cell(Cell const &cell)
    : Cell(cell.content)
{
}

CSV::Cell::Cell(const char *content)
    : width(20)
{
    if (content != nullptr) {
        this->content = new char[strlen(content) + 1]{0};
        sprintf(this->content, "%s", content);
    } else {
        this->content = new char[1]{0};
    }
}

CSV::Cell::~Cell()
{
    delete content;
}

CSV::Cell &CSV::Cell::operator =(Cell const &cell)
{
    width = cell.width;
    delete content;
    content = new char[strlen(cell.content) + 1]{0};
    sprintf(content, "%s", cell.content);
    return *this;
}

CSV::Cell &CSV::Cell::operator =(Cell &&cell)
{
    width = cell.width;
    delete content;
    content = cell.content;
    cell.content = nullptr;
    return *this;
}

std::ostream& operator << (std::ostream &stream, CSV::Cell &cell) {
    stream << std::setw(cell.width) << cell.string();
}

// Column
//===================================================================================
CSV::Column::Column(CSV &csv, int index)
    : header(&csv.header[index])
    , _size(csv.dim_v)
{
    cells = new Cell*[_size];
    for (int i = 0; i < _size; i++) {
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
CSV::Row::Row(CSV csv, int index)
    : _size(csv.dim_h)
{
    cells = new Cell*[_size];
    for (int i = 0; i < _size; i++) {
        cells[i] = &csv.content[index][i];
    }
}

CSV::Row::~Row()
{
    delete cells;
}

CSV::Cell& CSV::Row::operator [](int index)
{
    return *cells[index];
}

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
    FILE *file = fopen(filename, "r");
    char string[512] = {0};
    if (file != nullptr) {
        if (firstRowIsHeader) {
            char c;
            do {
                c = next(file, string);
                addColumn(string);
            } while (c != '\n' && c != EOF);
        }
        char c = ',';
        for (int row = 0; c != EOF; row++) {
            addRow();
            c = ',';
            for (int col = 0; c != EOF && c != '\n'; col++) {
                c = next(file, string);
                (*this)[col][row] = string;
            }
        }
    }
    fclose(file);
}

CSV::~CSV()
{
    for (int j = 0; j < dim_h; j++) {
        delete[] content[j];
    }
    delete[] header;
    delete[] content;
}

// Columns
//===================================================================================
bool CSV::addColumn()
{
    return addColumn(nullptr);
}

bool CSV::addColumn(const char *colName)
{
    bool success = true;
    try {
    if (dim_h + 1 > cap_h) {
        int ncap;
        if (cap_h == 0) ncap = 1;
        else            ncap = 2 * cap_h;
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

bool CSV::insertColumn(int index, const char *colName)
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
        if (dim_h < cap_h / 4) {
            int ncap = cap_h / 2;
            resizeh(ncap);
        }
    } catch (...) {
        success = false;
    }
    return success;
}

bool CSV::deleteColumn(const char *colName)
{
    deleteColumn(column(colName));
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
        moveRow(index, dim_v - 1);
        dim_v--;
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
int CSV::column(const char *colName) 
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
    Cell  *t_header  = new Cell[ncap];
    Cell **t_content = new Cell*[ncap];
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
        Cell *t_column = new Cell[ncap];
        for (int i = 0;     i < dim_v; i++) {
            t_column[i] = content[j][i];
        }
        delete[] content[j];
        content[j] = t_column;
    }
    cap_v = ncap;
}

char CSV::next(FILE *file, char *string) {
    memset(string, 0, strlen(string));
    char c;
    bool quotes = false;
    for (int i = 0; ; i++) {
        do {
            c = getc(file);
        } while (c == '\r');
        if (c == '"') quotes = !quotes;
        if ((c == ',' && !quotes) || c == '\n' || c == EOF) break;
        string[i] = c;
    }
    return c;
}