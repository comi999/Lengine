require ("utils")

generated_folder = "../Generated"
binaries_folder = "../Generated/Binaries"
intermediate_folder = "../Generated/Intermediate"
logs_folder = "../Generated/Logs"
project_folder = "../Generated/Project"

lengine_source_folder = "Source"
lengine_dependencies_folder = "Dependencies"
lengine_resources_folder = "Resources"
lengine_configs_folder = "Configs"

lengine_dependencies = {
	"assimp",
	"fmod",
	"glew",
	"glfw",
	"glm",
	"imgui",
	"json",
	"lua",
	"luabridge",
	"mini",
	"stb_image",
}

project "Lengine"
	location "../Generated/Project"
    kind "SharedItems"
    language "C++"

    files {
		path.join(lengine_source_folder, "**.c"),
		path.join(lengine_source_folder, "**.cpp"),
		path.join(lengine_source_folder, "**.h"),
		path.join(lengine_source_folder, "**.hpp"),
		path.join(lengine_source_folder, "**.inl"),
		path.join(lengine_source_folder, "**.natvis"),
		path.join(lengine_dependencies_folder, "**.c"),
		path.join(lengine_dependencies_folder, "**.cpp"),
		path.join(lengine_dependencies_folder, "**.h"),
		path.join(lengine_dependencies_folder, "**.hpp"),
		path.join(lengine_dependencies_folder, "**.inl"),
		path.join(lengine_dependencies_folder, "**.natvis"),
		path.join(lengine_configs_folder, "**.ini"),
	}
	
	add_dependencies(lengine_dependencies_folder, lengine_dependencies, false)