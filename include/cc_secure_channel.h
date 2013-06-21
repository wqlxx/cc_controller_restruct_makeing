/*
 * cc_secure_channel_send functions.
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

#ifndef CC_SECURE_CHANNEL_SEND_H
#define CC_SECURE_CHANNEL_SEND_H 1

#include "cc_basic.h"
//#include "cc_of_msg_send.h"
#include "cc_queue.h"
#include "cc_buffer.h"
#include "cc_log.h"
//#include "cc_of_msg_prep.h"
#include "cc_error.h"
//#include "cc_of_msg_recv.h"
#include "cc_thread_pool.h"
//#include "cc_of_msg_check.h"

#define CC_RECV_BUFFER_SIZE 1024

//FUNC_CB cc_select_handler(uint16_t type);
extern const struct cc_recv_ops_s cc_recv_ops;
extern const struct cc_send_ops_s cc_send_ops;

//extern const struct cc_recv_1_3_ops_s cc_recv_1_3_ops;/*prepare for spec 1.3*/

int cc_secure_channel_read(sw_info* cc_sw_info);

int cc_recv_from_secure_channel(sw_info* cc_sw_info);

int cc_recv_from_app(sw_info *cc_sw_info);

int cc_ofpmsg_handle(sw_info* cc_sw_info, uint8_t type);

int cc_send_to_secure_channel(sw_info* cc_sw_info,buffer* buf);

int cc_send_to_secure_channel_app(sw_info* cc_sw_info,buffer* buf);

int cc_flush_to_secure_channel(sw_info* cc_sw_info);


#endif

