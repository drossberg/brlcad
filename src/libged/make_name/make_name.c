/*                        M A K E _ N A M E . C
 * BRL-CAD
 *
 * Copyright (c) 2008-2025 United States Government as represented by
 * the U.S. Army Research Laboratory.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this file; see the file named COPYING for more
 * information.
 */
/** @file libged/make_name.c
 *
 * The make_name command.
 *
 */

#include "common.h"
#include <string.h>
#include "ged.h"

int
ged_make_name_core(struct ged *gedp, int argc, const char *argv[])
{
    struct bu_vls obj_name = BU_VLS_INIT_ZERO;
    char *cp, *tp;
    static int i = 0;
    int new_i;
    int len;
    static const char *usage = "template | -s [num]";

    GED_CHECK_DATABASE_OPEN(gedp, BRLCAD_ERROR);
    GED_CHECK_READ_ONLY(gedp, BRLCAD_ERROR);
    GED_CHECK_ARGC_GT_0(gedp, argc, BRLCAD_ERROR);

    /* initialize result */
    bu_vls_trunc(gedp->ged_result_str, 0);

    /* must be wanting help */
    if (argc == 1) {
	bu_vls_printf(gedp->ged_result_str, "Usage: %s %s", argv[0], usage);
	return GED_HELP;
    }

    switch (argc) {
	case 2:
	    if (!BU_STR_EQUAL(argv[1], "-s"))
		break;

	    i = 0;
	    return BRLCAD_OK;

	case 3:

	    if ((BU_STR_EQUAL(argv[1], "-s"))
		&& (sscanf(argv[2], "%d", &new_i) == 1)) {
		i = new_i;
		return BRLCAD_OK;
	    }
	    bu_vls_printf(gedp->ged_result_str, "Usage: %s %s", argv[0], usage);
	    return BRLCAD_ERROR;

	default:
	    bu_vls_printf(gedp->ged_result_str, "Usage: %s %s", argv[0], usage);
	    return BRLCAD_ERROR;
    }

    for (cp = (char *)argv[1], len = 0; *cp != '\0'; ++cp, ++len) {
	if (*cp == '@') {
	    if (*(cp + 1) == '@')
		++cp;
	    else
		break;
	}
	bu_vls_putc(&obj_name, *cp);
    }
    bu_vls_putc(&obj_name, '\0');
    tp = (*cp == '\0') ? "" : cp + 1;

    do {
	bu_vls_trunc(&obj_name, len);
	bu_vls_printf(&obj_name, "%d", i++);
	bu_vls_strcat(&obj_name, tp);
    }
    while (db_lookup(gedp->dbip, bu_vls_addr(&obj_name), LOOKUP_QUIET) != RT_DIR_NULL);

    bu_vls_printf(gedp->ged_result_str, "%s", bu_vls_addr(&obj_name));
    bu_vls_free(&obj_name);

    return BRLCAD_OK;
}


#ifdef GED_PLUGIN
#include "../include/plugin.h"
struct ged_cmd_impl make_name_cmd_impl = {
    "make_name",
    ged_make_name_core,
    GED_CMD_DEFAULT
};

const struct ged_cmd make_name_cmd = { &make_name_cmd_impl };
const struct ged_cmd *make_name_cmds[] = { &make_name_cmd, NULL };

static const struct ged_plugin pinfo = { GED_API,  make_name_cmds, 1 };

COMPILER_DLLEXPORT const struct ged_plugin *ged_plugin_info(void)
{
    return &pinfo;
}
#endif /* GED_PLUGIN */

/*
 * Local Variables:
 * mode: C
 * tab-width: 8
 * indent-tabs-mode: t
 * c-file-style: "stroustrup"
 * End:
 * ex: shiftwidth=4 tabstop=8
 */
