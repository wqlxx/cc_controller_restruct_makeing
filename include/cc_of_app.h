#include "cc_error.h"
#include "cc_of_action.h"

#ifndef CC_OF_APP_H
#define CC_OF_APP_H 1

union app_data_type_u{
    struct{
        uint64_t cookie;
        uint64_t command;
        uint64_t idle_timeout;
        uint64_t hard_timeout;
        uint16_t priority;
        uint32_t buffer_id;
        uint16_t out_port;
        uint16_t flags;
        openflow_actions *actions;
        struct ofp_match match;
        buffer *data;
    }app_flow_mod;
    struct{
        uint32_t buffer_id;
        uint16_t in_port; 
        openflow_actions *actions;
        buffer *data;
    }app_packet_out;
    struct{
        uint16_t port_no;
        uint8_t hw_addr[ OFP_ETH_ALEN ];
        uint32_t config;
        uint32_t mask;
        uint32_t advertise; 
    }app_port_mod;
    struct{
        uint16_t type;
        uint16_t length;
        uint16_t flags;
    }app_stats_req;
    struct{
        uint16_t flags;
        struct ofp_match match;
        uint8_t table_id;
        uint16_t out_port;
    }app_flow_stats_req;
    struct{
        uint16_t flags;
        uint16_t port_no;
    }app_port_stats_req;
    struct{
        uint16_t code;
        uint16_t type;
    }app_error;
        
};
typedef union app_data_type_u app_data_type_t;

#endif

