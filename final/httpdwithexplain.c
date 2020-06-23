/*copy from https://gist.github.com/laobubu/d6d0e9beb934b60b2e552c2d03e1409e */


#include "httpd.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>

#define CONNMAX 1000

static int listenfd, clients[CONNMAX];
static void error(char *);
static void startServer(const char *);
static void respond(int);

typedef struct { char *name, *value; } header_t;
static header_t reqhdr[17] = { {"\0", "\0"} };
static int clientfd;

static char *buf;

void serve_forever(const char *PORT)
{
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    char c;    
    
    int slot=0;
    
    printf(
            "Server started %shttp://127.0.0.1:%s%s\n",
            "\033[92m",PORT,"\033[0m"
            );

    //把所有元素設為-1，代表沒有連線
    int i;
    for (i=0; i<CONNMAX; i++)
        clients[i]=-1;
    startServer(PORT);
    
    // Ignore SIGCHLD to avoid zombie threads
    signal(SIGCHLD,SIG_IGN);

    // ACCEPT connections
    while (1)
    {
        addrlen = sizeof(clientaddr);
        clients[slot] = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);   //接受連線請求

        if (clients[slot]<0)    //接收失敗，輸出錯誤訊息
        {
            perror("accept() error");
        }
        else    //接收成功
        {
            if ( fork()==0 )    //使用 fork 來動作
            {
                respond(slot);  //呼叫 respond 函式
                exit(0);
            }
        }

        while (clients[slot]!=-1) slot = (slot+1)%CONNMAX;  //當 slot 不等於-1時繼續動作
    }
}

//start server
void startServer(const char *port)
{
    struct addrinfo hints, *res, *p;

    // getaddrinfo for host 
    memset (&hints, 0, sizeof(hints));  //確保 struct 為空
    hints.ai_family = AF_INET;  //指定為 IPv4
    hints.ai_socktype = SOCK_STREAM;    //TCP stream sockets
    hints.ai_flags = AI_PASSIVE;    //自動填好IP
    if (getaddrinfo( NULL, port, &hints, &res) != 0)    //偵錯
    {
        perror ("getaddrinfo() error");
        exit(1);
    }
    // socket and bind
    /*不斷執行，直到成功綁定port*/
    for (p = res; p!=NULL; p=p->ai_next)
    {
        int option = 1;
        listenfd = socket (p->ai_family, p->ai_socktype, 0);    //使用 getaddrinfo 取得的資訊來建立 socket

        /*SO_REUSEADDR 若沒有 listening socket 綁定到這個 port 就允許其他 socket bind 這個port*/
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        if (listenfd == -1) continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;  //bind成功時回傳0
    }
    if (p==NULL)
    {
        perror ("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);  //使用完畢時釋放這個資料結構的空間

    // listen for incoming connections
    if ( listen (listenfd, 1000000) != 0 )
    {
        perror("listen() error");
        exit(1);
    }
}


// get request header
/*
example
GET / HTTP/1.1
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.138 Safari/537.36
*/
char *request_header(const char* name)
{
    header_t *h = reqhdr;
    while(h->name) {
        if (strcmp(h->name, name) == 0) return h->value;
        h++;
    }
    return NULL;
}

//client connection
/*/r/n是封包裡頭換行的寫法*/
void respond(int n)
{
    int rcvd, fd, bytes_read;
    char *ptr;

    buf = malloc(65535);    //給buff參數分配記憶體
    rcvd=recv(clients[n], buf, 65535, 0);   //測試連線狀態

    if (rcvd<0)    //接受到的值<0，代表連線錯誤
        fprintf(stderr,("recv() error\n"));
    else if (rcvd==0)    //接受到的值為0，代表連線被關閉
        fprintf(stderr,"Client disconnected upexpectedly.\n");
    else    //接受到訊息
    {
        buf[rcvd] = '\0'; //不是很明白做什麼，但是猜測是在接受到訊息之結尾處加上\0方便處理

        method = strtok(buf,  " \t\r\n");   //用strtok分隔接收到的訊息
        uri    = strtok(NULL, " \t");
        prot   = strtok(NULL, " \t\r\n"); 

        fprintf(stderr, "\x1b[32m + [%s] %s\x1b[0m\n", method, uri);
        
        if (qs = strchr(uri, '?'))  //處理uri：以?開始分隔
        {
            *qs++ = '\0'; //分隔
        } else {
            qs = uri - 1; //使用空字串
        }

        header_t *h = reqhdr;
        char *t, *t2;
        while(h < reqhdr+16) {
            char *k,*v,*t;
            k = strtok(NULL, "\r\n: \t"); if (!k) break;
            v = strtok(NULL, "\r\n");     while(*v && *v==' ') v++;
            h->name  = k;
            h->value = v;
            h++;
            fprintf(stderr, "[H] %s: %s\n", k, v);
            t = v + 1 + strlen(v);      //尋找結尾+2的位置
            if (t[1] == '\r' && t[2] == '\n') break;       //若t指到\n，脫離迴圈
        }
        t++; // now the *t shall be the beginning of user payload
        t2 = request_header("Content-Length"); // and the related header if there is  
        payload = t;
        payload_size = t2 ? atol(t2) : (rcvd-(t-buf));

        // bind clientfd to stdout, making it easier to write
        clientfd = clients[n];
        dup2(clientfd, STDOUT_FILENO);
        close(clientfd);

        // call router
        route();

        // tidy up
        fflush(stdout);
        shutdown(STDOUT_FILENO, SHUT_WR);
        close(STDOUT_FILENO);
    }

    //Closing SOCKET
    shutdown(clientfd, SHUT_RDWR);         //All further send and recieve operations are DISABLED...
    close(clientfd);
    clients[n]=-1;
}