/*
 * queue functions.
 *
 * Author: qiang wang <wqlxx@yahoo.com.cn>
 *
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

#include "cc_queue.h"

int
queue_is_empty(message_queue *queue)
{
  	pthread_mutex_lock(queue->queue_lock);
    if(queue->length == 0)
    {    
        pthread_mutex_unlock(queue->queue_lock);
        return CC_E_NONE;
    }
    
  	pthread_mutex_unlock(queue->queue_lock);

    return CC_E_ERR;

}

message_queue *
create_message_queue( void ) {
	message_queue *new_queue = (message_queue *)malloc( sizeof( message_queue ) );
 	new_queue->head =(message_queue_element*)malloc( sizeof( message_queue_element ) );
	new_queue->head->data = NULL;
	new_queue->head->next = NULL;
    new_queue->tail = new_queue->head;
	new_queue->length = 0;
	new_queue->queue_lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(new_queue->queue_lock, NULL);
	//(*(new_queue->queue_lock) )= PTHREAD_MUTEX_INITIALIZER;
  	return new_queue;
}


/*
 * don't use it in the dequeue_message
 * we should use it after send msg
 */
int
free_message_element(message_queue_element * element)
{
	free(element);
	return CC_E_NONE;
}


int
delete_message_queue( message_queue *queue ) {

	message_queue_element* element;
  	pthread_mutex_lock(queue->queue_lock);
  	if ( queue == NULL ) {
    	perror( "queue must not be NULL" );
		pthread_mutex_unlock(queue->queue_lock);
		return CC_E_NONE;
  	}

  	while ( queue->head->next != NULL ) {
    	element = queue->head->next;
    	if ( element != NULL ){
            if( element->data != NULL ){ 
                queue->head->next = queue->head->next->next; 
      			free_buffer(element->data);
                free_message_element(element);
            }else{
            	free_message_element(element);
			}
        }
        
    }

    if(queue->head != NULL)
        free_message_element(queue->head);
    else
        pthread_mutex_unlock(queue->queue_lock);
        return CC_E_ERR;
    
    if(queue != NULL){
        pthread_mutex_unlock(queue->queue_lock);
        pthread_mutex_destory(queue->queue_lock);
        free(queue);
        return CC_E_NONE;
    }else{
        pthread_mutex_unlock(queue->queue_lock);        
        return CC_E_ERR;   
    }
}


int
enqueue_message( message_queue *queue, buffer *buf ) 
{
  	pthread_mutex_lock(queue->queue_lock);
  	if ( queue == NULL ) {
    	log_err_for_cc( "queues must not be NULL" );
		pthread_mutex_unlock(queue->queue_lock);
		return CC_E_ERR;
  	}
  	if ( buf == NULL ) {
    	log_err_for_cc( "message must not be NULL" );
		pthread_mutex_unlock(queue->queue_lock);
		return CC_E_ERR;
  	}

    struct ofp_header *header;
    header = (struct ofp_header*)(buf->data);
    printf("enqueue a type %d, %s\n", header->type, __func__);
    
  	message_queue_element *new_tail = (message_queue_element*)malloc( sizeof( message_queue_element ) );
  	new_tail->data = buf;
  	new_tail->next = NULL;

  	queue->tail->next = new_tail;
  	queue->tail = new_tail;
  	queue->length++;

  	pthread_mutex_unlock(queue->queue_lock);

  	return CC_E_NONE;
}

/*
 *free the element, but don't free the data.
 */
buffer*
dequeue_message( message_queue *queue) {

  	buffer* buf = NULL;
 	message_queue_element* tmp_element;

  	pthread_mutex_lock(queue->queue_lock);
  	if ( queue == NULL ) {
    	perror( "queue must not be NULL" );	
		pthread_mutex_unlock(queue->queue_lock);
		return NULL;
  	}
  	if ( queue->head->next == NULL ) {
  		pthread_mutex_unlock(queue->queue_lock);
  		return NULL;
  	}
	if( queue->length == 0 ){
  		pthread_mutex_unlock(queue->queue_lock);
		return NULL;
	}
    
#ifdef debug
        printf("queue_length is %d, in %s: %d\n",queue->length,__func__, __LINE__);
#endif

	tmp_element = queue->head->next;
	if(tmp_element == NULL){
		return NULL;
    }
    
    if(tmp_element->next == NULL){
        queue->head->next =	NULL;
    }else{
		queue->head->next = tmp_element->next; 
    }
    
	tmp_element->next = NULL;
	buf = tmp_element->data;
	
	queue->length--;
	free_message_element(tmp_element);
  	pthread_mutex_unlock(queue->queue_lock);

    if(buf == NULL)
        printf("buf is NULL\n");
    
  	return buf;
}


void 
foreach_message_queue( message_queue *queue, uint32_t function( buffer *message, void *user_data ), void *user_data ) {

  pthread_mutex_lock(queue->queue_lock);
  if ( queue == NULL || queue->head == NULL || queue->head->next == NULL ) {
 	pthread_mutex_unlock(queue->queue_lock);  	
    return;
  }
  
  message_queue_element *element;
  for ( element = queue->head->next; element != NULL; element = element->next ) {
	if( element == NULL )
        break;
    buffer *message = element->data;
   	if( message == NULL )
        break;
    if ( !function( message, user_data ) ) {
      break;
    }
  }
  pthread_mutex_unlock(queue->queue_lock);
  return;
}



