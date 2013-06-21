#ifndef CC_BASIC_H
#define CC_BASIC_H 1

#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdint.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <bits/types.h>
#include <pthread.h>
#include <signal.h>

/****add ssl****/
//#include <openssl/bio.h>
//#include <openssl/ssl.h>
//#include <openssl/err.h>

#include "cc_queue.h"
#include "cc_buffer.h"
#include "cc_single_link.h"
#include "cc_thread_pool.h"
#include "cc_xid.h"

#define CC_LISTEN_PORT 6633
#define CC_LENGTH_OF_LISTEN_QUEUE 20
 //#define CC_TRY_CONN_TIMES 5
#define CC_ACCEPT_FD 3
#define CC_CHILD 1
#define CC_PARENT 0
#define CC_CONN_TIMEOUT_SEC 0
#define CC_CONN_TIMEOUT_USEC 0
#define CC_MAX_SOCKET_BUFF 1024
#define CC_MAX_PORT 52
#define CC_MAX_THREAD_NUM 3


struct cc_socket_t{
    struct sockaddr_in cc_addr;
    //uint16_t port;
    int fd;
};
typedef struct cc_socket_t cc_socket;


struct each_sw_t{
    pid_t pid;
    uint64_t dpid;//datapath_id come from switch_feature_request/reply
    cc_socket cc_socket;
    //each_sw *next;
    char* profile_path;
    uint8_t version;
    uint32_t n_buffers;
    uint8_t n_tables;
    uint32_t actions;
    uint32_t capabilities;
    //port_info* port_head;
    //struct ofp_phy_port port[CC_MAX_PORT];
    list_element *ports;
};
typedef struct each_sw_t each_sw;

#define  BUF_QUEUE 1

struct sw_info_t{
    uint8_t version;
    each_sw cc_switch;
    
    pthread_t cc_sw_thread[CC_MAX_THREAD_NUM];
    CThread_pool* cc_recv_thread_pool;

    message_queue *send_queue;
    message_queue *recv_queue;
    message_queue *app_queue;

    cc_xid_table_t* xid_table;
    int xid_latest;
    
    /*for app func to handle the msg send to app
     *param: app_fd is the file description to connect to app server
     *param: app_server_ip is the ip address of app server
     *param: message to restore the app_msg to send to app server
     */
    /*int app_fd;
    char* app_server_ip;
    message_queue *app_recv_queue;
    message_queue *app_send_queue;*/

    /*add by 20130717*/
    struct cc_send_ops_s *send_handler;
    struct cc_recv_ops_s *recv_handler;
    
    uint32_t cookie;
    /*set config*/
    uint16_t config_flags;        // OFPC_* flags
    uint16_t miss_send_len;       /* Max bytes of new flow that datapath should
                                   send to the controller. */
};
typedef struct sw_info_t sw_info;


struct cc_send_ops_s{
    uint8_t version;
    
    int (*send_hello)(sw_info *cc_sw_info);

    int (*send_error)(sw_info *cc_sw_info);

    int (*send_echo_reply)(sw_info *cc_sw_info);

    int (*send_echo_request)(sw_info *cc_sw_info);

    int (*send_feature_request)(sw_info *cc_sw_info);

    int (*send_get_config_request)(sw_info *cc_sw_info);

    int (*send_set_config)(sw_info *cc_sw_info);

    int (*send_packet_out)(sw_info *cc_sw_info);

    int (*send_flow_mod)(sw_info *cc_sw_info);

    int (*send_port_mod)(sw_info *cc_sw_info);

    int (*send_flow_stats_request)(sw_info *cc_sw_info);

    int (*send_get_desc_stats)(sw_info *cc_sw_info);

    int (*send_aggregate_stats_request)(sw_info *cc_sw_info);

    int (*send_table_stats_request)(sw_info *cc_sw_info);

    int (*send_port_stats_request)(sw_info *cc_sw_info);

    int (*send_queue_stats_request)(sw_info *cc_sw_info);

    int (*send_vendor_stats_request)(sw_info *cc_sw_info);
};


struct cc_recv_ops_s{
    uint8_t version;

    int (*recv_hello)(sw_info *cc_sw_info, buffer *buf);    

    int (*recv_error)(sw_info *cc_sw_info, buffer *buf);

    int (*recv_echo_request)(sw_info *cc_sw_info, buffer *buf);

    int (*recv_echo_reply)(sw_info *cc_sw_info, buffer *buf);

    int (*recv_vendor)(sw_info *cc_sw_info, buffer *buf);

    int (*recv_get_config_reply)(sw_info *cc_sw_info, buffer *buf);

    int (*recv_flow_rm)(sw_info *cc_sw_info, buffer *buf);  

    int (*recv_barrier_reply)(sw_info *cc_sw_info, buffer *buf);

    int (*recv_packet_in)(sw_info *cc_sw_info, buffer *buf);

    int (*recv_port_status)(sw_info *cc_sw_info, buffer *buf);

    int (*recv_flow_stats_reply)(sw_info *cc_sw_info, buffer *buf);
};


#define CC_OFP_VER_1_0 0x01


#endif

