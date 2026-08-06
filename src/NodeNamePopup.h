#pragma once
#include <imgui.h>
#include <string>

#include "GraphLayout.h"

enum class PopupMode {
    Create,
    Rename
};

class NodeNamePopup {
public:
    void Open(PopupMode mode, const std::string& initName = "");
    void Draw(const ImGuiWindowFlags &flags, GraphLayout &layout);
    void Close();

    [[nodiscard]] bool IsOpen() const;

private:
    bool isOpen = false;
    bool inputFocused = false;
    PopupMode popMode = PopupMode::Create;
    char nameBuffer[256]{};

    void Confirm(GraphLayout& layout);

    [[nodiscard]] bool IsNameValid() const;
    [[nodiscard]] bool HasDirectoryPrefix() const;
};
