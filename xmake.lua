add_rules("mode.debug", "mode.release")
add_requires("glfw 3.3.5","imgui v1.85","luajit 2.1.0-beta3",{system = false})

target("xavine")
	set_kind("binary")

	add_files("src/main.cpp")
	add_files("src/scripting/*.cpp")
	add_files("src/graphics/*.cpp")
	add_includedirs("include")

	--third party libs
	--opengl stuff
	add_files("third-party/opengl/src/gl.c")
	add_includedirs("third-party/opengl/include")
	--imgui
	add_files("third-party/imgui/src/*.cpp")
	add_includedirs("third-party/imgui/include")

	--set_warnings("all", "error")
	set_warnings("error")
	--set_optimize("fastest")

	if is_plat("linux") then
		add_syslinks("dl")
	end
	add_packages("glfw","imgui","luajit")
