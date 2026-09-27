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

void __cdecl CGameInfo_Print16(float x, float y, float w, float h, wchar_t *format, ...)
{
	va_list	arg;	
	va_start (arg, format);
	vswprintf (CFrontend::m_msgStr_16, format, arg);
	va_end (arg); 
	
	CFrontend::Print16_(CFrontend::m_msgStr_16, x, y, w, h, 0.0f, 1);
}

wchar_t *__fastcall CLoadSave__GetSlotScene(int ecx0, int edx0, int sceneID)
{
	static wchar_t buf[512];
	swprintf(buf, L"%s %d", g_TextGlobal.GetFromKey16("SCENE"), sceneID + 1);
	return buf;
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
		
		// fix unicode issues
		{
			CPatch::RedirectJump(0x608420, CLoadSave__GetSlotScene);
			
			// CGameInfo__RenderFailScene
			unsigned char lea_edx_esp8_push_edx[] =  { 0x8D, 0x54, 0x24, 0x08, 0x52  };   // lea     edx, [esp+8] /*buffer*/, push    edx
			CPatch::Set(0x5DD284, lea_edx_esp8_push_edx, sizeof(lea_edx_esp8_push_edx));
			CPatch::RedirectCall(0x5DD289, (void *)0x5DDC20); // GetCenteredText8 -> GetCenteredText16
			unsigned char lea_edx_esp4_push_edx[] =  { 0x8D, 0x54, 0x24, 0x04, 0x52  };   // lea     edx, [esp+4] /*buffer*/, push    edx
			CPatch::Set(0x5DD293, lea_edx_esp4_push_edx, sizeof(lea_edx_esp4_push_edx));
			CPatch::RedirectCall(0x5DD2AE, CGameInfo_Print16);
			
			// CGameInfo::RenderLevelCompleted
			unsigned char do4nops_push_ebp[] =  { 0x90, 0x90, 0x90, 0x90, 0x55  };
			CPatch::Set(0x5DC64B, do4nops_push_ebp, sizeof(do4nops_push_ebp));
			CPatch::RedirectCall(0x5DC650, (void *)0x5DDC20); // GetCenteredText8 -> GetCenteredText16
			static wchar_t s[] = L"%s";
			CPatch::SetPointer(0x5DC663 + 1, &s[0]);
			CPatch::Set(0x5DC65E, do4nops_push_ebp, sizeof(do4nops_push_ebp));
			CPatch::RedirectCall(0x5DC682, CGameInfo_Print16);
			
			// CGameInfo RenderGameSaved, RenderMemCardSaveGame, RenderMemoryCardCreateDir (old unused leftovers)
			unsigned char lea_ecx_esp_push_ecx_nop[] =  { 0x8D, 0x0C, 0x24, 0x51, 0x90  }; // lea     ecx, [esp] /*buffer*/, push    ecx,  nop
			CPatch::Set(0x5DD7BE, lea_ecx_esp_push_ecx_nop, sizeof(lea_ecx_esp_push_ecx_nop));
			CPatch::RedirectCall(0x5DD7DB, CGameInfo_Print16);
			CPatch::Set(0x5DD806, lea_ecx_esp_push_ecx_nop, sizeof(lea_ecx_esp_push_ecx_nop));
			CPatch::RedirectCall(0x5DD823, CGameInfo_Print16);
			CPatch::Set(0x5DD3BE, lea_ecx_esp_push_ecx_nop, sizeof(lea_ecx_esp_push_ecx_nop));
			CPatch::RedirectCall(0x5DD3DB, CGameInfo_Print16);
			CPatch::Set(0x5DD406, lea_ecx_esp_push_ecx_nop, sizeof(lea_ecx_esp_push_ecx_nop));
			CPatch::RedirectCall(0x5DD423, CGameInfo_Print16);
			CPatch::Set(0x5DD719, lea_ecx_esp_push_ecx_nop, sizeof(lea_ecx_esp_push_ecx_nop));
			CPatch::RedirectCall(0x5DD736, CGameInfo_Print16);
			CPatch::Set(0x5DD761, lea_ecx_esp_push_ecx_nop, sizeof(lea_ecx_esp_push_ecx_nop));
			CPatch::RedirectCall(0x5DD77E, CGameInfo_Print16);
		}
	}
	return TRUE;
}