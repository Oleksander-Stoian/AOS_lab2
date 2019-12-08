#include <iostream>
#include <fstream>
#include <string> 
#include "register.h"

using namespace std;

const std::string file_name = "input.txt";

int main()
{
    processor proc;

    ifstream file;
    file.open(file_name);

    if(file.fail()){
        cout << "Problem opening the file!!" << endl;
        return 0;
    }
    try{
        while(!file.eof())
        {
            string str;
			getline(file, str);
            proc.execute(str);
        }
    }
    catch(exception &ex){cout << ex.what() << endl;}
    catch(...){cout << "unexpected exception" << endl;}

    file.close();
    return 0;
}
