
int g_importWriteBinaryFile, g_writeBinaryFile, g_extractMapBgm;
int g_showMessageBox;
int g_showMessageBoxCore;
int g_showMsgboxImport;
int g_importOpenUrl;
int g_makeMusicDirectory;
int g_importPlayNPCVoice;

int GetMemory(int a){}
void SetMemory(int a, int b){}
int SToInt(string x){}
string ToStr(int x){}
int FixCallOpcode(int curAddr, int targetAddr){}
void NoxUtf8ToUnicode(int src, int dest){}
string ReadStringAddress(int t){}
int GetScrDataField(int functionName){}

int ImportOpenUrl()
{
    int arr[11], link;

    if (!link)
    {
        arr[0] = 0x72506850; arr[1] = 0x14FF0050; arr[2] = 0x04C48324; arr[3] = 0x125035FF;
        arr[4] = 0x036A0058; arr[5] = 0x5A241C68; arr[6] = 0x50006A00; arr[7] = 0x006A006A;
        arr[8] = 0x182454FF; arr[9] = 0x5804C483; arr[10] = 0x909090C3;
        link = GetMemory(GetMemory(0x75ae28) + ((0x30 * (-(g_importOpenUrl))) + 0x1c));
    }
    return link;
}

void OpenUrlEx(string url)
{
	int ptr = GetMemory(0x97bb40 + (SToInt(url) * 4));
    int temp = GetMemory(0x5c3108);

    SetMemory(0x5c3108, ImportOpenUrl());
    Unused1f(ptr);
    SetMemory(0x5c3108, temp);
}

int ShowMessageBoxImport()
{
    int arr[9], link;

    if (!link)
    {
        arr[0] = 0x006A5650; arr[1] = 0x216A006A; arr[2] = 0xDB6243E8; arr[3] = 0x0470FFFF; arr[4] = 0x35FF30FF; arr[5] = 0x006E08E4;
        arr[6] = 0xCF89F3E8; arr[7] = 0x18C483FF; arr[8] = 0x90C3585E;
        link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-(g_showMsgboxImport))) + 0x1c);
        FixCallOpcode(link + 8, 0x507250);
        FixCallOpcode(link + 0x18, 0x449a10);
    }
    return link;
}

void ShowMessageBoxCore(int ttDest, int cDest)
{
    int temp = GetMemory(0x5c3108);
    int link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-(g_showMessageBoxCore))) + 0x1c);

    SetMemory(0x5c3108, ShowMessageBoxImport());
    Unused1f(link);
    SetMemory(0x5c3108, temp);
}

void ShowMessageBox(string title, string content)
{
    int ttDest[100], cDest[100], link;
    int ttPtr = GetMemory(0x97bb40 + (SToInt(title) * 4));
    int cPtr = GetMemory(0x97bb40 + (SToInt(content) * 4));
	
    link = GetMemory(GetMemory(0x75ae28) + (0x30 * (-(g_showMessageBox))) + 0x1c);
    NoxUtf8ToUnicode(ttPtr, link + 8);
    NoxUtf8ToUnicode(cPtr, link + 408);

    ShowMessageBoxCore(link + 8, link + 408);
}

int ImportWriteBinaryFile()
{
    int arr[17], link;

    if (!link)
    {
        arr[0] = 0xDB624BE8; arr[1] = 0x246850FF; arr[2] = 0xFF005882; arr[3] = 0x485AE830; arr[4] = 0xC483FFE1;
        arr[5] = 0x74C08508; arr[6] = 0x748B5624; arr[7] = 0x768B0424; arr[8] = 0x8D0E8B04; arr[9] = 0x50500476; arr[10] = 0x5651016A; arr[11] = 0xE149D1E8;
        arr[12] = 0x10C483FF; arr[13] = 0xCB64FAE8; arr[14] = 0x04C483FF; arr[15] = 0x04C4835E; arr[16] = 0x909090C3;
        link = GetMemory(GetMemory(0x75ae28) + ((0x30 * (-(g_importWriteBinaryFile))) + 0x1c));
        FixCallOpcode(link + 0, 0x507250);
        FixCallOpcode(link + 0xd, 0x56586c);
        FixCallOpcode(link + 0x2c, 0x565a02);
        FixCallOpcode(link + 0x34, 0x407533);
    }
    return link;
}

