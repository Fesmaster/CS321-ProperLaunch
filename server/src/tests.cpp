#include "pch.hpp"
#ifdef TESTS_ENABLED
#include "tests.hpp"

#include "options.hpp"

void option_tests(){
    LOG_S(INFO) << "Options check begin.";
    
    int argc1 = 1;
    char* argv1[] = { "/usr/bin/properlaunch" };
    Options option1(argc1, argv1);
    CHECK_EQ_F(option1.port, "8888", "Options: The default port is not \"8888\"");
    CHECK_EQ_F(option1.sourcePath, "./serverdata.json", "Options: The default path is not \"./serverdata.json\"");
    
    
    int argc2 = 4;
    char* argv2[] = { "/usr/bin/properlaunch", "-p", "/usr/local/properlaunch/serverdata.json", "-q"};
    Options option2(argc2, argv2);
    CHECK_EQ_F(option2.port, "8888", "Options: The default port is not \"8888\" when a path was supplied");
    CHECK_EQ_F(option2.sourcePath, "/usr/local/properlaunch/serverdata.json", "Options: The path is not Correct. was: %s", option2.sourcePath.c_str() );
    
    int argc3 = 3;
    char* argv3[] = { "/usr/bin/properlaunch", "--path", "/usr/local/properlaunch/serverdata.json"};
    Options option3(argc3, argv3);
    CHECK_EQ_F(option3.port, "8888", "Options: The default port is not \"8888\" when a path was supplied");
    CHECK_EQ_F(option3.sourcePath, "/usr/local/properlaunch/serverdata.json", "Options: The path is not Correct");
    
    int argc4 = 3;
    char* argv4[] = { "/usr/bin/properlaunch", "-o", "1234"};
    Options option4(argc4, argv4);
    CHECK_EQ_F(option4.port, "1234", "Options: The port is not correct");
    CHECK_EQ_F(option4.sourcePath, "./serverdata.json", "Options: The default path is not Correct when using a port");
    
    int argc5 = 3;
    char* argv5[] = { "/usr/bin/properlaunch", "--port", "5678"};
    Options option5(argc5, argv5);
    CHECK_EQ_F(option5.port, "5678", "Options: The port is not correct");
    CHECK_EQ_F(option5.sourcePath, "./serverdata.json", "Options: The default path is not Correct when using a port");
    
    int argc6 = 5;
    char* argv6[] = { "/usr/bin/properlaunch", "-o", "1234", "-p", "/usr/local/properlaunch/serverdata.json"};
    Options option6(argc6, argv6);
    CHECK_EQ_F(option6.port, "1234", "Options: The port is not correct");
    CHECK_EQ_F(option6.sourcePath, "/usr/local/properlaunch/serverdata.json", "Options: The path is not Correct");
    
    int argc7 = 5;
    char* argv7[] = { "/usr/bin/properlaunch", "-p", "/usr/local/properlaunch/serverdata.json", "-o", "5678"};
    Options option7(argc7, argv7);
    CHECK_EQ_F(option7.port, "5678", "Options: The port is not correct");
    CHECK_EQ_F(option7.sourcePath, "/usr/local/properlaunch/serverdata.json", "Options: The path is not Correct");
    
    LOG_S(INFO) << "Options check complete.";
}



int run_tests(){
    LOG_S(INFO) << "TESTS ARE RUNNING";
    
    option_tests();

    return 0;
}




#endif