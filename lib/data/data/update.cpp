#include <data/data.hpp>
#include <data/update.hpp>

#include <data/action.hpp>

#include <boost/hof/match.hpp>

#include <variant>

namespace
{
using books = ::xzr::learn::data::books;
using book = ::xzr::learn::data::book;
using chapter = ::xzr::learn::data::chapter;

auto add_book_to(books& bs, const std::string& name)
{
    bs.push_back(book{.name = name});
}
auto remove_book_from(books& bs, int id)
{
    if (bs.empty())
        return;
    if (id >= bs.size())
        return;
    bs.erase(std::next(bs.cbegin(), id));
}
auto add_chapter_to_book(book& b, const std::string& name)
{
    b.chapters.push_back(chapter{.name = name});
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
            [&](add_chapter a) {
                add_chapter_to_book(app.the_books.at(a.book_id), a.name);
            },
            [](auto a) {}),
        act);

    return app;
}
}
}
