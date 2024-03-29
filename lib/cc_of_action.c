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

#include "cc_of_action.h"

openflow_actions *
create_actions() {
  openflow_actions *actions;

  //log_debug_for_cc( "Creating an empty actions list." );

  actions = ( openflow_actions * ) malloc( sizeof( openflow_actions ) );

  if ( create_list( &actions->list ) == CC_E_ERR ) {
    //assert( 0 );
  }

  actions->n_actions = 0;

  return actions;
}


int
delete_actions( openflow_actions *actions ) {
  list_element *element;

  //log_debug_for_cc( "Deleting an actions list." );

  //assert( actions != NULL );

  //log_debug_for_cc( "# of actions = %d.", actions->n_actions );

  element = actions->list;
  while ( element != NULL ) {
    free( element->data );
    element = element->next;
  }

  delete_list( actions->list );
  free( actions );

  return CC_E_NONE;
}


int
append_action_output( openflow_actions *actions, const uint16_t port, const uint16_t max_len ) {
  int ret;
  struct ofp_action_output *action_output;

  //log_debug_for_cc( "Appending an output action ( port = %u, max_len = %u ).", port, max_len );

  //assert( actions != NULL );

  action_output = ( struct ofp_action_output * ) malloc( sizeof( struct ofp_action_output ) );
  action_output->type = OFPAT_OUTPUT;
  action_output->len = sizeof( struct ofp_action_output );
  action_output->port = port;
  action_output->max_len = max_len;

  ret = append_to_tail( &actions->list, ( void * ) action_output );
  if ( ret >= 0) {
    actions->n_actions++;
  }

  return ret;
}


int
append_action_set_vlan_vid( openflow_actions *actions, const uint16_t vlan_vid ) {
  int ret;
  struct ofp_action_vlan_vid *action_vlan_vid;

  //log_debug_for_cc( "Appending a set vlan action ( vlan_vid = %#x ).", vlan_vid );

  //assert( actions != NULL );
  //assert( ( vlan_vid & ~VLAN_VID_MASK ) == 0 );

  action_vlan_vid = ( struct ofp_action_vlan_vid * ) malloc( sizeof( struct ofp_action_vlan_vid ) );
  action_vlan_vid->type = OFPAT_SET_VLAN_VID;
  action_vlan_vid->len = sizeof( struct ofp_action_vlan_vid );
  action_vlan_vid->vlan_vid = vlan_vid;

  ret = append_to_tail( &actions->list, ( void * ) action_vlan_vid );
  if ( ret >= 0 ) {
    actions->n_actions++;
  }

  return ret;
}


int
append_action_set_vlan_pcp( openflow_actions *actions, const uint8_t vlan_pcp ) {
  int ret;
  struct ofp_action_vlan_pcp *action_vlan_pcp;

  //log_debug_for_cc( "Appending a set vlan pcp action ( vlan_pcp = %#x ).", vlan_pcp );

  //assert( actions != NULL );
  //assert( ( vlan_pcp & ~VLAN_PCP_MASK ) == 0 );

  action_vlan_pcp = ( struct ofp_action_vlan_pcp * ) malloc( sizeof( struct ofp_action_vlan_pcp ) );
  action_vlan_pcp->type = OFPAT_SET_VLAN_PCP;
  action_vlan_pcp->len = sizeof( struct ofp_action_vlan_pcp );
  action_vlan_pcp->vlan_pcp = vlan_pcp;

  ret = append_to_tail( &actions->list, ( void * ) action_vlan_pcp );
  if ( ret >= 0 ) {
    actions->n_actions++;
  }

  return ret;
}


int
append_action_strip_vlan( openflow_actions *actions ) {
  int ret;
  struct ofp_action_header *action_strip_vlan;

  //log_debug_for_cc( "Appending a strip vlan action." );

  //assert( actions != NULL );

  action_strip_vlan = ( struct ofp_action_header * ) malloc( sizeof( struct ofp_action_header ) );
  action_strip_vlan->type = OFPAT_STRIP_VLAN;
  action_strip_vlan->len = sizeof( struct ofp_action_header );

  ret = append_to_tail( &actions->list, ( void * ) action_strip_vlan );
  if ( ret >= 0) {
    actions->n_actions++;
  }

  return ret;
}


