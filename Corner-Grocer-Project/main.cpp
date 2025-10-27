/*
 * Corner Grocer - Item Tracking Program (Quantity-Only Version)
 * CS210 Project 3 (Codio)
 *
 * Author: Jacoby Bonton
 * Instructor: Bill Chan
 * Date: October 20, 2025
 *
 * Input format (expects ONLY "item count" per line), e.g.:
 *   Apples 3
 *   Broccoli 7
 *   Garlic 8
 *   Cucumbers 9
 *   Onions 4
 *   Zucchini 7
 *   Potatoes 4
 *
 * Features:
 *   - Reads item + quantity into std::map<string,int>
 *   - Creates frequency.dat automatically at startup
 *   - Menu:
 *       1) Look up item frequency
 *       2) Print all item frequencies
 *       3) Print histogram
 *       4) Exit
 */

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cctype>
#include <sstream>

// -------- utilities --------
static std::string trim(const std::string& s) {
    size_t i = 0, j = s.size();
    while (i < j && std::isspace(static_cast<unsigned char>(s[i]))) ++i;
    while (j > i && std::isspace(static_cast<unsigned char>(s[j - 1]))) --j;
    return s.substr(i, j - i);
}

static std::string toLower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

static std::string normalize(const std::string& s) {
    return toLower(trim(s));
}

// -------- class --------
class CornerGrocer {
public:
    explicit CornerGrocer(const std::string& inputFilePath,
                          const std::string& backupFilePath = "frequency.dat")
        : inputPath(inputFilePath), backupPath(backupFilePath) {
        loadInput();    // read item + quantity lines
        writeBackup();  // create frequency.dat immediately
    }

    int getFrequency(const std::string& itemQuery) const {
        std::string key = normalize(itemQuery);
        auto it = freq.find(key);
        return it == freq.end() ? 0 : it->second;
    }

    void printAllFrequencies(std::ostream& out = std::cout) const {
        if (freq.empty()) {
            out << "[No items loaded]\n";
            return;
        }
        for (const auto& kv : freq) {
            out << kv.first << " " << kv.second << "\n";
        }
    }

    void printHistogram(std::ostream& out = std::cout) const {
        if (freq.empty()) {
            out << "[No items loaded]\n";
            return;
        }
        for (const auto& kv : freq) {
            out << kv.first << " ";
            for (int i = 0; i < kv.second; ++i) {
                out << '*';
            }
            out << "\n";
        }
    }

private:
    std::string inputPath;
    std::string backupPath;
    std::map<std::string, int> freq; // sorted alphabetically

    void loadInput() {
        std::ifstream in(inputPath);
        if (!in) {
            std::cerr << "WARNING: Could not open input file: " << inputPath << "\n";
            return;
        }

        std::string item;
        int count;
        while (in >> item >> count) {
            if (count > 0) {
                freq[normalize(item)] += count;
            }
        }
    }

    void writeBackup() const {
        std::ofstream out(backupPath);
        if (!out) {
            std::cerr << "WARNING: Could not write backup file: " << backupPath << "\n";
            return;
        }
        for (const auto& kv : freq) {
            out << kv.first << " " << kv.second << "\n";
        }
    }
};

// -------- menu --------
static int readMenuChoice() {
    while (true) {
        std::cout << "\n==== Corner Grocer Menu ====\n"
                  << "1. Look up item frequency\n"
                  << "2. Print all item frequencies\n"
                  << "3. Print histogram\n"
                  << "4. Exit\n"
                  << "Choose 1-4: ";

        std::string line;
        if (!std::getline(std::cin, line)) {
            std::cout << "\nExiting.\n";
            return 4;
        }

        line = trim(line);
        if (line.size() == 1 && line[0] >= '1' && line[0] <= '4') {
            return line[0] - '0';
        }

        std::cout << "Invalid choice. Please enter 1, 2, 3, or 4.\n";
    }
}

// -------- main --------
int main() {
    const std::string inputFile = "CS210_Project_Three_Input_File.txt";
    CornerGrocer tracker(inputFile);

    while (true) {
        int choice = readMenuChoice();
        if (choice == 4) {
            std::cout << "Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1: {
                std::cout << "Enter item to look up: ";
                std::string query;
                std::getline(std::cin, query);
                int count = tracker.getFrequency(query);
                std::cout << "Frequency of \"" << toLower(trim(query)) << "\": " << count << "\n";
                break;
            }

            case 2:
                tracker.printAllFrequencies();
                break;

            case 3:
                tracker.printHistogram();
                break;

            default:
                std::cout << "Invalid option.\n";
        }
    }

    return 0;
}