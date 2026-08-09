#pragma once
#include <imgui.h>

#include "ICommand.h"

struct KeyBinding {
    ImGuiKey key;
    bool requiresCtrl;
    ICommand* command;
};
