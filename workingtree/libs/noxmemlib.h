

void EnableMemoryIO()
{
    int target = 0x979744;
    int exec[32];
    int arr[82];

    exec[0] = 0xBE515756; exec[1] = 0x009797C4; exec[2] = 0x750D80BF; exec[3] = 0x0052B900; exec[4] = 0xA5F30000; exec[5] = 0x5C33D8BE; exec[6] = 0xAC06C700; exec[7] = 0xC700750D;
    exec[8] = 0x0DC40446; exec[9] = 0x46C70075; exec[10] = 0x750DE008; exec[11] = 0x0C46C700; exec[12] = 0x00750DFC; exec[13] = 0x141046C7; exec[14] = 0xC700750E; exec[15] = 0x0E2C1446; 
    exec[16] = 0x46C70075; exec[17] = 0x750E4418; exec[18] = 0x1C46C700; exec[19] = 0x00750E44; exec[20] = 0x442046C7; exec[21] = 0xC700750E; exec[22] = 0x0E5C2446; exec[23] = 0x46C70075; 
    exec[24] = 0x750E7828; exec[25] = 0x2C46C700; exec[26] = 0x00750EA0; exec[27] = 0xA03046C7; exec[28] = 0xC700750E; exec[29] = 0x0EA03446; exec[30] = 0x5F590075; exec[31] = 0x9090C35E;

    arr[0] = 0x50685156; arr[1] = 0xFF005072; arr[2] = 0xFF502414; arr[3] = 0x8B042454; arr[4] = 0x01C901C8; arr[5] = 0x04E1C1C1; arr[6] = 0xAE28358B; arr[7] = 0x748B0075; arr[8] = 0x8D581C31; 
    arr[9] = 0x59598604; arr[10] = 0x9090C35E; arr[11] = 0xFFFFCFE8; arr[12] = 0x723068FF; arr[13] = 0xFF500050; arr[14] = 0x83042454; arr[15] = 0xC03108C4; arr[16] = 0x909090C3; arr[17] = 0xFFFFB7E8; 
    arr[18] = 0x723068FF; arr[19] = 0xFF500050; arr[20] = 0x2454FF30; arr[21] = 0xFF585808; arr[22] = 0xC0315808; arr[23] = 0x909090C3; arr[24] = 0xFFFF9BE8; arr[25] = 0x723068FF; arr[26] = 0xFF500050; 
    arr[27] = 0x2454FF30; arr[28] = 0xFF585808; arr[29] = 0xC0315800; arr[30] = 0x909090C3; arr[31] = 0xFFFF7FE8; arr[32] = 0x6808FFFF; arr[33] = 0x00507230; arr[34] = 0x54FF30FF; arr[35] = 0xC4830424; 
    arr[36] = 0xC3C03108; arr[37] = 0xFFFF67E8; arr[38] = 0x6800FFFF; arr[39] = 0x00507230; arr[40] = 0x54FF30FF; arr[41] = 0xC4830424; arr[42] = 0xC3C03108; arr[43] = 0x72506851; arr[44] = 0x14FF0050; 
    arr[45] = 0x54FF5024; arr[46] = 0x89590424; arr[47] = 0xC0315808; arr[48] = 0x9090C359; arr[49] = 0xFFFF37E8; arr[50] = 0x723068FF; arr[51] = 0x30FF0050; arr[52] = 0x042454FF; arr[53] = 0x3108C483; 
    arr[54] = 0x9090C3C0; arr[55] = 0x50725068; arr[56] = 0x2414FF00; arr[57] = 0x6804C483; arr[58] = 0x00507230; arr[59] = 0x54FF30FF; arr[60] = 0xC4830424; arr[61] = 0xC3C03108; arr[62] = 0x50725068; 
    arr[63] = 0x2414FF00; arr[64] = 0xD1E85056; arr[65] = 0x8BFFFFFE; arr[66] = 0x048D5830; arr[67] = 0x30685E86; arr[68] = 0x50005072; arr[69] = 0x042454FF; arr[70] = 0x310CC483; arr[71] = 0x9090C3C0; 
    arr[72] = 0x50725068; arr[73] = 0x2414FF00; arr[74] = 0xF9E85056; arr[75] = 0x8BFFFFFE; arr[76] = 0x048B5830; arr[77] = 0x30685E86; arr[78] = 0x50005072; arr[79] = 0x042454FF; arr[80] = 0x310CC483; 
    arr[81] = 0x9090C3C0;

    UnBlind();
}

void SetMemory(int addr, int value)
{
    OperatorSetVar(addr, value);
}

int GetMemory(int addr)
{
    return Unknownb8(addr);
}

void NOXLibraryEntryPointFunction()
{
    "export needinit EnableMemoryIO";
    "export SetMemory";
    "export GetMemory";
    EnableMemoryIO();
}