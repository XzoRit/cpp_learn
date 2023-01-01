#pragma once

#include <data/books.hpp>
#include <data/data.hpp>
#include <data/training.hpp>

#include <boost/describe/class.hpp>

namespace xzr::learn::data::books::actions
{
BOOST_DESCRIBE_STRUCT(add_book, (), (name))
BOOST_DESCRIBE_STRUCT(remove_book, (), (id))
BOOST_DESCRIBE_STRUCT(add_chapter, (), (book_id, name))
BOOST_DESCRIBE_STRUCT(remove_chapter, (), (book_id, id))
BOOST_DESCRIBE_STRUCT(add_card, (), (book_id, chapter_id, front, back))
BOOST_DESCRIBE_STRUCT(remove_card, (), (book_id, chapter_id, id))
}
namespace xzr::learn::data::books
{
BOOST_DESCRIBE_STRUCT(card, (), (front, back))
BOOST_DESCRIBE_STRUCT(chapter, (), (name, cards))
BOOST_DESCRIBE_STRUCT(book, (), (name, chapters))
}
namespace xzr::learn::data::training::states
{
BOOST_DESCRIBE_STRUCT(done, (), ())
BOOST_DESCRIBE_STRUCT(show_card, (), (card))
}
namespace xzr::learn::data::training::actions
{
BOOST_DESCRIBE_STRUCT(start, (), (cards))
BOOST_DESCRIBE_STRUCT(answer, (), (txt))
}
namespace xzr::learn::data::training
{
BOOST_DESCRIBE_STRUCT(training, (), (state, cards))
}
namespace xzr::learn::data
{
BOOST_DESCRIBE_STRUCT(data, (), (the_books, the_training))
}
