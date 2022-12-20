#include <data/data.hpp>

#include <boost/hof/match.hpp>

namespace xzr::learn::data
{
auto update(app app, actions::action act) -> data::app
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
