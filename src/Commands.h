#pragma once
#include "Clipboard.h"
#include "ICommand.h"
#include "NodeNamePopup.h"
#include "FileOperations.h"

class CreateNodeCommand : public ICommand {
public:
    explicit CreateNodeCommand(NodeNamePopup& nodeNamePopup, GraphLayout& layout)
        : nodeNamePopup(nodeNamePopup), layout(layout) {}
    void Execute() override;

private:
    NodeNamePopup& nodeNamePopup;
    GraphLayout& layout;
};

class RenameNodeCommand : public ICommand {
public:
    explicit RenameNodeCommand(GraphLayout& layout, NodeNamePopup& nodeNamePopup)
        : layout(layout), nodeNamePopup(nodeNamePopup) {}
    void Execute() override;

private:
    GraphLayout& layout;
    NodeNamePopup& nodeNamePopup;
};

class CopyNodeCommand : public ICommand {
public:
    CopyNodeCommand(Clipboard& clipboard, GraphLayout& layout)
    : clipboard(clipboard), layout(layout) {}
    void Execute() override;

private:
    Clipboard& clipboard;
    GraphLayout& layout;
};

class CutNodeCommand : public ICommand {
public:
    CutNodeCommand(Clipboard& clipboard, GraphLayout& layout)
        : clipboard(clipboard), layout(layout) {}
    void Execute() override;

private:
    Clipboard& clipboard;
    GraphLayout& layout;
};

class PasteNodeCommand : public ICommand {
public:
    PasteNodeCommand(Clipboard& clipboard, GraphLayout& layout)
        : clipboard(clipboard), layout(layout) {}
    void Execute() override;

private:
    Clipboard& clipboard;
    GraphLayout& layout;
};

class DeleteNodeCommand : public ICommand {
public:
    explicit DeleteNodeCommand(GraphLayout& layout)
        : layout(layout) {}
    void Execute() override;

private:
    GraphLayout& layout;
};
