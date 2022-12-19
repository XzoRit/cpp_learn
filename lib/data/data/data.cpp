#include <data/data.hpp>

#include <boost/hof/match.hpp>

namespace xzr::learn::data
{
inline namespace v1
{
auto update(app app, actions::action act) -> data::app
{
    std::visit([&](auto a) { app.the_books = books::update(app.the_books, a); },
               act);
    return app;
}
}
}
namespace xzr::learn::data
{
namespace v2
{
auto update(v2::app app, actions::action act) -> v2::app
{
    using ::boost::hof::match;

    std::visit(match(
                   [&](books::actions::action a) {
                       app.the_books = books::update(app.the_books, a);
                   },
                   [&](training::actions::action a) {
                       app.the_training = training::update(app.the_training, a);
                   }),
               act);
    return app;
}
}
}
