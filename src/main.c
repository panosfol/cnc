#include <stdbool.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#include "config.h"
#include "db/postgres.h"
#include "log.h"

extern config_t *yaml_config;
extern bool verbose;

static struct option options[] = {
	{ "config-file", required_argument, 0, 'f' },
	{ "verbose", no_argument, 0, 'v' },
};

int main(int argc, char **argv)
{
	int ret = 0;
	int c;
	char *config_file = NULL;

	while ((c = getopt_long(argc, argv, "f:v", options, NULL)) != -1) {
		switch (c) {
		case 'f':
			config_file = strdup(optarg);
			printf("%s\n", config_file);
			break;
		case 'v':
			verbose = true;
			break;
		}
	}

	if (config_file != NULL) {
		ret = initialize_config(config_file);
		if (ret != 0) {
			free((void *)config_file);
			return ret;
		}
	}
	ret = pg_connect();

	free((void *)config_file);
	free_config();

	return ret;
}
