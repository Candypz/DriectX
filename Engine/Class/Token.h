/*
   Demo Name:  Game Project 6
      Author:  Allen Sherrod
     Chapter:  Chapter 7
*/


#ifndef _UGP_TOKEN_H_
#define _UGP_TOKEN_H_

class CToken
{
   public:
      CToken() : m_length(0), m_startIndex(0), m_endIndex(0), m_data(0) {}
      ~CToken() { Shutdown(); }

      void Reset(){ m_startIndex = m_endIndex = 0; }

      void SetTokenStream(char *data);

      bool GetNextToken(char *buffer);
      bool GetNextToken(char *token, char *buffer);

      bool MoveToNextLine(char *buffer);

      void Shutdown();

   private:
      int m_length;
      int m_startIndex, m_endIndex;
      char *m_data;
};

#endif