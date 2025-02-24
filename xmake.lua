add_rules("mode.debug", "mode.release")

-- set flags
local WARNINGS = {
    "all",
    "extra",
    "pedantic",
    "error",
}

set_warnings(WARNINGS)

target("llvm_ir_parser")
    add_includedirs("include")
    add_files("src/*.cpp")
    add_files("src/utils/*.cpp")
    set_kind("binary")
    set_languages("c++23")

target("unit_test")
    add_includedirs("include")
    add_includedirs("src/tests")
    add_files("src/utils/*.cpp")
    add_files("src/tests/*.cpp")
    set_kind("binary")
    set_languages("c++23")
