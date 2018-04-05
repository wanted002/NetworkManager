/* -*- Mode: C; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* NetworkManager system settings service
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * (C) Copyright 2008 Novell, Inc.
 * (C) Copyright 2008 - 2013 Red Hat, Inc.
 */

#ifndef __NETWORKMANAGER_SETTINGS_CONNECTION_H__
#define __NETWORKMANAGER_SETTINGS_CONNECTION_H__

#include <net/ethernet.h>

#include "nm-dbus-object.h"
#include "nm-connection.h"

#define NM_TYPE_SETTINGS_CONNECTION            (nm_settings_connection_get_type ())
#define NM_SETTINGS_CONNECTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), NM_TYPE_SETTINGS_CONNECTION, NMSettingsConnection))
#define NM_SETTINGS_CONNECTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), NM_TYPE_SETTINGS_CONNECTION, NMSettingsConnectionClass))
#define NM_IS_SETTINGS_CONNECTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), NM_TYPE_SETTINGS_CONNECTION))
#define NM_IS_SETTINGS_CONNECTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), NM_TYPE_SETTINGS_CONNECTION))
#define NM_SETTINGS_CONNECTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), NM_TYPE_SETTINGS_CONNECTION, NMSettingsConnectionClass))

#define NM_SETTINGS_CONNECTION_REMOVED "removed"
#define NM_SETTINGS_CONNECTION_GET_SECRETS "get-secrets"
#define NM_SETTINGS_CONNECTION_CANCEL_SECRETS "cancel-secrets"
#define NM_SETTINGS_CONNECTION_UPDATED_INTERNAL "updated-internal"
#define NM_SETTINGS_CONNECTION_FLAGS_CHANGED    "flags-changed"

/* Properties */
#define NM_SETTINGS_CONNECTION_UNSAVED  "unsaved"

/* Internal properties */
#define NM_SETTINGS_CONNECTION_READY    "ready"
#define NM_SETTINGS_CONNECTION_FLAGS    "flags"
#define NM_SETTINGS_CONNECTION_FILENAME "filename"


/**
 * NMSettingsConnectionIntFlags:
 * @NM_SETTINGS_CONNECTION_INT_FLAGS_NONE: no flag set
 * @NM_SETTINGS_CONNECTION_INT_FLAGS_UNSAVED: the connection is not saved to disk
 * @NM_SETTINGS_CONNECTION_INT_FLAGS_NM_GENERATED: A connection is "nm-generated" if
 *  it was generated by NetworkManger. If the connection gets modified or saved
 *  by the user, the flag gets cleared. A nm-generated is implicitly unsaved.
 * @NM_SETTINGS_CONNECTION_INT_FLAGS_VOLATILE: The connection will be deleted
 *  when it disconnects. That is for in-memory connections (unsaved), which are
 *  currently active but cleanup on disconnect.
 * @NM_SETTINGS_CONNECTION_INT_FLAGS_VISIBLE: The connection is visible
 * @NM_SETTINGS_CONNECTION_INT_FLAGS_EXPORTED_MASK: the entire enum is
 *   internal, however, parts of it is public API as #NMSettingsConnectionFlags.
 *   This mask, are the public flags.
 * @NM_SETTINGS_CONNECTION_INT_FLAGS_ALL: special mask, for all known flags
 *
 * #NMSettingsConnection flags.
 **/
typedef enum {
	NM_SETTINGS_CONNECTION_INT_FLAGS_NONE                   = 0,

	NM_SETTINGS_CONNECTION_INT_FLAGS_UNSAVED                = (1LL <<  0),
	NM_SETTINGS_CONNECTION_INT_FLAGS_NM_GENERATED           = (1LL <<  1),
	NM_SETTINGS_CONNECTION_INT_FLAGS_VOLATILE               = (1LL <<  2),

	NM_SETTINGS_CONNECTION_INT_FLAGS_VISIBLE                = (1LL <<  3),

	__NM_SETTINGS_CONNECTION_INT_FLAGS_LAST,

	NM_SETTINGS_CONNECTION_INT_FLAGS_EXPORTED_MASK          = 0,

	NM_SETTINGS_CONNECTION_INT_FLAGS_ALL = ((__NM_SETTINGS_CONNECTION_INT_FLAGS_LAST - 1) << 1) - 1,
} NMSettingsConnectionIntFlags;

typedef enum {
	NM_SETTINGS_CONNECTION_COMMIT_REASON_NONE                       = 0,
	NM_SETTINGS_CONNECTION_COMMIT_REASON_USER_ACTION                = (1LL << 0),
	NM_SETTINGS_CONNECTION_COMMIT_REASON_ID_CHANGED                 = (1LL << 1),
} NMSettingsConnectionCommitReason;

