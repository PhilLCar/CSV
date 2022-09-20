#include <csv.h>

#include <memory>

CSV_BEGIN

CSV::ColumnSet::ColumnSet(CSV& csv)
  : ref(csv)
  , inset(new bool [csv.dim_h] {false})
{
}

CSV::ColumnSet::~ColumnSet()
{
  delete [] inset;
}

#ifdef CSV_EXTERN
void CSV::ColumnSet::M(ColumnSet, dispose)()
{
  delete this;
}
#endif

CSV::ColumnSet* CSV::ColumnSet::M(ColumnSet, add)(int index)
{
  inset[index] = true;
  return this;
}

CSV::ColumnSet* CSV::ColumnSet::M(ColumnSet, sub)(int index)
{
  inset[index] = false;
  return this;
}

CSV::ColumnSet* CSV::ColumnSet::M(ColumnSet, xor)(int index)
{
  inset[index] = !inset[index];
  return this;
}

CSV::ColumnSet* CSV::ColumnSet::M(ColumnSet, addN)(const char* colName)
{
  int index = ref._ioc(colName);
  if (index >= 0) inset[index] = true;
  return this;
}

CSV::ColumnSet* CSV::ColumnSet::M(ColumnSet, subN)(const char* colName)
{
  int index = ref._ioc(colName);
  if (index >= 0) inset[index] = false;
  return this;
}

CSV::ColumnSet* CSV::ColumnSet::M(ColumnSet, xorN)(const char* colName)
{
  int index = ref._ioc(colName);
  if (index >= 0) inset[index] = !inset[index];
  return this;
}

CSV::Selection* CSV::ColumnSet::M(ColumnSet, selectAll)()
{
  return M(ColumnSet, selectRange)(0, ref.dim_v - 1);
}

CSV::Selection* CSV::ColumnSet::M(ColumnSet, selectRange)(int row1, int row2)
{
  Selection* sel = new Selection(*this);

  if (row1 < 0) row1 += ref.dim_v;
  if (row2 < 0) row2 += ref.dim_v;
  if (row1 > row2) std::swap(row1, row2);
  for (int i = row1; i <= row2; i++) sel->M(Selection, add)(i);

  return sel;
}

CSV::Selection* CSV::ColumnSet::M(ColumnSet, selectRangeRel)(int row, int rel)
{
  return M(ColumnSet, selectRange)(row, row + rel);
}

CSV::Row* CSV::ColumnSet::M(ColumnSet, at)(int index)
{
  return new Row(*this, index);
}

int CSV::ColumnSet::size() const
{
  int s = 0;

  for (int i = 0; i < ref.dim_h; i++) if (inset[i]) s++;

  return s;
}

CSV_END