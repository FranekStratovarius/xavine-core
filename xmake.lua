set_project("xavine")
add_rules("mode.debug", "mode.release")
add_requires("glfw 3.3.5"--[[,"imgui v1.85","luajit 2.1.0-beta3"]],{system = false})
--add_repositories("xavine-repo xavine-repo")
--add_repositories("xavine-repo git@github.com:FranekStratovarius/xavine-repo.git main")
add_repositories("xavine-repo ../xavine-repo")
add_requires("xavine_graphics", {debug = true})

target("xavine")
	set_kind("binary")

	add_files("src/main.cpp")
	add_files("src/**.cpp")
	add_includedirs("include")

	set_warnings("error")
	set_optimize("fastest")

	--search in same folder as the executable for shared libraries
	add_rpathdirs(".")

	if is_plat("linux") then
		add_syslinks("dl")
	end
	add_packages("glfw"--[[,"imgui","luajit"]],"xavine_graphics")
