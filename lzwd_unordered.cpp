#include <algorithm>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

std::unordered_map<std::string, int> dictionary;
std::vector<int> compressed_data;
std::vector<std::string> check_compression;

struct ValueComparator {
    template <typename T>
    bool operator()(const T& a, const T& b) const {
        return a.second < b.second;
    }
};

void print_dict_to_file() {
    std::ofstream outputFile("dictionary.txt");

    for (const auto& pair : dictionary) {
        outputFile << pair.first << " " << pair.second << std::endl;
    }

    outputFile.close();
}

void print_dictionary() {
    std::cout << "Dictionary:" << std::endl;
    std::vector<std::pair<std::string, int>> vectorPairs(dictionary.begin(), dictionary.end());
    std::sort(vectorPairs.begin(), vectorPairs.end(), ValueComparator());

    for (const auto& pair : vectorPairs) {
        std::cout << pair.first.data() << ": " << pair.second << std::endl;
    }
}

void print_menu(int& choice) {
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "                   START MENU                   " << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "1. Dictionary settings" << std::endl;
    std::cout << "2. Compression settings" << std::endl;
    std::cout << "3. Start compression" << std::endl;
    std::cout << "4. Quit" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    while (std::cin.fail() || choice < 1 || choice > 4) {
        std::cout << "Invalid input, enter a valid option: ";
        std::cin >> choice;
    }
}

void print_dictionary_settings(size_t& dict_size, int dict_type, bool& clean_dict) {
    int choice;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "              DICTIONARY SETTINGS              " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "1. Change dictionary size (current: " << dict_size << ")" << std::endl;
    std::cout << "2. Change dictionary type (current: " << dict_type << ")" << std::endl;
    std::cout << "3. Change dictionary cleaning (current: " << clean_dict << ")" << std::endl;
    std::cout << "4. Return" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    while (std::cin.fail() || choice < 1 || choice > 5) {
        std::cout << "Invalid input, enter a valid option: ";
        std::cin >> choice;
    }
    switch (choice) {
        case 1:
            std::cout << "Current dictionary size: " + dict_size << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Enter new dictionary size: ";
            std::cin >> dict_size;
            while (std::cin.fail()) {
                std::cout << "Invalid dictionary size, enter valid size: ";
                std::cin >> dict_size;
            }
            break;
        case 2:
            std::cout << "Current dictionary type: " + dict_type << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Enter new dictionary type: ";
            std::cin >> dict_type;
            while (std::cin.fail()) {
                std::cout << "Invalid dictionary type, enter valid type: ";
                std::cin >> dict_type;
            }
            break;
        case 3:
            std::cout << "Current dictionary cleaning: " + clean_dict << std::endl;
            std::cout << "-----------------------------------------------" << std::endl;
            std::cout << "Enter new setting(1-CLEAN|0-NO CLEAN): ";
            std::cin >> choice;
            while (std::cin.fail()) {
                std::cout << "Invalid block size, enter valid size: ";
                std::cin >> choice;
            }
            clean_dict = (choice == 1) ? true : false;
    }
}

void print_info(std::string input_file, std::string output_file, int dict_size, int dict_type, bool clean_dict) {
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "                  INFORMATION                  " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Author 1: David Hipolito Santos 76338" << std::endl;
    std::cout << "Author 2: Joao Alonso Barbosa 68462" << std::endl;
    std::cout << "Input file: " << input_file << std::endl;
    std::cout << "Output file: " << output_file << std::endl;
    std::cout << "Dictionary size: " << dict_size << std::endl;
    std::cout << "Dictionary type: " << dict_type << std::endl;
    std::cout << "Dictionary cleaning: " << clean_dict << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
}

void print_block_settings(std::streamsize& block_size) {
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "                 BLOCK SETTINGS                " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Current block size: " << block_size << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    std::cout << "Enter new block size: ";
    std::cin >> block_size;
    while (std::cin.fail()) {
        std::cout << "Invalid block size, enter valid size: ";
        std::cin >> block_size;
    }
}

void print_comp_data(bool& show_comp_data) {
    int input;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "                 PRINT SETTINGS                " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Current setting: " << show_comp_data << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;

    std::cout << "Enter new setting(1-Print|0-Don't print): ";
    std::cin >> input;
    while (std::cin.fail()) {
        std::cout << "Invalid block size, enter valid size: ";
        std::cin >> input;
    }
    show_comp_data = (input = 1) ? true : false;
}

void print_in_file_settings(std::string& in_file_name) {
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "              INPUT FILE SETTINGS              " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Current input file name: " << in_file_name << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Enter new input file name: ";
    std::cin >> in_file_name;
    while (std::cin.fail()) {
        std::cout << "Invalid input, enter valid file name: ";
        std::cin >> in_file_name;
    }
}

