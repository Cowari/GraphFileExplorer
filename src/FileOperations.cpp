#include "FileOperations.h"

#include <fstream>
#include <iostream>

fs::path FileOperations::GetAvailablePath(const fs::path& parentPath, const fs::path& fileName, const bool isDir) {
    fs::path candidatePath = parentPath / fileName;
    int counter = 1;
    const std::string ext = fileName.extension().string();
    const std::string stem = fileName.stem().string();
    while (fs::exists(candidatePath)) {
        std::string candidateName;
        if (isDir) {
            candidateName = fileName.string() + " " + '(' + std::to_string(counter) + ')';
        } else {
            candidateName = stem + " (" + std::to_string(counter) + ")" += ext;
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
    const fs::path finalPath = GetAvailablePath(destination, source.filename(), fs::is_directory(source));
    std::error_code ec;
    fs::copy(source, finalPath, options, ec);

    if (ec) {
        std::cerr << "ERROR COPY:\n" << ec.message() << std::endl;
    } else {
        if (const auto parentIndex = layout.GetNodeIndexByPath(destination)) {
            if (!layout.IsNodeExists(finalPath)) {
                const bool isDir = fs::is_directory(source);
                layout.AddChildInOrbit(*parentIndex, isDir, finalPath);
                std::cout << "Copied " << source << " to " << destination << std::endl;
            }
        } else {
            std::cerr << "Copied to disk, but no destination node found for destination: " << destination << std::endl;
        }
    }
}

void FileOperations::CutFile(const fs::path &source, const fs::path &destination, GraphLayout& layout) {
    const fs::path finalPath = GetAvailablePath(destination, source.filename(), fs::is_directory(source));
    std::error_code ec;
    fs::rename(source, finalPath, ec);

    if (ec) {
        std::cerr << "ERROR CUT:\n" << ec.message() << std::endl;
    } else {
        if (const auto nodeIndex = layout.GetNodeIndexByPath(source)) {
            if (const auto parentIndex = layout.GetNodeIndexByPath(destination)) {
                layout.SetNodeParent(*nodeIndex, *parentIndex);

                const std::string oldPath = layout.GetNodes()[*nodeIndex].GetPath();
                layout.SetNodePath(*nodeIndex, finalPath);

                if (fs::is_directory(finalPath)) {
                    layout.UpdateDescendantPaths(oldPath, finalPath.string());
                }

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
            const std::string oldPath = layout.GetNodes()[*nodeIndex].GetPath();
            layout.SetNodePath(*nodeIndex, finalPath);

            if (fs::is_directory(finalPath)) {
                layout.UpdateDescendantPaths(oldPath, finalPath.string());
            }

            std::cout << "Renamed " << path << " to " << finalPath << std::endl;
        } else {
            std::cerr << "Renamed on disk, but no node found for path: " << path << std::endl;
        }
    }
}
