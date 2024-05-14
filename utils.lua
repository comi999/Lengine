function get_filename(url)
	local str = url
	local temp = ""
	local result = ""
	for i = str:len(), 1, -1 do
		if str:sub(i,i) ~= "/" then
			temp = temp..str:sub(i,i)
		else
			break
		end
	end
	for j = temp:len(), 1, -1 do
		result = result..temp:sub(j,j)
	end
	return result
end

function get_file_extension(url)
    local dotIndex = url:match(".*%.([^%.]+)$")
    if dotIndex then
        return dotIndex
    else
        return nil
    end
end

function is_static_lib_file(url)
	static_lib_file_extensions = { "a", "lib" }
	file_extension = get_file_extension(url)
	for _, extension in ipairs(static_lib_file_extensions) do
		if extension == file_extension then
			return true
		end
	end
	return false
end

function is_shared_lib_file(url)
	static_lib_file_extensions = { "so", "dll", "jar" }
	file_extension = get_file_extension(url)
	for _, extension in ipairs(static_lib_file_extensions) do
		if extension == file_extension then
			return true
		end
	end
	return false
end

function table.contains(table, element)
	for _, value in pairs(table) do
		if value == element then
			return true
		end
	end
	return false
end

function table.contains_key(table, element)
	for key, _ in pairs(table) do
		if key == element then
			return true
		end
	end
	return false
end

function table.length(table)
	local count = 0
	for _ in pairs(table) do
		count = count + 1 
	end
	return count
end

function table.append(table, source)
	for key, val in pairs(source) do
		table[key] = val
	end
end

-- This will look through a folder and construct a table listing found libraries.
-- Structure of result is as follows:
-- table LibraryTable
-- {
--    table LibraryTableEntry...
-- }
-- 
-- table LibraryTableEntry
-- {
--    path: string
--    include_path: string
--    lib_path: string
--    source_only: bool
--    DebugClient_Win64: LinkInfo
--    DebugEditor_Win64: LinkInfo
--    ReleaseClient_Win64: LinkInfo
--    ReleaseEditor_Win64: LinkInfo
-- }
--
-- table LinkInfo
-- {
--    path: string
--    config: string
--    platform: string
--    static: table
--    shared: table
-- }
function find_libraries(root)
	-- Create LibraryTable
	local library_table = {}
	
	-- Iterate through dependency folders and create entries for each dependency found.
	for _, dependency_folder in ipairs(os.matchdirs(path.join(root, "*"))) do
		local library_name = get_filename(dependency_folder)
		library_table[library_name] = {}
		
		-- Set the path to the library.
		library_table[library_name].path = path.getabsolute(dependency_folder)
		
		-- Set the include directory
		local include_dir = path.join(dependency_folder, "include")
		if os.isdir(include_dir) then
			library_table[library_name].include_path = path.getabsolute(include_dir)
		else
			library_table[library_name].include_path = nil
		end
		
		-- Set the lib directory
		local lib_dir = path.join(dependency_folder, "lib")
		if os.isdir(lib_dir) then
			library_table[library_name].lib_path = path.getabsolute(lib_dir)
		else
			library_table[library_name].lib_path = nil
		end
		
		-- We want to fill out the LinkInfo entries if the lib_path exists.
		local function create_link_info_entry(config_platform_pair)
		
			-- Initialise the config_platform entry.
			library_table[library_name][config_platform_pair] = {}
			
			-- We should extract the config and platform from the config_platform pair.
			local config, platform = string.match(config_platform_pair, "([^_]+)_([^_]+)")
			library_table[library_name][config_platform_pair].config = config
			library_table[library_name][config_platform_pair].platform = platform
			
			-- If lib folder exists, then we want to set the lib info.
			if library_table[library_name].lib_path ~= nil then
				local link_info_entry_dir = path.join(lib_dir, config_platform_pair)
				if os.isdir(link_info_entry_dir) then
					library_table[library_name][config_platform_pair].path = path.getabsolute(link_info_entry_dir)
					library_table[library_name][config_platform_pair].static = {}
					library_table[library_name][config_platform_pair].shared = {}
					
					-- Search through all files in directory and add to lists.
					local lib_file_paths = os.matchfiles(path.join(link_info_entry_dir, "*"))
					
					for _, lib_file_path in ipairs(lib_file_paths) do
						-- Let's get the file name of the lib file.
						local lib_file_name = get_filename(lib_file_path)
						
						-- Add the file name to the right list.
						if is_static_lib_file(lib_file_name) then
							table.insert(library_table[library_name][config_platform_pair].static, lib_file_name)
						elseif is_shared_lib_file(lib_file_name) then
							table.insert(library_table[library_name][config_platform_pair].shared, lib_file_name)
						end
					end
				end
			end
		end
		
		create_link_info_entry("DebugClient_Win64")
		create_link_info_entry("DebugEditor_Win64")
		create_link_info_entry("ReleaseClient_Win64")
		create_link_info_entry("ReleaseEditor_Win64")
	end
	return library_table
