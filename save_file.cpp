#include <iostream>
#include <fstream>
#include <string>
using namespace std;


void process(string* line) {
    cout << "line read: " << *line << endl;
    }


int main(int argc, char* argv[]) {
    string line;
    if(argc != 2) {
        cerr << "provide one argument" << endl;
        return 1;
        }
    cout << "* trying to open and read: " << argv[1] << endl;
    ifstream f (argv[1]);
    // Note that we can omit checking for f.is_open(), because
    // all errors will be catched correctly by f.fail() (!f) and
    // we do not want to print error messages here.
    // Also note that during the loop, the following rules are obeyed:
    //   - first the IO operation, then error check, then data processing
    //   - failbit and badbit prevent data processing, eofbit does not
    while(getline(f, line)) {
        process(&line);
        }
    f.close();
    return 0;
    }
