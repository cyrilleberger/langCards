#ifndef _TYPES_DEFINITIONS_H_
#define _TYPES_DEFINITIONS_H_

#include <QVariant>

namespace Sql
{
  enum Sort
  {
    ASC, DESC
  };
  enum Operand
  {
    EQ, GT, LT, LTE, GTE, NEQ, LIKE
  };
}

#endif
