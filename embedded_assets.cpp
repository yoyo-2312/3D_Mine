#include "embedded_assets.h"

#include <string>

namespace {

constexpr const char* kImageType = "PNG";
constexpr const char* kAudioType = "MP3";
constexpr const char* kAudioAssets[] = {
	"Win", "Loss", "Err", "Click", "Flag", "Back", "Other", "Deflag"
};

std::string g_audioDir;

std::string BuildResourceName(const char* prefix, const char* assetName)
{
	std::string resourceName = prefix;
	for (const char* ch = assetName; *ch; ++ch)
	{
		if ((*ch >= '0' && *ch <= '9') || (*ch >= 'A' && *ch <= 'Z'))
			resourceName.push_back(*ch);
		else if (*ch >= 'a' && *ch <= 'z')
			resourceName.push_back(static_cast<char>(*ch - 'a' + 'A'));
		else if (*ch == '_')
			resourceName.push_back('_');
		else if (*ch == '+')
			resourceName += "PLUS_";
		else if (*ch == '-')
			resourceName += "MINUS_";
		else
			resourceName.push_back('_');
	}
	return resourceName;
}

std::string BuildAudioPath(const char* assetName)
{
	return g_audioDir + "\\" + assetName + ".MP3";
}

bool EnsureAudioDirectory()
{
	if (!g_audioDir.empty())
		return true;

	char tempPath[MAX_PATH] = "";
	DWORD len = GetTempPathA(MAX_PATH, tempPath);
	if (len == 0 || len >= MAX_PATH)
		return false;

	char folderName[64] = "";
	sprintf(folderName, "Mine3D-assets-%lu", static_cast<unsigned long>(GetCurrentProcessId()));
	g_audioDir = std::string(tempPath) + folderName;
	if (CreateDirectoryA(g_audioDir.c_str(), NULL))
		return true;

	return GetLastError() == ERROR_ALREADY_EXISTS;
}

bool WriteResourceToFile(const char* resourceType, const char* resourceName, const char* outputPath)
{
	HMODULE module = GetModuleHandleA(NULL);
	HRSRC resource = FindResourceA(module, resourceName, resourceType);
	if (!resource)
		return false;

	HGLOBAL loaded = LoadResource(module, resource);
	if (!loaded)
		return false;

	DWORD size = SizeofResource(module, resource);
	const void* data = LockResource(loaded);
	if (!data || size == 0)
		return false;

	FILE* file = fopen(outputPath, "wb");
	if (!file)
		return false;

	size_t written = fwrite(data, 1, size, file);
	fclose(file);
	return written == size;
}

bool EnsureAudioExtracted(const char* assetName)
{
	if (!EnsureAudioDirectory())
		return false;

	std::string resourceName = BuildResourceName("MP3_", assetName);
	std::string outputPath = BuildAudioPath(assetName);
	return WriteResourceToFile(kAudioType, resourceName.c_str(), outputPath.c_str());
}

MCIERROR SendMciCommand(const std::string& command)
{
	return mciSendStringA(command.c_str(), NULL, 0, NULL);
}

}

bool PrepareEmbeddedAssets()
{
	for (const char* assetName : kAudioAssets)
	{
		if (!EnsureAudioExtracted(assetName))
			return false;
	}
	return true;
}

void CleanupEmbeddedAssets()
{
	if (g_audioDir.empty())
		return;

	for (const char* assetName : kAudioAssets)
	{
		std::string outputPath = BuildAudioPath(assetName);
		DeleteFileA(outputPath.c_str());
	}

	RemoveDirectoryA(g_audioDir.c_str());
}

int LoadEmbeddedImage(IMAGE* pDstImg, const char* assetName, int nWidth, int nHeight, bool bResize)
{
	std::string resourceName = BuildResourceName("IMG_", assetName);
	return loadimage(pDstImg, _T(kImageType), resourceName.c_str(), nWidth, nHeight, bResize);
}

bool OpenEmbeddedMusic(const char* alias, const char* assetName)
{
	if (!EnsureAudioExtracted(assetName))
		return false;

	CloseEmbeddedMusic(alias);

	std::string outputPath = BuildAudioPath(assetName);
	char command[MAX_PATH * 2] = "";
	sprintf(command, "open \"%s\" type mpegvideo alias %s", outputPath.c_str(), alias);
	return SendMciCommand(command) == 0;
}

bool PlayOpenedMusic(const char* alias, bool repeat)
{
	std::string command = std::string("play ") + alias;
	if (repeat)
		command += " repeat";
	return SendMciCommand(command) == 0;
}

bool PlayEmbeddedMusic(const char* alias, const char* assetName, bool repeat)
{
	return OpenEmbeddedMusic(alias, assetName) && PlayOpenedMusic(alias, repeat);
}

bool PauseEmbeddedMusic(const char* alias)
{
	return SendMciCommand(std::string("pause ") + alias) == 0;
}

bool ResumeEmbeddedMusic(const char* alias)
{
	return SendMciCommand(std::string("resume ") + alias) == 0;
}

void CloseEmbeddedMusic(const char* alias)
{
	SendMciCommand(std::string("close ") + alias);
}
