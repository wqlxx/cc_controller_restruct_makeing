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

#include "cc_hash_table.h"

//static hash_table_t hash_table[MAX_LEN];

void
hash_atom(const uint32_t key, int *pos)
{
    *pos = (key >> 2) % MAX_LEN;
    return;
}


hash_table_t*
init_hash_table(void)
{
	int i;
    hash_table_t *hash_table = NULL;

    hash_table = malloc(sizeof(hash_table_t)*MAX_LEN);
    if(hash_table == NULL)
        return NULL;
    
    for(i = 0; i < MAX_LEN; i++)
	{
		hash_table[i].key = -1;
		hash_table[i].next = NULL;
		hash_table[i].prev = NULL;   
	}
	return hash_table;
}

int
insert_hash_table(hash_table_t *hash_table,int key)
{
	int pos;
	hash_table_node_t new = NULL;
	hash_table_node_t tmp = NULL;

    hash_atom(key, &pos);
    
	if(pos < 0 || pos >= MAX_LEN)
		return CC_E_ERR;

    if(hash_table[pos].key == -1 && 
        hash_table[pos].next == NULL &&
        hash_table[pos].prev == NULL)
    {
        
		hash_table[pos].key = 0;
		new = malloc(sizeof(struct hash_table_node_s));
		if(new == NULL)
			goto err;
		new->key = key;
		gettimeofday(&(new->tv),NULL);
		hash_table[pos].next = new;
		new->next = NULL;
        new->prev = &(hash_table[pos]);
	}else{
		tmp = hash_table[pos].next;
		while(tmp->next != NULL)
			tmp = tmp->next;
		new = malloc(sizeof(struct hash_table_node_s));
		if(new == NULL)
			goto err;
		new->key = key;
		gettimeofday(&(new->tv),NULL);
		new->next = NULL;
        new->prev = tmp;
		tmp->next = new;
	}
	
	return CC_E_NONE;

err:
	return CC_E_ERR;
}


int
hash_search(hash_table_t *hash_table, uint32_t key, hash_table_node_t *find)
{
	int pos;
	hash_table_node_t tmp;

	hash_atom(key, &pos);
	tmp = hash_table[pos].next;
	while(tmp != NULL)
    {
        if(key == tmp->key)
            break;
		tmp = tmp->next;
	}			
	if(tmp == NULL){
        *find = NULL;
		return CC_E_ERR;
    }

    *find = tmp;
	return CC_E_NONE;	
}

int
hash_destroy(hash_table_t *hash_table)
{
	int i;
	hash_table_node_t tmp;
    
	for(i = 0; i < MAX_LEN; i++){
		tmp = hash_table[i].next;
		while(tmp != NULL){
			hash_table[i].next = tmp->next;
			free(tmp);
			tmp = hash_table[i].next;
		}
	}
    
	return CC_E_NONE;
}

int
hash_delete_by_key(hash_table_t *hash_table, uint32_t key)
{
    int pos;
    int ret;
    hash_table_node_t find;
    hash_table_node_t prev_find;
    hash_table_node_t next_find;

    find = NULL;
    prev_find = NULL;
    next_find = NULL;

	ret = hash_search(hash_table, key, &find);
    if(ret == CC_E_ERR || find == NULL){
        log_debug_for_cc("delete failed!\n");        
        return CC_E_ERR;
    }

    prev_find = find->prev;
    next_find = find->next;
    if(next_find == NULL){
        prev_find->next == NULL;
    }else{
        prev_find->next = next_find;
        next_find->prev = prev_find;
    }

    free(find);
    log_debug_for_cc("delete by key success!");
	//printf("key is %d, free find success!\n", key);

	return CC_E_NONE;
}

int
hash_delete_by_node(hash_table_node_t node)
{
    if(node == NULL)
        return CC_E_ERR;
    if(node->next == NULL){
        node->prev->next = NULL;
    }else{
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }
    
    free(node);
    log_debug_for_cc("delete by node success!");
    
    return CC_E_NONE;
    
}

#if 0 /*for test*/
int 
main(int argc, char **argv)
{
	int key;
	int value;
	int key_find;
	int value_find;
	int i;
	init_hash_table();
	srand((unsigned)time(NULL));
	for(i = 0; i < MAX_NUM; i++)	
	{
		key = rand()%100;
		//value = rand()%UINT_MAX;
		value = key*2-1;
		insert(key, value);
	}	
	printf("last key is %d\n", key);
	printf("please in put a key :\n");
	scanf("%d",&key_find);
	//hash_search(key, &value_find);
	while(hash_search(key_find, &value_find))
	{
		getchar();
		printf("please in put a key :\n");
		scanf("%d",&key_find);
	}	

	printf("value is %d\n", value_find);
	return 0;
}
#endif



/*
 * Local variables:
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * End:
 */

