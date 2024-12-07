/*
* This program hexdumb displays files in hexadecimal, decimal or octal.
* Copyright (C) 2024  Moualy Ali "Snophix" Balouz
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

// I am a rookie at this so if you see a war crime here just tell me my why is it bad
#include <iostream>
#include <filesystem>
#include <array>
#include <string>
#include <fstream>
#include <sstream>

// the things
#define VERSION "1.0.0"
// maybe this is bad, but i don't care at all
#define HELPY "Usage:\n\
    hexdumb [Options] <file>\n\
\n\
Displays files in hex and ascii, highlighting printable characters in green.\n\
\n\
Options:\n\
    --no-color       Display without color\n\
    -o  --octal      Display bytes in octal\n\
    -d  --decimal    Display bytes in decimal\n\
    \n\
    -h  --help       Display help\n\
    -v  --version    Display version\n\
    -c  --copyright  Show copyright\n"
// Well uh....
#define COPY "Hexdumb  Copyright (C) 2024  Moulay Ali \"Snophix\" Balouz\n\
This program comes with ABSOLUTELY NO WARRANTY\n\
This is free software, and you are welcome to redistribute it\n\
under certain conditions; Read the GPL3 Liscence to know more.\n"


enum PrintBase { HEX, OCTAL, DECIMAL };

// main argument
std::string path = "";
// options
bool color = true;
PrintBase printBase = HEX;


void hexdump();
std::string char_to_base_str(char c);
std::string int_to_hex_str(int theInt);
void print_is_printable(unsigned int numChars, char chars[16]);

int main(int argc, char* argv[]){
    // if you just write "hexdumb"
    if (argc < 2) {
        std::cout << HELPY << '\n';
        return 0;
    }

    //std::cout << "Where : " << argv[0] << '\n';

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg[0] == '-') {
            if (arg == "--no-color"){
                color = true;
            } else if (arg == "-o" || arg == "--octal"){
                printBase = OCTAL;
            } else if (arg == "-d" || arg == "--decimal"){
                printBase = DECIMAL;
            } else if (arg == "--help" || arg == "-h"){
                std::cout << HELPY << '\n';
                return 0;
            } else if (arg == "-v" || arg == "--version"){
                std::cout << "The version is " << VERSION << '\n';
                return 0;
            } else if (arg == "-c" || arg == "--copyright") {
                std::cout << COPY << '\n';
                return 0;
            } else{
                std::cout << "What is \"" << arg << "\", huh?!\n";
            }

        } else {
            // well uh...
            if (!std::filesystem::exists(arg)){
                std::cout << '"' << arg << "\" doesn't exist.\n"; return 1;
            }
            if (!path.empty()){
                std::cout << "There is more than one file path argument."; return 1;
            }

            // just remember
            path = arg;
        }
    }
    hexdump();

    return 0;
}


void hexdump() {
    std::fstream f;
    f.open(path, std::ios::in | std::ios::binary);

    unsigned int address = 0;
    int column = 0;
    char chars[16];

    std::cout << int_to_hex_str(address) << " |";
    while (f) {
        char c;
        f.get(c);
        std::cout << ' ' << char_to_base_str(c);

        chars[column] = c;
        column++;
        address++;
        // well time to print the chars
        if (column >= 16){
            std::cout << " | ";
            print_is_printable(column, chars); // print the chars
            std::cout << '\n' << int_to_hex_str(address) << " |";
            column = 0;
        }
    }

    // final line
    if (column > 0) {
        for (int i = 0; i < 16 - column; i++){
            std::cout << (printBase == HEX ? "   " : "    ");
        }
        std::cout << " | ";
        print_is_printable(column, chars);
        std::cout << '\n';
    }
}


// char to hex
std::string char_to_base_str(char theC){
    std::stringstream strOut;
    switch (printBase){
        case HEX:
            strOut << std::hex << (int)theC;
            if (strOut.str().length() == 1) {
                return strOut.str().insert(0, "0");
            } else {
                return strOut.str().substr(0, 2);
            }
            break;
        case OCTAL:
            strOut << std::oct << (int)theC;
            if (strOut.str().length() == 1) {
                return strOut.str().insert(0, "00");
            } else if (strOut.str().length() == 2){
                return strOut.str().insert(0, "0");
            } else {
                return strOut.str().substr(0, 3);
            }
            break;
        case DECIMAL:
            {unsigned char* p_theCU = reinterpret_cast<unsigned char*>(&theC);
            strOut << std::dec << (int)*p_theCU;}
            if (strOut.str().length() == 1) {
                return strOut.str().insert(0, "00");
            } else if (strOut.str().length() == 2) {
                return strOut.str().insert(0, "0");
            } else {
                return strOut.str().substr(0, 3);
            }
            break;
            
        default:
            std::cout << "HOW!!\n";
    }
    return "Nuh uh!!";
}


// integer to hex
std::string int_to_hex_str(int theInt){
    std::stringstream strOut;
    strOut << std::hex << theInt;
    std::string hexStr = strOut.str();
    while (hexStr.length() < 8){
        hexStr.insert(0, "0");
    }
    return hexStr;
}


// print the chars if they are printable
void print_is_printable(unsigned int numChars, char chars[16]){
    for (int i = 0; i < numChars; i++){
        char ascii = chars[i];
        if (std::isprint(ascii) && ascii != '\n'){
            if (color) // if the user want color output
                std::cout << "\033[0;32m";
            std::cout << ascii << "\033[0m";
        } else{
            std::cout << '.';
        }
    }
}