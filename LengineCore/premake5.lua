require ("utils")

source_folder_lengine = "Source"
dependencies_folder_lengine = "Dependencies"
generated_folder_lengine = "Generated"

dependencies_lengine = {
	"assimp",
	"fmod",
	"glew",
	"glfw",
	"glm",
	"imgui",
	"json",
	"lua",
	"luabridge",
	"stb_image",
}

include_dirs_lengine = { 
	source_folder_lengine
}

project "LengineCore"
	location "../Generated/Project"
    kind "SharedItems"
    language "C++"

    files {
		path.join(source_folder_lengine, "**.c"),
		path.join(source_folder_lengine, "**.cpp"),
		path.join(source_folder_lengine, "**.h"),
		path.join(source_folder_lengine, "**.hpp"),
		path.join(source_folder_lengine, "**.inl"),
		path.join(source_folder_lengine, "**.natvis"),
		path.join(dependencies_folder_lengine, "**.c"),
		path.join(dependencies_folder_lengine, "**.cpp"),
		path.join(dependencies_folder_lengine, "**.h"),
		path.join(dependencies_folder_lengine, "**.hpp"),
		path.join(dependencies_folder_lengine, "**.inl"),
		path.join(dependencies_folder_lengine, "**.natvis"),
	}
	
	add_dependencies(dependencies_folder_lengine, dependencies_lengine, false)