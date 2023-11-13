#include "LangLoader.h"
#include <Windows.h>
#include <math.h>


#define IS_STANDARDLANG(x) (((x) >= LANGUAGE_FIRST && (x) <= LANGUAGE_LAST))
#define LANGID_TO_CUSTOMLANGID(x) ((x)-MAX_LANGUAGES)
#define CUSTOMLANGID_TO_LANGID(x) ((x)+MAX_LANGUAGES)

wchar_t CLanguageLoader::ms_names[CLanguageLoader::MAX_VANILLALANGS][CLanguageLoader::NAME_LENGTH];
CLanguageFontData *CLanguageLoader::ms_data;
std::vector<CLanguage> CLanguageLoader::ms_langs;


CLanguageFontData::CLanguageFontData(const CFrontend::Font *from, bool destroy) :
	bDestroy(destroy)
{
	if ( !destroy )
		memcpy(&font, from, sizeof(font));
	else
	{
		memset(&font, 0, sizeof(font));

		for ( int i = 0; i < NUM_FONTS; i++ )
		{
			font.lastChar[i]    = from->lastChar[i];
			font.firstChar[i]   = from->firstChar[i];
			font.fontids[i]     = from->fontids[i];
			font.numSymbols[i]  = from->numSymbols[i];
			font.symbolSize[i]  = from->symbolSize[i];
			font.height[i]      = from->height[i];

			font.CharTable[i] = (int *)g_MemoryHeap.Malloc(sizeof(int) * (font.lastChar[i] + 1)); // it seems like a R* bug
			memcpy(font.CharTable[i], from->CharTable[i], sizeof(int) * font.lastChar[i]);

			font.SymTable[i] = (Symbol *)g_MemoryHeap.Malloc(sizeof(Symbol) * font.numSymbols[i]);
			memcpy(font.SymTable[i], from->SymTable[i], sizeof(Symbol) * font.numSymbols[i]);
		}

		font.numFonts = from->numFonts;
	}
}

CLanguageFontData::~CLanguageFontData()
{
	if ( bDestroy )
	{
		for ( int i = 0; i < NUM_FONTS; i++ )
		{
			if ( font.SymTable[i] )
				g_MemoryHeap.Free(font.SymTable[i]);
			if ( font.CharTable[i] )
				g_MemoryHeap.Free(font.CharTable[i]);
		}
	}
}

void CLanguageFontData::Store(CFrontend::Font *to)
{
	memcpy(to, &font, sizeof(font));
}

CLanguage::CLanguage(char *inipath) :
	data(NULL),
	txd(NULL),
	nameKey(""),
	dataPath(""),
	txdPath(""),
	textDirPath(""),
	gameGXTName(""),
	levelGXTPostfix(""),
	useWholeAscii(false),
	windowsLangID(-1)
{
	char path[MAX_PATH];
	char buf[MAX_PATH];

	strcpy_s(path, inipath);
	PathRemoveFileSpec(path);
	strcat_s(path, "\\");

	if ( GetPrivateProfileString("LANG",  "NameKey", "UNKNOWNLANG", buf, MAX_PATH, inipath) )
		nameKey = buf;

	if ( GetPrivateProfileString("LANG",  "TextureFile", "", buf, MAX_PATH, inipath) )
	{
		txdPath = path;
		txdPath += buf;
	}

	if ( GetPrivateProfileString("LANG",  "DataFile", "", buf, MAX_PATH, inipath) )
	{
		dataPath = path;
		dataPath += buf;
	}

	if ( GetPrivateProfileString("LANG",  "TextDirectory", "", buf, MAX_PATH, inipath) )
	{
		textDirPath = path;
		textDirPath += buf;
	}

	if ( GetPrivateProfileString("LANG",  "GameGXTName", "", buf, MAX_PATH, inipath) )
		gameGXTName = buf;

	if ( GetPrivateProfileString("LANG",  "LevelGXTPostfix", "", buf, MAX_PATH, inipath) )
		levelGXTPostfix = buf;

	useWholeAscii = GetPrivateProfileInt("LANG",  "UseWholeAsciiTable", 0, inipath) != 0;
	windowsLangID = GetPrivateProfileInt("LANG",  "WindowsLangID", -1, inipath);
}

