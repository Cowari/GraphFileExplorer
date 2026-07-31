#include "FileSystemScanner.h"

#include <iostream>

bool FileSystemScanner::IsHiddenUnix(const fs::directory_entry& entry) {
    const std::string fileName = entry.path().filename().string();
    return !fileName.empty() && fileName[0] == '.';
}

void FileSystemScanner::BuildFromDirectory(const fs::path& path, GraphLayout& layout, const size_t parentIdx) {
    try {
        for (const fs::directory_entry& entry : fs::directory_iterator(path)) {
            if ( !IsHiddenUnix(entry) ) {
                const bool isDir = entry.is_directory();

                if (!layout.IsNodeExists( entry.path()) ) {
                    layout.AddChildInOrbit(parentIdx, isDir, entry.path());
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "ERROR SCAN:\n" << e.what() << std::endl;
    }
}
