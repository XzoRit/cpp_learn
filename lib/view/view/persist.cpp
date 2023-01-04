#include <view/persist.hpp>

#include <view/serialize.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace
{
const auto books_path{fs::path{"xzr_learn_books.xml"}};
}
namespace xzr::learn::persist
{
auto save(const view::data& data) -> void
{
    using oarchive = ::boost::archive::xml_oarchive;
    auto f{std::ofstream{::books_path}};
    auto o{oarchive{f}};
    o << ::boost::serialization::make_nvp("data", data);
}
[[nodiscard]] auto load() -> view::data
{
    using iarchive = ::boost::archive::xml_iarchive;
    auto f{std::ifstream{::books_path}};
    auto i{iarchive{f}};
    view::data d{};
    i >> ::boost::serialization::make_nvp("data", d);
    return d;
}
[[nodiscard]] auto load_or_create_empty_data() -> view::data
{
    if (!fs::exists(::books_path))
        save(view::data{});
    return load();
}
}