std::string CLanguage::GetGXTPath(const std::string &gamepath)
{
	const std::string Keyword = "%GAMEPATH%";
	std::string path = textDirPath;
	while (path.find(Keyword) != std::string::npos)
		path.replace(path.find(Keyword), Keyword.length(), gamepath);
	return path;
}

void CLanguageLoader::LoadSystem(char *path, void *data)
{
	char inipath[MAX_PATH];
	const char *p = (const char *)data;

	sprintf_s(inipath, "%s%s\\lang.ini", p, path);

	if ( PathFileExists(inipath) )
	{
		DBG("%s\n", inipath);
		ms_langs.push_back(CLanguage(inipath));
	}
}

void CLanguageLoader::Initialise()
{
	char path[MAX_PATH];
	GetFilePath("MHLL\\", path, MAX_PATH);
	ForAllFolders(path, LoadSystem, path);
}

CLanguage *CLanguageLoader::Language()
{
	if ( !IsStandardLanguage() )
	{
		const unsigned int clangid = LANGID_TO_CUSTOMLANGID(g_mhGlobalData.Settings.Lang);
		if ( clangid < ms_langs.size() )
			return &ms_langs[clangid];
	}

	return NULL;
}

int CLanguageLoader::GetLanguageNumber()
{
	return MAX_VANILLALANGS+ms_langs.size();
}

int CLanguageLoader::GetDefaultLanguage()
{
	int gamelang    = LANGUAGE_ENGLISH;
	LANGID langid   = GetUserDefaultLangID();
	WORD   primlang = PRIMARYLANGID(langid);
	switch ( primlang )
	{
		case LANG_GERMAN:
			gamelang = LANGUAGE_GERMAN;
			break;
		case LANG_SPANISH:
			gamelang = LANGUAGE_SPANISH;
			break;
		case LANG_FRENCH:
			gamelang = LANGUAGE_FRENCH;
			break;
		case LANG_ITALIAN:
			gamelang = LANGUAGE_ITALIAN;
			break;
	}

	DBG("primlang 0x%X\n", primlang);

	for ( size_t i = 0; i < ms_langs.size(); i++ )
	{
		CLanguage &lang = ms_langs[i];
		if ( lang.windowsLangID != -1 )
		{
			if ( lang.windowsLangID == primlang || lang.windowsLangID == 0 )
			{
				gamelang = CUSTOMLANGID_TO_LANGID(i);
				break;
			}
		}
	}

	return gamelang;
}

bool CLanguageLoader::IsStandardLanguage()
{
	return IS_STANDARDLANG(g_mhGlobalData.Settings.Lang);
}

bool CLanguageLoader::IsUsingWholeAsciiTable()
{
	if ( !IsStandardLanguage() )
	{
		if ( Language() )
			return Language()->useWholeAscii;
	}
	return false;
}

int CLanguageLoader::GetLanguageIdByMenuId(int menuid)
{
	if ( !IS_STANDARDLANG(menuid) )
		return menuid;

	const int langid[] = {LANGUAGE_ENGLISH, LANGUAGE_FRENCH, LANGUAGE_ITALIAN, LANGUAGE_GERMAN, LANGUAGE_SPANISH};
	return langid[menuid];
}

wchar_t *CLanguageLoader::GetLanguageNameByMenuId(int menuid)
{
	if ( !IS_STANDARDLANG(menuid) )
	{
		const unsigned int clangid = LANGID_TO_CUSTOMLANGID(menuid);
		if ( clangid < ms_langs.size() )
			return ms_langs[clangid].name;

		return L"UNKNOWNLANG";
	}

	return ms_names[menuid];
}

void CLanguageLoader::SetLocalizationByMenuId(int menuid)
{
	if ( !IS_STANDARDLANG(menuid) )
	{
		const unsigned int clangid = LANGID_TO_CUSTOMLANGID(menuid);
		if ( clangid < ms_langs.size() )
		{
			CLanguage *lang = &ms_langs[clangid];

			if ( lang )
			{
				SetFontData(lang->data);
				SetFontTextures(lang->txd);
				return;
			}
		}
	}

	SetFontData(NULL);
	SetFontTextures(NULL);
}

