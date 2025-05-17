/* server_chat.c */ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080

typedef struct {
    int id;
    char name[50];
    float score;
} Student;

int client_sock;

void* receiver_thread(void* arg) {
    Student recv_data;
    while (1) {
        int bytes = recv(client_sock, &recv_data, sizeof(Student), 0);
        if (bytes <= 0) {
            printf("Client disconnected.\n");
            break;
        }
        printf("\n[From Client] ID=%d, Name=%s, Score=%.2f\n> ",
               recv_data.id, recv_data.name, recv_data.score);
        fflush(stdout);
    }
    return NULL;
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    Student send_data;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_fd, 1);

    printf("Server waiting for connection...\n");
    client_sock = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    printf("Client connected.\n");

    pthread_t recv_tid;
    pthread_create(&recv_tid, NULL, receiver_thread, NULL);

    while (1) {
        printf("> Enter ID: ");
        scanf("%d", &send_data.id);
        printf("  Enter Name: ");
        scanf(" %[^\n]", send_data.name);
        printf("  Enter Score: ");
        scanf("%f", &send_data.score);

        send(client_sock, &send_data, sizeof(Student), 0);
    }

    close(server_fd);
    return 0;
}
