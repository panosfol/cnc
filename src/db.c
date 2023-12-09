#include "db/db.h"
#include "db/postgres.h"
#include <stddef.h>
#include <stdlib.h>

/* We need to hold info of how many
 * available databases implementations
 * are out there.
 */
struct db_operations **available_dbs;

int execute_db_operations(void)
{
	available_dbs = (struct db_operations **)calloc(
		MAX_AVAILABLE_DBS, sizeof(struct db_operations **));

	section_foreach_entry(my_array, func_ptr_t, entry) {
		entry->func();
	}

	for (int i = 0; i < MAX_AVAILABLE_DBS; i++) {
		/* do db_operations for all available db impl. */
		if (available_dbs[i] != NULL) {
			struct db_t *available_db = available_dbs[i]->db;
			available_dbs[i]->connect(available_db);
			available_dbs[i]->close(available_db);
			available_dbs[i]->replicate(available_db, NULL);
			free(available_db);
		}
	}

	return 0;
}
