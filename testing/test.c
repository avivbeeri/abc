#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#if 1
#include <SDL2/SDL.h>
#endif

#define ABC_FIFO_IMPL
#include "../ABC_fifo.h"

// Example program

typedef enum {
  TASK_NOP,
  TASK_PRINT,
  TASK_LOAD_FILE
} TASK_TYPE;

int testTaskHandler(ABC_TASK* task) {
	if (task->type == TASK_PRINT) {
		printf("%s\n", task->data);
		task->resultCode = 0;
		// TODO: Push to SDL Event Queue
	}
  return 0;
}

int main(void) {

  // Create the queue
  ABC_FIFO queue;
  ABC_FIFO_create(&queue);
  queue.taskHandler = testTaskHandler;

  ABC_TASK task = {0};
  task.type = TASK_PRINT;
  task.data = "Hello world";

  uint64_t total = 0;

  for (uint64_t i = 0; i < 16*1000; i++) {
    ABC_FIFO_pushTask(&queue, task);
  }
  // SDL_Delay(20 /* seconds */ * 1000);

  ABC_FIFO_waitForEmptyQueue(&queue);
  ABC_FIFO_close(&queue);
}

