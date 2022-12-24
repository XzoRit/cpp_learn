#pragma once

#include <string>

namespace xzr::learn::console::commands
{
struct command
{
    std::string cmd{};
    std::string desc{};
};
extern const command select;
extern const command add;
extern const command remove;
extern const command start_training;
extern const command quit;
extern const command exit;
[[nodiscard]] auto is(const std::string& str_cmd, const command& c) -> bool;
[[nodiscard]] auto as_str(const command& c) -> std::string;
}
