#include <data/update.hpp>

#include <data/action.hpp>

#include <boost/hof/match.hpp>

#include <variant>

namespace
{
using books = ::xzr::learn::data::books;
using book = ::xzr::learn::data::book;

auto add_book_to(books& bs, const std::string& name)
{
    bs.push_back(book{.name = name});
}
}
namespace xzr::learn::data
{
inline namespace v1
{
auto update(app app, action act) -> data::app
{
    std::visit(boost::hof::match(
                   [&](add_book a) { add_book_to(app.the_books, a.name); },
                   [](auto a) {}),
               act);

    return app;
}
}
}
