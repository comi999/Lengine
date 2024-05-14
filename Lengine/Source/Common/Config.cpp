#include <string>
#include <map>
#include <mini/ini.h>
#include <Common/Config.hpp>
#include <IO/FileSystem/PlatformFileSystem.hpp>

map< string, ConfigValue >& Configs::GetDefaults()
{
	static map< string, ConfigValue > Defaults;
	return Defaults;
}