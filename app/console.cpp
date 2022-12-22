#include "console.hpp"

#include "commands.hpp"
#include "model.hpp"
#include "persist.hpp"
#include "view.hpp"

#include <data/books.hpp>
#include <data/data.hpp>
#include <data/training.hpp>

#include <boost/hof/match.hpp>

#include <algorithm>
#include <iterator>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <variant>

using ::xzr::learn::console::view::content;
using ::xzr::learn::console::view::menu;
using ::xzr::learn::console::view::print;
using ::xzr::learn::console::view::println;
using ::xzr::learn::console::view::readln;
namespace xzr::learn::console
{
auto run() -> void
{
    auto data{::xzr::learn::persist::load_or_create_empty_data()};
    auto console_data{::xzr::learn::console::model::data{
        .data_act = std::nullopt,
        .console_state = ::xzr::learn::console::model::states::books{}}};
    for (;;)
    {
        ::xzr::learn::console::view::draw(data, console_data.console_state);
        const auto act{
            ::xzr::learn::console::view::intent(console_data.console_state,
                                                readln())};
        console_data =
            ::xzr::learn::console::model::update(act,
                                                 console_data.console_state,
                                                 data);
        if (const auto data_act{console_data.data_act})
        {
            data = data::update(std::move(data), data_act.value());
            ::xzr::learn::persist::save(data);
        }
        if (std::holds_alternative<::xzr::learn::console::model::actions::exit>(
                act))
            break;
    }
}
}
