#include "utility.h"

int main(int argc, char *argv[]) {
    // Server IP + port
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // create listen socket
    int listerner = socket(PF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("listerner")
        exit(-1);
    }
    printf("listern socket created.\n");
    // bind address
    if (bind(listener, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind error");
        exit(-1);
    }
    // open listening
    int ret = listen(listener, 5);
    if (ret < 0) {
        perror("listen error");
        exit(-1);
    }
    printf("Start to listen: %s\n", SERVER_IP);
    // create event table in kernel
    int epfd = epoll_create(EPOLL_SIZE);
    if (epfd < 0) {
        perror("epfd error");
        exit(-1);
    }
    printf("epoll created, epollfd = %d\n", epfd);
    static struct epoll_event evnets[EPOLL_SIZE];
    // add new event to kernel event table
    addfd(epfd, listener, true);
    // main loop
    while(1) {
        //epoll_events_count: the number of the ready events
        int epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        if (epoll_events_count < 0) {
            perror("epoll failure");
            break;
        }

        printf("epoll_events_count = %d\n", epoll_events_count);
        // process current all epoll_events_count ready events.
        for (int i = 0; i < epoll_events_count; ++i) {
            int sockfd = events[i].data.fd;
            // new user connection
            if (sockfd == listener) {
                struct sockaddr_in client_address;
                socklen_t client_addrLength = sizeof(struct sockaddr_in);
                int clinetfd = accept(listener, (struct sockaddr*)&client_address, &client_addrLength);
                printf("client conection from: %s : % d(IP : port), clientfd = %d\n",
                            inet_ntoa(client_address.sin_addr),
                            ntohs(client_address.sin_port),
                            clientfd);
                addfd(epfd, clientfd, true);

                // using linklist to save the client connection in the server
                clients_list.push_back(clientfd);
                printf("Add new clientfd = %d to epoll\n", clientfd);
                printf("Now there are %d clients in the chat room\n", (int)clients_list.size());

                // server send welcome message
                printf("welcome message\n");
                char message[BUFF_SIZE];
                bzero(message, BUF_SIZE);
                sprintf(message, SERVER_WELCOME, clientfd);
                int ret = send(clientfd, message, BUF_SIZE, 0);
                if (ret < 0) {
                    perror("send error");
                    exit(-1);
                }
            } else { // process the message that from client, and broadcast it, to make others receive the message 
                int ret = sendBroadcastmessage(sockfd);
                if (ret < 0) { 
                    perror("error");
                    exit(-1);
                }
            }
        }
    }
    close(listener); // close socket
    close(epfd);    // close kernel
    return 0;
}
