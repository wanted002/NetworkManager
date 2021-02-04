/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2015 Red Hat, Inc.
 */

/* With autotools (and also meson), all our source files are expected to include <config.h>.
 * as very first header. Then they are expected to include "config-extra.h" and a small set
 * of headers that we always want to have included (depending on what sources we compile
 * (as determined by NETWORKMANAGER_COMPILATION define) that can be  "nm-glib-aux/nm-macros-internal.h"
 * and similar.
 *
 * To simplify that, all our source files are only expected to include "nm-default.h" as first,
 * and thereby rely on getting a basic set of headers already.
 */

#ifndef __NM_DEFAULT_1_H__
#define __NM_DEFAULT_1_H__

#include "nm-default-0.h"

#undef NETWORKMANAGER_COMPILATION
#define NETWORKMANAGER_COMPILATION 1

/*****************************************************************************/

#include <glib.h>

#if (NETWORKMANAGER_COMPILATION) & NM_NETWORKMANAGER_COMPILATION_WITH_GLIB_I18N_PROG
    #if (NETWORKMANAGER_COMPILATION) & NM_NETWORKMANAGER_COMPILATION_WITH_GLIB_I18N_LIB
        #error Cannot define NM_NETWORKMANAGER_COMPILATION_WITH_GLIB_I18N_PROG and NM_NETWORKMANAGER_COMPILATION_WITH_GLIB_I18N_LIB
    #endif
    #include <glib/gi18n.h>
#elif (NETWORKMANAGER_COMPILATION) & NM_NETWORKMANAGER_COMPILATION_WITH_GLIB_I18N_LIB
    #include <glib/gi18n-lib.h>
#endif

/*****************************************************************************/

#include "nm-gassert-patch.h"

#include "nm-std-aux/nm-std-aux.h"
#include "nm-std-aux/nm-std-utils.h"
#include "nm-glib-aux/nm-macros-internal.h"
#include "nm-glib-aux/nm-shared-utils.h"
#include "nm-glib-aux/nm-errno.h"
#include "nm-glib-aux/nm-hash-utils.h"

/*****************************************************************************/

#if (NETWORKMANAGER_COMPILATION) & NM_NETWORKMANAGER_COMPILATION_WITH_LIBNM_CORE
    #include "nm-version.h"
#endif

/*****************************************************************************/

#if (NETWORKMANAGER_COMPILATION) & NM_NETWORKMANAGER_COMPILATION_WITH_DAEMON
    #include "nm-core-types.h"
    #include "nm-types.h"
    #include "nm-log-core/nm-logging.h"
#endif

#if (NETWORKMANAGER_COMPILATION) & NM_NETWORKMANAGER_COMPILATION_WITH_LIBNM_PRIVATE
    #include "nm-libnm-utils.h"
#endif

#if ((NETWORKMANAGER_COMPILATION) &NM_NETWORKMANAGER_COMPILATION_WITH_LIBNM) \
    && !((NETWORKMANAGER_COMPILATION)                                        \
         & (NM_NETWORKMANAGER_COMPILATION_WITH_LIBNM_PRIVATE                 \
            | NM_NETWORKMANAGER_COMPILATION_WITH_LIBNM_CORE_INTERNAL))
    #include "NetworkManager.h"
#endif

/*****************************************************************************/

#endif /* __NM_DEFAULT_1_H__ */
