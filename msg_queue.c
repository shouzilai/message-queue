#include "msg_queue.h"

int msg_init(int *msg_id)
{
    int ret = 0;
    key_t key = ftok("./", 121);
    if (-1 == key) {
        printf("[ERROR] msg queue ftok\n");
    }

    ret = msgget(key, IPC_CREAT | 0777);
    if (-1 == ret) {
        perror("msg queue init msgget");
        return -1;
    }

    *msg_id = ret;
    printf("msg init\n");
    return 0;
}

int msg_deinit(int msg_id)
{
    int ret = 0;

    ret = msgctl(msg_id, IPC_RMID, NULL);
    if (-1 == ret) {
        perror("msg queue deinit msgctl");
        return -1;
    }

    return 0;
}

int msg_recive(int msg_id, msg_queue_p recv_msg)
{
    int ret = 0;
    printf("msg queue recive blocking......msg id is %d\n", msg_id);

    memset(recv_msg->data, 0x0, MSG_QUEUE_DATA_SIZE);
    ret = msgrcv(msg_id, recv_msg, sizeof(recv_msg->data), 0, 0);
    if (-1 == ret) {
        perror("msg queue recive msgrcv");
        return -1;
    }

    return ret;
}

int msg_send(int msg_id, int cmd, msg_type type, uint8_t *data, int data_len)
{
    int ret = 0;
    msg_queue_t send_msg;
    if (data == NULL) {
        printf("data is NULL!!!\n");
        return -1;
    }

    for (int i = 0; i < data_len; i++) {
        send_msg.data[i] = data[i];
    }
    send_msg.cmd = cmd;
    send_msg.type = type;

    ret = msgsnd(msg_id, &send_msg, data_len, 0);
    if (ret < 0) {
        perror("msg queue send msgsnd");
        return -1;
    }

    return 0;
}