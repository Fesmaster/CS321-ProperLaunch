workspace("ProperLaunchServer")
   configurations({ "Debug", "Release" })

project("ProperLaunchServer")
   kind("ConsoleApp")
   language("C++")
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
      defines({ "DEBUG" })
      symbols("On")

   filter("configurations:Release")
      defines({ "NDEBUG" })
      optimize("On")