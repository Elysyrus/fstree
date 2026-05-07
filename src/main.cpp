#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: fstree <path>" << std::endl;
        return 1;
    }

    std::string path = argv[1];
    std::cout << "fstree: scanning " << path << std::endl;

    return 0;
}