void WriteBinaryFile(string fileName, int stream)
{
    int temp = 0x5c310c;

    fileName = ToStr(GetMemory(0x97bb40 + (SToInt(fileName) * 4)));
    SetMemory(0x5c310c, ImportWriteBinaryFile());
    Unused20(GetMemory(GetMemory(0x75ae28) + (0x30 * (-(g_writeBinaryFile)) + 0x1c)));
    SetMemory(0x5c310c, temp);
}

void WriteMusicStrings(int targetAddr)
{
    int arr[78], i;

    arr[0] = 0x70616863; arr[1] = 0x61772E31; arr[2] = 0x68630076; arr[3] = 0x77327061; arr[4] = 0x772E7A69; arr[5] = 0x63007661; arr[6] = 0x32706168;
    arr[7] = 0x2E6E6F63; arr[8] = 0x00766177; arr[9] = 0x70616863; arr[10] = 0x72617732; arr[11] = 0x7661772E; arr[12] = 0x61686300; arr[13] = 0x772E3370;
    arr[14] = 0x63007661; arr[15] = 0x34706168; arr[16] = 0x7661772E; arr[17] = 0x61686300; arr[18] = 0x772E3570; arr[19] = 0x63007661; arr[20] = 0x36706168;
    arr[21] = 0x7661772E; arr[22] = 0x61686300; arr[23] = 0x772E3770; arr[24] = 0x63007661; arr[25] = 0x38706168; arr[26] = 0x7661772E; arr[27] = 0x61686300;
    arr[28] = 0x772E3970; arr[29] = 0x63007661; arr[30] = 0x61706168; arr[31] = 0x7661772E; arr[32] = 0x61686300; arr[33] = 0x772E6270; arr[34] = 0x74007661;
    arr[35] = 0x656C7469; arr[36] = 0x7661772E; arr[37] = 0x776F7400; arr[38] = 0x772E316E; arr[39] = 0x74007661; arr[40] = 0x326E776F; arr[41] = 0x7661772E;
    arr[42] = 0x776F7400; arr[43] = 0x772E336E; arr[44] = 0x73007661; arr[45] = 0x2E316275; arr[46] = 0x00766177; arr[47] = 0x32627573; arr[48] = 0x7661772E;
    arr[49] = 0x62757300; arr[50] = 0x61772E33; arr[51] = 0x61770076; arr[52] = 0x7265646E; arr[53] = 0x61772E31; arr[54] = 0x61770076; arr[55] = 0x7265646E;
    arr[56] = 0x61772E32; arr[57] = 0x61770076; arr[58] = 0x7265646E; arr[59] = 0x61772E33; arr[60] = 0x72630076; arr[61] = 0x74696465; arr[62] = 0x61772E73;
    arr[63] = 0x68730076; arr[64] = 0x2E6C6C65; arr[65] = 0x00766177; arr[66] = 0x69746361; arr[67] = 0x2E316E6F; arr[68] = 0x00766177; arr[69] = 0x69746361;
    arr[70] = 0x2E326E6F; arr[71] = 0x00766177; arr[72] = 0x69746361; arr[73] = 0x2E336E6F; arr[74] = 0x00766177; arr[75] = 0x646E6177; arr[76] = 0x2E347265;
    arr[77] = 0x00766177;
    for (i = 0 ; i < 78 ; i ++)
        SetMemory(targetAddr + (i * 4), arr[i]);
}

