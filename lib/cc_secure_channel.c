/*
 * cc_secure_channel_recv functions.
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

#include "cc_secure_channel.h"


static const struct cc_recv_ops_s *cc_of_handler_recv_list[] = {
    &cc_recv_ops,
};

/*
static const struct cc_send_ops_s *cc_of_handler_send_list[] = {
    &cc_send_ops,
};
*/

struct cc_recv_ops_s*
cc_recv_handle_register(uint8_t version)
{
    if(version == CC_OFP_VER_1_0)
        return cc_of_handler_recv_list[0];
    else
        return NULL;
}
/*
struct cc_send_ops_s*
cc_send_handle_register(uint8_t version)
{
    if(version == CC_OFP_VER_1_0)
        return cc_of_handler_send_list[0];
    else
        return NULL;
}
*/

int
cc_ofpmsg_recv_handle(sw_info *cc_sw_info)
{
#ifdef debug
	pthread_t tid = pthread_self();
    printf("tid is %d\n", tid);
#endif
	int ret;
	struct ofp_header *header;
    buffer *recv_buf;
    struct cc_recv_ops_s *of_ops;

    if(cc_sw_info->recv_handler == NULL)
	{
		log_err_for_cc("handler ofmsg error!");
		return CC_E_ERR;
	}
    of_ops = cc_sw_info->recv_handler;

	pthread_mutex_lock(cc_sw_info->recv_queue->queue_lock);
	if(cc_sw_info->recv_queue->length == 0){
        perror("len is 0");
        pthread_mutex_unlock(cc_sw_info->recv_queue->queue_lock);
        return CC_E_ERR;
    }
	pthread_mutex_unlock(cc_sw_info->recv_queue->queue_lock);

    printf("queue_length is %d, in %s: %d\n", cc_sw_info->recv_queue->length,__func__, __LINE__);
    recv_buf = dequeue_message(cc_sw_info->recv_queue);
  	if(recv_buf == NULL){
        perror("dequeue error");
        return CC_E_ERR;
    }
    header = (struct ofp_header*)(recv_buf->data);
   
	printf("----cc_ofpmsg_recv_handle recv type is %d----\n", header->type);

    switch(header->type){
        case OFPT_HELLO:        
		    printf("--------recv_hello--------------\n");
            if(of_ops->recv_hello)
                of_ops->recv_hello(cc_sw_info, recv_buf);
            break;
        case OFPT_ECHO_REQUEST:
			printf("--------recv_echo_request--------------\n");

            if(of_ops->recv_echo_request)
               of_ops->recv_echo_request(cc_sw_info, recv_buf); 
            break;        
        case OFPT_ECHO_REPLY:     
			printf("--------recv_echo_reply--------------\n");

            if(of_ops->recv_echo_reply)
                of_ops->recv_echo_reply(cc_sw_info, recv_buf);
            break;        
        case OFPT_ERROR:
            if(of_ops->recv_error)
                of_ops->recv_error(cc_sw_info, recv_buf);
            break;        
        case OFPT_PACKET_IN:
            if(of_ops->recv_packet_in)
                of_ops->recv_packet_in(cc_sw_info, recv_buf);
            break;        
        case OFPT_VENDOR:
            if(of_ops->recv_vendor)
                of_ops->recv_vendor(cc_sw_info, recv_buf);   
            break;           
        case OFPT_PORT_STATUS:
            if(of_ops->recv_port_status)
                of_ops->recv_port_status(cc_sw_info, recv_buf);
            break;  
        case OFPT_FLOW_REMOVED:
            if(of_ops->recv_flow_rm)
                of_ops->recv_flow_rm(cc_sw_info, recv_buf);
            break;  
        case OFPT_STATS_REPLY:
            if(of_ops->recv_flow_stats_reply)
                of_ops->recv_flow_stats_reply(cc_sw_info, recv_buf);
            break;  
        case OFPT_GET_CONFIG_REPLY:
            if(of_ops->recv_get_config_reply)
                of_ops->recv_get_config_reply(cc_sw_info, recv_buf);
            break;  
        default:
            break;
    }

	return CC_E_NONE;
}

