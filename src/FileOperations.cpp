#include "FileOperations.h"

#include <fstream>
#include <iostream>

fs::path FileOperations::GetAvailablePath(const fs::path& parentPath, const fs::path& fileName, const bool isDir) {
    fs::path candidatePath = parentPath / fileName;
    int counter = 1;
    while (fs::exists(candidatePath)) {
        std::string candidateName;
        if (isDir) {
            candidateName = fileName.string() + " " + '(' + std::to_string(counter) + ')';
        } else {
            candidateName = fileName.stem().string() + " " + '(' + std::to_string(counter) + ')'+fileName.extension().string();
        }
        candidatePath = parentPath / fs::path(candidateName);
        counter++;
    }
    return candidatePath;
}

void FileOperations::CreateFile(const fs::path& parentPath, const fs::path& fileName, const bool isDir, GraphLayout& layout) {
    const fs::path& finalPath = GetAvailablePath(parentPath, fileName, isDir);

    if (isDir) {
        std::error_code ec;
        fs::create_directory(finalPath, ec);
        if (ec) {
            std::cerr << "ERROR CREATE:\n" << ec.message() << std::endl;
            return;
        }
    } else {
        std::ofstream file(finalPath);
        if (!file.is_open()) {
            std::cerr << "ERROR CREATE FILE:\n" << "failed to open " << finalPath << std::endl;
            return;
        }
    }

    if ( const auto parentIndex = layout.GetNodeIndexByPath(parentPath.string()) ) {
        layout.AddChildInOrbit(*parentIndex, isDir, finalPath);
    } else {
        std::cerr << "Created on disk, but no destination node found for destination: " << parentPath << std::endl;
    }
}

void FileOperations::CopyFile(const fs::path &source, const fs::path &destination, GraphLayout& layout) {
    constexpr auto options = fs::copy_options::recursive | fs::copy_options::overwrite_existing;
    std::error_code ec;
    fs::copy(source, destination / source.filename(), options, ec);

    if (ec) {
        std::cerr << "ERROR COPY:\n" << ec.message() << std::endl;
    } else {
        if (const auto parentIndex = layout.GetNodeIndexByPath(destination)) {
            if (!layout.IsNodeExists(destination/source.filename())) {
                const bool isDir = fs::is_directory(source);
                layout.AddChildInOrbit(*parentIndex, isDir, destination / source.filename());
                std::cout << "Copied " << source << " to " << destination << std::endl;
            }
        } else {
            std::cerr << "Copied to disk, but no destination node found for destination: " << destination << std::endl;
        }
    }
}

void FileOperations::CutFile(const fs::path &source, const fs::path &destination, GraphLayout& layout) {
    std::error_code ec;

    fs::rename(source, destination / source.filename(), ec);

    if (ec) {
        std::cerr << "ERROR CUT:\n" << ec.message() << std::endl;
    } else {
        if (const auto nodeIndex = layout.GetNodeIndexByPath(source)) {
            if (const auto parentIndex = layout.GetNodeIndexByPath(destination)) {
                layout.SetNodeParent(*nodeIndex, *parentIndex);
                layout.SetNodePath(*nodeIndex, destination / source.filename());

                std::cout << "Move " << source << " to " << destination << std::endl;
            } else {
                std::cerr << "Moved to disk, but no destination node found for destination: " << destination << std::endl;
            }
        } else {
            std::cerr << "Moved to disk, but no source node found for destination: " << destination << std::endl;
        }
    }
}

void FileOperations::Rename(const fs::path &path, const fs::path& newName, GraphLayout& layout) {
    const fs::path& finalPath = GetAvailablePath(path.parent_path(), newName.filename(), fs::is_directory(path));
    std::error_code ec;
    fs::rename(path, finalPath, ec);

    if (ec) {
        std::cerr << "ERROR RENAME:\n" << ec.message() << std::endl;
    } else {
        if (const auto nodeIndex = layout.GetNodeIndexByPath(path.string())) {
            layout.SetNodePath(*nodeIndex, finalPath);
            std::cout << "Renamed " << path << " to " << finalPath << std::endl;
        } else {
            std::cerr << "Renamed on disk, but no node found for path: " << path << std::endl;
        }
    }
}
