set_project("xavine")
add_rules("mode.debug", "mode.release")
add_requires("glfw 3.3.5"--[[,"imgui v1.85"]],"luajit 2.1.0-beta3",{system = false})
add_repositories("xavine-repo xavine-repo")
add_requires("xavine_graphics_third_party")

target("xavine")
	set_kind("binary")

	add_files("src/main.cpp")
	add_files("src/**.cpp")
	add_includedirs("include")

	set_warnings("error")
	set_optimize("fastest")

	if is_plat("linux") then
		add_syslinks("dl")
	end
	add_packages("glfw"--[[,"imgui"]],"luajit","xavine_graphics_third_party")
