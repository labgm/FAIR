#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <fstream>

using namespace std;

int exec(const char* cmd) {
    array<char, 128> buffer;
    string result;
    unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return atoi(result.c_str());
}

int main (int argc, char *const argv[])
{
    string filepath = argv[1];
    string command = "wc -l " + filepath;

    ifstream fin;
    fin.open(filepath);

    if (fin.is_open())
    {
        cout << "File open." << endl;
    }

    int number_of_lines = exec(command.c_str());
    cout << "Number of lines:" << number_of_lines << endl;

    int threads = 2;
    int lines_by_thread = number_of_lines / threads;
    
    fin.seekg();
    string lines[lines_by_thread];
	for (int i = 0; i < lines_by_thread; i++)
		if (!getline(fin, lines[i]))
            cout << "EOF" << endl;
        else
            cout << lines[i] << endl;


    return 0;
}

