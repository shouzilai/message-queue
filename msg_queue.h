#ifndef __MSG_QUEUE_H__
#define __MSG_QUEUE_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MSG_QUEUE_DATA_SIZE     128
#define MSG_TYPE                int
typedef enum msg_queue_type {
    EM_MSG_UART_DATA = 1,
    EM_MSG_INET_DATA,
    EM_MSG_I2C_DATA,

} msg_type;

typedef struct message_queue_it {
    MSG_TYPE type;
    uint8_t data[MSG_QUEUE_DATA_SIZE];

} msg_queue_t, *msg_queue_p;


int msg_init(int *msg_id);

int msg_deinit(int msg_id);

int msg_recive(int msg_id, msg_queue_p recv_msg);

int msg_send(int msg_id, MSG_TYPE type, uint8_t *data, int data_len);


#endif // __MSG_QUEUE_H__