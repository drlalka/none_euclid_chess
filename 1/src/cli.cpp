#include "cli.hpp"
#include <fstream>
#include <limits>
#include <sstream>

#define YAML_INPUT_FILE "input.yaml"
#define YAML_OUTPUT_FILE "output.yaml"

void dialog_start_operation() {
    std::string choice;
    while (true) {
        try {
            print_menu();
            std::cout << "Select option: ";
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            if (choice == "exit" || choice == "0") {
                handle_exit();
                return;
            } else if (choice == "to_yaml" || choice == "1") {
                handle_convert_to_yaml();
            } else if (choice == "from_yaml" || choice == "2") {
                handle_convert_from_yaml();
            } else if (choice == "example" || choice == "3") {
                handle_example();
            } else {
                std::cout << "Unknown command: " << choice << "\n";
            }
        } 
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
}

void print_menu() {
    std::cout << "\n=== сonverter ===\n"
              << "1. сonvert Org to YAML\n"
              << "2. сonvert YAML to Org\n" 
              << "3. example\n"
              << "0. Exit\n";
}

void handle_convert_to_yaml() {
    try {
        Organization org;
        std::cout << "Enter organization name: ";
        std::getline(std::cin, org.name);
        
        std::cout << "Enter INN (10 digits): ";
        std::getline(std::cin, org.inn);
        
        std::cout << "Enter number of workers: ";
        std::string workers_str;
        std::getline(std::cin, workers_str);
        org.workers = std::stoi(workers_str);
        
        std::cout << "\nSelect overload type for conversion:\n"
                  << "1. std::string\n"
                  << "2. const char*\n"
                  << "3. buffer (const char* + size)\n"
                  << "Choice: ";
        
        std::string overload_choice;
        std::cin >> overload_choice;
        std::string yaml_result;
        
        if (overload_choice == "1") {
            std::string inp = "organization";
            yaml_result = organization_to_yaml(inp, org);
        } else if (overload_choice == "2") {
            const char* inp = "organization";
            char* result = organization_to_yaml(inp, org);
            yaml_result = result;
            delete[] result;
        } else if (overload_choice == "3") {
            char* result = organization_to_yaml("organization", 12, org);
            yaml_result = result;
            delete[] result;
        } else {
            std::cout << "Invalid choice, using std::string by default.\n";
            yaml_result = organization_to_yaml("organization", org);
        }
        std::ofstream out_file(YAML_OUTPUT_FILE);
        if (!out_file) {
            throw std::runtime_error("Cannot create file: " + std::string(YAML_OUTPUT_FILE));
        }
        
        out_file << yaml_result;
        std::cout << "YAML saved to: " << YAML_OUTPUT_FILE << "\n";
        std::cout << "Generated YAML:\n" << yaml_result << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void handle_convert_from_yaml() {
    try {
        std::ifstream yaml_file(YAML_INPUT_FILE);
        if (!yaml_file) {
            throw std::runtime_error("Cannot open file: " + std::string(YAML_INPUT_FILE));
        }
        
        std::string yaml_content;
        std::string line;
        while (std::getline(yaml_file, line)) {
            yaml_content += line + "\n";
        }
        
        std::cout << "YAML content from file " << YAML_INPUT_FILE << ":\n" << yaml_content << "\n";
        std::cout << "\nSelect overload type for parsing:\n"
                  << "1. std::string\n"
                  << "2. const char*\n"
                  << "3. buffer (const char* + size)\n"
                  << "Choice: ";
        std::string overload_choice;
        std::cin >> overload_choice;
        Organization org;
        if (overload_choice == "1") {
            org = organization_from_yaml(yaml_content);
        } else if (overload_choice == "2") {
            org = organization_from_yaml(yaml_content.c_str());
        } else if (overload_choice == "3") {
            org = organization_from_yaml(yaml_content.c_str(), yaml_content.size());
        } else {
            std::cout << "Invalid choice, using std::string by default.\n";
            org = organization_from_yaml(yaml_content);
        }
        std::cout << "Parsed Organization:\n"
                  << "  Name: " << org.name << "\n"
                  << "  INN: " << org.inn << "\n"
                  << "  Workers: " << org.workers << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void handle_example() {
    try {
        Organization example_org = {"MEPhI", "7724068140", 700};
        std::cout << "=== Testing all to_yaml overloads ===\n\n";
        std::string yaml1 = organization_to_yaml(std::string("organization"), example_org);
        std::cout << "1. std::string overload:\n" << yaml1 << "\n";
        char* yaml2 = organization_to_yaml("organization", example_org);
        std::cout << "2. const char* overload:\n" << yaml2 << "\n";
        delete[] yaml2;
        char* yaml3 = organization_to_yaml("organization", 12, example_org);
        std::cout << "3. buffer overload:\n" << yaml3 << "\n";
        delete[] yaml3;
        std::cout << "\n=== Testing all from_yaml overloads ===\n\n";
        Organization org1 = organization_from_yaml(yaml1);
        std::cout << "1. std::string overload result:\n"
                  << "   Name: " << org1.name << "\n"
                  << "   INN: " << org1.inn << "\n"
                  << "   Workers: " << org1.workers << "\n\n";
        Organization org2 = organization_from_yaml(yaml1.c_str());
        std::cout << "2. const char* overload result:\n"
                  << "   Name: " << org2.name << "\n"
                  << "   INN: " << org2.inn << "\n"
                  << "   Workers: " << org2.workers << "\n\n";
        Organization org3 = organization_from_yaml(yaml1.c_str(), yaml1.size());
        std::cout << "3. buffer overload result:\n"
                  << "   Name: " << org3.name << "\n"
                  << "   INN: " << org3.inn << "\n"
                  << "   Workers: " << org3.workers << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error in example: " << e.what() << std::endl;
    }
}

void handle_exit() {
    std::cout << "Goodbye!\n";
}