/*
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


#include <assert.h>
#include <string.h>

#include "cc_xid.h"


int
cc_xid_alloc(int xid_latest)
{
    xid_latest++;
    return xid_latest;
}


cc_xid_table_t*
cc_init_xid_table(void)
{
    cc_xid_table_t *new_xid_table;

    new_xid_table = malloc(sizeof(cc_xid_table_t));
    if(new_xid_table == NULL)
        return NULL;
    new_xid_table->hash_table = init_hash_table();
    if(new_xid_table->hash_table == NULL)
        return NULL;

    new_xid_table->count = 0;
    return new_xid_table;
}


int
cc_send_req_xid(cc_xid_table_t *xid_table, int xid_prev)
{
    int ret;
    hash_table_node_t find;
    int xid;

    find = NULL;
    xid = cc_xid_alloc(xid_prev);
    ret = hash_search(xid_table->hash_table, xid, &find);
    if( ret != CC_E_ERR && find != NULL)
    {  
        return CC_E_ERR;
    }
    
    ret = insert_hash_table(xid_table->hash_table, xid);
    if(ret == CC_E_ERR)
        return CC_E_ERR;

    xid_table->count++;
    return CC_E_NONE;
}


int
cc_recv_reply_xid(cc_xid_table_t *xid_table, int xid)
{
    int ret;
    hash_table_node_t find;
    struct timeval now_tv;

    ret = hash_search(xid_table->hash_table, xid, &find);
    if( ret == CC_E_ERR && find == NULL)
    {
        return CC_E_ERR;
    }
    
    gettimeofday(&(now_tv), NULL);
    long time;
    time = now_tv.tv_sec - find->tv.tv_sec;
    if(time >= 0 && time < CC_XID_TIMEOUT) 
    {
        ret = hash_delete_by_node(find);
        if(ret == CC_E_ERR)
            return CC_E_ERR;
        goto intime;
    }else{
        goto outtime;
    }

intime:
    xid_table->count--;
    return CC_E_UNTIMEOUT;

outtime:
    xid_table->count--;    
    return CC_E_TIMEOUT;
}


int 
cc_delete_xid(cc_xid_table_t *xid_table, int xid)
{
    int ret;

    ret = hash_delete_by_key(xid_table->hash_table, xid);
    if(ret == CC_E_ERR)
        return CC_E_ERR;
    xid_table->count--;

    return CC_E_NONE;
}


int
cc_destroy_xid_table(cc_xid_table_t *xid_table)
{
    int ret;

    ret = hash_destroy(xid_table->hash_table);    
    if(ret == CC_E_ERR)
        return CC_E_ERR;
    xid_table->count = 0;
    free(xid_table->hash_table);
    free(xid_table);

    return CC_E_NONE;
}

int
cc_dump_all_xid(cc_xid_table_t *xid_table)
{
    int i;
    hash_table_node_t tmp;
    for(i = 0; i < MAX_LEN; i++)
    {
        if(xid_table->hash_table[i].next != NULL)
            tmp = xid_table->hash_table[i].next;
        else
            continue;
        while(1){
            if(tmp == NULL)
                break;
            printf("hash_table[%d] has %d\n", i, tmp->key);
            tmp = tmp->next;
        }
    }
    printf("\n");

    return CC_E_NONE;
}

/*
 * Local variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 */

