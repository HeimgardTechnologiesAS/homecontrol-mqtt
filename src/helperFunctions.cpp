#include "helperFunctions.h"

void clearString(byte* text, unsigned int length)
{
  for(int i = 0; i < length; i++)
  {
    text[i] = 0;
  }
}

int lineContains(const char *str, const char *sfind)
{
  int found = 0;
  int index = 0;
  int len, slen;

  len = strlen(str);
  slen = strlen(sfind);
  if(slen > len)
  {
    return 0;
  }
  while(index < len)
  {
    if(str[index] == sfind[found])
    {
      found++;
      if(slen == found)
      {
        return (index + 1);
      }
    }
    else
    {
      found = 0;
    }
    index++;
  }
  return 0;
}

float extractFloat(byte* text, unsigned int length)
{
  float temp = 0;
  if(int n = lineContains((const char*)text, "."))
  {
    //it is float
    if(length > n)
    {
      for(int i = n; i < length; i++)
      {
        temp += (text[i] - '0') * pow(10, ((-1) - (i - n)));
      }
    }
    n--;
    for(int i = 0; i < n; i++)
    {
      temp += (text[i] - '0') * pow(10, (n - 1 - i));
    }
  }
  else
  {
    //not float
    for(int i = 0; i < length; i++)
    {
      temp += (text[i] - '0') * pow(10, ((length - 1) - i));
    }
  }
  clearString(text, length);
  return temp;
}

double extractDouble(byte* text, unsigned int length)
{
  double temp = 0;
  if(int n = lineContains((const char*)text, "."))
  {
    //it is double
    if(length > n)
    {
      for(int i = n; i < length; i++)
      {
        temp += (text[i] - '0') * pow(10, ((-1) - (i - n)));
      }
    }
    n--;
    for(int i = 0; i < n; i++)
    {
      temp += (text[i] - '0') * pow(10, (n - 1 - i));
    }
  }
  else
  {
    //not double
    for(int i = 0; i < length; i++)
    {
      temp += (text[i] - '0') * pow(10, ((length - 1) - i));
    }
  }
  clearString(text, length);
  return temp;
}

int extractInteger(byte* text, unsigned int length)
{
  int temp = 0;

  if(text[length] != '\0')
  {
    text[length] = '\0';
  }

  if (text[0] != '\0')
  {
    temp = atoi((const char*)text);
  }
  clearString(text, length);
  return temp;
}

bool extractState(byte* text, unsigned int length)
{
  bool temp = false;
  if (text[0] != '\0')
  {
    if(lineContains((const char*)text, "ON"))
    {
      temp = true;
    }
    else if(lineContains((const char*)text, "OFF"))
    {
      temp = false;
    }
  }
  clearString(text, length);
  return temp;
}


bool extractBool(byte* text, unsigned int length)
{
  bool temp = false;
  if (text[0] != '\0')
  {
    if(text[1] != '\0')
    {
      text[1] = '\0';
    }

    if(lineContains((const char*)text, "1"))
    {
      temp = true;
    }
    else if(lineContains((const char*)text, "0"))
    {
      temp = false;
    }
  }
  clearString(text, length);
  return temp;
}
