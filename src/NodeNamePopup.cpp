#include "NodeNamePopup.h"

#include <cstdio>
#include <cstring>

#include "FileOperations.h"

void NodeNamePopup::Open(const PopupMode mode, const std::string& initName) {
    isOpen = true;
    popMode = mode;
    inputFocused = true;

    std::snprintf(nameBuffer, sizeof(nameBuffer), "%s", initName.c_str());
}

void NodeNamePopup::Draw(const ImGuiWindowFlags &flags, GraphLayout &layout) {
    constexpr auto targetSize = ImVec2(400.f, 200.f);
    ImGui::SetNextWindowSize(targetSize);

    const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    const auto pos = ImVec2(
        center.x - targetSize.x * 0.5f,
        center.y - targetSize.y * 0.5f
    );
    ImGui::SetNextWindowPos(pos);

    ImGui::Begin("Setup Node", nullptr, flags);
    const char* text = "Let's name your node";
    const ImVec2 textSize = ImGui::CalcTextSize(text);

    const float avail = ImGui::GetContentRegionAvail().x;
    ImGui::SetCursorPosX((avail - textSize.x) * 0.5f);

    ImGui::TextUnformatted(text);


    constexpr float inputWidth = 250.f;
    ImGui::SetCursorPosX((avail - inputWidth) * 0.5f);

    if (inputFocused) {
        ImGui::SetKeyboardFocusHere();
        inputFocused = false;
    }
    ImGui::InputText("##node_name", nameBuffer, sizeof(nameBuffer));
    if (ImGui::IsItemFocused() && ImGui::IsKeyPressed(ImGuiKey_Enter, false)) {
        Confirm(layout);
        Close();
    }


    constexpr auto colNormal = ImVec4(0.3f, 0.5f, 0.7f, 0.7f);
    constexpr auto colHovered = ImVec4(0.3f, 0.5f, 0.7f, 1.f);
    constexpr auto colActive = ImVec4(0.3f, 0.5f, 0.6f, 1.f);
    ImGui::PushStyleColor(ImGuiCol_Button, colNormal);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colHovered);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, colActive);


    constexpr float buttonWidth = 125.f;
    const float spacing = ImGui::GetStyle().ItemSpacing.x;
    const float group = buttonWidth + spacing + buttonWidth;

    ImGui::SetCursorPosX((avail - group) * 0.5f);
    if (ImGui::Button("Cancel", ImVec2(buttonWidth, 25.f))) {
        Close();
    }
    ImGui::SameLine();

    const char* label = (popMode == PopupMode::Create) ? "Create" : "Rename";
    if (ImGui::Button(label, ImVec2(buttonWidth, 25.f))) {
        Confirm(layout);
        Close();
    }

    ImGui::PopStyleColor(3);
    ImGui::End();
}

void NodeNamePopup::Confirm(GraphLayout &layout) {
    if (!IsNameValid()) return;
    if (const auto selectIdxOpt = layout.GetSelectedNodeIndex()) {
        const std::string parentPath = layout.GetNodes()[*selectIdxOpt].GetPath();

        const std::string childName = (HasDirectoryPrefix())
            ? std::string(nameBuffer + 2)
            : std::string(nameBuffer);

        switch (popMode) {
            case PopupMode::Create:
                FileOperations::CreateFile(parentPath, childName, HasDirectoryPrefix(), layout);
                break;
            case PopupMode::Rename:
                FileOperations::RenameFile(parentPath, childName, layout);
                break;
        }
    }
}

void NodeNamePopup::Close() {
    std::memset(nameBuffer, 0, sizeof(nameBuffer));
    isOpen = false;
}

bool NodeNamePopup::IsOpen() const {
    return isOpen;
}

bool NodeNamePopup::IsNameValid() const {
    const std::string_view name{nameBuffer};

    if (name.empty()) {
        return false;
    }

    return name.find('/') == std::string::npos;
}

bool NodeNamePopup::HasDirectoryPrefix() const {
    return std::strncmp(nameBuffer, "d:", 2) == 0;
}
