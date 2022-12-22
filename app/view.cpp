#include "view.hpp"

#include <boost/hof/match.hpp>

namespace
{
auto str_to_id(const std::string& s) -> std::optional<int>
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
auto extract_id(const std::string& s)
{
    return str_to_id(s.substr(1));
}
}
namespace xzr::learn::console::view
{
auto intent(::xzr::learn::console::model::states::state s,
            const std::string& cmd_str)
    -> ::xzr::learn::console::model::actions::action
{
    using ::boost::hof::match;
    return std::visit(
        match(
            [&](::xzr::learn::console::model::states::books)
                -> ::xzr::learn::console::model::actions::action {
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::select))
                    return ::xzr::learn::console::model::actions::select{
                        .id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::add))
                    return ::xzr::learn::console::model::actions::add{};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::remove))
                    return ::xzr::learn::console::model::actions::remove{
                        .id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::exit))
                    return ::xzr::learn::console::model::actions::exit{};
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](::xzr::learn::console::model::states::add_book)
                -> ::xzr::learn::console::model::actions::action {
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](::xzr::learn::console::model::states::book)
                -> ::xzr::learn::console::model::actions::action {
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::select))
                    return ::xzr::learn::console::model::actions::select{
                        .id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::add))
                    return ::xzr::learn::console::model::actions::add{};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::remove))
                    return ::xzr::learn::console::model::actions::remove{
                        .id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::quit))
                    return ::xzr::learn::console::model::actions::quit{};
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](::xzr::learn::console::model::states::add_chapter)
                -> ::xzr::learn::console::model::actions::action {
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](::xzr::learn::console::model::states::chapter)
                -> ::xzr::learn::console::model::actions::action {
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::add))
                    return ::xzr::learn::console::model::actions::add{};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::remove))
                    return ::xzr::learn::console::model::actions::remove{
                        .id = extract_id(cmd_str)};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::start_training))
                    return ::xzr::learn::console::model::actions::
                        start_training{};
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::quit))
                    return ::xzr::learn::console::model::actions::quit{};
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](::xzr::learn::console::model::states::add_card_front)
                -> ::xzr::learn::console::model::actions::action {
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](::xzr::learn::console::model::states::add_card_back)
                -> ::xzr::learn::console::model::actions::action {
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](::xzr::learn::console::model::states::training)
                -> ::xzr::learn::console::model::actions::action {
                if (::xzr::learn::console::commands::is(
                        cmd_str,
                        ::xzr::learn::console::commands::quit))
                    return ::xzr::learn::console::model::actions::quit{};
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            },
            [&](auto) -> ::xzr::learn::console::model::actions::action {
                return ::xzr::learn::console::model::actions::text_input{
                    cmd_str};
            }),
        s);
}
auto draw(const ::xzr::learn::data::data& data,
          ::xzr::learn::console::model::states::state s) -> void
{
    using ::boost::hof::match;
    std::visit(
        match(
            [&](::xzr::learn::console::model::states::books) {
                content{}.books(data.the_books);
                menu{"book"}.select().add().remove().exit();
            },
            [](::xzr::learn::console::model::states::add_book) {
                println("add book");
                println("name: ");
            },
            [&](::xzr::learn::console::model::states::book s) {
                content{}.book(data.the_books.at(s.book_id));
                menu{"chapter"}.select().add().remove().quit();
            },
            [](::xzr::learn::console::model::states::add_chapter) {
                println("add chapter");
                println("name: ");
            },
            [&](::xzr::learn::console::model::states::chapter s) {
                content{}.chapter(
                    data.the_books.at(s.book_id).chapters.at(s.chapter_id));
                menu{"card"}.add().remove().start_training().quit();
            },
            [](::xzr::learn::console::model::states::add_card_front) {
                println("add chard");
                println("front: ");
            },
            [](::xzr::learn::console::model::states::add_card_back) {
                println("back: ");
            },
            [&](::xzr::learn::console::model::states::training) {
                std::visit(
                    match(
                        [](::xzr::learn::data::training::states::show_card s) {
                            println(s.card.front);
                        },
                        [](::xzr::learn::data::training::states::done) {
                            menu{"training"}.quit();
                        }),
                    data.the_training.state);
            },
            [](auto) {}),
        s);
}
}