void print_out_file_settings(std::string& out_file_name) {
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "              OUTPUT FILE SETTINGS             " << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Current output file name: " << out_file_name << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Enter new output file name: ";
    std::cin >> out_file_name;
    while (std::cin.fail()) {
        std::cout << "Invalid input, enter valid file name: ";
        std::cin >> out_file_name;
    }
}

void print_compression_settings(std::streamsize& block_size, std::string& in_file_name, std::string& out_file_name, bool& comp_data) {
    int choice;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "              COMPRESSION SETTINGS              " << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "1. Change block size" << std::endl;
    std::cout << "2. Change input file name" << std::endl;
    std::cout << "3. Change output file name" << std::endl;
    std::cout << "4. Print information while running" << std::endl;
    std::cout << "5. Return" << std::endl;
    std::cout << "-----------------------------------------------" << std::endl;
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    while (std::cin.fail() || choice < 1 || choice > 5) {
        std::cout << "Invalid input, enter a valid option: ";
        std::cin >> choice;
    }
    switch (choice) {
        case 1:
            print_block_settings(block_size);
            break;
        case 2:
            print_in_file_settings(in_file_name);
            break;
        case 3:
            print_out_file_settings(out_file_name);
        case 4:
            print_comp_data(comp_data);
            break;
        case 5:
            break;
    }
}

void count_occurrences(int arr[], int size) {
    std::unordered_map<int, int> occurrenceMap;

    // Conta o número de ocorrências de cada número
    for (int i = 0; i < size; i++) {
        occurrenceMap[arr[i]]++;
    }

    // Escreve o número de ocorrências de cada número no console
    for (const auto& pair : occurrenceMap) {
        std::cout << "The pattern " << pair.first << " appears " << pair.second << " time(s)." << std::endl;
    }
}

void print_codes() {
    int pattern_number = 0, pattern_sum = 0, average_pattern_size = 0, total_patterns = 0, array_size = 0, n = 0;
    std::cout << "Compressed Data: ";
    // Imprime no stdout os dados codificados e enviados para o ficheiro de saída
    for (int code : compressed_data) {
        std::cout << code << " ";
        pattern_number++;  // Numero total de padróes enviados para o ficheiro de saída
    }
    std::cout << std::endl;
    std::cout << std::endl;

    // Cria um array com os valores codificados e enviados para o ficheiro de saída
    int array[pattern_number];
    for (int code : compressed_data) {
        array[n] = code;
        n++;
    }
    array_size = sizeof(array) / sizeof(array[0]);
    count_occurrences(array, array_size);

    std::cout << std::endl;
    std::cout << "Number of patterns sent to output: " << pattern_number << std::endl;
    std::cout << std::endl;
    std::vector<std::pair<std::string, int>> vectorPairs(dictionary.begin(), dictionary.end());
    std::sort(vectorPairs.begin(), vectorPairs.end(), ValueComparator());

    // Contabiliza o numero de padrões que existem no dicionário
    for (const auto& pair : vectorPairs) {
        pattern_sum = pattern_sum + pair.first.size();
        total_patterns++;
    }
    average_pattern_size = pattern_sum / total_patterns;  // Cálculo da média do tamanho dos padrões que existem no dicionário
    std::cout << "Avegare size of all dictionary patterns: " << average_pattern_size << std::endl;
    std::cout << std::endl;
}

void initialize_dictionary(int mode) {
    // Preenche os primeiros 256 espaços do dicionário com os 256 padrões possíveis de um byte
    dictionary.clear();
    switch (mode) {
        case 1:
            for (int i = 0; i < 256; ++i) {
                std::string key(1, static_cast<char>(i));
                dictionary[key] = i;
            }
            break;
        case 2:

            break;
    }
}

int search(const std::string& pattern) {
    auto it = dictionary.find(pattern);
    if (it != dictionary.end()) {
        return it->second;
    }
    return 0;
}

int insert(const std::string& pattern, bool clean_dict, size_t dict_size, int dict_type, int& dict_full, int& fail_insert) {
    size_t current_size = dictionary.size();
    if (current_size >= dict_size) {
        if (clean_dict) {
            initialize_dictionary(dict_type);
            dict_full++;
        } else {
            fail_insert++;
            dict_full = 1;
        }
        return 0;
    }
    auto result = dictionary.emplace(pattern, current_size + 1);
    if (result.second) {
        std::cout << "Result: " << result.first->first << " " << result.second << std::endl;
        return current_size + 1;
    }
    return 0;
}

void output(int code) {
    compressed_data.push_back(code);  // Adiciona um novo bloco de dados comprimidos ao vetor final
}

