#pragma once
#include <shlwapi.h>

#define NAK                              __declspec(naked)
#define EAXJMP(a)                        { _asm mov eax, a _asm jmp eax }

void GetIniPath(char *buffer, size_t bufferlen);
void GetFilePath(const char *path, char *buffer, size_t bufferlen);
void ForAllFolders(char *path, void(*callback)(char *, void *), void *data);
void ErrorMsg(const char *msg, ...);
