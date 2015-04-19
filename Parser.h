#ifndef PARSER_H
#define PARSER_H

#include <QStringList>

namespace LangCardsDB {
  class Words;
}

class fWord;

class Parser
{
public:
  Parser();
  QList<fWord> words(QIODevice* _device, LangCardsDB::Words* _words);
  QList<fWord> words(const QString& _file, LangCardsDB::Words* _words);
private:
};

#endif // PARSER_H
