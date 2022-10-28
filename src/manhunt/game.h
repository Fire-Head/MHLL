#pragma once
#include <rwcore.h>
#include "common.h"

enum eMenuTypes
{
	MENU_EMPTY = 0x0,
	MENU_CHANGE_LANGUAGE = 0x1,
	MENU_EMPTY_2 = 0x2,
	MENU_DIFFICULTY_SELECT = 0x3,
	MENU_START_LOAD_GAME = 0x4,
	MENU_LOAD_GAME = 0x5,
	MENU_SAVE_GAME = 0x6,
	MENU_FRONTEND = 0x7,
	MENU_PAUSE = 0x8,
	MENU_LEVEL_SELECT = 0x9,
	MENU_SETTINGS = 0xA,
	MENU_AUDIO_SETTINGS = 0xB,
	MENU_HEADSET_SETTINGS = 0xC,
	MENU_VIDEO_SETTINGS = 0xD,
	MENU_BONUS_FEATURES = 0xE,
	MENU_BONUS_PREVIEW = 0xF,
	MENU_GAME_COMPLETED = 0x10,
	MENU_GAMMA_SETTINGS = 0x11,
	MENU_CONTROL_SETTINGS = 0x12,
	MENU_RESTORE_DEFAULTS = 0x13,
	MENU_19 = 0x14,
	MENU_CREDITS = 0x15,
	MENU_REMAP_CONTROLS = 0x16,
	MENU_QUIT = 0x17,
};

class CMemoryHeap
{
public:
	void *Malloc(unsigned long size)
	{
		return ((void *(__thiscall *)(CMemoryHeap *, unsigned long))0x401350)(this, size);
	}

	void Free(void *ptr)
	{
		((void (__thiscall *)(CMemoryHeap *, void *))0x401B00)(this, ptr);
	}
};

enum eLanguage
{
	LANGUAGE_ENGLISH = 0,
	LANGUAGE_GERMAN,
	LANGUAGE_FRENCH,
	LANGUAGE_ITALIAN,
	LANGUAGE_SPANISH,
	MAX_LANGUAGES,

	LANGUAGE_FIRST = LANGUAGE_ENGLISH,
	LANGUAGE_LAST  = LANGUAGE_SPANISH,
};

class CMhGlobalData
{
public:
	void *vtable;
	int LevelsComplete;
	int LevelInProgress;
	int field_C;
	struct
	{
		int Lang;
		int Subtitles;
		int SoundVolume;
		int MusicVolume;
		int field_10;
		int field_14;
		int field_18;
		int field_1C;
		int field_20;
		int IsNoiseFxOn;
		int AspectRatio;
		char field_2C[12];
		int GameDetail;
		int ShadowDetail;
		int IsMenuCameraFXOn;
		int field_44;
		int InvertY;
		int field_4C;
		int SpeedX;
		int SpeedY;
		int Brightness;
		int DifficultyLevel;
	}Settings;
	char unk[0x1090];

	bool SetLanguage(eLanguage lang, int reloadText)
	{
		return ((bool (__thiscall *)(CMhGlobalData *, eLanguage, int))0x604DA0)(this, lang, reloadText);
	}
};

class CRGBA
{
public:
	int r;
	int g;
	int b;
	int a;
};

class CString
{
public:
	char *str;
	short len;
	short cap;

	CString(CString const &str)
	{
		((void (__thiscall *)(CString *, CString const &))0x4F2400)(this, str);
	}

	CString(const char *str)
	{
		((void (__thiscall *)(CString *, const char *))0x4F2550)(this, str);
	}

	~CString()
	{
		((void (__thiscall *)(CString *))0x4F2520)(this);
	}

	void Empty()
	{
		((void (__thiscall *)(CString *))0x4F24F0)(this);
	}

	CString &operator +=(char const *str)
	{
		return ((CString & (__thiscall *)(CString *, const char *))0x4F2A40)(this, str);
	}

	CString &operator +=(CString const &str)
	{
		return ((CString & (__thiscall *)(CString *, CString const &))0x4F2A90)(this, str);
	}
};

class CKeyEntry
{
public:
	char *value;
	char key[8];
};

class CKeyArray
{
public:
	CKeyEntry *entries;
	int numEntries;

	CKeyArray(void) : entries(NULL), numEntries(0) {}
};

class CData
{
public:
	CData(void) : chars(NULL), numChars(0) {}

	char *chars;
	int numChars;
};


class CText
{
	CKeyArray keyArray;
	CData data;
	char encoding;
	char _pad[3];

public:
	CText()
	{
		encoding = 'e';
	}

	~CText()
	{
		((void (__thiscall *)(CText *))0x493900)(this);
	}

	wchar_t * GetFromKey16(char *str)
	{
		return ((wchar_t * (__thiscall *)(CText *, char *))0x4937E0)(this, str);
	}

	wchar_t *Get(char *str)
	{
		return ((wchar_t * (__thiscall *)(CText *, char *))0x493830)(this, str);
	}

