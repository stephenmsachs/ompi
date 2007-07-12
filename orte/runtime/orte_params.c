/*
 * Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2005 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2005 High Performance Computing Center Stuttgart, 
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2007      Cisco, Inc.  All rights reserved.
 * $COPYRIGHT$
 * 
 * Additional copyrights may follow
 * 
 * $HEADER$
 */

#include "orte_config.h"

#include "orte/orte_constants.h"

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include "opal/mca/base/mca_base_param.h"

#include "orte/runtime/runtime.h"
#include "orte/runtime/params.h"

/* globals used by RTE */
bool orte_debug_flag, orte_timing, orte_infrastructure;
bool orte_debug_daemons_flag, orte_debug_daemons_file_flag;
bool orted_spin_flag, orte_no_daemonize_flag;
struct timeval orte_abort_timeout;


/*
 * Whether we have completed orte_init or not
 */
bool orte_initialized = false;

/* whether we have registered params or not */
static bool params_set = false;

int orte_register_params(bool infrastructure)
{
    int value;
    
    if (params_set) {
        return ORTE_SUCCESS;
    }
    
    mca_base_param_reg_int_name("orte", "debug",
                                "Top-level ORTE debug switch",
                                false, false, (int)false, &value);
    orte_debug_flag = OPAL_INT_TO_BOOL(value);
    
    mca_base_param_reg_int_name("orte", "debug_daemons_file",
                                "Whether want stdout/stderr of daemons to go to a file or not",
                                false, false, (int)false, &value);
    orte_debug_daemons_file_flag = OPAL_INT_TO_BOOL(value);

    mca_base_param_reg_int_name("orte", "no_daemonize",
                                "Whether to properly daemonize the ORTE daemons or not",
                                false, false, (int)false, NULL);
    orte_no_daemonize_flag = OPAL_INT_TO_BOOL(value);

    mca_base_param_reg_int_name("orte", "debug_daemons",
                                "Whether to debug the ORTE daemons or not",
                                false, false, (int)false, &value);
    orte_debug_daemons_flag = OPAL_INT_TO_BOOL(value);

    mca_base_param_reg_int_name("orted", "spin",
                                "Have any orteds spin until we can connect a debugger to them",
                                false, false, (int)false, &value);
    orted_spin_flag = OPAL_INT_TO_BOOL(value);
    
    /* save this in a global location as others will need to reference it */
    orte_infrastructure = infrastructure;

    /* check for timing requests */
    mca_base_param_reg_int_name("orte", "timing",
                                "Request that critical timing loops be measured",
                                false, false, (int)false, &value);
    orte_timing = OPAL_INT_TO_BOOL(value);
    
    /* User-level debugger info string */

    mca_base_param_reg_string_name("orte", "base_user_debugger",
                                   "Sequence of user-level debuggers to search for in orterun",
                                   false, false, "totalview @mpirun@ -a @mpirun_args@ : ddt -n @np@ -start @executable@ @executable_argv@ @single_app@ : fxp @mpirun@ -a @mpirun_args@", NULL);


    mca_base_param_reg_int_name("orte", "abort_timeout",
                                "Time to wait [in seconds] before giving up on aborting an ORTE operation",
                                false, false, 10, &value);
    orte_abort_timeout.tv_sec = value;
    orte_abort_timeout.tv_usec = 0;

    /* All done */
    params_set = true;
    return ORTE_SUCCESS;
}

