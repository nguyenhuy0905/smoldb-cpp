#ifndef SMOLDB_CPP_TABLE_HXX
#define SMOLDB_CPP_TABLE_HXX

#include "row.hxx"
#include <string>
#include <vector>

namespace smoldb {

class Table {
  public:
  private:
    Table(std::string_view name) : m_name(name) {}
    Table(std::string_view name, std::size_t n_rows) : m_name(name) {
        m_rows.reserve(n_rows);
    }
    std::vector<Row> m_rows;
    std::string m_name;
    std::size_t m_size{0};

    friend class TableBuilder;
};

} // namespace smoldb

#endif // !SMOLDB_CPP_TABLE_HXX
