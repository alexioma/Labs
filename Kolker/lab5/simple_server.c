#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>

// Функция, корректно отправляющая данные в сокет
int sendallbuf(int s, char *buf, int len, int flags)
{
    int total = 0;
    int n;
    while(total < len)
    {
        n = send(s, buf+total, len-total, flags);
        if(n == -1) { break; }
        total += n;
    }
    if(n==-1) return -1;
    else return total;
}

int main()
{
    int serversock;
    int clientsock;

    serversock = socket(AF_INET6,SOCK_STREAM,0); // Создание сокета

    struct sockaddr_in6 serveraddr; // Структура для хранения адреса

    serveraddr.sin6_family = AF_INET6;
    serveraddr.sin6_port = htons(31337);
    serveraddr.sin6_addr = in6addr_any;

     if(serversock == -1)
    {
        printf("Create server socket error\n");
        return -1;
    }
// Освобождение порта
    int opt = 1;
    if(setsockopt(serversock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))==-1)
    {
	printf("setsockopt failed\n");
    }
//  Привязка сокета
    if (bind(serversock, (struct sockaddr*)&serveraddr, sizeof(serveraddr))==-1)
    {
        printf("Bing socket error\n");
        close(serversock);
        return -1;
    }
// Прослушивание порта и ожидание соединения
    if(listen(serversock,10)==-1)
    {
        printf("Listen error\n");
        close(serversock);
        return -1;
    }
// Установление соединения от клиента
    clientsock = accept(serversock,0,0);

    if(clientsock == -1)
    {
        printf("Accept error\n");
        close(serversock);
        return -1;
    }

    char recvdata[1024];
    char senddata[1024];
// Время сервера
    time_t current_time;
    current_time = time(NULL);
    sprintf(senddata,"%ld (UNIX-время сервера) \n",current_time);

    int len = strlen(senddata);
// Отправка клиенту
    if(sendallbuf(clientsock,senddata,len,0) == -1)
    {
        printf("Send time error\n");
        shutdown(clientsock,2);
        close(clientsock);
        close(serversock);
        return -1;
    }
// Получение клиентом
    int res;
    res = recv(clientsock,recvdata,1024,0);
    if(res == -1)
    {
        printf("recv error\n");
        shutdown(clientsock,2);
        close(clientsock);
        close(serversock);
        return -1;
    }
    else if (res == 0)
    {
        printf("No data from client\n");
        shutdown(clientsock,2);
        close(clientsock);
        close(serversock);
        return -1;
    }
 // Время клиента и подсчет разницы
    time_t client_time = atol(recvdata);
    current_time = time(NULL);

    time_t diff_time = current_time - client_time;
    sprintf(senddata,"%ld (разница) \n",diff_time);
    len = strlen(senddata);    
    printf("%ld разница \n",diff_time);
// Отправка
    if(sendallbuf(clientsock,senddata,len,0) == -1)
    {
        printf("Send client error\n");
        shutdown(clientsock,2);
        close(clientsock);
        close(serversock);
        return -1;
    }

    shutdown(clientsock,2);
    close(clientsock);
    close(serversock);
    return 0;
}
