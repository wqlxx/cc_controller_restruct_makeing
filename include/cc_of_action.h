/*
 * cc_of_msg_action functions.
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
#ifndef CC_OF_MSG_ACTION 
#define CC_OF_MSG_ACTION 1

#include "cc_basic.h"
#include "cc_single_link.h"
#include "cc_log.h"
#include "cc_error.h"

struct openflow_actions_s{
  int n_actions;
  list_element *list;
};
typedef struct openflow_actions_s openflow_actions;

openflow_actions *create_actions( void );
int delete_actions( openflow_actions *actions );
int append_action_output( openflow_actions *actions, const uint16_t port, const uint16_t max_len );
int append_action_set_vlan_vid( openflow_actions *actions, const uint16_t vlan_vid );
int append_action_set_vlan_pcp( openflow_actions *actions, const uint8_t vlan_pcp );
int append_action_strip_vlan( openflow_actions *actions );
int append_action_set_dl_src( openflow_actions *actions, const uint8_t hw_addr[ OFP_ETH_ALEN ] );
int append_action_set_dl_dst( openflow_actions *actions, const uint8_t hw_addr[ OFP_ETH_ALEN ] );
int append_action_set_nw_src( openflow_actions *actions, const uint32_t nw_addr );
int append_action_set_nw_dst( openflow_actions *actions, const uint32_t nw_addr );
int append_action_set_nw_tos( openflow_actions *actions, const uint8_t nw_tos );
int append_action_set_tp_src( openflow_actions *actions, const uint16_t tp_port );
int append_action_set_tp_dst( openflow_actions *actions, const uint16_t tp_port );
int append_action_enqueue( openflow_actions *actions, const uint16_t port,
                            const uint32_t queue_id );
int append_action_vendor( openflow_actions *actions, const uint32_t vendor,
                           const buffer *data );
int get_actions_length( const openflow_actions *actions );

#endif

