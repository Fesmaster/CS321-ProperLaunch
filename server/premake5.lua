workspace("ProperLaunchServer")
   configurations({ "Debug", "Release" })

project("ProperLaunchServer")
   kind("ConsoleApp")
   language("C++")
   targetdir("bin/")

   pchheader("include/pch.hpp")

   files({ "include/**.h","include/**.hpp", "src/**.cpp" })
   
   includedirs({ "include", "." })



   filter("configurations:Debug")
      defines({ "DEBUG" })
      symbols("On")

   filter("configurations:Release")
      defines({ "NDEBUG" })
      optimize("On")