void CLanguageLoader::ResetLocalization()
{
	SetupLocalization();
}

char *CLanguageLoader::UnicodeToAscii(wchar_t *src)
{
	int len;

	for ( len = 0; *src != '\0' && len < 256-1; len++, src++ )
	{
		if ( *src < 128 || (IsUsingWholeAsciiTable() && *src < 256) )
			CUniCodeUtils::ms_strLastConvertedAscii[len] = (char)*src;
		else
			CUniCodeUtils::ms_strLastConvertedAscii[len] = '#';
	}

	CUniCodeUtils::ms_strLastConvertedAscii[len] = '\0';
	return CUniCodeUtils::ms_strLastConvertedAscii;
}

wchar_t *CLanguageLoader::AsciiNToUnicode(const char *src, int len)
{
	if ( 2 * len + 1 > 512+2 )
		return NULL;

	for ( int i = 0; i < len; i++ )
		CUniCodeUtils::ms_strLastConvertedUniCode[i] = (unsigned char)*src++;

	CUniCodeUtils::ms_strLastConvertedUniCode[len] = L'\0';
	return CUniCodeUtils::ms_strLastConvertedUniCode;
}

void CLanguageLoader::Str8ToStr16(wchar_t *dst, const char *src)
{
	int len = strlen(src);
	for ( int i = 0; i < len; i++ )
		dst[i] = (unsigned char)*src++;

	dst[len] = L'\0';
}

bool CLanguageLoader::FontLoadDatas(char *path)
{	
	if ( CFrontend::FontLoadDatas(path) )
	{
		ms_data = new CLanguageFontData(&CFrontend::ms_font, false);
		return TRUE;
	}
	
	return FALSE;
}

void CLanguageLoader::FontInitialise()
{
	CFrontend::FontInitialise();
	LoadStrings();
	LoadTxd();
	LoadDatas();
}

void CLanguageLoader::FontShutdown()
{
	DestroyDatas();
	DestroyTxd();
	CFrontend::FontShutdown();
}

void CLanguageLoader::LoadDatas()
{
	for( auto it = ms_langs.begin(); it != ms_langs.end(); ++it )
	{
		CLanguage &lang = *it;
		if ( !lang.dataPath.empty() )
		{
			if ( CFrontend::FontLoadDatas((char *)lang.dataPath.c_str()) )
				lang.data = new CLanguageFontData(&CFrontend::ms_font, true);
		}
	}
}

void CLanguageLoader::DestroyDatas()
{
	for( auto it = ms_langs.begin(); it != ms_langs.end(); ++it )
	{
		CLanguage &lang = *it;
		delete lang.data;
		lang.data = NULL;
	}

	if ( ms_data )
	{
		ms_data->Store(&CFrontend::ms_font);
		delete ms_data;
		ms_data = NULL;
	}
}

void CLanguageLoader::LoadTxd()
{
	for( auto it = ms_langs.begin(); it != ms_langs.end(); ++it )
	{
		CLanguage &lang = *it;

		if ( !lang.txdPath.empty() )
		{
			DBG("Load font.txd %s\n", lang.txdPath.c_str());
			lang.txd = TexDictLoad(lang.txdPath.c_str());
		}
	}
}

void CLanguageLoader::DestroyTxd()
{
	for( auto it = ms_langs.begin(); it != ms_langs.end(); ++it )
	{
		CLanguage &lang = *it;

		if ( lang.txd )
		{
			RwTexDictionaryDestroy(lang.txd);
			lang.txd = NULL;
		}
	}
}

RwTexture *CLanguageLoader::FindFontTexture(RwTexDictionary *dict, const RwChar *name)
{	
	if ( !IsStandardLanguage() )
	{
		if ( Language() )
		{
			if ( Language()->txd )
			{
				RwTexture *t = RwTexDictionaryFindNamedTexture(Language()->txd, name);
				if ( t )
					return t;
			}
		}
	}
	
	return RwTexDictionaryFindNamedTexture(dict, name);
}

