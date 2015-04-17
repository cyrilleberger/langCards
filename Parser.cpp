#include "Parser.h"

#include <QFile>

Parser::Parser()
{
}

WordsSerie Parser::words(QIODevice* _device, qreal _probability)
{
  QList<Word> words;
  qreal total_probability = 0.0;
  while(not _device->atEnd())
  {
    QString line = _device->readLine();
    line.chop(1);
    QStringList splited = line.split("=");
    if(splited.length() == 2)
    {
      Word word;
      word.word_lang1   = splited[0];
      word.word_lang2   = splited[1];
      word.probability  = _probability;
      words.append(word);
      total_probability += _probability;
    }
  }
  return WordsSerie(words, total_probability);
}

WordsSerie Parser::words(const QString& _file, qreal _probability)
{
  QFile file(_file);
  file.open(QIODevice::ReadOnly);
  return words(&file, _probability);
}

