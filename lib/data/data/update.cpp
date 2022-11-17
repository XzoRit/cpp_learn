#include "data/data.hpp"
#include <data/update.hpp>

#include <data/action.hpp>

#include <boost/hof/match.hpp>

#include <variant>

using books = ::xzr::learn::data::books;
using book = ::xzr::learn::data::book;

namespace
{
auto add_book_to(books& bs, const std::string& name)
{
    bs.push_back(book{.name = name});
}
}
namespace xzr::learn::update
{
inline namespace v1
{
auto update(data::app app, data::action act) -> data::app
{
    using namespace data;

    std::visit(boost::hof::match(
                   [&](add_book a) { add_book_to(app.the_books, a.name); },
                   [](auto a) {}),
               act);

    return app;
}
}
}
