#include "pch.hpp"
#include "options.hpp"

Options::Options(int argc, char** argv)
: sourcePath("./serverdata.json"), port("8888")
{
    int next_code = 0;
    for (int i=0;i<argc;i++){
        std::string s = std::string(argv[i]);
        LOG_S(1) << "Handling option: [" << s << "]";
        if (next_code == 0){
            if (s == "-p" || s == "--path"){
                next_code = 1;
            }
            #ifdef TESTS_ENABLED
            else if(s == "-t" || s == "--test"){
                this->runTests = true;
            }
            #endif
            else if (s == "-o" || s == "--port"){
                next_code = 2;
            }
        }
        else {
            switch (next_code){
                case 1 : {
                    this->sourcePath = s; 
                    break;
                    }
                case 2 : {this->port = s; break;}
                default: {}
            }
            next_code = 0;
        }

    }
}