typedef enum {
	NM_SETTINGS_AUTO_CONNECT_BLOCKED_REASON_NONE                    = 0,

	NM_SETTINGS_AUTO_CONNECT_BLOCKED_REASON_USER_REQUEST            = (1LL << 0),
	NM_SETTINGS_AUTO_CONNECT_BLOCKED_REASON_FAILED                  = (1LL << 1),
	NM_SETTINGS_AUTO_CONNECT_BLOCKED_REASON_NO_SECRETS              = (1LL << 2),

	NM_SETTINGS_AUTO_CONNECT_BLOCKED_REASON_ALL                     = (  NM_SETTINGS_AUTO_CONNECT_BLOCKED_REASON_USER_REQUEST
	                                                                   | NM_SETTINGS_AUTO_CONNECT_BLOCKED_REASON_FAILED
	                                                                   | NM_SETTINGS_AUTO_CONNECT_BLOCKED_REASON_NO_SECRETS),
} NMSettingsAutoconnectBlockedReason;

struct _NMSettingsConnectionCallId;
typedef struct _NMSettingsConnectionCallId NMSettingsConnectionCallId;

typedef struct _NMSettingsConnectionClass NMSettingsConnectionClass;

struct _NMSettingsConnectionPrivate;

struct _NMSettingsConnection {
	NMDBusObject parent;
	struct _NMSettingsConnectionPrivate *_priv;
	CList _connections_lst;
};

struct _NMSettingsConnectionClass {
	NMDBusObjectClass parent;

	gboolean (*commit_changes) (NMSettingsConnection *self,
	                            NMConnection *new_connection,
	                            NMSettingsConnectionCommitReason commit_reason,
	                            NMConnection **out_reread_connection,
	                            char **out_logmsg_change,
	                            GError **error);

	gboolean (*delete) (NMSettingsConnection *self,
	                    GError **error);

	gboolean (*supports_secrets) (NMSettingsConnection *self,
	                              const char *setting_name);
};

GType nm_settings_connection_get_type (void);

guint64 nm_settings_connection_get_last_secret_agent_version_id (NMSettingsConnection *self);

gboolean nm_settings_connection_has_unmodified_applied_connection (NMSettingsConnection *self,
                                                                   NMConnection *applied_connection,
                                                                   NMSettingCompareFlags compare_flage);

typedef enum {
	NM_SETTINGS_CONNECTION_PERSIST_MODE_KEEP,

	/* like KEEP, but always clears the UNSAVED flag */
	NM_SETTINGS_CONNECTION_PERSIST_MODE_KEEP_SAVED,
	NM_SETTINGS_CONNECTION_PERSIST_MODE_DISK,

	/* unsaved, only sets the unsaved flag, but it doesn't touch
	 * the NM_GENERATED nor VOLATILE flag. */
	NM_SETTINGS_CONNECTION_PERSIST_MODE_UNSAVED,

	NM_SETTINGS_CONNECTION_PERSIST_MODE_IN_MEMORY,
	NM_SETTINGS_CONNECTION_PERSIST_MODE_IN_MEMORY_DETACHED,
	NM_SETTINGS_CONNECTION_PERSIST_MODE_IN_MEMORY_ONLY,
	NM_SETTINGS_CONNECTION_PERSIST_MODE_VOLATILE_DETACHED,
	NM_SETTINGS_CONNECTION_PERSIST_MODE_VOLATILE_ONLY,
} NMSettingsConnectionPersistMode;

gboolean  nm_settings_connection_update (NMSettingsConnection *self,
                                         NMConnection *new_connection,
                                         NMSettingsConnectionPersistMode persist_mode,
                                         NMSettingsConnectionCommitReason commit_reason,
                                         const char *log_diff_name,
                                         GError **error);

gboolean nm_settings_connection_delete (NMSettingsConnection *self,
                                        GError **error);

typedef void (*NMSettingsConnectionSecretsFunc) (NMSettingsConnection *self,
                                                 NMSettingsConnectionCallId *call_id,
                                                 const char *agent_username,
                                                 const char *setting_name,
                                                 GError *error,
                                                 gpointer user_data);

gboolean nm_settings_connection_new_secrets (NMSettingsConnection *self,
                                             NMConnection *applied_connection,
                                             const char *setting_name,
                                             GVariant *secrets,
                                             GError **error);

