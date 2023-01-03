#include <commands.hpp>

#include <algorithm>

namespace
{
inline auto str_to_id(const std::string& s) -> std::optional<int>
{
    try
    {
        return std::stoi(s) - 1;
    }
    catch (...)
    {
        return std::nullopt;
    }
}
}
namespace xzr::learn::console::commands
{
const auto select{command{.cmd = "!a<n>", .desc = "select"}};
const auto add{command{.cmd = "!b", .desc = "add"}};
const auto remove{command{.cmd = "!c<n>", .desc = "remove"}};
const auto start_training{command{.cmd = "!s", .desc = "start training"}};
const auto quit{command{.cmd = "!d", .desc = "quit"}};
const auto exit{command{.cmd = "!e", .desc = "exit"}};
[[nodiscard]] auto as_str(const command& c) -> std::string
{
    return c.cmd + ":\t" + c.desc;
}
[[nodiscard]] auto is(const std::string& str_cmd, const command& c) -> bool
{
    if (str_cmd == c.cmd)
        return true;
    const auto num_start_idx{c.cmd.find_first_of('<')};
    return (num_start_idx != std::string::npos) &&
           (str_cmd.substr(0, num_start_idx) ==
            c.cmd.substr(0, num_start_idx)) &&
           std::all_of(std::next(str_cmd.cbegin(), num_start_idx),
                       str_cmd.cend(),
                       [](auto a) { return !!std::isdigit(a); });
}
[[nodiscard]] auto extract_id(const std::string& s) -> std::optional<int>
{
    const auto match{std::find_if(s.cbegin(), s.cend(), [](auto a) {
        return !!std::isdigit(a);
    })};
    if (std::all_of(match, s.cend(), [](auto a) { return !!std::isdigit(a); }))
        return ::str_to_id(std::string{match, s.cend()});
    else
        return std::nullopt;
}
}
