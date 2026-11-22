#include <windows.h>

#include <stdio.h>

static void ShowInfo(char *file)
{
#ifdef NOT_READY_YET
	PVOID lpData, lpBuffer;
	DWORD dwH, uLen;
	DWORD dwLen = GetFileVersionInfoSizeA(file, &dwH);
	if (dwLen <= 0)
	{	printf("GetFileVersionInfoSizeA(%s) returned %ld error %ld\n", file, (long) dwLen, (long) GetLastError());
		return;
	}
	lpData = malloc(dwLen);
	if (!GetFileVersionInfoA(file, dwH, dwLen, &lpData))
	{	printf("GetFileVersionInfoA(%s) Failed with %ld\n", file, (long) GetLastError());
		return;
	}
	if (!VerQueryValueA(lpData, "\\", &lpBuffer, &uLen))
	{	printf("VerQueryValueA(%s) Failed with %ld\n", file, (long) GetLastError());
		return;
	}
	if (uLen != sizeof(VS_FIXEDFILEINFO))
	{	printf("VerQueryValueA(%s) Root Block Size %ld, Expected %ld\n", file, (long) uLen, (long) sizeof(VS_FIXEDFILEINFO));
		return;
	}
	printf("I have NO idea how to display VS_FIXEDFILEINFO(%s)\n", file);
	return;
#ifdef FOR_INFO_ONLYtypedef
typedef struct tagVS_FIXEDFILEINFO {
  DWORD dwSignature;
  DWORD dwStrucVersion;
  DWORD dwFileVersionMS;
  DWORD dwFileVersionLS;
  DWORD dwProductVersionMS;
  DWORD dwProductVersionLS;
  DWORD dwFileFlagsMask;
  DWORD dwFileFlags;
  DWORD dwFileOS;
  DWORD dwFileType;
  DWORD dwFileSubtype;
  DWORD dwFileDateMS;
  DWORD dwFileDateLS;
} VS_FIXEDFILEINFO;
#endif
#endif
}



int main(int argc, char *argv[])
{
	if (argc < 2)
	{	printf("Usage: %s DLLorEXE\n", argv[0]);
	} else
	{
		int a;
		for (a=1; a<argc; a++)
		{	HMODULE h = LoadLibrary(argv[a]);
			if (h != (HMODULE) NULL)
			{	printf("LoadLibrary(%s) Succeeded!\n", argv[a]);
				FreeLibrary(h);
				ShowInfo(argv[a]);
			} else
			{	printf("LoadLibrary(%s) FAILED with error %ld\n", argv[a], (long) GetLastError());
			}
		}
	}
	return 0;
}
