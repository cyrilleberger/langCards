#ifndef PARSER_H
#define PARSER_H

#include <QStringList>

class QIODevice;

namespace LangCardsDB
{
  class Words;
}

class fWord;

class Parser
{
public:
  Parser();
  QList<fWord> words(QIODevice* _device, LangCardsDB::Words* _words, qreal& _total_proba);
  QList<fWord> words(const QString& _file, LangCardsDB::Words* _words, qreal& _total_proba);
private:
};

#endif // PARSER_H