end

-- Will add any dependencies by name found in the depependency list. Library table is the result of 'find_libraries'.
function add_dependencies(root, dependencies, headers_only)
	-- Get library table.
	local library_table = find_libraries(root)

	-- Reset filter.
	filter {}

	-- We need to go through all dependencies requested and figure out which ones we can add.
	for _, dependency_name in ipairs(dependencies) do
		if not table.contains_key(library_table, dependency_name) then
			print(string.format("Dependency '%s' not found.", dependency_name))
			goto continue
		end
		
		-- Let's get all header and source files.
		local header_files = {}
		table.append(header_files, os.matchfiles(path.join(library_table[dependency_name].include_path, "**.h")))
		table.append(header_files, os.matchfiles(path.join(library_table[dependency_name].include_path, "**.hpp")))
		table.append(header_files, os.matchfiles(path.join(library_table[dependency_name].include_path, "**.inl")))
		
		local source_files = {}
		table.append(source_files, os.matchfiles(path.join(library_table[dependency_name].include_path, "**.c")))
		table.append(source_files, os.matchfiles(path.join(library_table[dependency_name].include_path, "**.cpp")))
		
		local has_header_files = table.length(header_files) > 0
		local has_source_files = table.length(source_files) > 0
		
		-- Let's create a function that can add each profile.
		-- Decision tree:
		-- For the given profile, we want to check to see if there's a folder for libs for that combination.
		-- -- If there are libs for the given profile:
		-- -- -- Add static libs to input.
		-- -- -- Add commands to copy shared libs to output.
		-- -- If there are no libs:
		-- -- -- We want to 
		
		local function add_dependency(library_table_entry, profile)			
			-- We want to set the filter that we're modifying.
			filter { string.format("configurations:%s", library_table_entry[profile].config), string.format("platforms:%s", library_table_entry[profile].platform) }
			
			-- We want to first add the includedirs. These are in the form of 'include_path' and 'include_path/[dependency]'
			includedirs { library_table_entry.include_path }
			includedirs { string.format("%s/%s", library_table_entry.include_path, dependency_name) }
			
			-- We want to add the header files always.
			if headers_only == false then
				files { header_files }
			end
			
			-- If we have lib files, we should add the directory to it.
			local has_lib_files = library_table_entry[profile] ~= nil
			if has_lib_files then
				
				-- We should add the path to the lib file.
				libdirs { library_table_entry[profile].path }
				
				-- We want to link all static libs.
				if library_table_entry[profile].static ~= nil then
					for _, lib_file_to_link in ipairs(library_table_entry[profile].static) do
						links { lib_file_to_link }
					end
				end
				
				-- For all shared libs, we want to add a post process command to copy this to the output folder.
				if library_table_entry[profile].shared ~= nil then
					for _, lib_file_to_link in ipairs(library_table_entry[profile].shared) do
						postbuildcommands {
							string.format("xcopy /Q /E /Y /I \"%s\" \"$(OutDir)\"", path.translate(path.join(path.join(library_table_entry.lib_path, profile), lib_file_to_link)))
						}
					end
				end
				
			-- If not, we want to add the source files.
			elseif headers_only == false then
				files { source_files }
			end
			
			-- Reset filter.
			filter {}
		end
		
		add_dependency(library_table[dependency_name], "DebugClient_Win64")
		add_dependency(library_table[dependency_name], "DebugEditor_Win64")
		add_dependency(library_table[dependency_name], "ReleaseEditor_Win64")
		add_dependency(library_table[dependency_name], "ReleaseEditor_Win64")
		
		::continue::
	end
	
	-- Reset filter.
	filter{}
end