	bool Load(char *path)
	{
		return ((bool (__thiscall *)(CText *, char *))0x4933E0)(this, path);
	}
};

class CFileNames
{
public:
	static CString &ms_GameGXTDirectory;
	static CString &ms_pathLevels;
	static CString &ms_filenameFrontendDataFonts;
	static CString &ms_GameGXTFilenameEnglish;
	static CString &ms_GameGXTFilenameGerman;
	static CString &ms_GameGXTFilenameFrench;
	static CString &ms_GameGXTFilenameItalian;
	static CString &ms_GameGXTFilenameSpanish;
};

class CApp
{
public:
	static int &ms_currLevelNum;
	static char (&ms_levelFileIONames)[24][20];
};


class CUniCodeUtils
{
public:
	static char (&ms_strLastConvertedAscii)[512+1];
	static wchar_t (&ms_strLastConvertedUniCode)[512+1];
};

class CFrontend
{
public:
	enum
	{
		NUM_FONTS = 4,
	};

	struct Font
	{
		struct Symbol
		{
			int charId;
			float width;
			float u1;
			float v1;
			float u2;
			float v2;
		};

		int numFonts;
		int currentfont;

		Symbol *SymTable[NUM_FONTS];

		int *CharTable[NUM_FONTS];

		int lastChar[NUM_FONTS];
		int firstChar[NUM_FONTS];
		int fontids[NUM_FONTS];

		int numSymbols[NUM_FONTS];
		int symbolSize[NUM_FONTS];
		float height[NUM_FONTS];

		CRGBA colors[34];
	};


	static RwTexDictionary *&ms_tex;
	static Font &ms_font;

	static bool FontLoadDatas(char *path)
	{
		return ((bool (__cdecl *)(char *))0x5F6AA0)(path);
	}

	static void FontInitialise()
	{
		((void (__cdecl *)())0x5F64F0)();
	}

	static void FontShutdown()
	{
		((void (__cdecl *)())0x5F6D50)();
	}
	
	static void Print8(char *text, float x, float y, float sizex, float sizey, float unk, long font)
	{
		((void (__cdecl *)(char *, float, float, float, float, float, long))0x5E55E0)(text, x, y, sizex, sizey, unk, font);
	}
};

class CFrontendMenu
{
public:
	static int &m_menuLastParent;
	static RwTexDictionary *&m_pTexDictCurrent;
	static float &fOptionX;
	static float &fOptionY;
	static float &fOptionScaleX;
	static float &fOptionScaleY;
	static float &fOptionMargin;

	struct menuLanguageCTRL
	{
		int option;
	};

	static menuLanguageCTRL &ms_menuLanguageCTRL;

	static void SetMenu(long menu)
	{
		((void (__cdecl *)(long))0x5D7A40)(menu);
	}

	static int InputUp()
	{
		return ((int (__cdecl *)())0x5D8A10)();
	}

	static int InputDown()
	{
		return ((int (__cdecl *)())0x5D8AA0)();
	}

	static int InputSelect()
	{
		return ((int (__cdecl *)())0x5D8B30)();
	}

	static int InputBack()
	{
		return ((int (__cdecl *)())0x5D8C60)();
	}

	static int InputMouse()
	{
		return ((int (__cdecl *)())0x5DACC0)();
	}

	static int InputMouseBack()
	{
		return *(int *)0x7C8F9C;
	}

	static int GetMouseOption()
	{
		return ((int (__cdecl *)())0x5DAD00)();
	}

	static void DrawOptionText(wchar_t *text, float x, float y, float w, float h)
	{
		((void (__cdecl *)(wchar_t *, float, float, float, float))0x5D5B30)(text, x, y, w, h);
	}

	static void DrawOptionSelection(wchar_t *text, float x, float y, float w, float h, int isselected)
	{
		((void (__cdecl *)(wchar_t *, float, float, float, float, int))0x5D55C0)(text, x, y, w, h, isselected);
	}

	static void DrawCameraCounter(wchar_t *text)
	{
		((void (__cdecl *)(wchar_t *))0x5D5740)(text);
	}

	static void PrintInfo(wchar_t *text1, wchar_t *text2, wchar_t *text3, wchar_t *text4)
	{
		((void (__cdecl *)(wchar_t *, wchar_t *, wchar_t *, wchar_t *))0x5D5BB0)(text1, text2, text3, text4);
	}
};

class CBasePage
{
public:
	virtual int Update() { return 0; }
	virtual void Draw() { }
};

extern CMhGlobalData &g_mhGlobalData;
extern CText &g_TextGlobal;
extern CMemoryHeap &g_MemoryHeap;
extern RwTexDictionary *&load_tex;
extern RwTexture *&pManhuntFont;
extern RwTexture *&pConsoleFont;
extern RwTexture *&pLCDFont;
extern RwTexture *&pLoadingFont;

extern RwTexDictionary *TexDictLoad(const char * path);