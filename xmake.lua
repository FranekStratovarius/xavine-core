add_rules("mode.debug", "mode.release")
add_requires("glfw 3.3.5","imgui v1.85")

target("xavine")
	set_kind("binary")

	add_files("src/*.cpp")
	add_includedirs("include")
	--third party libs
	add_files("third-party/src/gl.c")
	add_files("third-party/src/imgui/*.cpp")
	add_includedirs("third-party/include")

	--set_warnings("all", "error")
	set_warnings("error")
	--set_optimize("fastest")

	if is_plat("linux") then
		add_syslinks("dl")
	end
	add_packages("glfw","imgui")
