#include <data/data.hpp>
#include <data/ostream.hpp>
#include <view/data.hpp>

#include <boost/test/unit_test.hpp>

#include <initializer_list>
#include <string>

namespace
{
using namespace std::string_literals;
namespace view_actions = ::xzr::learn::view::actions;
namespace view_states = ::xzr::learn::view::states;
using ::xzr::learn::view::update;
using view_data = ::xzr::learn::view::data;
using model_data = ::xzr::learn::data::data;
BOOST_AUTO_TEST_SUITE(view_tests)
BOOST_AUTO_TEST_SUITE(data_tests)
BOOST_AUTO_TEST_CASE(update_model_with_view_actions)
{
    auto one_book{update(view_data{.view_state = view_states::books{}},
                         view_actions::add{})};
    one_book = update(one_book, view_actions::text_input{.txt = "book one"});
    BOOST_REQUIRE(one_book.model_data.the_books.size() == 1);
    BOOST_TEST(one_book.model_data.the_books.at(0).name == "book one");
    {
        auto one_chapter{update(one_book, view_actions::select{.id = 0})};
        one_chapter = update(one_chapter, view_actions::add{});
        one_chapter =
            update(one_chapter, view_actions::text_input{.txt = "chapter one"});
        BOOST_REQUIRE(one_chapter.model_data.the_books.at(0).chapters.size() ==
                      1);
        {
            auto one_card{update(one_chapter, view_actions::select{.id = 0})};
            one_card = update(one_card, view_actions::add{});
            one_card =
                update(one_card, view_actions::text_input{.txt = "card front"});
            one_card =
                update(one_card, view_actions::text_input{.txt = "card back"});
            BOOST_REQUIRE(one_card.model_data.the_books.at(0)
                              .chapters.at(0)
                              .cards.size() == 1);
            BOOST_TEST(one_card.model_data.the_books.at(0)
                           .chapters.at(0)
                           .cards.at(0)
                           .front == "card front");
            BOOST_TEST(one_card.model_data.the_books.at(0)
                           .chapters.at(0)
                           .cards.at(0)
                           .back == "card back");
            one_card = update(one_card, view_actions::remove{.id = 0});
            BOOST_TEST(one_chapter.model_data.the_books.at(0)
                           .chapters.at(0)
                           .cards.empty());
            {
                auto on_training{
                    update(one_chapter, view_actions::select{.id = 0})};
                for (const auto& txt : {"a"s, "b"s, "c"s})
                {
                    on_training = update(on_training, view_actions::add{});
                    on_training = update(on_training,
                                         view_actions::text_input{.txt = txt});
                    on_training = update(on_training,
                                         view_actions::text_input{.txt = txt});
                }
                const auto training_cards{on_training.model_data.the_books.at(0)
                                              .chapters.at(0)
                                              .cards};
                on_training =
                    update(on_training, view_actions::start_training{});
                BOOST_REQUIRE(
                    !on_training.model_data.the_training.cards.empty());
                BOOST_REQUIRE_EQUAL(
                    on_training.model_data.the_training.cards.size(),
                    training_cards.size());
                for (int card_idx{}; card_idx < training_cards.size();
                     ++card_idx)
                {
                    const auto card{training_cards.at(card_idx)};
                    for (int i{}; i < training_cards.size(); ++i)
                    {
                        on_training =
                            update(on_training,
                                   view_actions::text_input{.txt = card.back});
                    }
                    BOOST_CHECK_EQUAL(
                        on_training.model_data.the_training.cards.size(),
                        training_cards.size() - (card_idx + 1));
                }
                BOOST_TEST(on_training.model_data.the_training.cards.empty());
            }
            one_chapter = update(one_chapter, view_actions::remove{.id = 0});
            BOOST_TEST(one_chapter.model_data.the_books.at(0).chapters.empty());
        }
        one_book = update(one_book, view_actions::remove{.id = 0});
        BOOST_TEST(one_book.model_data.the_books.empty());
    }
}
BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
}
