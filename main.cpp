
#include "pac.h"
#include <iostream>

using std::string;

int main(int argc, char *argv[]) {

    string input = R"({"expand":false,"lefFiles":"NangateOpenCellLibrary.lef","min_space":0.06,"min_width":0.06,"path":"/home/ranhaooge/code/cpp/r-tools/example"})";
    auto s = calcPinScore(input);
    std::cout << s;

    return 0;
}
