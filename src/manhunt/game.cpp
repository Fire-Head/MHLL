#include "game.h"


CString &CFileNames::ms_GameGXTDirectory = *(CString*)0x7395BC;
CString &CFileNames::ms_pathLevels = *(CString*)0x7394A4;
CString &CFileNames::ms_filenameFrontendDataFonts = *(CString*)0x7397AC;
CString &CFileNames::ms_GameGXTFilenameEnglish = *(CString*)0x739664;
CString &CFileNames::ms_GameGXTFilenameGerman = *(CString*)0x73966C;
CString &CFileNames::ms_GameGXTFilenameFrench = *(CString*)0x739674;
CString &CFileNames::ms_GameGXTFilenameItalian = *(CString*)0x73967C;
CString &CFileNames::ms_GameGXTFilenameSpanish = *(CString*)0x739684;

int &CApp::ms_currLevelNum = *(int*)0x75622C;
char (&CApp::ms_levelFileIONames)[24][20] = *(char(*)[24][20])*(int *)(0x756034);
char (&CUniCodeUtils::ms_strLastConvertedAscii)[512+1] = *(char(*)[512+1])*(int *)(0x725E64);
wchar_t (&CUniCodeUtils::ms_strLastConvertedUniCode)[512+1] = *(wchar_t(*)[512+1])*(int *)(0x725A60);

RwTexDictionary *&CFrontend::ms_tex = *(RwTexDictionary **)0x7D366C;
CFrontend::Font &CFrontend::ms_font = *(CFrontend::Font *)0x7D2B7C;

CFrontendMenu::menuLanguageCTRL &CFrontendMenu::ms_menuLanguageCTRL = *(CFrontendMenu::menuLanguageCTRL*)0x7C87B4;
float &CFrontendMenu::fOptionX = *(float*)0x7C8718;
float &CFrontendMenu::fOptionY = *(float*)0x7C871C;
float &CFrontendMenu::fOptionScaleX = *(float*)0x7C8720;
float &CFrontendMenu::fOptionScaleY = *(float*)0x7C8724;
float &CFrontendMenu::fOptionMargin = *(float*)0x7C8728;
RwTexDictionary *&CFrontendMenu::m_pTexDictCurrent = *(RwTexDictionary **)0x7C8704;
int &CFrontendMenu::m_menuLastParent = *(int*)0x7C8700;


CMhGlobalData &g_mhGlobalData = *(CMhGlobalData*)0x7D6AE0;
CText &g_TextGlobal = *(CText*)0x725A38;
CMemoryHeap &g_MemoryHeap = *(CMemoryHeap*)0x67D000;
RwTexDictionary *&load_tex = *(RwTexDictionary **)0x7D3590;
RwTexture *&pManhuntFont = *(RwTexture**)0x7D38D8;
RwTexture *&pConsoleFont = *(RwTexture**)0x7D38DC;
RwTexture *&pLCDFont     = *(RwTexture**)0x7D38E0;
RwTexture *&pLoadingFont = *(RwTexture**)0x7D38E4;


NAK RwTexDictionary *TexDictLoad(const char * path) { EAXJMP(0x4777D0); }
NAK RwBool RwTexDictionaryDestroy(RwTexDictionary * dict) { EAXJMP(0x62F340); }
NAK RwTexture *RwTexDictionaryFindNamedTexture(RwTexDictionary * dict, const RwChar * name) { EAXJMP(0x62F660); }