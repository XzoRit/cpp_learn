#include <data/books.hpp>
#include <data/training.hpp>
#include <data/update.hpp>

#include <data/action.hpp>

#include <boost/hof/match.hpp>

#include <variant>

namespace
{
using books = ::xzr::learn::data::books::books;
using book = ::xzr::learn::data::books::book;
using chapter = ::xzr::learn::data::books::chapter;
using chapters = ::xzr::learn::data::books::chapters;
using card = ::xzr::learn::data::books::card;
using cards = ::xzr::learn::data::books::cards;

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
auto add_chapter_to(chapters& cs, const std::string& name)
{
    cs.push_back(chapter{.name = name});
}
auto remove_chapter_from(chapters& cs, int id)
{
    ::erase_id(cs, id);
}
auto add_card_to(cards& cs, const std::string& front, const std::string& back)
{
    cs.push_back(card{.front = front, .back = back});
}
auto remove_card_from(cards& cs, int id)
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
        match(
            [&](add_book a) { ::add_book_to(app.the_books, a.name); },
            [&](remove_book a) { ::remove_book_from(app.the_books, a.id); },
            [&](add_chapter a) {
                ::add_chapter_to(app.the_books.at(a.book_id).chapters, a.name);
            },
            [&](remove_chapter a) {
                ::remove_chapter_from(app.the_books.at(a.book_id).chapters,
                                      a.id);
            },
            [&](add_card a) {
                ::add_card_to(
                    app.the_books.at(a.book_id).chapters.at(a.chapter_id).cards,
                    a.front,
                    a.back);
            },
            [&](remove_card a) {
                ::remove_card_from(
                    app.the_books.at(a.book_id).chapters.at(a.chapter_id).cards,
                    a.id);
            },
            [](auto) {}),
        act);

    return app;
}
}
}