RwTexture *CLanguageLoader::GetFontTexure(RwTexDictionary *dict, RwTexture *vanilatex)
{
	RwTexture *t = NULL;

	if ( vanilatex && dict )
	{
		t = RwTexDictionaryFindNamedTexture(dict, RwTextureGetName(vanilatex));
		if ( t == NULL )
			return vanilatex;
	}

	return t;
}

void CLanguageLoader::SetFontData(CLanguageFontData *data)
{
	if ( data )
		data->Store(&CFrontend::ms_font);
	else if ( ms_data )
		ms_data->Store(&CFrontend::ms_font);
}

void CLanguageLoader::SetFontTextures(RwTexDictionary *txd)
{
	if ( txd )
	{
		pManhuntFont = GetFontTexure(txd, pManhuntFont);
		pConsoleFont = GetFontTexure(txd, pConsoleFont);
		pLoadingFont = GetFontTexure(txd, pLoadingFont);
		pLCDFont     = GetFontTexure(txd, pLCDFont);
	}
	else
	{
		pManhuntFont = GetFontTexure(CFrontend::ms_tex, pManhuntFont);
		pConsoleFont = GetFontTexure(CFrontend::ms_tex, pConsoleFont);
		pLoadingFont = GetFontTexure(load_tex, pLoadingFont);
		pLCDFont     = GetFontTexure(CFrontendMenu::m_pTexDictCurrent, pLCDFont);
	}
}

void CLanguageLoader::SetupLocalization()
{
	if ( !IsStandardLanguage() )
	{
		if ( Language() )
		{
			SetFontData(Language()->data);
			SetFontTextures(Language()->txd);
			return;
		}
	}

	SetFontData(NULL);
	SetFontTextures(NULL);
}

void CLanguageLoader::LoadStrings()
{
	for( auto it = ms_langs.begin(); it != ms_langs.end(); ++it )
	{
		CLanguage &lang = *it;

		if ( !lang.textDirPath.empty() && !lang.gameGXTName.empty() )
		{
			std::string dir = lang.GetGXTPath(std::string(CFileNames::ms_GameGXTDirectory.str)
				+ "/PC_Text/");

			CString p(dir.c_str());
			p += lang.gameGXTName.c_str();

			CText text;
			text.Load(p.str);
			wchar_t *name = text.Get((char *)lang.nameKey.c_str());

			if ( name )
				wcscpy_s(lang.name, name);
			else
				wcscpy_s(lang.name, AsciiNToUnicode((char *)lang.nameKey.c_str(), lang.nameKey.length()));
		}
	}

	struct
	{
		char *key;
		CString *gxt;
	} langs[MAX_VANILLALANGS] =
	{
		{ "ENG", &CFileNames::ms_GameGXTFilenameEnglish },
		{ "FRA", &CFileNames::ms_GameGXTFilenameFrench  },
		{ "ITA", &CFileNames::ms_GameGXTFilenameItalian },
		{ "GER", &CFileNames::ms_GameGXTFilenameGerman  },
		{ "ESP", &CFileNames::ms_GameGXTFilenameSpanish }
	};

	for ( int i = 0; i < MAX_VANILLALANGS; i++ )
	{
		CString p(CFileNames::ms_GameGXTDirectory);
		p += "/PC_Text/PC_";
		p += *langs[i].gxt;

		CText text;
		text.Load(p.str);
		wchar_t *name = text.GetFromKey16(langs[i].key);

		wcscpy_s(ms_names[i], name);
	}
}

bool CLanguageLoader::LoadGlobal(CText *text, char *path)
{
	SetupLocalization();

	if ( !IsStandardLanguage() )
	{
		if ( Language() )
		{
			if ( !Language()->textDirPath.empty() && !Language()->gameGXTName.empty() )
			{
				std::string dir = Language()->GetGXTPath(std::string(CFileNames::ms_GameGXTDirectory.str)
					+ "/PC_Text/");

				CString p(dir.c_str());
				p += Language()->gameGXTName.c_str();
				DBG("LoadGlobal %s\n", p.str);

				return text->Load(p.str);
			}
		}
	}

	return text->Load(path);
}

