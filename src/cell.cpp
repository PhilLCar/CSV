#include <csv.h>

#include <cstring>
#include <cstdio>

CSV_BEGIN

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

#ifdef CSV_EXTERN
static CSV::Cell* CSV::Cell::M(Cell, build)(Type type)
{
  return new Cell(type);
}

void CSV::Cell::M(Cell, dispose)()
{
  delete this;
}
#endif

const char* CSV::Cell::M(Cell, string)()
{
  return content;
}

const long CSV::Cell::M(Cell, integer)()
{
  return atol(content);
}

const double CSV::Cell::M(Cell, decimal)()
{
  return atof(content);
}

void CSV::Cell::M(Cell, setContent)(const char* content)
{
  delete[] this->content;
  this->content = new char[strlen(content) + 1] {0};
  sprintf(this->content, "%s", content);
}

void CSV::Cell::M(Cell, setInteger)(long integer)
{
  char content[512];

  itoa(integer, content, 10);
  delete[] this->content;
  this->content = new char[strlen(content) + 1] {0};
  sprintf(this->content, "%s", content);
}

void CSV::Cell::M(Cell, setDecimal)(double decimal)
{
  char content[512];

  gcvt(decimal, width, content);
  delete[] this->content;
  this->content = new char[strlen(content) + 1] {0};
  sprintf(this->content, "%s", content);
}

void CSV::Cell::M(Cell, setType)(Type type)
{
  this->type = type;
}

#ifndef CSV_EXTERN
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
#endif

CSV_END
