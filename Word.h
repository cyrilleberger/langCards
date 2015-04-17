#ifndef WORD_H
#define WORD_H

#include <QString>

struct Word
{
  QString word_lang1, word_lang2;
  QString context_lang1, context_lang2;
  qreal probability;
};

#endif // WORD_H