bool CLanguageLoader::LoadLevel(CText *text, char *path)
{
	if ( !IsStandardLanguage() )
	{
		if ( Language() )
		{
			if ( !Language()->textDirPath.empty() )
			{
				std::string dir = Language()->GetGXTPath(std::string(CFileNames::ms_pathLevels.str)
					+ std::string(CApp::ms_levelFileIONames[CApp::ms_currLevelNum])
					+ "/PC_Text/");

				CString p(dir.c_str());
				p += "PC_";
				p += CApp::ms_levelFileIONames[CApp::ms_currLevelNum];
				p += Language()->levelGXTPostfix.c_str();
				p += ".GXT";
				DBG("LoadLevel %s\n", p.str);

				return text->Load(p.str);
			}
		}
	}

	return text->Load(path);
}

int CFEP_LanguageEx::m_menuLast = -1;
int CFEP_LanguageEx::m_currentPage = 0;
int CFEP_LanguageEx::m_maxPages = 0;
bool CFEP_LanguageEx::m_nextPage = false;
bool CFEP_LanguageEx::m_prevPage = false;

void CFEP_LanguageEx::Ctor()
{
	m_menuLast = CFrontendMenu::m_menuCurrent;
	m_maxPages = (int)ceil(double(CLanguageLoader::GetLanguageNumber()) / MAX_PAGEITEMS);
}

void CFEP_LanguageEx::Dtor()
{
	int &option = CFrontendMenu::ms_menuLanguageCTRL.option;
	
	if ( m_menuLast != CFrontendMenu::m_menuCurrent )
	{
		m_currentPage = 0;
		option = 0;
	}
}

bool CFEP_LanguageEx::Update()
{
	int &option = CFrontendMenu::ms_menuLanguageCTRL.option;
	
	if ( m_nextPage )
	{
		if ( CFrontendMenu::Mouse.bLMBOnce )
		{
			if ( ++m_currentPage >= m_maxPages )
				m_currentPage = m_maxPages - 1;
		}
		
		m_nextPage = false;
	}
	
	if ( m_prevPage )
	{
		if ( CFrontendMenu::Mouse.bLMBOnce )
		{
			if ( --m_currentPage < 0 )
				m_currentPage = 0;
		}
		
		m_prevPage = false;
	}
	
	if ( CFrontendMenu::InputUp() )
	{
		{
			int first = m_currentPage * MAX_PAGEITEMS;
			int last  = Min(first + MAX_PAGEITEMS, CLanguageLoader::GetLanguageNumber()) - 1;
			if ( option < first || option > last )
				option = last + 1;
		}
		
		if ( --option < 0 )
			option = 0;
				
		m_currentPage = ((int)ceil(float(option + 1) / float(MAX_PAGEITEMS))) - 1;
	}
	
	if ( CFrontendMenu::InputDown() )
	{
		{
			int first = m_currentPage * MAX_PAGEITEMS;
			int last  = Min(first + MAX_PAGEITEMS, CLanguageLoader::GetLanguageNumber()) - 1;
			if ( option < first || option > last )
				option = first - 1;
		}
		
		if ( ++option >= CLanguageLoader::GetLanguageNumber() )
			option = CLanguageLoader::GetLanguageNumber() - 1;
		
		m_currentPage = ((int)ceil(float(option + 1) / float(MAX_PAGEITEMS))) - 1;
	}
	
	int pageoff = m_currentPage * MAX_PAGEITEMS;

	if ( CFrontendMenu::InputMouse() )
		option = CFrontendMenu::GetMouseOption() + pageoff;

	if ( CFrontendMenu::InputBack() || CFrontendMenu::InputPrintInfoButton1() )
		CFrontendMenu::SetMenu(MENU_SETTINGS);

	if ( CFrontendMenu::InputSelect() )
	{
		if ( option >= 0 && option < CLanguageLoader::GetLanguageNumber() )
		{
			g_mhGlobalData.SetLanguage((eLanguage)CLanguageLoader::GetLanguageIdByMenuId(option), TRUE);

			if ( CFrontendMenu::m_menuLastParent == MENU_SETTINGS )
				CFrontendMenu::SetMenu(MENU_SETTINGS);
			else
				CFrontendMenu::SetMenu(MENU_EMPTY_2);
		}
	}

	return TRUE;
}

