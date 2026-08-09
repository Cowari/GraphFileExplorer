#include "Commands.h"

void CreateNodeCommand::Execute() {
    if (const auto selectedIdxOpt = layout.GetSelectedNodeIndex()) {
        if (!layout.GetNodes()[*selectedIdxOpt].IsDirectory()) return;
        nodeNamePopup.Open(PopupMode::Create);
    }
}

void RenameNodeCommand::Execute() {
    if (const auto selectedIdxOpt = layout.GetSelectedNodeIndex()) {
        const std::string name = layout.GetNodes()[*selectedIdxOpt].GetName();

        nodeNamePopup.Open(PopupMode::Rename, name);
    }
}

void CopyNodeCommand::Execute() {
    if (const auto selectedIdxOpt = layout.GetSelectedNodeIndex()) {
        clipboard.clipboardMode = ClipboardMode::Copy;
        clipboard.clipboardPath = layout.GetNodes()[*selectedIdxOpt].GetPath();
    }
}

void CutNodeCommand::Execute() {
    if (const auto selectedIdxOpt = layout.GetSelectedNodeIndex()) {
        clipboard.clipboardMode = ClipboardMode::Cut;
        clipboard.clipboardPath = layout.GetNodes()[*selectedIdxOpt].GetPath();
    }
}

void PasteNodeCommand::Execute() {
    if (const auto selectedNodeIdx = layout.GetSelectedNodeIndex()) {
        if (!layout.GetNodes()[*selectedNodeIdx].IsDirectory()) return;

        const std::string destinationPath = layout.GetNodes()[*selectedNodeIdx].GetPath();

        if (const auto sourcePath = clipboard.clipboardPath) {
            switch (clipboard.clipboardMode) {
                case ClipboardMode::Copy:
                    FileOperations::CopyFile(*sourcePath, destinationPath, layout);
                    break;
                case ClipboardMode::Cut:
                    FileOperations::CutFile(*sourcePath, destinationPath, layout);
                    clipboard.clipboardPath = std::nullopt;
                    break;
            }
        }
    }
}

void DeleteNodeCommand::Execute() {
    if (const auto selectedNodeIdx = layout.GetSelectedNodeIndex()) {
        const std::string path = layout.GetNodes()[*selectedNodeIdx].GetPath();

        FileOperations::DeleteFile(path, layout);
    }
}
