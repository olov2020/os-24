#include <time.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <signal.h>

#define BUF_SIZE 10 
typedef struct {
  int store[BUF_SIZE];  // буфер для заполнения ячеек
  int next_write;   // индекс для записи в буфер
  int next_read;   // индекс для чтения из буфера
  int have_reader;  // индикатор наличия читателя
  int reader_pid;  // идентификатор процесса читателя
  int writer_pid;  // идентификатор процесса писателя
} shared_memory;

extern const char* shar_object ;
extern int buf_id;       
extern shared_memory *buffer;  

extern const char *full_sem_name;
extern sem_t *full;  

extern const char *empty_sem_name;
extern sem_t *empty;

extern const char *mutex_write_sem_name;
extern sem_t *write_mutex;

extern const char *mutex_read_sem_name;
extern sem_t *read_mutex; 

extern const char *admin_sem_name;
extern sem_t *admin; 

void init(void);

void close_common_semaphores(void);

void unlink_all(void);
