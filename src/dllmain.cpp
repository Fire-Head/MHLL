#include <Windows.h>
#include <stdio.h>
#include "LangLoader.h"
#include "CPatch.h"

bool __fastcall patch_LoadGlobal(CText *This, int edx0, char *path)
{
	return CLanguageLoader::LoadGlobal(This, path);
}

bool __fastcall patch_LoadLevel(CText *This, int edx0, char *path)
{
	return CLanguageLoader::LoadLevel(This, path);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if(reason == DLL_PROCESS_ATTACH)
	{
#ifdef DEVBUILD
		AllocConsole();

		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
#endif

		CLanguageLoader::Initialise();

		CPatch::RedirectJump(0x493960, CLanguageLoader::UnicodeToAscii);
		CPatch::RedirectJump(0x4939B0, CLanguageLoader::AsciiNToUnicode);
		CPatch::RedirectJump(0x5FC000, CLanguageLoader::Str8ToStr16);

		CPatch::SetShort(0x49325B, 0x0777);
		CPatch::RedirectCall(0x4932C8, patch_LoadGlobal);

		CPatch::RedirectCall(0x4933B3, patch_LoadLevel);

		CPatch::RedirectJump(0x5FFF81, FUNC2PTR(&CFEP_LanguageEx::Ctor));
		CPatch::RedirectJump(0x5FFFA1, FUNC2PTR(&CFEP_LanguageEx::Dtor));
		CPatch::RedirectJump(0x5FFFB0, FUNC2PTR(&CFEP_LanguageEx::Update));
		CPatch::RedirectJump(0x6000F0, FUNC2PTR(&CFEP_LanguageEx::Draw));

		CPatch::RedirectCall(0x5EC3EA, CLanguageLoader::FindFontTexture);
		CPatch::RedirectCall(0x5EC45D, CLanguageLoader::FindFontTexture);
		CPatch::RedirectCall(0x5EEBEB, CLanguageLoader::FindFontTexture);
		CPatch::RedirectCall(0x5D7EA0, CLanguageLoader::FindFontTexture);

		CPatch::RedirectCall(0x5E27D3, CLanguageLoader::FontLoadDatas);
		CPatch::RedirectCall(0x5E27A6, CLanguageLoader::FontInitialise);
		CPatch::RedirectCall(0x5E4F85, CLanguageLoader::FontShutdown);

		CPatch::RedirectCall(0x604E55, CLanguageLoader::GetDefaultLanguage);
		CPatch::SetShort(0x604E55+5, 0xC389);
		CPatch::RedirectJump(0x604E55+5+2, (void *)0x604EA2);
	}
	return TRUE;
}