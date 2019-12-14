#include <csv.h>

#include <string.h>

CSV::CSV(char *filename, bool firstRowIsHeader)
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
        char  **t_header  = new char *[ncap];
        char ***t_content = new char**[ncap];
        for (int i = 0;     i < cap_h; i++) {
            t_header[i]  = header[i];
            t_content[i] = content[i];
        }
        for (int i = cap_h; i < ncap;  i++) {
            t_header[i]  = nullptr;
            t_content[i] = nullptr;
        }
        delete[] header;
        delete[] content;
        header  = t_header;
        content = t_content;
        cap_h = ncap;
    }
    if (columnName != nullptr) {
        int length = strlen(columnName);
        header[dim_h] = new char[length + 1];
        strcpy(header[dim_h], columnName);
        header[dim_h][length] = '\0';
    }
    content[dim_h] = new char*[dim_v];
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
            char  *t_columnName = header[from];
            char **t_column     = content[from];
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
            char  *t_columnName = header[col1];
            char **t_column     = content[col1];
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

bool CSV::addRow()
{
    bool success = true;
    try {
    if (dim_v + 1 > cap_v) {
        int ncap;
        if (cap_v == 0) ncap = 1;
        else            ncap = 2 * cap_v;
        for (int j = 0; j < dim_h; j++) {
            char **t_column = new char*[ncap];
            for (int i = 0;     i < cap_h; i++) {
                t_column[i] = content[j][i];
            }
            for (int i = cap_h; i < ncap;  i++) {
                t_column[i] = nullptr;
            }
            delete[] content[j];
            content[j] = t_column;
        }
        cap_v = ncap;
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
                char  *t_cell = content[j][from];
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
                char  *t_cell = content[i][row1];
                content[i][row1] = content[i][row2];
                content[i][row2] = t_cell;
            }
        }
        success = true;
    }
    return success;
}

int CSV::column(const char *columnName) {
    int index = -1;
    for (int i = 0; i < dim_h; i++) {
        if (!strcmp(columnName, header[i])) {
            index = i;
            break;
        }
    }
    return index;
}