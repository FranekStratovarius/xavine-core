add_rules("mode.debug", "mode.release")
add_requires("glfw")

target("xavine")
	set_kind("binary")
	add_files("src/*.c")
	--add_includedirs("/usr/include", "/usr/local/include")
	add_includedirs("include")
	set_warnings("all", "error")
	--set_optimize("fastest")
	if is_plat("linux") then
		add_syslinks("dl")
	end
	add_packages("glfw")