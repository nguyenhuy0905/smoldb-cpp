#ifndef SMOLDB_CPP_DBFILE_HXX
#define SMOLDB_CPP_DBFILE_HXX

#include <expected>
#include <filesystem>
#include <fstream>

namespace smoldb {

/**
 * @class DbFile
 * @brief a wrapper around an fstream of a database file.
 *
 */
class DbFile {
  public:
    enum class CreateFileError : uint8_t {
        FileExists,
        InvalidPath,
    };

    enum class ReadFileError : uint8_t {
        FileNotExists,
        InvalidPath,
        FileFormatError,
    };

    DbFile() = delete;

  private:
    explicit DbFile(const std::filesystem::path& path) : m_handle(path) {}
    /**
     * @brief Database file handler.
     */
    std::fstream m_handle;
};

}; // namespace smoldb

#endif // !SMOLDB_CPP_DBFILE_HXX
