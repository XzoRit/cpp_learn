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
}

namespace
{
namespace console
{
[[nodiscard]] auto readln()
{
    std::string str{};
    std::getline(std::cin, str);
    return str;
}
}
}

namespace
{
namespace console::render
{
[[nodiscard]] auto console(const ::xzr::learn::data::app& app_data)
    -> std::optional<::xzr::learn::data::action>
{
    for (int i{}; const auto& b : app_data.the_books)
        println(++i, ".\t", b.name);

    println("");
    println("a<n>:\tselect");
    println("b:\tadd");
    println("c<n>:\tremove");
    println("d:\tquit");

    const auto books_cmd{readln()};
    if (books_cmd == "b")
    {
        println("add book");
        println("name: ");
        return ::xzr::learn::data::add_book{.name = readln()};
    }
    else if (books_cmd.starts_with('c'))
    {
        const auto book_id{std::stoi(
            std::string(std::next(books_cmd.cbegin()), books_cmd.cend()))};
        return ::xzr::learn::data::remove_book{.id = book_id - 1};
    }
    else if (books_cmd == "d")
    {
        return ::xzr::learn::data::quit{};
    }
    else
    {
        return std::nullopt;
    }
}
}
}

namespace xzr::learn::console
{
inline namespace v1
{
auto run() -> int
{
    auto app_data{::persist::read_or_create_app_data()};

    for (;;)
    {
        if (const auto data_act{::console::render::console(app_data)})
        {
            app_data = ::xzr::learn::data::update(std::move(app_data),
                                                  data_act.value());
            ::persist::save(app_data);
            if (std::holds_alternative<::xzr::learn::data::quit>(
                    data_act.value()))
                return 0;
        }
        else
        {
            println("sorry, unknown command");
            println();
        }
    }
}
}
}
