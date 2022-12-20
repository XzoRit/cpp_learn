#pragma once

#include <string>
#include <variant>
#include <vector>

namespace xzr::learn::data::books::actions
{
struct add_book
{
    std::string name{};
};
struct remove_book
{
    int id{};
};
struct add_chapter
{
    int book_id{};
    std::string name{};
};
struct remove_chapter
{
    int book_id{};
    int id{};
};
struct add_card
{
    int book_id{};
    int chapter_id{};
    std::string front{};
    std::string back{};
};
struct remove_card
{
    int book_id{};
    int chapter_id{};
    int id{};
};
using action = std::variant<add_book,
                            remove_book,
                            add_chapter,
                            remove_chapter,
                            add_card,
                            remove_card>;
}
namespace xzr::learn::data::books
{
template <class A>
struct container : public std::vector<A>
{
  public:
    using std::vector<A>::vector;

    [[nodiscard]] auto base() const -> const std::vector<A>&
    {
        return *this;
    }

    [[nodiscard]] auto base() -> std::vector<A>&
    {
        return *this;
    }
};
struct card
{
    std::string front{};
    std::string back{};

    [[nodiscard]] auto operator<=>(const card&) const = default;
};
using cards = container<card>;
struct chapter
{
    std::string name{};
    cards cards{};

    [[nodiscard]] auto operator<=>(const chapter&) const = default;
};
using chapters = container<chapter>;
struct book
{
    std::string name{};
    chapters chapters{};

    [[nodiscard]] auto operator<=>(const book&) const = default;
};
using books = container<book>;
}
namespace xzr::learn::data::books
{
auto update(books bs, actions::action act) -> books;
}