NMSettingsConnectionCallId *nm_settings_connection_get_secrets (NMSettingsConnection *self,
                                                                NMConnection *applied_connection,
                                                                NMAuthSubject *subject,
                                                                const char *setting_name,
                                                                NMSecretAgentGetSecretsFlags flags,
                                                                const char *const*hints,
                                                                NMSettingsConnectionSecretsFunc callback,
                                                                gpointer callback_data);

void nm_settings_connection_cancel_secrets (NMSettingsConnection *self,
                                            NMSettingsConnectionCallId *call_id);

void nm_settings_connection_recheck_visibility (NMSettingsConnection *self);

gboolean nm_settings_connection_check_permission (NMSettingsConnection *self,
                                                  const char *permission);

void nm_settings_connection_added (NMSettingsConnection *self);

void nm_settings_connection_signal_remove (NMSettingsConnection *self);

gboolean nm_settings_connection_get_unsaved (NMSettingsConnection *self);

NMSettingsConnectionIntFlags nm_settings_connection_get_flags (NMSettingsConnection *self);
NMSettingsConnectionIntFlags nm_settings_connection_set_flags (NMSettingsConnection *self, NMSettingsConnectionIntFlags flags, gboolean set);
NMSettingsConnectionIntFlags nm_settings_connection_set_flags_full (NMSettingsConnection *self, NMSettingsConnectionIntFlags mask, NMSettingsConnectionIntFlags value);

int nm_settings_connection_cmp_timestamp (NMSettingsConnection *ac, NMSettingsConnection *ab);
int nm_settings_connection_cmp_timestamp_p_with_data (gconstpointer pa, gconstpointer pb, gpointer user_data);
int nm_settings_connection_cmp_autoconnect_priority (NMSettingsConnection *a, NMSettingsConnection *b);
int nm_settings_connection_cmp_autoconnect_priority_p_with_data (gconstpointer pa, gconstpointer pb, gpointer user_data);

gboolean nm_settings_connection_get_timestamp (NMSettingsConnection *self,
                                               guint64 *out_timestamp);

void nm_settings_connection_update_timestamp (NMSettingsConnection *self,
                                              guint64 timestamp,
                                              gboolean flush_to_disk);

void nm_settings_connection_read_and_fill_timestamp (NMSettingsConnection *self);

char **nm_settings_connection_get_seen_bssids (NMSettingsConnection *self);

gboolean nm_settings_connection_has_seen_bssid (NMSettingsConnection *self,
                                                const char *bssid);

void nm_settings_connection_add_seen_bssid (NMSettingsConnection *self,
                                            const char *seen_bssid);

void nm_settings_connection_read_and_fill_seen_bssids (NMSettingsConnection *self);

int nm_settings_connection_autoconnect_retries_get (NMSettingsConnection *self);
void nm_settings_connection_autoconnect_retries_set (NMSettingsConnection *self,
                                                     int retries);
void nm_settings_connection_autoconnect_retries_reset (NMSettingsConnection *self);

gint32 nm_settings_connection_autoconnect_retries_blocked_until (NMSettingsConnection *self);

NMSettingsAutoconnectBlockedReason nm_settings_connection_autoconnect_blocked_reason_get (NMSettingsConnection *self,
                                                                                          NMSettingsAutoconnectBlockedReason mask);
gboolean nm_settings_connection_autoconnect_blocked_reason_set_full (NMSettingsConnection *self,
                                                                     NMSettingsAutoconnectBlockedReason mask,
                                                                     NMSettingsAutoconnectBlockedReason value);

static inline gboolean
nm_settings_connection_autoconnect_blocked_reason_set (NMSettingsConnection *self,
                                                       NMSettingsAutoconnectBlockedReason mask,
                                                       gboolean set)
{
	return nm_settings_connection_autoconnect_blocked_reason_set_full (self, mask, set ? mask : NM_SETTINGS_AUTO_CONNECT_BLOCKED_REASON_NONE);
}

gboolean nm_settings_connection_autoconnect_is_blocked (NMSettingsConnection *self);

gboolean nm_settings_connection_get_ready (NMSettingsConnection *self);
void     nm_settings_connection_set_ready (NMSettingsConnection *self,
                                           gboolean ready);

void        nm_settings_connection_set_filename (NMSettingsConnection *self,
                                                 const char *filename);
const char *nm_settings_connection_get_filename (NMSettingsConnection *self);

const char *nm_settings_connection_get_id   (NMSettingsConnection *connection);
const char *nm_settings_connection_get_uuid (NMSettingsConnection *connection);

#endif /* __NETWORKMANAGER_SETTINGS_CONNECTION_H__ */
