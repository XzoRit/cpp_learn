#include <console/console.hpp>

#include <console/commands.hpp>
#include <console/model.hpp>
#include <console/persist.hpp>
#include <console/view.hpp>

#include <data/data.hpp>

#include <optional>
#include <variant>

namespace xzr::learn::console
{
using persist::load_or_create_empty_data;
using persist::save;
auto run() -> void
{
    auto view_model_data{
        view::model::data{.view_state = view::model::states::books{},
                          .model_data = load_or_create_empty_data()}};
    for (;;)
    {
        view::draw(view_model_data);
        const auto view_model_act{view::intent(view::readln())};
        view_model_data = view::model::update(view_model_data, view_model_act);
        save(view_model_data.model_data);
        if (std::holds_alternative<view::model::actions::exit>(
                view_model_act) &&
            std::holds_alternative<view::model::states::books>(
                view_model_data.view_state))
            break;
    }
}
}
