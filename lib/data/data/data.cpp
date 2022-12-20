#include <data/data.hpp>

#include <boost/hof/match.hpp>

namespace xzr::learn::data
{
auto update(data d, actions::action act) -> data
{
    using ::boost::hof::match;
    std::visit(match(
                   [&](books::actions::action a) {
                       d.the_books = books::update(d.the_books, a);
                   },
                   [&](training::actions::action a) {
                       d.the_training = training::update(d.the_training, a);
                   }),
               act);
    return d;
}
}
