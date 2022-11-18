#include "console.hpp"

#include <data/action.hpp>
#include <data/app.hpp>
#include <data/data.hpp>
#include <data/serialize.hpp>
#include <data/update.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/hof/match.hpp>
#include <boost/hof/result.hpp>

#include <filesystem>
#include <fstream>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>

using ::xzr::learn::console::println;

namespace
{
namespace console
{
struct add_book
{
};
struct quit
{
};
using action = std::variant<add_book, quit>;
}
namespace persist
{
namespace fs = std::filesystem;

const auto books_path{fs::path{"xzr_learn_books.txt"}};

auto save(const ::xzr::learn::data::app& app_data)
{
    using oarchive = ::boost::archive::text_oarchive;
    using ::xzr::learn::data::save;

    auto f{std::ofstream{books_path}};
    auto o{oarchive{f}};
    save(o, app_data);
}
[[nodiscard]] auto load()
{
    using iarchive = ::boost::archive::text_iarchive;
    using ::xzr::learn::data::load;

    auto f{std::ifstream{books_path}};
    auto i{iarchive{f}};
    return load(i);
}
[[nodiscard]] auto read_or_create_app_data()
{
    if (!fs::exists(books_path))
        save(::xzr::learn::data::app{});
    return load();
}
}
[[nodiscard]] auto readln()
{
    std::string str{};
    std::getline(std::cin, str);
    return str;
}
[[nodiscard]] auto dialog_add_book()
{
    println("add book");
    println("name: ");
    return ::xzr::learn::data::add_book{.name = readln()};
}
[[nodiscard]] auto update_console(const ::xzr::learn::data::app&,
                                  console::action act)
{
    using ::boost::hof::match;
    using ::boost::hof::result;

    return std::visit(result<std::optional<::xzr::learn::data::action>>(match(
                          [&](console::add_book) { return dialog_add_book(); },
                          [&](console::quit) { return std::nullopt; })),
                      act);
}
[[nodiscard]] auto update(::xzr::learn::data::app app_data, console::action act)
{
    using ::xzr::learn::data::update;

    if (const auto data_act{update_console(app_data, act)})
        return update(std::move(app_data), data_act.value());
    return app_data;
}
[[nodiscard]] auto intent(std::string_view cmd)
    -> std::optional<console::action>
{
    static const std::map<std::string_view, console::action> cmd_actions{
        {{"b", console::add_book{}}, {"d", console::quit{}}}};

    if (const auto match{cmd_actions.find(cmd)}; match != cmd_actions.cend())
        return match->second;
    return std::nullopt;
}
auto render_books(const ::xzr::learn::data::books& bs)
{
    for (int i{}; const auto& b : bs)
        println(++i, ".\t", b.name);

    println("");
    println("a<n>:\tselect");
    println("b:\tadd");
    println("c<n>:\tremove");
    println("d:\tquit");
}
auto render(const ::xzr::learn::data::app& app_data)
{
    render_books(app_data.the_books);
}
}
namespace xzr::learn::console
{
inline namespace v1
{
auto run() -> void
{
    auto app_data{::persist::read_or_create_app_data()};
    for (;;)
    {
        ::render(app_data);
        if (const auto console_act{::intent(::readln())})
        {
            app_data = ::update(std::move(app_data), console_act.value());
            ::persist::save(app_data);
            if (std::holds_alternative<::console::quit>(console_act.value()))
                return;
        }
    }
}
}
}
