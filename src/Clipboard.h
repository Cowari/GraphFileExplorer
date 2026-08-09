#pragma once
#include <optional>
#include <string>

enum class ClipboardMode { Copy, Cut };

struct Clipboard {
    ClipboardMode clipboardMode = ClipboardMode::Copy;
    std::optional<std::string> clipboardPath;
};
