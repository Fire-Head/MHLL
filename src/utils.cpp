#include "utils.h"
#include "config.h"
#include <signal.h>
#include <functional>

#pragma comment(lib, "Shlwapi.lib")

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

void GetIniPath(char *buffer, size_t bufferlen)
{
	GetModuleFileName((HINSTANCE)&__ImageBase, buffer, bufferlen);
	PathRenameExtension(buffer, ".ini");
}

void GetFilePath(const char *path, char *buffer, size_t bufferlen)
{
	GetModuleFileName((HINSTANCE)&__ImageBase, buffer, bufferlen - strlen(path) - 1);
	PathRemoveFileSpec(buffer);
	PathAppend(buffer, path);
}

void ForAllFolders(char *path, void(*callback)(char *, void *), void *data)
{
	char search_path[MAX_PATH];
	WIN32_FIND_DATA fd;

	sprintf_s(search_path, "%s*.*", path);
	HANDLE hFind = FindFirstFile(search_path, &fd);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && fd.cFileName[0] != '.')
				callback(fd.cFileName, data);
		}
		while (FindNextFile(hFind, &fd));

		FindClose(hFind);
	}
}

void ErrorMsg(const char *msg, ...)
{
	va_list ap;

	char buff[4096];
	char massageBuff[2048];
	char boxtitle[512];

	sprintf_s(boxtitle, "%s\n", PROJECT_NAME);

	va_start(ap, msg);
	vsprintf_s(massageBuff, msg, ap);

	sprintf_s(buff, "Fatal Error!\n\n%s\n\n\n(Press OK to exit application)", massageBuff);

	HWND hActive = GetActiveWindow();
	bool bHideMouseAgain = false;
	if (hActive)
	{
		ShowWindow(hActive, SW_HIDE);
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		bHideMouseAgain = ShowCursor(true) <= 0;
		while (ShowCursor(true) < 0);
	}

	MessageBoxA(NULL, buff, boxtitle, MB_ICONERROR|MB_OK|MB_TOPMOST);

	raise(SIGABRT);
}