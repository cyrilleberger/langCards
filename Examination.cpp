#include "Examination.h"

Examination::Examination(LangCardsDB::Database* _db, QObject *parent) :
  LanguageBase(_db, parent), m_index(0)
{
  reload();
}

void Examination::userAnswer(const QString& _answer, qreal _time)
{
  Q_UNUSED(_time);
  if(_answer == m_correctAnswer)
  {
    m_currentWord->setLang1success(m_currentWord->lang1success() + 1.0);
    m_currentWord->setLang2success(m_currentWord->lang2success() + 1.0);
  } else {
    m_currentWord->erase();
  }
}

void Examination::reload()
{
  m_currentWords = (LangCardsDB::Word::byLang1success(m_words, 5, Sql::GT)
                    and LangCardsDB::Word::byLang2success(m_words, 5, Sql::GT)).exec();
  nextWord();
}

void Examination::nextWord()
{
  if(m_index == m_currentWords.size()) return;

  m_currentWord = &m_currentWords[m_index];
  updateWords();

  ++m_index;
}

