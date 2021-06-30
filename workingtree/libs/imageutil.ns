
int g_importStreamCopy;
int g_imgDrawFuncFix;

//dependency

int GetMemory(int a){}
void SetMemory(int a, int b){}

int FixCallOpcode(int curAddr, int targetAddr)
{
    SetMemory(curAddr + 1, targetAddr - curAddr - 5);
    return 0;
}

int GetScrDataField(int functionName)
{
    return GetMemory(GetMemory(0x75ae28) + (0x30 * functionName + 0x1c));
}

int ImportStreamCopy()
{
    int arr[8], link;

    if (!link)
    {
        arr[0] = 0x50515756; arr[1] = 0xDB6247E8; arr[2] = 0xE8C88BFF; arr[3] = 0xFFDB6240; arr[4] = 0x39E8F88B; arr[5] = 0x8BFFDB62; arr[6] = 0x58A5F3F0; arr[7] = 0xC35E5F59;
        int flink = g_importStreamCopy;
        link = GetScrDataField(-flink);
        FixCallOpcode(link + 4, 0x507250);
        FixCallOpcode(link + 0xb, 0x507250);
        FixCallOpcode(link + 0x12, 0x507250);
    }
    return link;
}

void StreamCopy(int src, int dst, int len)
{
    int temp = GetMemory(0x5c3320);

    SetMemory(0x5c3320, ImportStreamCopy());
    GroupRunAway(src, dst, len);
    SetMemory(0x5c3320, temp);
}

int ImageResourceDrawFunctionFix(int bptr)
{
    int arr[58];

    arr[0] = 0x83EC8B55; arr[1] = 0x8B5608EC; arr[2] = 0x358B0845; arr[3] = 0x0069F224; arr[4] = 0x8BFC4589; arr[5] = 0x348B6C40; arr[6] = 0xF8758986;
    arr[7] = 0x5D5350A1; arr[8] = 0x60768B00; arr[9] = 0x1374F039; arr[10] = 0xB8F8758B; arr[11] = 0x004BCC20; arr[12] = 0x8B584689; arr[13] = 0x4689FC75;
    arr[14] = 0x5EC03158; arr[15] = 0x5D08C483; arr[16] = 0x909090C3; arr[17] = 0x83EC8B55; arr[18] = 0x8B5620EC; arr[19] = 0xE8560C75; arr[20] = 0xFFFFFFAC;
    arr[21] = 0x8504C483; arr[22] = 0x8B2274C0; arr[23] = 0xC0850846; arr[24] = 0x31561B74; arr[25] = 0x708B66F6; arr[26] = 0x4860A108; arr[27] = 0x348D0069;
    arr[28] = 0xB0048DF6; arr[29] = 0x8BF04589; arr[30] = 0xEC458900; arr[31] = 0xE445895E; arr[32] = 0x3474C085; arr[33] = 0x896C468B; arr[34] = 0xB68BF445;
    arr[35] = 0x00000130; arr[36] = 0x8904768B; arr[37] = 0x068BFC75; arr[38] = 0x8BF84589; arr[39] = 0x69F22435; arr[40] = 0xF4458B00; arr[41] = 0x8B86348D;
    arr[42] = 0x74468B36; arr[43] = 0x89F0758B; arr[44] = 0xFC458B06; arr[45] = 0x3089F631; arr[46] = 0x8B08758D; arr[47] = 0xFF500446; arr[48] = 0xBA96E836;
    arr[49] = 0xC483FFD6; arr[50] = 0x458B5008; arr[51] = 0x74C085E4; arr[52] = 0xFC458B10; arr[53] = 0x89F8758B; arr[54] = 0xF0458B30; arr[55] = 0x89EC758B;
    arr[56] = 0x835E5830; arr[57] = 0xC35D20C4;
    int flink = g_imgDrawFuncFix;
    StreamCopy(GetScrDataField(-flink) + 4, bptr, 58);
    return bptr + 0x44;
}

void ApplyImageResourceFix(int thingId, int rscPtr, int drawF)
{
    int chandle = GetMemory(0x5d5350);
    int tPtr = GetMemory(GetMemory(0x69f224) + (thingId * 4));

    //SetMemory(tPtr + 0x54, chandle);
    SetMemory(tPtr + 0x60, chandle);
    SetMemory(tPtr + 0x58, drawF);
    SetMemory(tPtr + 0x74, rscPtr);
}

void NOXLibraryEntryPointFunction()
{
    "export FixCallOpcode";
    "export GetScrDataField";
    "export ImportStreamCopy";
    "export StreamCopy";
    "export ImageResourceDrawFunctionFix";
    "export ApplyImageResourceFix";

    g_importStreamCopy = ImportStreamCopy;
    g_imgDrawFuncFix = ImageResourceDrawFunctionFix;
}

