#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stddef.h>
#include <zlib.h>
#include <string.h>
#include <assert.h>
#include <zmq.h>
#define SERVER "tcp://127.0.0.1:5563" // Описание адресса и порта сервера
int main (void)
{
void *context, *socketzmq;
int ret;
 char msg[8192], Umsg[8192];
 context=zmq_init (2); if(context==NULL) exit(0); // создали контекст.
 socketzmq = zmq_socket (context, ZMQ_SUB); // установили тип контекста- чтение данных - подписчик.
 assert (socketzmq); 

 ret = zmq_connect (socketzmq, SERVER); // подключились по сети
 if (ret < 0)
  {
    perror("Error Zmq sock connect");
    return 1;
  }

ret = zmq_setsockopt (socketzmq, ZMQ_SUBSCRIBE, "RU", 2); // подписка на новости из сокет. Подписывается на сообщения, которые начинаются с символов RU. Прочие сообщения игнорируются.
zmq_setsockopt(socketzmq, ZMQ_SUBSCRIBE, "x", 1); // Подписка на сжатый поток. Ключ "x" длинной 1
assert (ret == 0); //и можно получать сообщения.

int rc = zmq_msg_init (&msg);
assert (rc == 0);

memset(msg,0,sizeof(msg));
rc = zmq_recv (socketzmq, msg, 8192, 0);
if (rc > 0)
   {
    printf("\n----- Полученно сообщение -----\n%s\n----- Конец сообщения -----\n", msg); // Вывод полученного сообщения
   }
    memset(msg,0,sizeof(msg));
    memset(Umsg,0,sizeof(Umsg));

    rc = zmq_recv (socketzmq, msg, 8192, 0);
    if (rc > 0)
    {
        unsigned long Ulen=8192; // Переменная с максимально допустимым размером буфера. Ее использование исключает переполнение.
        printf("\n----- Получено сжатое сообщение -----\n%s\n-----Конец сжатого соощения -----\n", msg); // Полученное сжатое сообщение выводится в неизмененном виде
        ret=uncompress(Umsg, &Ulen, msg, rc)==Z_OK; // Сжатое сообщение из буфера "message" распоковывается в буфер "messageU". "rc" - полученная ранее от "zmq_recv" длинна принятого сообщения.
        printf("\n----- Получено распакованное сообщение -----\n%s\n----- Конец распакованного сообщения -----\n", Umsg); // Вывод распокованного сообщения
    }
 zmq_close (socketzmq);
 zmq_ctx_destroy(context);
 return 0;
}
