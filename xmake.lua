add_rules("mode.debug", "mode.release")
add_requires("glfw","imgui")

target("xavine")
	set_kind("binary")
	add_files("src/*.cpp")
	add_files("src/imgui/*.cpp")
	--add_includedirs("/usr/include", "/usr/local/include")
	add_includedirs("include")
	--set_warnings("all", "error")
	set_warnings("error")
	--set_optimize("fastest")
	if is_plat("linux") then
		add_syslinks("dl")
	end
	add_packages("glfw","imgui")
