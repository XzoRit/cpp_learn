#include <data/update.hpp>

#include <data/books.hpp>

#include <variant>

namespace xzr::learn::data
{
inline namespace v1
{
auto update(app app, books::actions::action act) -> data::app
{
    app.the_books = books::update(app.the_books, act);
    return app;
}
}
}
