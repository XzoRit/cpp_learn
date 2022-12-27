#include <console/console.hpp>

#include <console/commands.hpp>
#include <console/model.hpp>

#include <boost/hof/match.hpp>

namespace xzr::learn::console
{
struct menu
{
    menu(const std::string& name)
    {
        println("");
        println("=== " + name);
    }
    ~menu()
    {
        println("===");
        print("-> ");
    }
    auto select() -> menu&
    {
        println(commands::as_str(commands::select));
        return *this;
    }
    auto add() -> menu&
    {
        println(commands::as_str(commands::add));
        return *this;
    }
    auto remove() -> menu&
    {
        println(commands::as_str(commands::remove));
        return *this;
    }
    auto start_training() -> menu&
    {
        println(commands::as_str(commands::start_training));
        return *this;
    }
    auto quit() -> menu&
    {
        println(commands::as_str(commands::quit));
        return *this;
    }
    auto exit() -> menu&
    {
        println(commands::as_str(commands::exit));
        return *this;
    }
};
struct content
{
    content()
    {
        println();
    }
    ~content()
    {
        println("===");
        println();
    }
    auto chapter(const data::books::chapter& ch)
    {
        println("=== chapter: ", ch.name);
        println("cards:");
        for (int i{}; const auto& c : ch.cards)
        {
            println(++i, ".\t", c.front);
            println(i, ".\t", c.back);
        }
    }
    auto book(const data::books::book& b)
    {
        println("=== book: ", b.name);
        println("chapters:");
        for (int i{}; const auto& c : b.chapters)
            println(++i, ".\t", c.name);
    }
    auto books(const data::books::books& bs)
    {
        println("=== books:");
        for (int i{}; const auto& b : bs)
            println(++i, ".\t", b.name);
    }
};
auto intent(const std::string& cmd_str) -> view::model::actions::action
{
    if (commands::is(cmd_str, commands::select))
        return view::model::actions::select{.id =
                                                commands::extract_id(cmd_str)};
    if (commands::is(cmd_str, commands::add))
        return view::model::actions::add{};
    if (commands::is(cmd_str, commands::remove))
        return view::model::actions::remove{.id =
                                                commands::extract_id(cmd_str)};
    if (commands::is(cmd_str, commands::start_training))
        return view::model::actions::start_training{};
    if (commands::is(cmd_str, commands::quit))
        return view::model::actions::quit{};
    if (commands::is(cmd_str, commands::exit))
        return view::model::actions::exit{};
    return view::model::actions::text_input{cmd_str};
}
auto draw(const view::model::data view_model) -> void
{
    using ::boost::hof::match;
    std::visit(
        match(
            [&](view::model::states::books) {
                content{}.books(view_model.model_data.the_books);
                menu{"book"}.select().add().remove().exit();
            },
            [](view::model::states::add_book) {
                println("add book");
                println("name: ");
            },
            [&](view::model::states::book s) {
                content{}.book(view_model.model_data.the_books.at(s.book_id));
                menu{"chapter"}.select().add().remove().quit();
            },
            [](view::model::states::add_chapter) {
                println("add chapter");
                println("name: ");
            },
            [&](view::model::states::chapter s) {
                content{}.chapter(
                    view_model.model_data.the_books.at(s.book_id).chapters.at(
                        s.chapter_id));
                menu{"card"}.add().remove().start_training().quit();
            },
            [](view::model::states::add_card_front) {
                println("add chard");
                println("front: ");
            },
            [](view::model::states::add_card_back) { println("back: "); },
            [&](view::model::states::training) {
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
