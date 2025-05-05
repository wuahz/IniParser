#include <iostream>
#include "ini_file.h"
using namespace std;
using namespace wuah::iniparser;

int main()
{
    IniFile iniFile;
    iniFile.load_file("../test.ini"); // current directory is build, so use "test.ini"
    iniFile.show_all();
    
    string s_server = "server";
    string k_ip = "ip";

    string s_client = "client";
    string k_age = "age";

    iniFile.set_value(s_server, k_ip, "0.0.0.0");
    iniFile.remove_value(s_client, k_age);

    iniFile.show_all();
    
    iniFile.save_file("../tmp.ini");

    return 0;
}