static int
append_action_set_dl_addr( openflow_actions *actions, const uint16_t type,
                           const uint8_t hw_addr[ OFP_ETH_ALEN ] ) {
  int ret;
  struct ofp_action_dl_addr *action_dl_addr;

  //log_debug_for_cc( "Appending a set dl_src/dl_dst action ( type = %#x, hw_addr = %02x:%02x:%02x:%02x:%02x:%02x ).",
  //       type, hw_addr[ 0 ], hw_addr[ 1 ], hw_addr[ 2 ], hw_addr[ 3 ], hw_addr[ 4 ], hw_addr[ 5 ] );

  //assert( actions != NULL );

  action_dl_addr = ( struct ofp_action_dl_addr * ) malloc( sizeof( struct ofp_action_dl_addr ) );
  action_dl_addr->type = type;
  action_dl_addr->len = sizeof( struct ofp_action_dl_addr );
  memcpy( action_dl_addr->dl_addr, hw_addr, OFP_ETH_ALEN );

  ret = append_to_tail( &actions->list, ( void * ) action_dl_addr );
  if ( ret >= 0) {
    actions->n_actions++;
  }

  return ret;
}


int
append_action_set_dl_src( openflow_actions *actions, const uint8_t hw_addr[ OFP_ETH_ALEN ] ) {
  //log_debug_for_cc( "Appending a set dl_src action ( hw_addr ");

  //assert( actions != NULL );
  return append_action_set_dl_addr( actions, OFPAT_SET_DL_SRC, hw_addr );
}


int
append_action_set_dl_dst( openflow_actions *actions, const uint8_t hw_addr[ OFP_ETH_ALEN ] ) {
 // log_debug_for_cc( "Appending a set dl_dst action ( hw_addr = %02x:%02x:%02x:%02x:%02x:%02x ).",
 //        hw_addr[ 0 ], hw_addr[ 1 ], hw_addr[ 2 ], hw_addr[ 3 ], hw_addr[ 4 ], hw_addr[ 5 ] );

  //assert( actions != NULL );
  return append_action_set_dl_addr( actions, OFPAT_SET_DL_DST, hw_addr );
}


static int
append_action_set_nw_addr( openflow_actions *actions, const uint16_t type, const uint32_t nw_addr ) {
  int ret;
  char addr_str[ 16 ];
  struct in_addr addr;
  struct ofp_action_nw_addr *action_nw_addr;

  addr.s_addr = htonl( nw_addr );
  memset( addr_str, '\0', sizeof( addr_str ) );
  inet_ntop( AF_INET, &addr, addr_str, sizeof( addr_str ) );
  //log_debug_for_cc( "Appending a set nw_src/nw_dst action ( type = %#x, nw_addr = %s ).", type, addr_str );

  //assert( actions != NULL );

  action_nw_addr = ( struct ofp_action_nw_addr * ) malloc( sizeof( struct ofp_action_nw_addr ) );
  action_nw_addr->type = type;
  action_nw_addr->len = sizeof( struct ofp_action_nw_addr );
  action_nw_addr->nw_addr = nw_addr;

  ret = append_to_tail( &actions->list, ( void * ) action_nw_addr );
  if ( ret >= 0) {
    actions->n_actions++;
  }

  return ret;
}


int
append_action_set_nw_src( openflow_actions *actions, const uint32_t nw_addr ) {
  char addr_str[ 16 ];
  struct in_addr addr;

  addr.s_addr = htonl( nw_addr );
  memset( addr_str, '\0', sizeof( addr_str ) );
  inet_ntop( AF_INET, &addr, addr_str, sizeof( addr_str ) );
  //log_debug_for_cc( "Appending a set nw_src action ( nw_addr = %s ).", addr_str );

  //assert( actions != NULL );
  return append_action_set_nw_addr( actions, OFPAT_SET_NW_SRC, nw_addr );
}


int
append_action_set_nw_dst( openflow_actions *actions, const uint32_t nw_addr ) {
  char addr_str[ 16 ];
  struct in_addr addr;

  addr.s_addr = htonl( nw_addr );
  memset( addr_str, '\0', sizeof( addr_str ) );
  inet_ntop( AF_INET, &addr, addr_str, sizeof( addr_str ) );
  //log_debug_for_cc( "Appending a set nw_dst action ( nw_addr = %s ).", addr_str );

  //assert( actions != NULL );
  return append_action_set_nw_addr( actions, OFPAT_SET_NW_DST, nw_addr );
}


int
append_action_set_nw_tos( openflow_actions *actions, const uint8_t nw_tos ) {
  int ret;
  struct ofp_action_nw_tos *action_nw_tos;

  //log_debug_for_cc( "Appending a set nw_tos action ( nw_tos = %#x ).", nw_tos );

  //assert( actions != NULL );
  //assert( ( nw_tos & ~NW_TOS_MASK ) == 0 );

  action_nw_tos = ( struct ofp_action_nw_tos * ) malloc( sizeof( struct ofp_action_nw_tos ) );
  action_nw_tos->type = OFPAT_SET_NW_TOS;
  action_nw_tos->len = sizeof( struct ofp_action_nw_tos );
  action_nw_tos->nw_tos = nw_tos;

  ret = append_to_tail( &actions->list, ( void * ) action_nw_tos );
  if ( ret >= 0) {
    actions->n_actions++;
  }

  return ret;
}


