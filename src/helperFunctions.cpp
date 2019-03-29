#include "helperFunctions.h"

#ifdef LINUX
using namespace std;
#include <cmath>
#include <cstdlib>
#include <cstring>
#endif

void clearByte(uint8_t* text, unsigned int length)
{
    for(int i = 0; i < length; i++)
    {
        text[i] = 0;
    }
}

void clearBuffer(char* text, uint8_t length)
{
    for(uint8_t i = 0; i < length; i++)
    {
        text[i] = 0;
    }
}

int lineContains(const char* str, const char* sfind)
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

float extractFloat(uint8_t* text, unsigned int length)
{
    float temp = 0;
    if(int n = lineContains((const char*)text, "."))
    {
        // it is float
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
        // not float
        for(int i = 0; i < length; i++)
        {
            temp += (text[i] - '0') * pow(10, ((length - 1) - i));
        }
    }
    clearByte(text, length);
    return temp;
}

double extractDouble(uint8_t* text, unsigned int length)
{
    double temp = 0;
    if(int n = lineContains((const char*)text, "."))
    {
        // it is double
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
        // not double
        for(int i = 0; i < length; i++)
        {
            temp += (text[i] - '0') * pow(10, ((length - 1) - i));
        }
    }
    clearByte(text, length);
    return temp;
}

int extractInteger(uint8_t* text, unsigned int length)
{
    int temp = 0;

    if(text[length] != '\0')
    {
        text[length] = '\0';
    }

    if(text[0] != '\0')
    {
        temp = atoi((const char*)text);
    }
    clearByte(text, length);
    return temp;
}

bool extractState(uint8_t* text, unsigned int length)
{
    bool temp = false;
    if(text[0] != '\0')
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
    clearByte(text, length);
    return temp;
}

bool extractBool(uint8_t* text, unsigned int length)
{
    bool temp = false;
    if(text[0] != '\0')
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
    clearByte(text, length);
    return temp;
}

RGB extractRGB(uint8_t* text, unsigned int length)
{
    RGB rgb;
    if(text[length] != '\0')
    {
        text[length] = '\0';
    }
    int values[3] = {0};
    for(int i = 2; i >= 0; i--)
    {
        char* k1 = strrchr((char*)text, ';');
        // check if it not found
        if(k1 == nullptr)
        {
            rgb.r = 0;
            rgb.g = 0;
            rgb.b = 0;
            return rgb;
        }
        int k = k1 - (char*)text + 1;
        if(k < 0)
            k = 0;
        char temp[3];
        for(int j = k, j1 = 0; j < length; j++, j1++)
        {
            temp[j1] = text[j];
        }
        text[k - 1] = '\0';
        values[i] = atoi(temp);
    }
    rgb.r = values[0];
    rgb.g = values[1];
    rgb.b = values[2];
    clearByte(text, length);
    return rgb;
}