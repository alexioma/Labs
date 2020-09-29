#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>

#define PORT 12345
#define BUFFER_SIZE 1024
 
 int udp_socket=-1;
 char buf_send[BUFFER_SIZE];       // Буфер отправки
 char buf_receive[BUFFER_SIZE];    // Буфер приёма 
 unsigned char xorbyte=0;

void main()
{
 struct sockaddr_in sockaddr; //Структура для хранения адреса

 udp_socket = socket(AF_INET, SOCK_DGRAM, 0); // Инициализация сокета
    if(udp_socket < 0)
    {
        perror("socket");
        exit(1);
    }

 sockaddr.sin_family = AF_INET;
 sockaddr.sin_port = htons(PORT);
 sockaddr.sin_addr.s_addr = INADDR_ANY;

 //Заполнение буфера
 buf_send[0]=129;// ключ
 buf_send[1]=10; // день
 buf_send[2]=04; // месяц
 buf_send[3]=3; // номер бригады
 sprintf(&buf_send[4],"Kalashnikova Alexandra"); //Фамилия Имя

 int len=sizeof(buf_send);
 for(int i=0;i<len;i++) xorbyte=xorbyte^buf_send[i];
 buf_send[81]=(char)xorbyte; 

 //Отправка пакета с проверкой на кол-во байт, что удалось отправить за раз

  sendto(udp_socket, (const char *)buf_send, strlen(buf_send), 0 , (const struct sockaddr*) &sockaddr, sizeof(sockaddr));

  int buf_size=sizeof(buf_send);

 //Ожидания ответа
 while(1){
  int recv=recvfrom(udp_socket, buf_receive, BUFFER_SIZE, 0, (struct sockaddr*) &sockaddr, &buf_size);
  if (recv!=-1){
   printf("recv bytes: %d\n", recv); // Количество полученных байт
   printf("buf: %s\n", buf_receive); // Содержимое пакета (секретное слово)
   break;
  }
 }
}
