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

#ifndef __NM_DEFAULT_2049_H__
#define __NM_DEFAULT_2049_H__

#include "nm-networkmanager-compilation.h"

#ifdef NETWORKMANAGER_COMPILATION
    #error Dont define NETWORKMANAGER_COMPILATION
#endif

#ifndef G_LOG_DOMAIN
    #error Define G_LOG_DOMAIN
#endif

/*****************************************************************************/

#define NETWORKMANAGER_COMPILATION 2049

/*****************************************************************************/

/* always include these headers for our internal source files. */

#ifndef ___CONFIG_H__
    #define ___CONFIG_H__
    #include <config.h>
#endif

#include "config-extra.h"

/* for internal compilation we don't want the deprecation macros
 * to be in effect. Define the widest range of versions to effectively
 * disable deprecation checks */
#define NM_VERSION_MIN_REQUIRED NM_VERSION_0_9_8

#ifndef NM_MORE_ASSERTS
    #define NM_MORE_ASSERTS 0
#endif

#if NM_MORE_ASSERTS == 0
    /* The cast macros like NM_TYPE() are implemented via G_TYPE_CHECK_INSTANCE_CAST()
 * and _G_TYPE_CIC(). The latter, by default performs runtime checks of the type
 * by calling g_type_check_instance_cast().
 * This check has a certain overhead without being helpful.
 *
 * Example 1:
 *     static void foo (NMType *obj)
 *     {
 *         access_obj_without_check (obj);
 *     }
 *     foo ((NMType *) obj);
 *     // There is no runtime check and passing an invalid pointer
 *     // leads to a crash.
 *
 * Example 2:
 *     static void foo (NMType *obj)
 *     {
 *         access_obj_without_check (obj);
 *     }
 *     foo (NM_TYPE (obj));
 *     // There is a runtime check which prints a g_warning(), but that doesn't
 *     // avoid the crash as NM_TYPE() cannot do anything then passing on the
 *     // invalid pointer.
 *
 * Example 3:
 *     static void foo (NMType *obj)
 *     {
 *         g_return_if_fail (NM_IS_TYPE (obj));
 *         access_obj_without_check (obj);
 *     }
 *     foo ((NMType *) obj);
 *     // There is a runtime check which prints a g_critical() which also avoids
 *     // the crash. That is actually helpful to catch bugs and avoid crashes.
 *
 * Example 4:
 *     static void foo (NMType *obj)
 *     {
 *         g_return_if_fail (NM_IS_TYPE (obj));
 *         access_obj_without_check (obj);
 *     }
 *     foo (NM_TYPE (obj));
 *     // The runtime check is performed twice, with printing a g_warning() and
 *     // a g_critical() and avoiding the crash.
 *
 * Example 3 is how it should be done. Type checks in NM_TYPE() are pointless.
 * Disable them for our production builds.
 */
    #ifndef G_DISABLE_CAST_CHECKS
        #define G_DISABLE_CAST_CHECKS
    #endif
#endif

#if NM_MORE_ASSERTS == 0
    #ifndef G_DISABLE_CAST_CHECKS
        /* Unless compiling with G_DISABLE_CAST_CHECKS, glib performs type checking
 * during G_VARIANT_TYPE() via g_variant_type_checked_(). This is not necessary
 * because commonly this cast is needed during something like
 *
 *   g_variant_builder_init (&props, G_VARIANT_TYPE ("a{sv}"));
 *
 * Note that in if the variant type would be invalid, the check still
 * wouldn't make the buggy code magically work. Instead of passing a
 * bogus type string (bad), it would pass %NULL to g_variant_builder_init()
 * (also bad).
 *
 * Also, a function like g_variant_builder_init() already validates
 * the input type via something like
 *
 *   g_return_if_fail (g_variant_type_is_container (type));
 *
 * So, by having G_VARIANT_TYPE() also validate the type, we validate
 * twice, whereas the first validation is rather pointless because it
 * doesn't prevent the function to be called with invalid arguments.
 *
 * Just patch G_VARIANT_TYPE() to perform no check.
 */
        #undef G_VARIANT_TYPE
        #define G_VARIANT_TYPE(type_string) ((const GVariantType *) (type_string))
    #endif
#endif

#include <stdlib.h>

/*****************************************************************************/

#if (NETWORKMANAGER_COMPILATION) & NM_NETWORKMANAGER_COMPILATION_WITH_GLIB

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

#endif /* NM_NETWORKMANAGER_COMPILATION_WITH_GLIB */

/*****************************************************************************/

#endif /* __NM_DEFAULT_2049_H__ */
