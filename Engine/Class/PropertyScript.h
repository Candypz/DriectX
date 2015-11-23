/*
   Demo Name:  Game Project 6
      Author:  Allen Sherrod
     Chapter:  Chapter 7
*/


#ifndef _UGP_PROPERTY_SCRIPT_H_
#define _UGP_PROPERTY_SCRIPT_H_



enum enVarType { NULL_VAR = 0, INT_VAR, FLOAT_VAR,
                 STRING_VAR, VECTOR_VAR };


struct stVector
{
   stVector() : x(0), y(0), z(0) {}
   float x, y, z;
};


class CVariable
{
   public:
      CVariable() : type(0), floatVal(0), intVal(0), stringVal(0)
      {
         name[0] = '\0';
         vecVal.x = vecVal.y = vecVal.z = 0;
      }
      ~CVariable()
      {
         if(stringVal) delete[] stringVal; stringVal = 0;
      }

      void SetData(int t, char* n, void *data);
      void SetData(int t, void *data);

      char *GetName() { return name; }
      int GetType() { return type; }
      int GetDataAsInt() { return intVal; }
      float GetDataAsFloat() { return floatVal; }
      char *GetDataAsString() { return stringVal; }
      stVector GetDataAsVector() { return vecVal; }

   private:
      char name[128];
      int type;

      int intVal;
      float floatVal;
      char *stringVal;
      stVector vecVal;
};


class CPropertyScript
{
   public:
      CPropertyScript();
      ~CPropertyScript();

      bool IncreaseVariableList();
      bool LoadScriptFile(char *filename);

   private:
      void ParseNext(char *tempLine, char *varName);

   public:
      bool AddVariable(char *name, int t, void *val);
      bool SetVariable(char *name, int t, void *val);
      int GetVariableAsInt(char *name);
      float GetVariableAsFloat(char *name);
      char *GetVariableAsString(char *name);
      stVector GetVariableAsVector(char *name);

      void Shutdown();

   private:
      CVariable *variableList;
      int m_totalVars;
      int currentLineChar;
};

#endif