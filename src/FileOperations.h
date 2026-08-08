#pragma once
#include <filesystem>

#include "GraphLayout.h"

namespace fs = std::filesystem;

class FileOperations {
public:
    static void CreateFile(const fs::path& parentPath, const fs::path& fileName, bool isDir, GraphLayout& layout);

    static void CopyFile(const fs::path& source, const fs::path& destination, GraphLayout& layout);
    static void CutFile(const fs::path& source, const fs::path& destination,  GraphLayout& layout);
    static void RenameFile(const fs::path& path, const fs::path& newName, GraphLayout& layout);
    static void DeleteFile(const fs::path& path, GraphLayout& layout);

private:
    static fs::path GetAvailablePath(const fs::path& parentPath, const fs::path& fileName, bool isDir);
};
