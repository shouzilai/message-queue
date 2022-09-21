#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

#include "msg_queue.h"

int message_id;

static void print_data(msg_queue_p msg)
{
    printf("check data of message: \n");
    for (int i = 0; i < MSG_QUEUE_DATA_SIZE && (msg->data[i] != 0x0); i++) {
        printf("%c", msg->data[i]);
    }
    printf("\nthe end of check\n\n");
}


static void *message_recv_thread(void *argc)
{
    int ret = -1;
    int msg_id = message_id;
    msg_queue_t msg; 

    while (1) {
        memset(&msg, 0x0, sizeof(msg));

        ret = msg_recive(msg_id, &msg);

        printf("msg ret is %d\n", ret);
        printf("message has recived. the type of message is %d %d, data is \n", msg.type, msg.cmd);
        print_data(&msg);

        sleep(2);
    }

}

static int message_thread(int mag_id)
{
    int ret = 0;
    pthread_t message_recv_tid;
    ret = pthread_create(&message_recv_tid, NULL, message_recv_thread, (void*)&mag_id);
    if (ret == -1) {
        perror("message recv thread create");
        return -1;
    }
    pthread_detach(message_recv_tid);

    return 0;
}

static int chose_message(msg_type type, msg_queue_p msg_p)
{
    switch (type)
    {
        case 1:
        {
            char message[] = "this is uart data";
            memcpy(msg_p->data, message, sizeof(message));
            break;
        }
        

        case 2:
        {
            char message1[] = "this is inet data";
            memcpy(msg_p->data, message1, sizeof(message1));
            break;
        }

        case 3:
        {
            char message2[] = "this is i2c data";
            memcpy(msg_p->data, message2, sizeof(message2));
            break;
        }

        defalut:
            break;
    }
    print_data(msg_p);

    return 0;
}

int main()
{
    int ret = 0, cmd = 0, msg_id = -1;
    msg_type type;
    msg_queue_t message_t;

    msg_init(&msg_id);
    message_id = msg_id;
    printf("msg_id is %d\n", msg_id);

    srand(time(NULL));
    message_thread(msg_id);
    printf("start send message to queue\n");
    while (1)
    {
        memset(message_t.data, 0x0, sizeof(message_t.data));

        cmd = rand() % 10 + 1; // 生成 1~10
        type = rand() % 3 + 1; // 生成 1~3 类型的消息队列类型
        chose_message(type, &message_t);

        printf("type random is %d\n", type);
        print_data(&message_t);
        msg_send(msg_id, type, cmd, message_t.data, sizeof(message_t.data));
        printf("msg send success\n\n");
        sleep(2);
    }
    
    msg_deinit(msg_id);

    return 0;
}