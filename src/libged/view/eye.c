/*                         E Y E . C
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
/** @file libged/eye.c
 *
 * The eye command.
 *
 */

#include "common.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "../ged_private.h"
#include "./ged_view.h"


int
ged_eye_core(struct ged *gedp, int argc, const char *argv[])
{
    point_t eye_model;
    double scan[3];
    vect_t xlate;
    vect_t new_cent;
    static const char *usage = "x y z";

    GED_CHECK_VIEW(gedp, BRLCAD_ERROR);
    GED_CHECK_ARGC_GT_0(gedp, argc, BRLCAD_ERROR);

    /* initialize result */
    bu_vls_trunc(gedp->ged_result_str, 0);

    /* get eye */
    if (argc == 1) {
	point_t eye;

	/* calculate eye point */
	VSET(xlate, 0.0, 0.0, 1.0);
	MAT4X3PNT(eye, gedp->ged_gvp->gv_view2model, xlate);
	if (gedp->dbip)
	    VSCALE(eye, eye, gedp->dbip->dbi_base2local);

	bn_encode_vect(gedp->ged_result_str, eye, 1);
	return BRLCAD_OK;
    }

    if (argc != 2 && argc != 4) {
	bu_vls_printf(gedp->ged_result_str, "Usage: %s %s", argv[0], usage);
	return BRLCAD_ERROR;
    }

    if (argc == 2) {
	if (bn_decode_vect(eye_model, argv[1]) != 3) {
	    bu_vls_printf(gedp->ged_result_str, "Usage: %s %s", argv[0], usage);
	    return BRLCAD_ERROR;
	}
    } else {
	if (sscanf(argv[1], "%lf", &scan[X]) < 1) {
	    bu_vls_printf(gedp->ged_result_str, "ged_eye: bad X value %s\n", argv[1]);
	    return BRLCAD_ERROR;
	}

	if (sscanf(argv[2], "%lf", &scan[Y]) < 1) {
	    bu_vls_printf(gedp->ged_result_str, "ged_eye: bad Y value %s\n", argv[2]);
	    return BRLCAD_ERROR;
	}

	if (sscanf(argv[3], "%lf", &scan[Z]) < 1) {
	    bu_vls_printf(gedp->ged_result_str, "ged_eye: bad Z value %s\n", argv[3]);
	    return BRLCAD_ERROR;
	}

	/* convert from double to fastf_t */
	VMOVE(eye_model, scan);
    }

    if (gedp->dbip)
	VSCALE(eye_model, eye_model, gedp->dbip->dbi_local2base);

    /* First step:  put eye at view center (view 0, 0, 0) */
    MAT_DELTAS_VEC_NEG(gedp->ged_gvp->gv_center, eye_model);
    bv_update(gedp->ged_gvp);

    /* Second step:  put eye at view 0, 0, 1.
     * For eye to be at 0, 0, 1, the old 0, 0, -1 needs to become 0, 0, 0.
     */
    VSET(xlate, 0.0, 0.0, -1.0);	/* correction factor */
    MAT4X3PNT(new_cent, gedp->ged_gvp->gv_view2model, xlate);
    MAT_DELTAS_VEC_NEG(gedp->ged_gvp->gv_center, new_cent);
    bv_update(gedp->ged_gvp);

    return BRLCAD_OK;
}


/*
 * Local Variables:
 * tab-width: 8
 * mode: C
 * indent-tabs-mode: t
 * c-file-style: "stroustrup"
 * End:
 * ex: shiftwidth=4 tabstop=8
 */
