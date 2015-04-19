#include "Parser.h"

#include <QDebug>
#include <QFile>

#include "fWord.h"
#include "Word.h"
#include "Words.h"

Parser::Parser()
{
}

QList<fWord> Parser::words(QIODevice* _device, LangCardsDB::Words* _words)
{
  QList<fWord> words;
  while(not _device->atEnd())
  {
    QString line = _device->readLine();
    line.chop(1);
    QStringList splited = line.split("=");
    if(splited.length() == 3)
    {
      fWord word;
      word.tag          = splited[0].trimmed();
      word.word_lang1   = splited[1].trimmed();
      word.word_lang2   = splited[2].trimmed();
      if(( LangCardsDB::Word::byLang1word(*_words, word.word_lang1)
           && LangCardsDB::Word::byLang2word(*_words, word.word_lang2)).count() == 0)
      {
        words.append(word);
      }
    }
  }
  return words;
}

QList<fWord> Parser::words(const QString& _file, LangCardsDB::Words* _words)
{
  QFile file(_file);
  file.open(QIODevice::ReadOnly);
  return words(&file, _words);
}