size_t compress(const std::string& block, bool clean_dict, size_t dict_size, int dict_type, int& dict_full, int& fail_insert) {
    std::string Pa = block.substr(0, 1);
    size_t ind = 1, added_patterns = 0;
    bool added = false;

    while (ind < block.length()) {
        std::string Pb = block.substr(ind, 1);
        int i = 1;
        int code = search(Pa);

        while ((ind + i <= block.length()) && search(Pb + block.substr(ind + i, 1))) {
            Pb += block.substr(ind + i, 1);
            i++;
        }

        output(code);
        check_compression.push_back(Pa);

        int j = 0;
        while (j < i && insert(Pa + block.substr(ind, j + 1), clean_dict, dict_size, dict_type, dict_full, fail_insert)) {
            j++;
            added = true;
        }
        if (added) {
            std::cout << Pb;
            added = false;
        }

        if (ind + i >= block.length()) {
            output(search(Pb));
            return added_patterns;
        }

        ind += i;
        Pa = Pb;
    }
    return -1;
}

void write_output_to_file(const std::string& filename) {
    std::ofstream output_file(filename, std::ios::binary);  // Abre o ficheiro de saída
    if (!output_file) {
        std::cerr << "Failed to open the output file." << std::endl;
        return;
    }
    // Escreve para o ficheiro de saída
    for (int code : compressed_data) {
        output_file.write(reinterpret_cast<const char*>(&code), sizeof(code));
    }

    output_file.close();
}

void write_check_to_file(const std::string filename) {
    std::ofstream output_file(filename);
    if (!output_file) {
        std::cerr << "Failed to open the output file." << std::endl;
        return;
    }
    for (std::string& str : check_compression) {
        output_file << str;
    }
    output_file.close();
}

void compression_handler(std::string in_file_name, std::streamsize block_size, size_t dict_size, int dict_type, bool clean_dict, int& dict_full, int& fail_insert, bool show_comp_data) {
    std::string in_file_path = "files/" + in_file_name;
    std::ifstream input_file(in_file_path, std::ios::binary);  // Abre o ficheiro de entrada
    if (!input_file) {
        std::cerr << "Failed to open the input file: " << in_file_path << std::endl;
        return;
    }

    char buffer[block_size];  // Cria o buffer
    int block_number = 0, file_size = 0;
    compressed_data.clear();

    // Captura o tempo antes da execução
    auto start = std::chrono::high_resolution_clock::now();
    while (true) {
        input_file.read(buffer, block_size);  // Lê do ficheiro de entrada e escreve no buffer

        std::streamsize bytes_read = input_file.gcount();  // Conta o número de bytes lidos
        if (bytes_read == 0) {
            if (input_file.eof()) {
                // End of file reached
                break;
            } else {
                std::cerr << "Error occurred while reading the input file." << std::endl;
                return;
            }
        }
        std::string block(buffer, bytes_read);  // Escreve o conteúdo do buffer para uma string
        size_t new_patterns = compress(block, clean_dict, dict_size, dict_type, dict_full, fail_insert);
        size_t previous_size = 0;
        size_t new_outputs = compressed_data.size() - previous_size;
        previous_size = new_outputs;
        if (show_comp_data) {
            std::cout << "Bytes read: " << std::setw(5) << std::setfill(' ') << bytes_read << " | Outputs: " << std::setw(5) << std::setfill(' ') << new_outputs << " | New patterns: " << std::setw(5) << std::setfill(' ') << new_patterns << " | Dictionary size: " << std::setw(5) << std::setfill(' ') << dictionary.size() << std::endl;
        }
        block_number++;
        file_size = file_size + bytes_read;
    }

    auto end = std::chrono::high_resolution_clock::now();
    input_file.close();  // Captura o tempo após a execução
    // Calcula a duração em segundos
    std::chrono::duration<double> duration = end - start;
    double executionTime = duration.count();
}

void handler() {
    int choice;
    std::string in_file_name = "input.txt", out_file_name = "output.bin";
    bool clean_dict = false, keep_running = true, comp_data = false;
    size_t dict_size = 65536;  // Default
    int dict_type = 1;
    std::streamsize block_size = 64 * 1024;  // Default
    int fail_insert = 0, dict_full = 0;

    // Start Menu
    while (keep_running) {
        // system("clear");
        print_menu(choice);
        switch (choice) {
            case 1:
                system("clear");
                print_dictionary_settings(dict_size, dict_type, clean_dict);
                break;
            case 2:
                system("clear");
                print_compression_settings(block_size, in_file_name, out_file_name, comp_data);
                break;
            case 3: {
                system("clear");
                print_info(in_file_name, out_file_name, dict_size, dict_type, clean_dict);
                compression_handler(in_file_name, block_size, dict_size, dict_type, clean_dict, dict_full, fail_insert, comp_data);
                print_dict_to_file();
                write_output_to_file(out_file_name);
                write_check_to_file("output_check.txt");
                break;
            }
            case 4:
                keep_running = false;
                system("clear");
                break;
        }
    }
}

int main() {  // Esta função serve para a interação inicial com o utilizador e para apresentar resultados estatístics
    // Function that handles program flow
    handler();
    return 0;
}
