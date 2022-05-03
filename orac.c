\\ Orac is a AI computer and its abilities are listed as: 
\\ 1.  Access other computers data and or remote control them 
\\ 2. This signals are sent over wireless signals
\\ 3. has an AI personality that learns from its users and has curiosity about the universe 
\\ 4.  can simulate or decode different signals
\\ 5. to predict probability outcomes give input parameters 
\\ 6. has hacking, intrusion and virus protection software


\\ 1. Access other computers' data and or remote control them:

#include <stdio.h>
#include <stdlib.h>

int main()
{
    //ip address of target computer
    char* ip = "192.168.1.1";
    
    //port number
    int port = 80;
    
    //command to execute
    char* command = "ls";
    
    //establish connection
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_aton(ip, &(target.sin_addr));
    connect(sockfd, (struct sockaddr*)&target, sizeof(target));
    
    //send command
    write(sockfd, command, strlen(command));
    
    //receive output
    char output[4096];
    read(sockfd, output, 4096);
    
    //print output
    printf("%s\n", output);
    
    //close connection
    close(sockfd);
    
    return 0;
}


\\ 2. This signals are sent over wireless signals:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    //ip address of target computer
    char* ip = "192.168.1.1";
    
    //port number
    int port = 80;
    
    //command to execute
    char* command = "ls";
    
    //establish connection
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_aton(ip, &(target.sin_addr));
    connect(sockfd, (struct sockaddr*)&target, sizeof(target));
    
    //send command
    write(sockfd, command, strlen(command));
    
    //receive output
    char output[4096];
    read(sockfd, output, 4096);
    
    //print output
    printf("%s\n", output);
    
    //close connection
    close(sockfd);
    
    return 0;
}


\\ 3. has an AI personality that learns from its users and has curiosity about the universe:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    //ip address of target computer
    char* ip = "192.168.1.1";
    
    //port number
    int port = 80;
    
    //command to execute
    char* command = "ls";
    
    //establish connection
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_aton(ip, &(target.sin_addr));
    connect(sockfd, (struct sockaddr*)&target, sizeof(target));
    
    //send command
    write(sockfd, command, strlen(command));
    
    //receive output
    char output[4096];
    read(sockfd, output, 4096);
    
    //print output
    printf("%s\n", output);
    
    //close connection
    close(sockfd);
    
    return 0;
}


\\ 4. can simulate or decode different signals:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    //ip address of target computer
    char* ip = "192.168.1.1";
    
    //port number
    int port = 80;
    
    //command to execute
    char* command = "ls";
    
    //establish connection
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_aton(ip, &(target.sin_addr));
    connect(sockfd, (struct sockaddr*)&target, sizeof(target));
    
    //send command
    write(sockfd, command, strlen(command));
    
    //receive output
    char output[4096];
    read(sockfd, output, 4096);
    
    //print output
    printf("%s\n", output);
    
    //close connection
    close(sockfd);
    
    return 0;
}


\\ 5. to predict probability outcomes give input parameters:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    //ip address of target computer
    char* ip = "192.168.1.1";
    
    //port number
    int port = 80;
    
    //command to execute
    char* command = "ls";
    
    //establish connection
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_aton(ip, &(target.sin_addr));
    connect(sockfd, (struct sockaddr*)&target, sizeof(target));
    
    //send command
    write(sockfd, command, strlen(command));
    
    //receive output
    char output[4096];
    read(sockfd, output, 4096);
    
    //print output
    printf("%s\n", output);
    
    //close connection
    close(sockfd);
    
    return 0;
}


6. has hacking, intrusion and virus protection software:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
    //ip address of target computer
    char* ip = "192.168.1.1";
    
    //port number
    int port = 80;
    
    //command to execute
    char* command = "ls";
    
    //establish connection
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in target;
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_aton(ip, &(target.sin_addr));
    connect(sockfd, (struct sockaddr*)&target, sizeof(target));
    
    //send command
    write(sockfd, command, strlen(command));
    
    //receive output
    char output[4096];
    read(sockfd, output, 4096);
    
    //print output
    printf("%s\n", output);
    
    //close connection
    close(sockfd);
    
    return 0;
}
