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
using chapters = ::xzr::learn::data::chapters;

auto erase_id(auto& container, int id)
{
    using std::cbegin;

    if (container.empty())
        return;
    if (id >= container.size())
        return;
    container.erase(std::next(cbegin(container), id));
}
auto add_book_to(books& bs, const std::string& name)
{
    bs.push_back(book{.name = name});
}
auto remove_book_from(books& bs, int id)
{
    ::erase_id(bs, id);
}
auto add_chapter_to_book(book& b, const std::string& name)
{
    b.chapters.push_back(chapter{.name = name});
}
auto remove_chapter_from_book(chapters& cs, int id)
{
    ::erase_id(cs, id);
}
}
namespace xzr::learn::data
{
inline namespace v1
{
auto update(app app, action act) -> data::app
{
    using boost::hof::match;

    std::visit(
        match([&](add_book a) { ::add_book_to(app.the_books, a.name); },
              [&](remove_book a) { ::remove_book_from(app.the_books, a.id); },
              [&](add_chapter a) {
                  ::add_chapter_to_book(app.the_books.at(a.book_id), a.name);
              },
              [&](remove_chapter a) {
                  ::remove_chapter_from_book(
                      app.the_books.at(a.book_id).chapters,
                      a.id);
              },
              [](auto a) {}),
        act);

    return app;
}
}
}
