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
    auto data{load_or_create_empty_data()};
    auto view_model_data{
        view::model::data{.data_act = std::nullopt,
                          .view_state = view::model::states::books{}}};
    for (;;)
    {
        view::draw(data, view_model_data.view_state);
        const auto view_model_act{
            view::intent(view_model_data.view_state, view::readln())};
        view_model_data = view::model::update(view_model_act,
                                              view_model_data.view_state,
                                              data);
        if (const auto data_act{view_model_data.data_act})
        {
            data = data::update(data, data_act.value());
            save(data);
        }
        if (std::holds_alternative<view::model::actions::exit>(view_model_act))
            break;
    }
}
}
