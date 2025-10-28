#include <iostream>
#include <vector>
#include <string>

int main(int argc, char** argv) {
    std::cout << "CACA Test - Program Started\n";
    std::cout << "Arguments received: " << argc << "\n";
    
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " -f <file>\n";
        return 1;
    }
    
    for (int i = 0; i < argc; i++) {
        std::cout << "  Arg[" << i << "]: " << argv[i] << "\n";
    }
    
    std::cout << "Program ending normally\n";
    return 0;
}