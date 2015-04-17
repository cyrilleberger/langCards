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
qDebug() << total_probability << basic_probability << common_probability;
  qreal random_word = qrand() / qreal(RAND_MAX);
  if(random_word < basic_probability)
  {
    m_basic.selectWord(m_currentWord, m_wordToGuess, m_correctAnswer, m_answers);
    m_currentWordsSerie = &m_basic;
  }
  else if(random_word < basic_probability + common_probability)
  {
    m_common.selectWord(m_currentWord, m_wordToGuess, m_correctAnswer, m_answers);
    m_currentWordsSerie = &m_common;
  } else {
    m_rare.selectWord(m_currentWord, m_wordToGuess, m_correctAnswer, m_answers);
    m_currentWordsSerie = &m_rare;
  }
  emit(wordChanged());
}

void Language::userAnswer(const QString& _answer)
{
  if(_answer == m_correctAnswer)
  {
    if(m_currentWord->word_lang1 == m_wordToGuess)
    {
      m_currentWord->lang1_success += 1;
    } else {
      m_currentWord->lang2_success += 1;
    }
  } else {
    if(m_currentWord->word_lang1 == m_wordToGuess)
    {
      m_currentWord->lang1_failed += 1;
    } else {
      m_currentWord->lang2_failed += 1;
    }
  }
  qreal new_proba = m_currentWord->probability_base
      * (1.0 - m_currentWord->lang1_success / (qreal)std::max(1, m_currentWord->lang1_success + m_currentWord->lang1_failed))
      * (1.0 - m_currentWord->lang2_success / (qreal)std::max(1, m_currentWord->lang2_success + m_currentWord->lang2_failed));
  m_currentWordsSerie->update_total_probability(new_proba - m_currentWord->probability);
  m_currentWord->probability = new_proba;
}

