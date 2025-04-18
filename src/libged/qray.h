/*                          Q R A Y . H
 * BRL-CAD
 *
 * Copyright (c) 2007-2025 United States Government as represented by
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
/** @addtogroup libged */
/** @{ */
/** @file libged/qray.h
 *
 * Private header file for "Query Ray" variables.
 *
 */
/** @} */

#include "ged.h"
#include "./ged_private.h"

#ifndef LIBGED_QRAY_H
#define LIBGED_QRAY_H

__BEGIN_DECLS

#define DG_QRAY_TEXT(_dgop) ((_dgop)->gd_qray_effects == 't' || (_dgop)->gd_qray_effects == 'b')
#define DG_QRAY_GRAPHICS(_dgop) ((_dgop)->gd_qray_effects == 'g' || (_dgop)->gd_qray_effects == 'b')

#define DG_QRAY_BASENAME "query_ray"
#define DG_QRAY_BOTH ((_dgop)->gd_qray_effects == 'b')

GED_EXPORT extern struct ged_qray_color def_qray_odd_color;
GED_EXPORT extern struct ged_qray_color def_qray_even_color;
GED_EXPORT extern struct ged_qray_color def_qray_void_color;
GED_EXPORT extern struct ged_qray_color def_qray_overlap_color;

struct qray_fmt_data {
    char type;
    const char *fmt;
};

GED_EXPORT extern struct qray_fmt_data def_qray_fmt_data[];

struct qray_dataList {
    struct bu_list l;
    fastf_t x_in;
    fastf_t y_in;
    fastf_t z_in;
    fastf_t los;
};

/* defined in qray.c */
GED_EXPORT extern void qray_init(struct ged_drawable *gdp);
GED_EXPORT extern void qray_free(struct ged_drawable *gdp);
GED_EXPORT extern void qray_data_to_vlist(struct ged *gdp, struct bv_vlblock *vbp, struct qray_dataList *headp, vect_t dir, int do_overlaps);


__END_DECLS

#endif

/*
 * Local Variables:
 * mode: C
 * tab-width: 8
 * indent-tabs-mode: t
 * c-file-style: "stroustrup"
 * End:
 * ex: shiftwidth=4 tabstop=8
 */
