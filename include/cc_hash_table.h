/*
 * Author: Yasuhito Takamiya <yasuhito@gmail.com>
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


/**
 * @file
 *
 * @brief Associations between keys and values so that given a key the
 * value can be found quickly.
 *
 * @code
 * // Create hash table.
 * table = create_hash( compare_string, hash_string );
 *
 * // Insert three key/value pairs.
 * insert_hash_entry( table, "alpha", &object_a );
 * insert_hash_entry( table, "bravo", &object_b );
 * insert_hash_entry( table, "charlie", &object_c );
 *
 * // Look up by a key = "alpha".
 * lookup_hash_entry( table, "alpha" ); // => object_a
 *
 * // Delete entire hash table.
 * delete_hash( table );
 * @endcode
 */


#ifndef HASH_TABLE_H
#define HASH_TABLE_H 1

#include "cc_log.h"
#include "cc_error.h"

#define MAX_LEN 53

struct hash_table_node_s{
	int key;
	struct timeval tv;	
    //int count;
	struct hash_table_node_s *next;
    struct hash_table_node_s *prev;
};
typedef struct hash_table_node_s* hash_table_node_t;
typedef struct hash_table_node_s hash_table_t;


hash_table_t* init_hash_table(void);
int insert_hash_table(hash_table_t *hash_table, int key);
int hash_search(hash_table_t *hash_table, uint32_t key, hash_table_node_t *find);
int hash_destroy(hash_table_t *hash_table);
int hash_delete_by_key(hash_table_t *hash_table, uint32_t key);


#endif
/*
 * Local variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 */


