#include "pch.hpp"
#include "options.hpp"
#include "tests.hpp"

Options::Options(int argc, char** argv)
: sourcePath("./serverdata.json"), port("8888")
{
    COVERAGE_BRANCH
    int next_code = 0;
    for (int i=0;i<argc;i++){
        std::string s = std::string(argv[i]);
        LOG_S(1) << "Handling option: [" << s << "]";
        if (next_code == 0){
            COVERAGE_BRANCH
            if (s == "-p" || s == "--path"){
                COVERAGE_BRANCH
                next_code = 1;
            }
            #ifdef TESTS_ENABLED
            else if(s == "-t" || s == "--test"){
                COVERAGE_BRANCH
                this->runTests = true;
            }
            #endif
            else if (s == "-o" || s == "--port"){
                COVERAGE_BRANCH
                next_code = 2;
            }
            COVERAGE_BRANCH_ELSE
        }
        else {
            COVERAGE_BRANCH
            switch (next_code){
                case 1 : {
                    COVERAGE_BRANCH
                    this->sourcePath = s; 
                    break;
                    }
                case 2 : {COVERAGE_BRANCH this->port = s; break;}
                default: {COVERAGE_NEVER}/*This should never happen*/
            }
            next_code = 0;
        }

    }
}



#ifdef TESTS_ENABLED
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
#endif