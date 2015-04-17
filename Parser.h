#ifndef PARSER_H
#define PARSER_H

#include <QStringList>
#include "WordsSerie.h"

class Parser
{
public:
  Parser();
  WordsSerie words(QIODevice* _device, qreal _probability);
  WordsSerie words(const QString& _file, qreal _probability);
private:
};

#endif // PARSER_H
