require ("utils")



workspace "Lengine"
	location "."
    configurations { "DebugEditor", "ReleaseEditor", "DebugClient", "ReleaseClient" }
    platforms { "Win64" }
	startproject "Lengine-Example"
	targetdir("$(SolutionDir)Generated/Binaries/%{cfg.buildcfg}/%{cfg.platform}/%{prj.name}")
	objdir("$(SolutionDir)Generated/Intermediate/%{cfg.buildcfg}/%{cfg.platform}/%{prj.name}")
	defaultplatform "Win64"
	os.mkdir("Generated/Binaries")
	os.mkdir("Generated/Intermediate")
	os.mkdir("Generated/Logs")
	os.mkdir("Generated/Project")
	
include "LengineCore"

local apps_path = "Applications"

for _, app_folder in ipairs(os.matchdirs(path.join(apps_path, "*"))) do
	local app_name = string.sub(app_folder, string.len(apps_path) + 2, -1)
	print(string.format("Found App: %s", app_name))
	CreateLengineApp(app_name, apps_path)
end