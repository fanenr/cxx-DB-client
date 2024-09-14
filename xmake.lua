set_policy("build.sanitizer.address", false)
add_rules("mode.debug", "mode.release")
set_policy("run.autobuild", true)

set_languages("cxx17")

target("client")
    add_rules("qt.widgetapp")
    add_links("Qt6Network")
    
    add_includedirs("include")
    add_files("**.ui")
    add_files("**.cc")
    add_files("**.h")