static int
append_action_set_tp_port( openflow_actions *actions, const uint16_t type, const uint16_t tp_port ) {
  int ret;
  struct ofp_action_tp_port *action_tp_port;

  //log_debug_for_cc( "Appending a set tp_src/tp_dst action ( type = %#x, tp_port = %u ).", type, tp_port );

  //assert( actions != NULL );

  action_tp_port = ( struct ofp_action_tp_port * ) malloc( sizeof( struct ofp_action_tp_port ) );
  action_tp_port->type = type;
  action_tp_port->len = sizeof( struct ofp_action_tp_port );
  action_tp_port->tp_port = tp_port;

  ret = append_to_tail( &actions->list, ( void * ) action_tp_port );
  if ( ret >= 0) {
    actions->n_actions++;
  }

  return ret;
}


int
append_action_set_tp_src( openflow_actions *actions, const uint16_t tp_port ) {
  //log_debug_for_cc( "Appending a set tp_src action ( tp_port = %u ).", tp_port );

  //assert( actions != NULL );
  return append_action_set_tp_port( actions, OFPAT_SET_TP_SRC, tp_port );
}


int
append_action_set_tp_dst( openflow_actions *actions, const uint16_t tp_port ) {
  //log_debug_for_cc( "Appending a set tp_dst action ( tp_port = %u ).", tp_port );

  //assert( actions != NULL );
  return append_action_set_tp_port( actions, OFPAT_SET_TP_DST, tp_port );
}


int
append_action_enqueue( openflow_actions *actions, const uint16_t port, const uint32_t queue_id ) {
  int ret;
  struct ofp_action_enqueue *action_enqueue;

  //log_debug_for_cc( "Appending an enqueue action ( port = %u, queue_id = %u ).", port, queue_id );

  //assert( actions != NULL );

  action_enqueue = ( struct ofp_action_enqueue * ) malloc( sizeof( struct ofp_action_enqueue ) );
  action_enqueue->type = OFPAT_ENQUEUE;
  action_enqueue->len = sizeof( struct ofp_action_enqueue );
  action_enqueue->port = port;
  action_enqueue->queue_id = queue_id;

  ret = append_to_tail( &actions->list, ( void * ) action_enqueue );
  if ( ret >= 0) {
    actions->n_actions++;
  }

  return ret;
}

int
append_action_vendor( openflow_actions *actions, const uint32_t vendor, const buffer *body ) {
  int ret;
  uint16_t body_length = 0;
  struct ofp_action_vendor_header *action_vendor;

  if ( ( body != NULL ) && ( body->length > 0 ) ) {
    body_length = ( uint16_t ) body->length;
  }

  //log_debug_for_cc( "Appending a vendor action ( vendor = %#" PRIx32 ", body length = %u ).", vendor, body_length );

  //assert( actions != NULL );

  action_vendor = ( struct ofp_action_vendor_header * )
                  malloc( sizeof( struct ofp_action_vendor_header ) + body_length );
  action_vendor->type = OFPAT_VENDOR;
  action_vendor->len = ( uint16_t ) ( sizeof( struct ofp_action_vendor_header ) + body_length );
  action_vendor->vendor = vendor;

  if ( body_length > 0 ) {
    memcpy( ( char * ) action_vendor + sizeof( struct ofp_action_vendor_header ), body->data, body_length );
  }

  ret = append_to_tail( &actions->list, ( void * ) action_vendor );
  if ( ret >= 0) {
    actions->n_actions++;
  }

  return ret;
}

int
get_actions_length( const openflow_actions *actions ) {
  int actions_length = 0;
  struct ofp_action_header *action_header;
  list_element *action;

  //log_debug_for_cc( "Calculating the total length of actions." );

  //assert( actions != NULL );

  action = actions->list;
  while ( action != NULL ) {
    action_header = ( struct ofp_action_header * ) action->data;

    if ( ( action_header->type <= OFPAT_ENQUEUE ) || ( action_header->type == OFPAT_VENDOR ) ) {
      actions_length += action_header->len;
    }
    else {
      //critical( "Undefined action type ( type = %#x ).", action_header->type );
      //assert( 0 );
        //log_err_for_cc("Too many actions");
		return CC_E_ERR;
    }

    action = action->next;
  }

  //log_debug_for_cc( "Total length of actions = %d.", actions_length );

  if ( actions_length > USHRT_MAX ) {
    //critical( "Too many actions ( # of actions = %d, actions length = %d ).",
    //          actions->n_actions, actions_length );
    //assert( 0 );
    //log_err_for_cc("Too many actions");
	return CC_E_ERR;
  }

  return ( uint16_t ) actions_length;
}


