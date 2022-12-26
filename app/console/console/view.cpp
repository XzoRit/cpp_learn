#include "console/model.hpp"
#include <console/view.hpp>

#include <boost/hof/match.hpp>

namespace xzr::learn::console::view
{
auto intent(const std::string& cmd_str) -> model::actions::action
{
    if (commands::is(cmd_str, commands::select))
        return model::actions::select{.id = commands::extract_id(cmd_str)};
    if (commands::is(cmd_str, commands::add))
        return model::actions::add{};
    if (commands::is(cmd_str, commands::remove))
        return model::actions::remove{.id = commands::extract_id(cmd_str)};
    if (commands::is(cmd_str, commands::start_training))
        return model::actions::start_training{};
    if (commands::is(cmd_str, commands::quit))
        return model::actions::quit{};
    if (commands::is(cmd_str, commands::exit))
        return model::actions::exit{};
    return model::actions::text_input{cmd_str};
}
auto draw(const view::model::data view_model) -> void
{
    using ::boost::hof::match;
    std::visit(
        match(
            [&](model::states::books) {
                content{}.books(view_model.model_data.the_books);
                menu{"book"}.select().add().remove().exit();
            },
            [](model::states::add_book) {
                println("add book");
                println("name: ");
            },
            [&](model::states::book s) {
                content{}.book(view_model.model_data.the_books.at(s.book_id));
                menu{"chapter"}.select().add().remove().quit();
            },
            [](model::states::add_chapter) {
                println("add chapter");
                println("name: ");
            },
            [&](model::states::chapter s) {
                content{}.chapter(
                    view_model.model_data.the_books.at(s.book_id).chapters.at(
                        s.chapter_id));
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
                           view_model.model_data.the_training.state);
            },
            [](auto) {}),
        view_model.view_state);
}
}
