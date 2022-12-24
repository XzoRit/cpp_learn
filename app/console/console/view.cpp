#include <console/view.hpp>

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
auto intent(model::states::state s, const std::string& cmd_str)
    -> model::actions::action
{
    using ::boost::hof::match;
    return std::visit(
        match(
            [&](model::states::books) -> model::actions::action {
                if (commands::is(cmd_str, commands::select))
                    return model::actions::select{.id = extract_id(cmd_str)};
                if (commands::is(cmd_str, commands::add))
                    return model::actions::add{};
                if (commands::is(cmd_str, commands::remove))
                    return model::actions::remove{.id = extract_id(cmd_str)};
                if (commands::is(cmd_str, commands::exit))
                    return model::actions::exit{};
                return model::actions::text_input{cmd_str};
            },
            [&](model::states::add_book) -> model::actions::action {
                return model::actions::text_input{cmd_str};
            },
            [&](model::states::book) -> model::actions::action {
                if (commands::is(cmd_str, commands::select))
                    return model::actions::select{.id = extract_id(cmd_str)};
                if (commands::is(cmd_str, commands::add))
                    return model::actions::add{};
                if (commands::is(cmd_str, commands::remove))
                    return model::actions::remove{.id = extract_id(cmd_str)};
                if (commands::is(cmd_str, commands::quit))
                    return model::actions::quit{};
                return model::actions::text_input{cmd_str};
            },
            [&](model::states::add_chapter) -> model::actions::action {
                return model::actions::text_input{cmd_str};
            },
            [&](model::states::chapter) -> model::actions::action {
                if (commands::is(cmd_str, commands::add))
                    return model::actions::add{};
                if (commands::is(cmd_str, commands::remove))
                    return model::actions::remove{.id = extract_id(cmd_str)};
                if (commands::is(cmd_str, commands::start_training))
                    return model::actions::start_training{};
                if (commands::is(cmd_str, commands::quit))
                    return model::actions::quit{};
                return model::actions::text_input{cmd_str};
            },
            [&](model::states::add_card_front) -> model::actions::action {
                return model::actions::text_input{cmd_str};
            },
            [&](model::states::add_card_back) -> model::actions::action {
                return model::actions::text_input{cmd_str};
            },
            [&](model::states::training) -> model::actions::action {
                if (commands::is(cmd_str, commands::quit))
                    return model::actions::quit{};
                return model::actions::text_input{cmd_str};
            },
            [&](auto) -> model::actions::action {
                return model::actions::text_input{cmd_str};
            }),
        s);
}
auto draw(const data::data& data, model::states::state s) -> void
{
    using ::boost::hof::match;
    std::visit(
        match(
            [&](model::states::books) {
                content{}.books(data.the_books);
                menu{"book"}.select().add().remove().exit();
            },
            [](model::states::add_book) {
                println("add book");
                println("name: ");
            },
            [&](model::states::book s) {
                content{}.book(data.the_books.at(s.book_id));
                menu{"chapter"}.select().add().remove().quit();
            },
            [](model::states::add_chapter) {
                println("add chapter");
                println("name: ");
            },
            [&](model::states::chapter s) {
                content{}.chapter(
                    data.the_books.at(s.book_id).chapters.at(s.chapter_id));
                menu{"card"}.add().remove().start_training().quit();
            },
            [](model::states::add_card_front) {
                println("add chard");
                println("front: ");
            },
            [](model::states::add_card_back) { println("back: "); },
            [&](model::states::training) {
                std::visit(match(
                               [](data::training::states::show_card s) {
                                   println(s.card.front);
                               },
                               [](data::training::states::done) {
                                   menu{"training"}.quit();
                               }),
                           data.the_training.state);
            },
            [](auto) {}),
        s);
}
}
