#include <stdlib.h> 
#include <string.h>
#include <stdio.h> 
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>  
#include <sys/ioctl.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/soundcard.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <zlib.h>

#define AUDIO_DEVICE "/dev/dsp"
#define FRAME_SIZE 320 
#define DATA 7
#define PACKAGE 6

void init_UDP(void);
void sound_set_eth(void);
void quickSort(int *, int, int);
int binarySearch(int *, int, int);

int audio_fd;
int udp_socket; // Сокет UDP
struct sockaddr_in addr_UDP_BR; // Адрес для бродкаста в рамках текущего сеанса

int main ()
{
    init_UDP();
    sound_set_eth();

    char buf[FRAME_SIZE+DATA];     // буфер для изначально полученных данных
    char Ubuf[FRAME_SIZE*DATA];    // буфер для распакованных
    char Fbuf[FRAME_SIZE*PACKAGE]; // буфер для распакованных данных без избытка
	char JB[FRAME_SIZE*PACKAGE];   // буфер для отсортированных данных

    int Fbuf_num[PACKAGE];         // буфер для номеров кадров по мере их поступления
	int JB_num[PACKAGE];           // буфер для номеров кадров, отсортированных по порядку
	
    int Frame_ctr = 0;      // число поступивших кадров
	int Frame_num;          // номер кадра
	int num, pos; 

while(1)    
{
memset(Fbuf, 0, sizeof(Fbuf));
memset(Fbuf_num, 0, sizeof(Fbuf_num));
memset(JB, 0, sizeof(JB));
memset(JB_num, 0, sizeof(JB_num));
memset(Ubuf, 0, sizeof(Ubuf));

for (Frame_ctr = 0; Frame_ctr < PACKAGE; Frame_ctr++) //считаем пакеты
{
memset(buf, 0, sizeof(buf));
memset(Ubuf, 0, sizeof(Ubuf));
int len=sizeof(buf);
unsigned long Ulen=8192;

int recv=recvfrom(udp_socket, (char *)buf, sizeof(buf), 0, (struct sockaddr*) &addr_UDP_BR, &len); // получили сжатое сообщение
if (recv < 0) {perror("recvfrom");}
if ((buf[0] == 90) && (buf[1] == 48))  //проверили на тип Z08
{ 
Frame_num = *((int*)&buf[3]); // получили номера кадра с 3 по 7 байт (4 байта). 
int ret=uncompress((Bytef *)Ubuf, &Ulen, &buf[7], sizeof(buf)); // сжатое сообщение из буфера "buf" распоковывается в буфер "Ubuf" c 7-го байта
memcpy(&Fbuf[Frame_ctr*FRAME_SIZE], &Ubuf, FRAME_SIZE); //записали данные в Fbuf
Fbuf_num[Frame_ctr] = Frame_num;
}
}

memcpy(JB_num, Fbuf_num, sizeof(Fbuf_num)); // перенесли номера в JB_num
quickSort(JB_num, 0, PACKAGE); // отсортировали номера по порядку

for (int i = 0; i < PACKAGE; i++)
{
num = Fbuf_num[i]; 
pos = binarySearch(JB_num, num, PACKAGE); // бинарный поиск соответствующей позиции
memcpy(&JB[pos*FRAME_SIZE], &Fbuf[i*FRAME_SIZE], FRAME_SIZE); // сопоставляем данные по найденной позиции = заполняем JB данными в правильной последовательности
}
int res2 = write(audio_fd, JB, FRAME_SIZE*PACKAGE);  // воспроизводим буфер в файл-устройство
}
close(udp_socket);
return(0);
}

// Функция бинарного посика
int binarySearch(int *numbers, int num, int len)
{
    int left = 0; // задаем левую и правую границы поиска
    int right = len;
    int search = -1; // найденный индекс элемента равен -1 (элемент не найден)
    while (left <= right) // пока левая граница не "перескочит" правую
    {
        int mid = (left + right)/2;  // ищем середину отрезка (округляем в меньшую сторону)
        if (numbers[mid] == num) // если ключевое поле равно искомому
        {
            search = mid; // мы нашли требуемый элемент,
            return search; // выходим из цикла
        }
        else if (numbers[mid] > num) // если искомое ключевое поле меньше найденной середины
        {
            right = mid - 1; // смещаем правую границу, продолжим поиск в левой части
        }
        else
        {
            left = mid + 1; // смещаем левую границу, продолжим поиск в правой части
        }
    }
    return search; 
}

// Функция быстрой сортировки
void quickSort(int *numbers, int left, int right)
{
  int pivot; // разрешающий элемент
  int l_hold = left; //левая граница
  int r_hold = right; // правая граница
  pivot = numbers[left];
  while (left < right) // пока границы не сомкнутся
  {
    while ((numbers[right] >= pivot) && (left < right))
      right--; // сдвигаем правую границу пока элемент [right] больше [pivot]
    if (left != right) // если границы не сомкнулись
    {
      numbers[left] = numbers[right]; // перемещаем элемент [right] на место разрешающего
      left++; // сдвигаем левую границу вправо
    }
    while ((numbers[left] <= pivot) && (left < right))
      left++; // сдвигаем левую границу пока элемент [left] меньше [pivot]
    if (left != right) // если границы не сомкнулись
    {
      numbers[right] = numbers[left]; // перемещаем элемент [left] на место [right]
      right--; // сдвигаем правую границу вправо
    }
  }
  numbers[left] = pivot; // ставим разрешающий элемент на место
  pivot = left;
  left = l_hold;
  right = r_hold;
  if (left < pivot) // Рекурсивно вызываем сортировку для левой и правой части массива
    quickSort(numbers, left, pivot - 1);
  if (right > pivot)
    quickSort(numbers, pivot + 1, right);
}

void sound_set_eth(void)
{
    int format,nchans,rate;
    printf("SETTINGS\n\r"); 
    audio_fd = open(AUDIO_DEVICE, O_RDWR);
    if(audio_fd<0) { printf("no dsp\n\r") ;exit(0); }
    format = AFMT_U8;
    ioctl(audio_fd, SNDCTL_DSP_SETFMT, &format);
    nchans = 1;
    ioctl(audio_fd, SNDCTL_DSP_CHANNELS, &nchans);
    rate = 8000;
    ioctl(audio_fd, SNDCTL_DSP_SPEED, &rate);
    printf("Recording mode: 1000 samples\n\r");
}

void init_UDP(void) // Инициализируем систему рассылки UDP
{
    memset(&addr_UDP_BR, 0, sizeof(addr_UDP_BR));
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0); // инициализировали сокет
    if(udp_socket < 0)
    {
        perror("socket");
        exit(1);
    }

    addr_UDP_BR.sin_family = AF_INET;
    addr_UDP_BR.sin_port = htons(3301); // принимаем на этот порт
    inet_aton("127.0.0.1" , &addr_UDP_BR.sin_addr);
    
    if(bind(udp_socket, (struct sockaddr *)&addr_UDP_BR, sizeof(addr_UDP_BR)) < 0) // привязали сокет
    {
        perror("bind");
        exit(2);
    }
    int aparam=1;
    int a = SO_BROADCAST;
    int rc=setsockopt(udp_socket, SOL_SOCKET, a, &aparam, sizeof(aparam)); // Включили широковещательный режим
    if(rc==-1)
       {
       perror("UDP_PARAM BRODCAST");
       exit(0);
       }
}
