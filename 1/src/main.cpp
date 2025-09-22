#include "cli.hpp"

int main() {
    try {
        dialog_start_operation();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}