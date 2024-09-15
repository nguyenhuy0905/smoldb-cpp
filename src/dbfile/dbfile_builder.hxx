#ifndef SMOLDB_CPP_DBFILE_BUILDER_HXX
#define SMOLDB_CPP_DBFILE_BUILDER_HXX

#include <filesystem>
#include "dbfile.hxx"
namespace smoldb {

/**
 * @class DbFileBuilder
 * @brief Builds a database file.
 *
 */
class DbFileBuilder {
    public:
        DbFileBuilder(const std::filesystem::path& path);
    private:
        DbFile m_build;
};

}

#endif // !SMOLDB_CPP_DBFILE_BUILDER_HXX
