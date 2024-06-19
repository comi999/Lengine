require ("utils")

local generated_folder = "Generated"
local binaries_folder = "Generated/Binaries"
local intermediate_folder = "Generated/Intermediate"
local logs_folder = "Generated/Logs"
local project_folder = "Generated/Project"

local apps_folder = "Applications"
local engine_folder = "Lengine"

workspace "Lengine"
	location "."
    configurations { "DebugEditor", "ReleaseEditor", "DebugClient", "ReleaseClient", "ShippingClient" }
    platforms { "Win64" }
	startproject "Template"
	debugdir("$(OutDir)")
	targetdir("$(SolutionDir)Generated/Binaries/%{cfg.buildcfg}/%{cfg.platform}/%{prj.name}")
	objdir("$(SolutionDir)Generated/Intermediate/%{cfg.buildcfg}/%{cfg.platform}/%{prj.name}")
	defaultplatform "Win64"
	
	os.mkdir(binaries_folder)
	os.mkdir(intermediate_folder)
	os.mkdir(logs_folder)
	os.mkdir(project_folder)
	
include "Lengine"

for _, app_folder in ipairs(os.matchdirs(path.join(apps_folder, "*"))) do
	local app_name = string.sub(app_folder, string.len(apps_folder) + 2, -1)
	print(string.format("Found App: %s", app_name))
	create_app(app_name, apps_folder, engine_folder)
end