
int ToInt(float x)
{
    StopScript(x);
    "export ToInt";
}

float ToFloat(int x)
{
    StopScript(x);
    "export ToFloat";
}

int SToInt(string x)
{
    StopScript(x);
    "export SToInt";
}
string ToStr(int x)
{
    StopScript(x);
    "export ToStr";
}

int FloatToInt(float x)
{
    int i, result = 0;
    float pos = x;

    if (pos < 0.0) pos = -pos;
    pos = pos / 2147483648.0;
    if (pos < 2.0)
    {
        for (i = 0 ; i < 32 ; i ++)
        {
            if (pos >= 1.0)
            {
                result ++;
                pos -= 1.0;
            }
            if (i != 31) result = result << 1;
            pos *= 2.0;
        }
    }
    else result = 0x7fffffff;
    if (x < 0.0) return -result;
    else return result;
    "export FloatToInt";
}

float IntToFloat(int x)
{
    int i, pos = x;
    float result = 0.0;
    if (pos < 0)
        pos = -pos;
    for (i = 0; i < 32; i+=1)
    {
        if (pos & 0x80000000)
            result += 1.0;
        pos = pos << 1;
        if (i != 31)
            result *= 2.0;
    }
    if (x < 0)
        return -result;
    else
        return result;
    "export IntToFloat";
}

void NOXLibraryEntryPointFunction()
{ }

