#include <fstream>
#include <iostream>

int main() {
    // trunc creates navigation.txt fresh on every run.
    std::fstream file("navigation.txt", std::ios::in | std::ios::out | std::ios::trunc);
    if (!file) {
        std::cerr << "Error: Could not open navigation.txt\n";
        return 1;
    }

    file << "ABCDE";
    std::cout << "Output position after writing: " << file.tellp() << '\n';
    file.flush();

    file.seekg(0, std::ios::beg);
    char firstCharacter;
    file.get(firstCharacter);
    std::cout << "First character: " << firstCharacter << '\n';
    std::cout << "Input position after reading one character: " << file.tellg() << '\n';

    file.seekg(2, std::ios::beg);
    char thirdCharacter;
    file.get(thirdCharacter);
    std::cout << "Character at position 2: " << thirdCharacter << '\n';

    file.seekp(5, std::ios::beg);
    file << "F";
    file.flush();

    // Modification: read the last character using seekg(-1, end).
    file.clear();
    file.seekg(-1, std::ios::end);
    char lastCharacter;
    file.get(lastCharacter);
    std::cout << "Last character (read with seekg(-1, end)): " << lastCharacter << '\n';

    file.close();
    std::cout << "Navigation completed. Check navigation.txt (should contain ABCDEF)\n";
    return 0;
}
