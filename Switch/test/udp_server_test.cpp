#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 4000
#define BUFSIZE 2048


int main() {
    struct sockaddr_in myaddr;              /* our address */
    struct sockaddr_in remaddr;             /* remote address */
    socklen_t addrlen = sizeof(remaddr);    /* length of addresses */
    int recvlen;                            /* # bytes received */
    int fd;                                 /* our socket */
    unsigned char buf[BUFSIZE];             /* receive buffer */


    /* receive socket from system */
    if((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
        perror("cannot create socket");
        return 0;
    }

    /* set address */
    memset((char *)&myaddr, 0, sizeof(myaddr));
    myaddr.sin_family = AF_INET;
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    myaddr.sin_port = htons(PORT);

    /* bind the address */
    if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0) {
        perror("bind failed");
        return 0;
    }

    /* now loop, receiving data and printing what we received */
    for (;;) {
        std::cout << "witing on port" << PORT << std::endl;
        recvlen = recvfrom(fd, buf, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
        printf("received %d bytes\n", recvlen);
        if (recvlen > 0) {
            buf[recvlen] = 0;
            printf("received message: \"%s\"\n", buf);
        }
        if (sendto(fd, buf, recvlen-3, 0, (struct sockaddr *)&remaddr, sizeof(remaddr)) < 0) {
            perror("sendto failed");
            return 0;
        }
    }

    return 0;
}