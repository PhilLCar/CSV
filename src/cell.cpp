#include <csv.h>

#include <cstring>
#include <cstdio>
#include <iomanip>

namespace csv {
  CSV::Cell::Cell(Type type) : Cell("")
  {
    this->type = type;
  }

  CSV::Cell::Cell(Cell const& cell) : Cell(cell.content)
  {
    this->type = cell.type;
  }

  CSV::Cell::Cell(const char* content)
    : width(20)
    , type(LITTERAL)
  {
    if (content != nullptr) {
      this->content = new char[strlen(content) + 1] {0};
      sprintf(this->content, "%s", content);
    }
    else {
      this->content = new char[1] {0};
    }
  }

  CSV::Cell::Cell(int integer)
    : width(20)
    , type(INTEGER)
  {
    content = new char[20] {0};
    sprintf(content, "%d", integer);
  }

  CSV::Cell::Cell(double decimal)
    : width(20)
    , type(DECIMAL)
  {
    content = new char[20] {0};
    sprintf(content, "%f", decimal);
  }

  CSV::Cell::~Cell()
  {
    delete[] content;
  }

  CSV::Cell& CSV::Cell::operator =(Cell const& cell)
  {
    width = cell.width;
    type  = cell.type;
    delete[] content;
    content = new char[strlen(cell.content) + 1] {0};
    sprintf(content, "%s", cell.content);
    return *this;
  }

  CSV::Cell& CSV::Cell::operator =(Cell&& cell) noexcept
  {
    width = cell.width;
    type  = cell.type;
    delete[] content;
    content = cell.content;
    cell.content = nullptr;
    return *this;
  }
}