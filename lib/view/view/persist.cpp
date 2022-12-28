#include <view/persist.hpp>

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
auto save(const data::data& data) -> void
{
    using oarchive = ::boost::archive::text_oarchive;
    using data::save;
    auto f{std::ofstream{::books_path}};
    auto o{oarchive{f}};
    save(o, data);
}
[[nodiscard]] auto load() -> data::data
{
    using iarchive = ::boost::archive::text_iarchive;
    using ::xzr::learn::data::load;
    auto f{std::ifstream{::books_path}};
    auto i{iarchive{f}};
    return load(i);
}
[[nodiscard]] auto load_or_create_empty_data() -> data::data
{
    if (!fs::exists(::books_path))
        save(data::data{});
    return load();
}
}
