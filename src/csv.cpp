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

void CSV::addColumn()
{
    addColumn(nullptr);
}

void CSV::addColumn(const char *columnName)
{
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
}

void CSV::addRow()
{
    if (dim_v + 1 > cap_v) {
        int ncap;
        if (cap_v == 0) ncap = 1;
        else            ncap = 2 * cap_v;
        for (int j = 0; j < dim_h; j++) {
            char **column = new char*[ncap];
            for (int i = 0;     i < cap_h; i++) {
                column[i] = content[j][i];
            }
            for (int i = cap_h; i < ncap;  i++) {
                column[i] = nullptr;
            }
            delete[] content[j];
            content[j] = column;
        }
        cap_v = ncap;
    }
    dim_v++;
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