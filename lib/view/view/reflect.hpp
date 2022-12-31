#pragma once

#include <view/data.hpp>

#include <boost/describe/class.hpp>

namespace xzr::learn::view::states
{
BOOST_DESCRIBE_STRUCT(books, (), ())
BOOST_DESCRIBE_STRUCT(add_book, (), ())
BOOST_DESCRIBE_STRUCT(book, (), (book_id))
BOOST_DESCRIBE_STRUCT(add_chapter, (), (book_id))
BOOST_DESCRIBE_STRUCT(chapter, (), (book_id, chapter_id))
BOOST_DESCRIBE_STRUCT(add_card_front, (), (book_id, chapter_id))
BOOST_DESCRIBE_STRUCT(add_card_back, (), (book_id, chapter_id, front_txt))
BOOST_DESCRIBE_STRUCT(training, (), ())
}
namespace xzr::learn::view
{
BOOST_DESCRIBE_STRUCT(data, (), (view_state, model_data))
};
