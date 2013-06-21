/*
 * OpenFlow Switch Manager
 *
 * Author: Kazushi SUGYO
 *
 * Copyright (C) 2008-2012 NEC Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */


#ifndef CC_XID_H
#define CC_XID_H 1

#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <limits.h>

#include "cc_log.h"
#include "cc_error.h"
#include "cc_hash_table.h"
#include "cc_single_link.h"
#include "cc_utility.h"
#include "openflow.h"

#define CC_XID_TIMEOUT 5

struct cc_xid_table_s{
    hash_table_t *hash_table;
    int count;
};
typedef struct cc_xid_table_s cc_xid_table_t;

int  cc_xid_alloc(int xid_latest);

cc_xid_table_t* cc_init_xid_table(void);

int cc_send_req_xid(cc_xid_table_t *xid_table, int xid);

int cc_recv_reply_xid(cc_xid_table_t *xid_table, int xid);

int cc_delete_xid_table(cc_xid_table_t *xid_table, int xid);

int cc_destroy_xid_table(cc_xid_table_t *xid_table);

int cc_dump_all_xid(cc_xid_table_t *xid_table);


#endif // CC_XID_H


/*
 * Local variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 */


