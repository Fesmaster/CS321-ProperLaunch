


workspace("ProperLaunchServer")
   configurations({ "Debug", "Release" })

project("ProperLaunchServer")
   kind("ConsoleApp")
   language("C++")
   cppdialect("C++2a")
   targetdir("bin/")

   pchheader("include/pch.hpp")

   files({ 
      "include/**.h",
      "include/**.hpp", 
      "src/**.cpp", 
   })
   
   includedirs({ 
      ".", 
      "include", 
      "lib/libasyncd/include",
      "lib/libasyncd/lib/qlibc/include",
      "lib/json/single_include",
      "lib/loguru",
   })

   links({
      --asyncd and dependancies
      "asyncd",
      "qlibc",
      "pthread",
      "event",
      "event_pthreads",
      "event_openssl",
      "ssl",
      "crypto",
      --loguru
      "dl",
      --standard (math)
      "m",
   })

   libdirs({
      "lib/libasyncd/src",
      "lib/libasyncd/lib/qlibc/lib"
   })

   
   filter("configurations:Debug")
      --read in how many COVERAGE_BRANCH_* statements there are in the source files
      local h = io.popen("./count_branches.sh")
   
      defines({ "DEBUG", "T_BRANCH_COUNT="..h:read("*a"):sub(1,-2) })
      symbols("On")

      h:close()

   filter("configurations:Release")
      defines({ "NDEBUG" })
      optimize("On")