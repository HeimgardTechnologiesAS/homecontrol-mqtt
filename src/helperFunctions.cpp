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

int atoiExtended(char*& str_start, const char* str_end, char delimitor)
{
    int result = 0;
    while(str_start <= str_end && *str_start != delimitor)
    {
        if(*str_start < '0' || *str_start > '9')
        {
            ++str_start;
            continue;
        }
        result = 10 * result + (*str_start - '0');
        ++str_start;
    }
    return result;
}

int lineContains(const char* str, const char* sfind, const unsigned int length)
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

float extractFloat(const uint8_t* text, const unsigned int length)
{
    float temp = 0;
    if(int n = lineContains((const char*)text, ".", length))
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
    return temp;
}

double extractDouble(const uint8_t* text, const unsigned int length)
{
    double temp = 0;
    if(int n = lineContains((const char*)text, ".", length))
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
    return temp;
}

int extractInteger(const uint8_t* text, const unsigned int length)
{
    char* start = (char*)text;
    return atoiExtended(start, start + length, '\0');
}

bool extractState(const uint8_t* text, const unsigned int length)
{
    bool temp = false;
    if(text[0] != '\0')
    {
        if(lineContains((const char*)text, "ON", length))
        {
            temp = true;
        }
        else if(lineContains((const char*)text, "OFF", length))
        {
            temp = false;
        }
    }
    return temp;
}

bool extractBool(const uint8_t* text, const unsigned int length)
{
    bool temp = false;
    if(text[0] != '\0' && length > 0)
    {
        if(*text == '1')
        {
            temp = true;
        }
        else if(*text == '0')
        {
            temp = false;
        }
    }
    return temp;
}

RGB extractRGB(const uint8_t* text, const unsigned int length)
{
    RGB rgb;
    char* here = (char*)text;
    char* end = (char*)text + length;
    rgb.r = atoiExtended(here, end, ';');
    rgb.g = atoiExtended(here, end, ';');
    rgb.b = atoiExtended(here, end, ';');
    return rgb;
}