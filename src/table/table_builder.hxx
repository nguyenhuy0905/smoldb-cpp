#ifndef SMOLDB_CPP_TABLE_BUILDER_HXX
#define SMOLDB_CPP_TABLE_BUILDER_HXX

#include "module_cfg.h"

#ifndef ENABLE_MODULE
#include "table.hxx"
#include <expected>
#endif // !ENABLE_MODULE

/**
 * `TableBuilder` is not exposed to the end user.
 */

SMOLDB_CPP_EXPORT namespace smoldb {

    /**
     * @class `TableBuilder`
     * @brief The whole purpose of this is to prevent the end-user from playing
     * with the `ColumnMeta` builder alone.
     *
     * Constructing a `ColumnMeta` should be done in the context of a database
     * file. (coming soon)
     */
    class TableBuilder {
      public:
        TableBuilder() = delete;
        // TODO: stop being lazy and provide the arguments needed.
        template <typename... Args> static auto from(Args&&... args) -> Table {
            return Table{ std::forward<Args>(args)... };
        }

      private:
        Table m_build;
    };

} // namespace smoldb

#endif // !SMOLDB_CPP_TABLE_BUILDER_HXX
