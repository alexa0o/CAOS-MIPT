#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int clnts[1000];
int clnts_cnt = 0;
int d = 0;
char *buffer;
 
void cln()
{
    for (int i = 0; i < clnts_cnt; ++i) {
        if (clnts[i] != -1) {
            shutdown(clnts[i], SHUT_RDWR);
            close(clnts[i]);
        }
    }
    
    close(d);
    free(buffer);
    exit(0);
}

int main(int argc, const char * argv[]) {
    d  = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_IP);
    int c;
    ssize_t s;
    buffer = malloc(1024*sizeof(char));
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(0, &rfds);
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));
    if(bind(d, (struct sockaddr *)&servaddr, sizeof(servaddr))<0){
        perror("bind");
        exit(1);
    }
    if(listen(d, 10)<0){
        perror("listen");
        exit(1);
    }
    signal(SIGINT, cln);
    int size = 1024 - 1;
    int m = 0;
    while(1){
        if((c = accept(d, NULL, NULL)) != -1) {
            clnts[clnts_cnt] = c;
            ++clnts_cnt;
        }
        m = c > m ? c : m;
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        for (int i = 0; i < clnts_cnt; ++i) {
            if (clnts[i] != -1) {
                FD_SET(clnts[i], &rfds);
            }
        }

        int cnt = select(m + 1, &rfds, NULL, NULL, &timeout);
        if (cnt < 0) {
            perror("");
            exit(1);
        }
        if (cnt > 0) {
            for (int i = 0; i < clnts_cnt; ++i) {
                if (FD_ISSET(clnts[i], &rfds)) {
                    s = recv(clnts[i], buffer, size, 0);
                    if (s >= 0) {
                        buffer[s] = '\0';
                    }
                    if (s != 0 && strcmp(buffer, "quit\n")) {
                        for (int j = 0; j < clnts_cnt; ++j) {
                            if (i != j && clnts[j] != -1) {
                                send(clnts[j], buffer, s + 1, 0);
                            }
                        }
                    } else {
                        clnts[i] = -1;
                    }
                }
            }
        }
    }
    return 0;
}
