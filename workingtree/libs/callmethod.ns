
int g_callee, g_calleeArgInt, g_calleeArg;

int GetMemory(int a){}
void SetMemory(int a, int b){}

int GetScrDataField(int functionName)
{
    return GetMemory(GetMemory(0x75ae28) + (0x30 * functionName + 0x1c));
    "export GetScrDataField";
}

int GetScrCodeField(int functionName)
{
    return GetMemory(GetMemory(0x75ae28) + (0x30 * functionName + 0x20));
    "export GetScrCodeField";
}

void Callee()
{
    Callee();
    "export Callee";
}

void CallFunction(int func)
{
    int flink = g_callee;

    SetMemory(GetScrCodeField(-flink) + 4, func);
    Callee();
    "export CallFunction";
}

int CalleeArgInt(int arg)
{
    return CalleeArgInt(arg);
    "export CalleeArgInt";
}

int CallFunctionWithArgInt(int func, int arg)
{
    int flink = g_calleeArgInt;

    SetMemory(GetScrCodeField(-flink) + 0x10, func);
    return CalleeArgInt(arg);

    "export CallFunctionWithArgInt";
}

void CalleeArg(int arg)
{
    CalleeArg(arg);
    "export CalleeArg";
}

void CallFunctionWithArg(int func, int arg)
{
    "export CallFunctionWithArg";
    int flink = g_calleeArg;
    SetMemory(GetScrCodeField(-flink) + 0x10, func);
    CalleeArg(arg);
}

void NOXLibraryEntryPointFunction()
{
    g_callee = Callee;
    g_calleeArgInt = CalleeArgInt;
    g_calleeArg = CalleeArg;
}

