#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/screen.hpp>
#include <view/data.hpp>
#include <view/persist.hpp>

#include <ftxui/component/component.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include <boost/hof/match.hpp>

#include <algorithm>
#include <iterator>

namespace xzr::learn::ftxui
{
struct app_view : ::ftxui::ComponentBase
{
    app_view(view::data data)
        : view_model{data}
    {
        book_names.reserve(view_model.model_data.the_books.size());
        std::ranges::transform(view_model.model_data.the_books,
                               std::back_inserter(book_names),
                               [](auto b) { return b.name; });
        this->Add(::ftxui::Menu(&book_names, &book_selection));
    }
    auto OnEvent(::ftxui::Event event) -> bool override
    {
        return ::ftxui::ComponentBase::OnEvent(event);
    }
    auto Render() -> ::ftxui::Element override
    {
        return ::ftxui::ComponentBase::Render();
    }
    view::data view_model{};
    int book_selection{};
    std::vector<std::string> book_names{};
};
// auto draw(view::data view_model) -> void
// {
//     using ::boost::hof::match;
//     ::ftxui::Elements names{};
//     names.reserve(view_model.model_data.the_books.size());
//     std::ranges::transform(view_model.model_data.the_books,
//                            std::back_inserter(names),
//                            [](auto b) { return ::ftxui::text(b.name); });
//     ::ftxui::Element vnames{::ftxui::vbox(names)};
//     auto screen{::ftxui::Screen::Create(::ftxui::Dimension::Full())};
//     ::ftxui::Render(screen, ::ftxui::window(::ftxui::text("books"), vnames));
//     screen.Print();
//     // std::visit(
//     //     match([&](view::states::books) {},
//     // [](view::states::add_book) {
//     // },
//     // [&](view::states::book s) {},
//     // [](view::states::add_chapter) {},
//     // [&](view::states::chapter s) {},
//     // [](view::states::add_card_front) {},
//     // [](view::states::add_card_back) {},
//     // [&](view::states::training) {
//     //     std::visit(
//     //         match([](data::training::states::show_card s) {},
//     //               [](data::training::states::done) {}),
//     //         view_model.model_data.the_training.state);
//     // },
//     //       [](auto) { return ::ftxui::text("Hello text"); }),
//     // view_model.view_state));
// }
auto intent(auto ui_event) -> ::xzr::learn::view::actions::action
{
    return {};
}
auto event() -> ::ftxui::Event
{
    return {};
}
}
auto main(int argc, const char* argv[]) -> int
{
    auto view_model_data{::xzr::learn::view::data{
        ::xzr::learn::persist::load_or_create_empty_data()}};
    ::ftxui::Component app{
        ::ftxui::Make<::xzr::learn::ftxui::app_view>(view_model_data)};
    auto screen{::ftxui::ScreenInteractive::TerminalOutput()};
    screen.Loop(app);
    // ::xzr::learn::ftxui::draw(view_model_data);
    // const auto view_model_act{
    //     ::xzr::learn::ftxui::intent(::xzr::learn::ftxui::event())};
    // view_model_data =
    //     ::xzr::learn::view::update(view_model_data, view_model_act);
    // ::xzr::learn::persist::save(view_model_data);
    // if (std::holds_alternative<::xzr::learn::view::actions::exit>(
    //         view_model_act) &&
    //     std::holds_alternative<::xzr::learn::view::states::books>(
    //         view_model_data.view_state))
    //     break;
    return 0;
}
