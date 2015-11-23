/*
   Demo Name:  Game Project 6
      Author:  Allen Sherrod
     Chapter:  Chapter 7
*/


#include"PropertyScript.h"
#include<fstream>

using std::ifstream;


int DetermineType(int startIndex, char *buffer)
{
   int numComponents = 0;
   int type = NULL_VAR;
   bool decimalFound = false;
   bool charFound = false;
   int index = startIndex;

   // Loop through the string and get information about it.
   while(index < (int)strlen(buffer))
      {
         // Since there are no new lines we auto add this if we
         // get inside this loop.
         if(numComponents == 0) numComponents++;
         if(buffer[index] == ' ') numComponents++;

         if(buffer[index] == '.') decimalFound = true;

         if((buffer[index] >= 'a' && buffer[index] <= 'z') ||
            (buffer[index] >= 'A' && buffer[index] <= 'Z') ||
            buffer[index] == '_') charFound = true;

         index++;
         
      }

   // If only one variable is show after the name then it can
   // be any type other than vector since vector should have 3.
   switch(numComponents)
      {
         case 1:
            // If there are any characters then it is a string.
            if(charFound) type = STRING_VAR;
            else type = INT_VAR;

            // If there is a decimal and no chars then its a float.
            if(decimalFound == true && charFound == false) type = FLOAT_VAR;
            break;

         case 3:
            // Since vectors are just floats, if we see any characters
            // in the group then it must be a string.
            if(charFound) type = STRING_VAR;
            else type = VECTOR_VAR;
            break;

         default:
            // If there are more than 1 word after the name
            // then it must be a string since strings can have spaces.
            if(numComponents > 0) type = STRING_VAR;
            break;
      }
   
   return type;
}


void CVariable::SetData(int t, char *n, void *data)
{
   if(!name) return;

   // Set this variables name then set the type and data.
   memcpy(name, n, strlen(n));
   name[strlen(n)] = '\0';
   SetData(t, data);
}


void CVariable::SetData(int t, void *data)
{
   stVector *vec = NULL;
   int len = 0;

   // Depending on the type will depend where the
   // value is stored.
   switch(t)
      {
         case INT_VAR:
            intVal = (int)data;
            break;

         case FLOAT_VAR:
            floatVal = *(float*)data;
            break;

         case STRING_VAR:
            len = strlen((char*)data);
            stringVal = new char[len + 1];
            memcpy(stringVal, (char*)data, len);
            stringVal[len] = '\0';
            break;

         case VECTOR_VAR:
            vec = (stVector*)data;
            vecVal.x = vec->x;
            vecVal.y = vec->y;
            vecVal.z = vec->z;
            break;

         default:
            // If we get here then it is a NULL variable.
            return;
            break;
      };

   type = t;
}


CPropertyScript::CPropertyScript() : variableList(NULL),
   m_totalVars(0), currentLineChar(0)
{

}


CPropertyScript::~CPropertyScript()
{
   // Release all resources.
   Shutdown();
}


bool CPropertyScript::IncreaseVariableList()
{
   if(!variableList)
      {
         variableList = new CVariable[1];
         if(!variableList) return false;
      }
   else
      {
         CVariable *temp;
         temp = new CVariable[m_totalVars + 1];
         if(!temp) return false;

         memcpy(temp, variableList,
               sizeof(CVariable) * m_totalVars);

         delete[] variableList;
         variableList = temp;
      }

   return true;
}


