#include <console/persist.hpp>

#include <data/serialize.hpp>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

namespace
{
const auto books_path{fs::path{"xzr_learn_books.txt"}};
}
namespace xzr::learn::persist
{
auto save(const ::xzr::learn::data::data& data) -> void
{
    using oarchive = ::boost::archive::text_oarchive;
    using ::xzr::learn::data::save;
    auto f{std::ofstream{::books_path}};
    auto o{oarchive{f}};
    save(o, data);
}
[[nodiscard]] auto load() -> ::xzr::learn::data::data
{
    using iarchive = ::boost::archive::text_iarchive;
    using ::xzr::learn::data::load;
    auto f{std::ifstream{::books_path}};
    auto i{iarchive{f}};
    return load(i);
}
[[nodiscard]] auto load_or_create_empty_data() -> ::xzr::learn::data::data
{
    if (!fs::exists(::books_path))
        save(::xzr::learn::data::data{});
    return load();
}
}