void CFEP_LanguageEx::Draw()
{
	int &option = CFrontendMenu::ms_menuLanguageCTRL.option;

	wchar_t *menu = g_TextGlobal.GetFromKey16("LANGUA");
	CFrontendMenu::DrawCameraCounter(menu);

	float x = CFrontendMenu::fOptionX;
	float y = CFrontendMenu::fOptionY;
	float w = CFrontendMenu::fOptionScaleX;
	float h = CFrontendMenu::fOptionScaleY;
	float m = CFrontendMenu::fOptionMargin;
	
	float yoff = 0.0075f*1.5f;
	
	if ( m_currentPage > 0 )
	{
		float ah = CFrontend::GetFontHeight(1, 1.25f);
		float aw = CFrontend::GetTextWidth8("~up~", 1.25f, 1);
		float ay = y-ah*0.723f-yoff;
		float ax = x;
		
		if ( CFrontendMenu::Mouse.fPointerX >= ax && CFrontendMenu::Mouse.fPointerX < ax+aw
		  && CFrontendMenu::Mouse.fPointerY >= ay && CFrontendMenu::Mouse.fPointerY < ay+ah)
			m_prevPage = true;
		
		if ( m_prevPage )
		{
			if ( CFrontendMenu::BlinkVar != 0 )
				CFrontendMenu::SetPrintColor(255, 255, 255, 255);
			else
				CFrontendMenu::SetPrintColor(200, 200, 200, 255);
		}
		else
			CFrontendMenu::SetColorGray();
		
		CFrontend::Print8("~up~",   ax, ay, 1.25f, 1.25f, 0.0, 1);
	}
	if ( m_maxPages > 1 && m_currentPage < m_maxPages-1 )
	{
		float bh = CFrontend::GetFontHeight(2, h) + CFrontendMenu::fSelectionBoxScaleY - 0.002f;
		float ah = CFrontend::GetFontHeight(1, 1.25f);
		float aw = CFrontend::GetTextWidth8("~down~", 1.25f, 1);
		float ay = y + m*(MAX_PAGEITEMS-1)+bh-ah*0.384f+yoff;
		float ax = x;
		
		if ( CFrontendMenu::Mouse.fPointerX >= ax && CFrontendMenu::Mouse.fPointerX < ax+aw
		  && CFrontendMenu::Mouse.fPointerY >= ay && CFrontendMenu::Mouse.fPointerY < ay+ah )
			m_nextPage = true;
		
		if ( m_nextPage )
		{
			if ( CFrontendMenu::BlinkVar != 0 )
				CFrontendMenu::SetPrintColor(255, 255, 255, 255);
			else
				CFrontendMenu::SetPrintColor(200, 200, 200, 255);
		}
		else
			CFrontendMenu::SetColorGray();

		CFrontend::Print8("~down~", ax, ay, 1.25f, 1.25f, 0.0, 1);
	}
	
	int first = m_currentPage * MAX_PAGEITEMS;
	int last  = Min(first + MAX_PAGEITEMS, CLanguageLoader::GetLanguageNumber());
	
	for ( int i = first; i < last; i++ )
	{
		CLanguageLoader::SetLocalizationByMenuId(i);
		wchar_t *text = CLanguageLoader::GetLanguageNameByMenuId(i);
		CFrontendMenu::DrawOptionSelection(text, x, y, w, h, option == i);
		CFrontendMenu::DrawOptionText     (text, x, y, w, h);
		y += m;
	}

	CLanguageLoader::ResetLocalization();

	wchar_t *back = g_TextGlobal.GetFromKey16("IBACK");
	CFrontendMenu::PrintInfo(L"", L"", back, L"");
}