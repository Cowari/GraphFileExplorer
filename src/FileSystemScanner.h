#pragma once
#include <filesystem>
#include "GraphLayout.h"

namespace fs = std::filesystem;

class FileSystemScanner {
public:
    static void BuildFromDirectory(const fs::path& path, GraphLayout& layout, size_t parentIdx = 0);
private:
    static bool IsHiddenUnix(const fs::directory_entry& entry);
    //bool IsHiddenWin(const fs::path& path);
};