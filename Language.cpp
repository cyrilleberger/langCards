#include "Language.h"

#include "Parser.h"

#include <QDebug>

Language::Language(QObject *parent) :
  QObject(parent)
{
  Parser parser;
  m_basic   = parser.words(":/data/sv-en/basic.txt", 1.0);
  m_common  = parser.words(":/data/sv-en/common.txt", 0.1);
  m_rare    = parser.words(":/data/sv-en/rare.txt", 0.01);

  nextWord();
}

void Language::nextWord()
{
  m_answers.clear();

  qreal total_probability = m_basic.total_probability() + m_common.total_probability() + m_rare.total_probability();
  qreal basic_probability = m_basic.total_probability() / total_probability;
  qreal common_probability = m_common.total_probability() / total_probability;

  qreal random_word = qrand() / qreal(RAND_MAX);
  if(random_word < basic_probability)
  {
    m_basic.selectWord(m_wordToGuess, m_correctAnswer, m_answers);
  }
  else if(random_word < basic_probability + common_probability)
  {
    m_common.selectWord(m_wordToGuess, m_correctAnswer, m_answers);
  } else {
    m_rare.selectWord(m_wordToGuess, m_correctAnswer, m_answers);
  }

  emit(wordChanged());
}

