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
auto remove_book_from(books& bs, int id)
{
    bs.erase(std::next(bs.cbegin(), id));
}
}
namespace xzr::learn::data
{
inline namespace v1
{
auto update(app app, action act) -> data::app
{
    std::visit(
        boost::hof::match(
            [&](add_book a) { add_book_to(app.the_books, a.name); },
            [&](remove_book a) { remove_book_from(app.the_books, a.id); },
            [](auto a) {}),
        act);

    return app;
}
}
}
