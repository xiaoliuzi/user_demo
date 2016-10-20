#include "utility.h"

int main(int argc, char *argv[]) {

    // server ip + port that to be connected by client
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = PF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // create socket
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("sock error");
        exit(-1);
    }

    // connect server
    if (connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("connect error");
        exit(-1);
    }

    // create pipe, fd[0] is used for parent process to read,
    // fd[1] is used for children process to write
    int pipe_fd[2];
    if (pipe(pipe_fd) < 0) {
        perror("pipe error");
        exit(-1);
    }

    // create epoll
    int epfd = epoll_create(EPOLL_SIZE);
    if (epfd < 0) {
        perror("epfd error");
        exit(-1);
    }
    
    static struct epoll_event events[2];
    // add sock and pipe read descriptor to kernel event table
    addfd(epfd, sock, true);
    addfd(epfd, pipe_fd[0], true);

    // determine whether client work normally
    bool isClientwork = true;
    
    // chat message buffer
    char message[BUF_SIZE];

    // Fork
    int pid = fork();
    if (pid < 0) {
        perror("fork error");
        exit(-1);
    } else if (pid == 0) { // children process
        // children process write data to pipe only, so close read.
        close(pipe_fd[0]);
        printf("Please input 'exit' to exit the chat root\n");

        while (isClientwork) { 
            bzero(&message, BUF_SIZE);
            fgets(message, BUF_SIZE, stdin);

            // client input exit to quit
            if (strncasecmp(message, EXIT, strlen(EXIT)) == 0) {
                isClientwork = 0;
            } else {// children process write message into pipe
                if (write(pipe_fd[1], message, strlen(message) - 1) < 0) {
                    perror("fork error");
                    exit(-1);
                }
            }
        }
    } else { // pid > 0  is the parent process
        // parent process read data from pipe only, so close write
        close(pipe_fd[1]);

        // main loop (epoll_wait)
        while (isClientwork) {
            int epoll_events_count = epoll_wait(epfd, events, 2, -1);
            // process ready events
            for (int i = 0; i < epoll_events_count; ++i) {
                bzero(&message, BUF_SIZE);

                // message from server
                if (events[i].data.fd == sock) {
                    // accept server message
                    int ret = recv(sock, message, BUF_SIZE, 0);

                    // ret == 0 close server
                    if (ret == 0) {
                        printf("Server closed connection: %d\n", sock);
                        close(sock);
                        isClientwork = 0;
                    } else
                        printf("%s\n", message);

                } else { // children process write event happened,
                         // parent process process it and send it to server
                    // parent process read data from pipe
                    int ret = read(events[i].data.fd, message, BUF_SIZE);

                    // ret == 0
                    if (ret == 0)
                        isClientwork = 0;
                    else // send message to server
                      send(sock, message, BUF_SIZE, 0);

                }
            } // end of for
        } // end of while
    }
    if (pid) { 
        // close parent process and sock
        close(pipe_fd[0]);
        close(sock);
    } else {
        // close children process
        close(pipe_fd[1]);

    }
    return 0;
}
