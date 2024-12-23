#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <random>

using namespace std;
namespace fs = std::filesystem;

// Завдання 1
void createFilesAndLinks(const string& dirName) {
    try {
        fs::create_directory(dirName);
        cout << "Directory created: " << dirName << endl;

        vector<string> fileNames = {"file1.txt", "file2.txt", "file3.txt"};
        for (const auto& fileName : fileNames) {
            ofstream file(dirName + "/" + fileName);
            file << "Random content: " << rand() % 1000 << endl;
            file.close();
            cout << "File created: " << fileName << endl;
        }

        fs::create_directory(dirName + "/hard_links");
        fs::create_directory(dirName + "/sym_links");

        for (const auto& fileName : fileNames) {
            string filePath = dirName + "/" + fileName;
            fs::create_hard_link(filePath, dirName + "/hard_links/" + fileName);
            fs::create_symlink(filePath, dirName + "/sym_links/" + fileName);
            cout << "Links created for: " << fileName << endl;
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// Завдання 2
void compareDirectories(const string& dir1, const string& dir2) {
    try {
        set<string> dir1Files, dir2Files;

        for (const auto& entry : fs::directory_iterator(dir1)) {
            if (fs::is_regular_file(entry)) {
                dir1Files.insert(entry.path().filename().string());
            }
        }

        for (const auto& entry : fs::directory_iterator(dir2)) {
            if (fs::is_regular_file(entry)) {
                dir2Files.insert(entry.path().filename().string());
            }
        }

        // Виявляємо відмінності
        cout << "Files only in " << dir1 << ":\n";
        for (const auto& file : dir1Files) {
            if (dir2Files.find(file) == dir2Files.end()) {
                cout << file << endl;
            }
        }

        cout << "Files only in " << dir2 << ":\n";
        for (const auto& file : dir2Files) {
            if (dir1Files.find(file) == dir1Files.end()) {
                cout << file << endl;
            }
        }

    } catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// Завдання 3
void deleteOldFiles(const string& dirName, chrono::hours ageLimit) {
    try {
        auto now = chrono::system_clock::now();

        for (const auto& entry : fs::directory_iterator(dirName)) {
            if (fs::is_regular_file(entry)) {
                auto lastWriteTime = fs::last_write_time(entry);
                auto fileAge = chrono::duration_cast<chrono::hours>(now - lastWriteTime);

                if (fileAge > ageLimit) {
                    fs::remove(entry);
                    cout << "Deleted: " << entry.path().filename().string() << endl;
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Filesystem error: " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

int main() {
    srand(time(0));

    // Завдання 1
    string mainDir = "test_directory";
    createFilesAndLinks(mainDir);

    // Завдання 2
    compareDirectories("test_directory/hard_links", "test_directory/sym_links");

    // Завдання 3
    deleteOldFiles(mainDir, chrono::hours(24));

    return 0;
}
