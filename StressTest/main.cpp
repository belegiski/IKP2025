#include <stdio.h>
#include <stdlib.h>
#include "../Common/common.h"

#define DEBUG_PATH "C:\\Users\\beleg\\OneDrive\\Desktop\\wetransfer_projekti_2024-12-28_1915\\IKPProjekat\\Projekat\\IKP\\IKPProject\\x64\\Debug\\"

void stress_tests();
void start_server();
void start_subscriber(int min);
void start_publisher(int min);
int pub_test(char topic[], char msg[], int num);
void clear_input_buffer();

int main() {
    printf("Prepare stress tests...\n\n");

    if (!InitializeWinsock()) {
        printf("WSAStartup failed\n");
        return 1;
    }

    stress_tests();

    WSACleanup();
    return 0;
}

void stress_tests() {
    start_server();

    while (1) {
        char option;
        printf("Choose an option:\n");
        printf("1. 1 publisher and messages\n");
        printf("2. Connect publishers and subscribers\n");
        printf("s. Start subscriber\n");
        printf("p. Start publisher\n");
        printf("q. Quit\n");
        printf("Enter option: ");
        option = getchar();
        clear_input_buffer();

        if (option == 'q') {
            break;
        }
        else if (option == '1') {
            char topic[] = "topic1";
            char msg[] = "message1";
            printf("Number of messages:\n");
            int num;
            scanf_s("%d", &num);
            clear_input_buffer();

            pub_test(topic, msg, num);
        }
        else if (option == '2') {
            int pubs, subs;
            printf("Enter number of publishers: ");
            scanf_s("%d", &pubs);
            clear_input_buffer();
            printf("Enter number of subscribers: ");
            scanf_s("%d", &subs);
            clear_input_buffer();

            for (int i = 0; i < pubs; i++) {
                printf("[%d] ", i + 1);
                start_publisher(TRUE);
            }
            printf("\n\n");
            for (int i = 0; i < subs; i++) {
                printf("[%d] ", i + 1);
                start_subscriber(TRUE);
            }
        }
        else if (option == 's') {
            start_subscriber(FALSE);
        }
        else if (option == 'p') {
            start_publisher(FALSE);
        }
        else {
            printf("Invalid option\n\n");
        }
    }
}

int pub_test(char topic[], char msg[], int num) {
    printf("Preparing publisher...\n\n");

    SOCKET sock = connect(PUB_PORT);
    if (sock == INVALID_SOCKET) {
        printf("Failed to connect to server\n");
        return 1;
    }

    PUB_INFO pi;
    memset(&pi, 0, sizeof(PUB_INFO));
    strcpy_s(pi.topic, topic);
    strcpy_s(pi.msg, msg);

    for (int i = 0; i < num; i++) {
        printf("Publishing message %d for \"%s\": %s\n", i + 1, pi.topic, pi.msg);
        if (send(sock, (char*)&pi, sizeof(PUB_INFO), 0) == SOCKET_ERROR) {
            printf("send() failed: %d\n", WSAGetLastError());
            break;
        }
        Sleep(100);
    }

    strcpy_s(pi.msg, sizeof(pi.msg), "exit");
    send(sock, (char*)&pi, sizeof(PUB_INFO), 0);
    shutdown(sock, SD_BOTH);
    closesocket(sock);
    return 0;
}

void start_server() {
    char server_cmd[1024];
    snprintf(server_cmd, sizeof(server_cmd),
        "cd \"%s\" && start /i PubSubEngine.exe", DEBUG_PATH);

    if (system(server_cmd) == -1) {
        printf("Failed to launch server\n");
        return;
    }
    printf("Server started\n");
}

void start_publisher(int min) {
    char pub_cmd[1024];
    snprintf(pub_cmd, sizeof(pub_cmd),
        "cd \"%s\" && start /i %sPublisher.exe", DEBUG_PATH, min ? "/min " : "");

    if (system(pub_cmd) == -1) {
        printf("Failed to launch publisher\n");
        return;
    }
    printf("Publisher started\n");
}

void start_subscriber(int min) {
    char sub_cmd[1024];
    snprintf(sub_cmd, sizeof(sub_cmd),
        "cd \"%s\" && start /i %sSubscriber.exe", DEBUG_PATH, min ? "/min " : "");

    if (system(sub_cmd) == -1) {
        printf("Failed to launch subscriber\n");
        return;
    }
    printf("Subscriber started\n");
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