bool CPropertyScript::LoadScriptFile(char *filename)
{
   int totalScriptLines = 0;
   ifstream input, input2;
   char tempLine[256];
   char varName[128];
   char param[3072];
   int type = 0;

   // Open the file to get the number of lines from it.
   input.open(filename);
   if(!input.is_open()) return false;

   // Clear all previous data.   
   Shutdown();

   // Open and get number of lines.
   while(!input.eof())
      {
         input.getline(tempLine, 256, '\n');
         totalScriptLines++;
      }
   
   input.close();

   // Open it this time to get the variables out.
   input2.open(filename);
   if(!input2.is_open()) return false;

   // Loop through each line of the script and get all variables.
   for(int i = 0; i < totalScriptLines; i++)
      {
         // Reset line counter to the beginning.
         currentLineChar = 0;

         // Read the entire line from the file.
         input2.getline(tempLine, 256, '\n');
         tempLine[strlen(tempLine)] = '\0';
         
         // Check if this is a comment.  If not keep going.
         if(tempLine[0] != '#')
            {
               // Read the name then determine the type.
               ParseNext(tempLine, varName);
               type = DetermineType(currentLineChar, tempLine);

               // Depending on the type will depend on how many
               // words we need to read after the name.  For
               // ints we need 1, vectors 3, strings 1, etc.
               // Once we get the data we convert it to the
               // type we need and set it to the variable.
               if(type == INT_VAR)
                  {
                     if(IncreaseVariableList())
                        {
                           ParseNext(tempLine, param);
                           variableList[m_totalVars].SetData(INT_VAR, varName, (void*)atoi(param));
                           m_totalVars++;
                        }
                  }
               else if(type == FLOAT_VAR)
                  {
                     if(IncreaseVariableList())
                        {
                           float fVal = 0;
                           ParseNext(tempLine, param);
                           fVal = (float)atof(param);
                           variableList[m_totalVars].SetData(FLOAT_VAR, varName, &fVal);
                           m_totalVars++;
                        }
                  }
               else if(type == STRING_VAR)
                  {
                     if(IncreaseVariableList())
                        {
                           ParseNext(tempLine, param);
                           variableList[m_totalVars].SetData(STRING_VAR, varName, (void*)param);
                           m_totalVars++;
                        }
                  }
               else if(type == VECTOR_VAR)
                  {
                     if(IncreaseVariableList())
                        {
                           stVector vecVal;

                           ParseNext(tempLine, param);
                           vecVal.x = (float)atof(param);
                           ParseNext(tempLine, param);
                           vecVal.y = (float)atof(param);
                           ParseNext(tempLine, param);
                           vecVal.z = (float)atof(param);

                           variableList[m_totalVars].SetData(VECTOR_VAR, varName, &vecVal);
                           m_totalVars++;
                        }
                  }
            }
      }

   // Close file, return true.
   input2.close();
   return true;
}


void CPropertyScript::ParseNext(char *tempLine, char *outData)
{
   int commandSize = 0;
   int paramSize = 0;

   // Error checking.
   if(!tempLine || !outData) return;

   // Init string.
   outData[0] = '\0';

   // Loop through every character until you find a space or newline.
   while(currentLineChar < (int)strlen(tempLine))
      {
         if(tempLine[currentLineChar] == ' ' || tempLine[currentLineChar] == '\n')
            break;

         // Save the text in the array.
         outData[paramSize] = tempLine[currentLineChar];
         paramSize++;
         currentLineChar++;
      }

   // End the out string and move the line char past the next space.
   // If there is no space then the system will move to the next line.
   outData[paramSize] = '\0';
   currentLineChar++;
}


bool CPropertyScript::AddVariable(char *name, int t, void *val)
{
   // We can use this to see if the variable exist already.
   if(!SetVariable(name, t, val))
      {
         if(!IncreaseVariableList()) return false;

         // Set the variables data then add to the counter.
         variableList[m_totalVars].SetData(t, name, val);
         m_totalVars++;
      }

   return true;
}


bool CPropertyScript::SetVariable(char *name, int t, void *val)
{
   // Loop through the list and compare names.
   // If we find the variable set its data.
   for(int i = 0; i < m_totalVars; i++)
      {
         if(_stricmp(variableList[i].GetName(), name) == 0)
            {
               variableList[i].SetData(t, val);
               return true;
            }
      }

   return false;
}


int CPropertyScript::GetVariableAsInt(char *name)
{
   // Loop through the list and compare names.
   // If we find the variable return its data.
   for(int i = 0; i < m_totalVars; i++)
      {
         if(_stricmp(variableList[i].GetName(), name) == 0)
            return variableList[i].GetDataAsInt();
      }

   return 0;
}


float CPropertyScript::GetVariableAsFloat(char *name)
{
   // Loop through the list and compare names.
   // If we find the variable return its data.
   for(int i = 0; i < m_totalVars; i++)
      {
         if(_stricmp(variableList[i].GetName(), name) == 0)
            return variableList[i].GetDataAsFloat();
      }

   return 0;
}


char *CPropertyScript::GetVariableAsString(char *name)
{
   // Loop through the list and compare names.
   // If we find the variable return its data.
   for(int i = 0; i < m_totalVars; i++)
      {
         if(_stricmp(variableList[i].GetName(), name) == 0)
            return variableList[i].GetDataAsString();
      }

   return NULL;
}


stVector CPropertyScript::GetVariableAsVector(char *name)
{
   stVector null;

   // Loop through the list and compare names.
   // If we find the variable return its data.
   for(int i = 0; i < m_totalVars; i++)
      {
         if(_stricmp(variableList[i].GetName(), name) == 0)
            return variableList[i].GetDataAsVector();
      }

   return null;
}


void CPropertyScript::Shutdown()
{
   // Delete list.
   if(variableList) delete[] variableList;
   variableList = NULL;
}