function create_app(name, apps_folder, engine_folder)

	local lengine_source_folder = path.join(engine_folder, "Source")
	local lengine_dependencies_folder = path.join(engine_folder, "Dependencies")
	local lengine_resources_folder = path.join(engine_folder, "Resources")
	local lengine_configs_folder = path.join(engine_folder, "Configs")
	
	local app_folder = path.join(apps_folder, name)
	local app_source_folder = path.join(app_folder, "Source")
	local app_resources_folder = path.join(app_folder, "Resources")
	local app_configs_folder = path.join(app_folder, "Configs")
	
	local logs_folder = path.join(apps_folder, "../Generated/Logs")
	
	local app_dependencies = {
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
	
	project(name)
	location "Generated/Project"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	toolset "v143"
	links { "Lengine" }
	dependson { "Lengine" }
	includedirs { lengine_source_folder, app_source_folder }
	forceincludes { path.join(lengine_source_folder, "Common/Common.hpp") }
	add_dependencies(lengine_dependencies_folder, app_dependencies, true)
	
	files {
		path.join(app_source_folder, "**.h"),
		path.join(app_source_folder, "../**.hpp"),
		path.join(app_source_folder, "**.inl"),
		path.join(app_source_folder, "**.c"),
		path.join(app_source_folder, "**.cpp"),
		path.join(app_source_folder, "**.natvis"),
		path.join(app_configs_folder, "**.ini"),
	}
	
	prebuildcommands {
	}
	
	local APPLICATION_NAME = string.format("APPLICATION_NAME=\"%s\"", name)
	local EDITOR_APPLICATION_NAME = string.format("APPLICATION_NAME=\"%s Editor\"", name)
	local ENGINE_NAME = "ENGINE_NAME=\"Lengine\""
	local PATH_LOGS = string.format("PATH_LOGS=\"%s\"", logs_folder)
	local PATH_ENGINE_CONFIGS = string.format("PATH_ENGINE_CONFIGS=\"%s\"", lengine_configs_folder)
	local PATH_CONFIGS = string.format("PATH_CONFIGS=\"%s\"", app_configs_folder)
	local PATH_ENGINE_RESOURCES = string.format("PATH_ENGINE_RESOURCES=\"%s\"", lengine_resources_folder)
	local PATH_RESOURCES = string.format("PATH_RESOURCES=\"%s\"", app_resources_folder)
	local IMPLEMENTATION_RENDERING = "IMPLEMENTATION_RENDERING=BACKEND_OPENGL"
	local IMPLEMENTATION_AUDIO = "IMPLEMENTATION_AUDIO=BACKEND_FMOD"
	local IMPLEMENTATION_PHYSICS = "IMPLEMENTATION_PHYSICS=BACKEND_PHYSX"
	
	defines {
		"IS_EXE",
		"_CRT_SECURE_NO_WARNINGS",
		
		ENGINE_NAME,
		PATH_LOGS,
		PATH_ENGINE_CONFIGS,
		PATH_CONFIGS,
		PATH_ENGINE_RESOURCES,
		PATH_RESOURCES,
		IMPLEMENTATION_RENDERING,
		IMPLEMENTATION_AUDIO,
		-- IMPLEMENTATION_PHYSICS
	}
	
	filter "configurations:DebugEditor"
		symbols "On"
		defines {
			"CONFIG_DEBUG",
			"IS_EDITOR",
			"LOGGING_LEVEL=1",
			"ENABLE_MESSAGES",
			EDITOR_APPLICATION_NAME,
		}
	
	filter "configurations:ReleaseEditor"
		optimize "On"
		defines {
			"CONFIG_RELEASE",
			"IS_EDITOR",
			"LOGGING_LEVEL=1",
			"ENABLE_MESSAGES",
			EDITOR_APPLICATION_NAME,
		}
		
	filter "configurations:DebugClient"
		symbols "On"
		defines {
			"CONFIG_DEBUG",
			"IS_CLIENT",
			"LOGGING_LEVEL=1",
			"ENABLE_MESSAGES",
			APPLICATION_NAME,
		}
	
	filter "configurations:ReleaseClient"
		optimize "On"
		defines {
			"CONFIG_RELEASE",
			"IS_CLIENT",
			"LOGGING_LEVEL=3",
			APPLICATION_NAME,
		}
	
	filter "platforms:Win64"
		architecture "x86_64"
		links { "opengl32.lib" }
		defines {
			"PLATFORM_WIN64"
		}	
end