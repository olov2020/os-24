#include "common.h"
#include <semaphore.h>

const char *shar_object = "/posix-shar-object";
int buf_id;
shared_memory *buffer; 

const char *full_sem_name = "/full-semaphore";
sem_t *full;

const char *empty_sem_name = "/empty-semaphore";
sem_t *empty;  

const char *mutex_read_sem_name = "/mutex-read-semaphore";
sem_t *read_mutex; 

const char *mutex_write_sem_name = "/mutex-write-semaphore";
sem_t *write_mutex;

const char *admin_sem_name = "/admin-semaphore";
sem_t *admin; 

void init(void) {
  if ((admin = sem_open(admin_sem_name, O_CREAT, 0666, 0)) == 0) {
    perror("sem_open: Can not create admin semaphore");
    exit(-1);
  }
  
  if ((write_mutex = sem_open(mutex_write_sem_name, O_CREAT, 0666, 1)) == 0) {
    perror("sem_open: Can not create mutex semaphore");
    exit(-1);
  }
  
  if ((read_mutex = sem_open(mutex_read_sem_name, O_CREAT, 0666, 1)) == 0) {
    perror("sem_open: Can not create mutex semaphore");
    exit(-1);
  }
  
  if ((empty = sem_open(empty_sem_name, O_CREAT, 0666, BUF_SIZE)) == 0) {
    perror("sem_open: Can not create free semaphore");
    exit(-1);
  };
  
  if ((full = sem_open(full_sem_name, O_CREAT, 0666, 0)) == 0) {
    perror("sem_open: Can not create busy semaphore");
    exit(-1);
  };
}


void close_common_semaphores(void) {
  if (sem_close(empty) == -1) {
    perror("sem_close: Incorrect close of empty semaphore");
    exit(-1);
  };
  
  if (sem_close(full) == -1) {
    perror("sem_close: Incorrect close of busy semaphore");
    exit(-1);
  };
  
  if (sem_close(admin) == -1) {
    perror("sem_close: Incorrect close of admin semaphore");
    exit(-1);
  };
  
  if (sem_close(write_mutex) == -1) {
    perror("sem_close: Incorrect close of mutex semaphore");
    exit(-1);
  };
  
  if (sem_close(read_mutex) == -1) {
    perror("sem_close: Incorrect close of mutex semaphore");
    exit(-1);
  };
}


void unlink_all(void) {
  if (sem_unlink(mutex_write_sem_name) == -1) {
    perror("sem_unlink: Incorrect unlink of mutex semaphore");
  };
  
  if (sem_unlink(mutex_read_sem_name) == -1) {
    perror("sem_unlink: Incorrect unlink of mutex semaphore");
  };
  
  if (sem_unlink(empty_sem_name) == -1) {
    perror("sem_unlink: Incorrect unlink of empty semaphore");
  };
  
  if (sem_unlink(full_sem_name) == -1) {
    perror("sem_unlink: Incorrect unlink of full semaphore");
  };
  
  if (sem_unlink(admin_sem_name) == -1) {
    perror("sem_unlink: Incorrect unlink of admin semaphore");
  };

  if (shm_unlink(shar_object) == -1) {
    perror("shm_unlink");
  }
}
