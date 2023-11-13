#pragma once
#include "common.h"
#include "game.h"
#include <string>
#include <vector>

class CLanguage;
class CLanguageFontData;

class CLanguageLoader
{
	enum
	{
		MAX_VANILLALANGS = MAX_LANGUAGES,
		NAME_LENGTH = 64
	};

	friend class CLanguage;
	friend class CFEP_LanguageEx;

	static wchar_t                ms_names[MAX_VANILLALANGS][NAME_LENGTH];
	static CLanguageFontData     *ms_data;
	static std::vector<CLanguage> ms_langs;

	static CLanguage *Language();

	static bool IsStandardLanguage();
	static bool IsUsingWholeAsciiTable();

	static RwTexture *GetFontTexure(RwTexDictionary *dict, RwTexture *vanilatex);

	static void LoadSystem(char *path, void *data);
	static void LoadStrings();
	static void LoadDatas();
	static void DestroyDatas();
	static void LoadTxd();
	static void DestroyTxd();

	static void SetupLocalization();
	static void SetFontData(CLanguageFontData *data);
	static void SetFontTextures(RwTexDictionary *txd);

	static int GetLanguageNumber();
	static int GetLanguageIdByMenuId(int menuid);
	static wchar_t *GetLanguageNameByMenuId(int menuid);
	static void SetLocalizationByMenuId(int menuid);
	static void ResetLocalization();
public:
	static void Initialise();

	static int GetDefaultLanguage();

	static bool FontLoadDatas(char *path);
	static void FontInitialise();
	static void FontShutdown();

	static char *UnicodeToAscii(wchar_t *src);
	static wchar_t *AsciiNToUnicode(const char *src, int len);
	static void Str8ToStr16(wchar_t *dst, const char *src);

	static bool LoadGlobal(CText *text, char *path);
	static bool LoadLevel(CText *text, char *path);

	static RwTexture *FindFontTexture(RwTexDictionary *dict, const RwChar *name);
};

class CLanguageFontData
{
	typedef CFrontend::Font::Symbol Symbol;

	enum
	{
		NUM_FONTS = CFrontend::NUM_FONTS
	};

	struct
	{
		int     numFonts;
		int     currentfont;
		Symbol *SymTable  [NUM_FONTS];
		int    *CharTable [NUM_FONTS];
		int     lastChar  [NUM_FONTS];
		int     firstChar [NUM_FONTS];
		int     fontids   [NUM_FONTS];
		int     numSymbols[NUM_FONTS];
		int     symbolSize[NUM_FONTS];
		float   height    [NUM_FONTS];
	}font;

	bool bDestroy;
public:

	CLanguageFontData(const CFrontend::Font *from, bool destroy);
	~CLanguageFontData();

	void Store(CFrontend::Font *to);
};

class CLanguage
{
public:
	// gamedata
	RwTexDictionary   *txd;
	CLanguageFontData *data;
	wchar_t            name[CLanguageLoader::NAME_LENGTH];

	// config
	std::string        nameKey;
	std::string        dataPath;
	std::string        txdPath;
	std::string        textDirPath;
	std::string        gameGXTName;
	std::string        levelGXTPostfix;
	bool               useWholeAscii;
	int                windowsLangID;

	CLanguage(char *inipath);
	std::string GetGXTPath(const std::string &gamepath);
};

class CFEP_LanguageEx //: public CBasePage
{
	static const int MAX_PAGEITEMS = 10;
	static int m_menuLast;
	static int m_currentPage;
	static int m_maxPages;
	static bool m_nextPage;
	static bool m_prevPage;
public:
	void Ctor();
	void Dtor();
	bool Update();
	void Draw();
};