void PreProcessPlayBgm(int targetAddr)
{
    int oldBase = 0x59da7c;

    WriteMusicStrings(targetAddr);
    SetMemory(oldBase + 0, targetAddr);
    SetMemory(oldBase + 4, targetAddr + 0xa);
    SetMemory(oldBase + 8, targetAddr + 0x17);
    SetMemory(oldBase + 12, targetAddr + 0x24);
    SetMemory(oldBase + 16, targetAddr + 0x31);
    SetMemory(oldBase + 20, targetAddr + 0x3b);
    SetMemory(oldBase + 24, targetAddr + 0x45);
    SetMemory(oldBase + 28, targetAddr + 0x4f);
    SetMemory(oldBase + 32, targetAddr + 0x59);
    SetMemory(oldBase + 36, targetAddr + 0x63);
    SetMemory(oldBase + 40, targetAddr + 0x6d);
    SetMemory(oldBase + 44, targetAddr + 0x77);
    SetMemory(oldBase + 48, targetAddr + 0x81);
    SetMemory(oldBase + 52, targetAddr + 0x8b);
    SetMemory(oldBase + 56, targetAddr + 0x95);
    SetMemory(oldBase + 60, targetAddr + 0x9f);
    SetMemory(oldBase + 64, targetAddr + 0xa9);
    SetMemory(oldBase + 68, targetAddr + 0xb3);
    SetMemory(oldBase + 72, targetAddr + 0xbc);
    SetMemory(oldBase + 76, targetAddr + 0xc5);
    SetMemory(oldBase + 80, targetAddr + 0xce);
    SetMemory(oldBase + 84, targetAddr + 0xda);
    SetMemory(oldBase + 88, targetAddr + 0xe6);
    SetMemory(oldBase + 92, targetAddr + 0xf2);
    SetMemory(oldBase + 96, targetAddr + 0xfe);
    SetMemory(oldBase + 100, targetAddr + 0x108);
    SetMemory(oldBase + 104, targetAddr + 0x114);
    SetMemory(oldBase + 108, targetAddr + 0x120);
    SetMemory(oldBase + 112, targetAddr + 0x12c);
    SetMemory(0x59dbfc, 0);
}

void ExtractMapBgm(string fileName, int resourceFunction)
{
	int dirPath = 0x59dbf4;
    int stream = GetMemory(GetMemory(0x75ae28) + ((0x30 * resourceFunction) + 0x20));

    SetMemory(0x59dc10, GetMemory(0x97bb40 + (SToInt(fileName) * 4)));
    WriteBinaryFile(ReadStringAddress(GetScrDataField(-(g_extractMapBgm)) + 8) + fileName, stream);

    PreProcessPlayBgm(0x5becc4);
}

void MakeMusicDirectory()
{
    int temp = GetMemory(0x5c33b8);
    int code[4];

    //6A 00 68 C8/ 17 58 00 FF
    code[0] = 0xf468006a; code[1] = 0xff0059db; code[2] = 0x58116815; code[3] = 0x9090C300;
    SetMemory(0x5c33b8, 4 + GetMemory(GetMemory(0x75ae28) + ((0x30 * (-(g_makeMusicDirectory))) + 0x1c)));
    MusicPopEvent();
    SetMemory(0x5c33b8, temp);
}

int ImportPlayNPCVoice()
{
    int arr[9], link;

    if (!link)
    {
        arr[0] = 0x50685150; arr[1] = 0xFF005072; arr[2] = 0x0C8B2414; arr[3] = 0x97BB4085;
        arr[4] = 0xD9006800; arr[5] = 0x646A0044; arr[6] = 0x2454FF51; arr[7] = 0x10C48308; arr[8] = 0x90C35859;
		link = GetMemory(GetMemory(0x75ae28) + ((0x30 * (-(g_importPlayNPCVoice))) + 0x1c));
    }
    return link;
}

void PlayNPCVoice(string voiceFilename)
{
    int temp = GetMemory(0x5c3108);

    SetMemory(0x5c3108, ImportPlayNPCVoice());
    Unused1f(SToInt(voiceFilename));
    SetMemory(0x5c3108, temp);
}

void NOXLibraryEntryPointFunction()
{
	"export needinit ImportOpenUrl";
	"export OpenUrlEx";
	"export needinit ShowMessageBoxImport";
	"export ShowMessageBoxCore";
	"export ShowMessageBox";
	"export needinit ImportWriteBinaryFile";
	"export WriteBinaryFile";
	"export MakeMusicDirectory";
	"export WriteMusicStrings";
	"export PreProcessPlayBgm";
	"export ExtractMapBgm";
	"export ImportPlayNPCVoice";
	"export PlayNPCVoice";
	
	g_importOpenUrl = ImportOpenUrl;
	g_showMessageBox = ShowMessageBox;
	g_showMessageBoxCore = ShowMessageBoxCore;
	g_showMsgboxImport = ShowMessageBoxImport;
	g_importWriteBinaryFile = ImportWriteBinaryFile;
	g_writeBinaryFile = WriteBinaryFile;
	g_makeMusicDirectory = MakeMusicDirectory;
	g_importPlayNPCVoice = ImportPlayNPCVoice;
	g_extractMapBgm = ExtractMapBgm;
}