int
cc_recv_from_secure_channel(sw_info *cc_sw_info)
{
	buffer *tmp_buff; 
	size_t tmp_recv_buff_length;
	size_t msg_len;
	struct ofp_header *header;
    int ret;
    void *recv_data;//mod 20130619

    recv_data = malloc(CC_RECV_BUFFER_SIZE*sizeof(char));
	tmp_recv_buff_length = read(cc_sw_info->cc_switch.cc_socket.fd, recv_data, CC_RECV_BUFFER_SIZE);
	if(tmp_recv_buff_length < 0)
	{
		log_err_for_cc("recv failed!");
		if( errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK )
		{
			return CC_E_ERR;
		}
		//reconnect();
		return CC_E_ERR_RECV;
	}else if(tmp_recv_buff_length == 0){
		return CC_E_ERR;
	}
    
	if( tmp_recv_buff_length >= sizeof(struct ofp_header) ) 
	{
		
		/*it will reload the tmp_buff->length with thc really recv length ,
		 *no more than the CC_BUFFER_SIZE
		 */
	    tmp_buff = alloc_buffer_with_length(tmp_recv_buff_length);
	    if(tmp_buff == NULL)
	    {
		    perror("create buffer failed!");
		    return CC_E_ERR;
	    }
		tmp_buff->length = tmp_recv_buff_length;
        memcpy(tmp_buff->data, recv_data, tmp_recv_buff_length);
        
		header = (struct ofp_header*)(tmp_buff->data);
/*
#ifdef debug
		printf("header type is %d\n", header->type);
		printf("header length is %d\n", ntohs(header->length));
		printf("header version is %d\n", header->version);
		printf("header xid is %d\n", header->xid);
#endif  
*/
		if ( header->version != OFP_VERSION ) {
      		perror( "Receive error: invalid version ");
            free_buffer(tmp_buff);
            free(recv_data);
      		return CC_E_ERR;
   		}
		if( ntohs(header->length) > CC_RECV_BUFFER_SIZE )
		{
			perror("recv msg size is larger than buff");
            free_buffer(tmp_buff);
            free(recv_data);
			return CC_E_ERR;
		}
        
		ret = enqueue_message(cc_sw_info->recv_queue, tmp_buff);
        if(ret == CC_E_ERR){
            perror("enqueue message errro");
            free_buffer(tmp_buff);
			free(recv_data);
            return CC_E_ERR;
        }
    }	
	if(cc_sw_info->recv_queue < 0){
		free(recv_data);
       	return CC_E_ERR;
	}
    
	ret = pool_add_worker(cc_sw_info->cc_recv_thread_pool, cc_ofpmsg_recv_handle, cc_sw_info);
	if(ret == CC_E_ERR){
		free(recv_data);
       	return CC_E_ERR;
	}
    
    free(recv_data);//mod 20130619
    return CC_E_NONE;
}


int
cc_secure_channel_read(sw_info* cc_sw_info)
{
	int ret;
	buffer* buf;
	uint8_t type;

    type = cc_recv_from_secure_channel(cc_sw_info);
	if(ret == CC_E_ERR)
		return CC_E_ERR;

	return CC_E_NONE;
}


int
cc_send_to_secure_channel_app(sw_info* cc_sw_info,buffer* buf)
{
	int ret;
	buffer* msg;

    while(queue_is_empty(cc_sw_info->app_queue) == CC_E_ERR){
        msg = dequeue_message(cc_sw_info->app_queue);
        free_buffer(msg);
    }
/*
	if( cc_sw_info->app_send_queue == NULL )
		cc_sw_info->app_send_queue = create_message_queue();

	ret = enqueue_message(cc_sw_info->send_queue, buf);
*/
    
	return CC_E_NONE;
}


int
cc_flush_to_secure_channel(sw_info* cc_sw_info)
{
	buffer* msg;
	int count = 0;
    if(cc_sw_info->send_queue->length == 0){
        return CC_E_EMPTY;
    }
    
    count = cc_sw_info->send_queue->length;
	while(count > 0)
	{
	    count--;
        msg = dequeue_message(cc_sw_info->send_queue);
        if(msg == NULL)
            return CC_E_ERR;
        if(msg == NULL){
            return CC_E_ERR;
		}
        if(count > 10){
            return CC_E_ERR;
		}
		struct ofp_header* header = (struct ofp_header*)msg->data;

		int len = ntohs(header->length);

#ifdef debug
		printf("len is %d, %s\n", len, __func__);
#endif

		ssize_t write_length = write(cc_sw_info->cc_switch.cc_socket.fd, msg->data, ntohs(header->length));
		if( write_length < 0 )
		{
			if ( errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK ) 
			{
				free_buffer(msg);
				return CC_E_ERR;
			}else{
				log_err_for_cc("fail to write a message to secure channel");
				free_buffer(msg);
				return CC_E_ERR;
            }
		}else if( (size_t)write_length > 0 && (size_t)write_length < msg->length ) {
			log_err_for_cc("write msg to secure channel error!");
			continue;
		}

#ifdef debug
        printf("socket write le is %d\n", write_length);
#endif

		free_buffer(msg);
	}
    
	return CC_E_NONE;
}


int
cc_secure_channel_send(sw_info* cc_sw_info)
{
    int ret;
	buffer* buf;
	uint8_t type;
	
    type = cc_flush_to_secure_channel(cc_sw_info);
	if(ret == CC_E_ERR)
		return CC_E_ERR;

	return CC_E_NONE;
}


