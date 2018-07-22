#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#define TASK_QUEUE_SIZE 4
#define TASK_SUCCESS 0
#define TASK_FAILURE 1

const int THREAD_COUNT = 2;

typedef enum {
  TASK_NOP,
  TASK_PRINT,
  TASK_LOAD_FILE
} TASK_TYPE;

typedef struct {
  TASK_TYPE type;
  int resultCode;
  void* data;
  void* returnData;
} TASK;


typedef int (*TASK_QUEUE_TaskHandler)(TASK* task);
typedef struct {
  SDL_sem* semaphore;
  SDL_atomic_t readEntry;
  SDL_atomic_t completionCount;
  volatile uint16_t writeEntry;

  volatile bool shutdown;
  TASK_QUEUE_TaskHandler taskHandler;
  SDL_Thread* threads[THREAD_COUNT];

  TASK tasks[TASK_QUEUE_SIZE];
} TASK_QUEUE;

int16_t mask(int16_t v) {
  return v & (TASK_QUEUE_SIZE - 1);
}

int testTaskHandler(TASK* task) {
	if (task->type == TASK_PRINT) {
		printf("%s\n", task->data);
		task->resultCode = TASK_SUCCESS;
		// TODO: Push to SDL Event Queue
	}
  return 0;
}

int TASK_QUEUE_executeTask(void* data) {
  SDL_threadID threadId =  SDL_ThreadID();
  TASK_QUEUE* queue = (TASK_QUEUE*)data;
  while (!queue->shutdown) {
   int16_t originalReadEntry = SDL_AtomicGet(&queue->readEntry);
   if (mask(originalReadEntry) != mask(queue->writeEntry)) {
     if (SDL_AtomicCAS(&queue->readEntry, originalReadEntry, originalReadEntry + 1)) {
       int16_t entryIndex = mask(originalReadEntry + 1);
       SDL_CompilerBarrier();
       TASK task = queue->tasks[mask(entryIndex)];

       queue->taskHandler(&task);
       SDL_AtomicAdd(&queue->completionCount, 1); // Post-increment
     }
    } else {
      printf("%lu: Sleeping... \n", threadId);
      SDL_SemWait(queue->semaphore);
      printf("%lu: Waking... \n", threadId);
    }
  }
  printf("%li: Shutdown \n", threadId);

  return TASK_SUCCESS;
}

bool TASK_QUEUE_isFull(TASK_QUEUE* queue) {
  return mask(queue->writeEntry + 1) == mask(SDL_AtomicGet(&queue->readEntry));
}

bool TASK_QUEUE_isEmpty(TASK_QUEUE* queue) {
  return mask(queue->writeEntry) == mask(SDL_AtomicGet(&queue->readEntry));
}

void TASK_QUEUE_waitForEmptyQueue(TASK_QUEUE* queue) {
  while (!TASK_QUEUE_isEmpty(queue));
}

// PRE: Only the main thread pushes tasks
void TASK_QUEUE_pushTask(TASK_QUEUE* queue, TASK task) {
  assert(queue->shutdown != true);
  while(TASK_QUEUE_isFull(queue));
  assert(!TASK_QUEUE_isFull(queue));

  queue->tasks[mask(queue->writeEntry)] = task;
  SDL_CompilerBarrier();
  ++queue->writeEntry;

  if (SDL_SemValue(queue->semaphore) < THREAD_COUNT) {
    SDL_SemPost(queue->semaphore);
  }
}


// Shutdown pool and block til all threads close
void TASK_QUEUE_close(TASK_QUEUE* queue) {
  // TASK_QUEUE_waitForEmptyQueue(queue);
  printf("shutting down\n");
  queue->shutdown = true;
  // Tidy up our resources
  for (int i = 0; i < THREAD_COUNT; i++) {
    SDL_SemPost(queue->semaphore);
    // SDL_WaitThread(queue->threads[i], NULL);
  }

  // Destroy queue
  SDL_DestroySemaphore(queue->semaphore);
}

void TASK_QUEUE_create(TASK_QUEUE* queue, int poolSize) {
  memset(queue, 0, sizeof(TASK_QUEUE));
  queue->semaphore = SDL_CreateSemaphore(0);

  for (int i = 0; i < poolSize; i++) {
    queue->threads[i] = SDL_CreateThread(TASK_QUEUE_executeTask, NULL, queue);
    SDL_DetachThread(queue->threads[i]);
  }
}

int main(void) {

  // Create the queue
  TASK_QUEUE queue;
  TASK_QUEUE_create(&queue, THREAD_COUNT);
  queue.taskHandler = testTaskHandler;

  TASK task = {0};
  task.type = TASK_PRINT;
  task.data = "Hello world";

  uint64_t total = 0;
  for (uint64_t i = 0; i < 316; i++) {
    TASK_QUEUE_pushTask(&queue, task);
  }
  SDL_Delay(20 /* seconds */ * 1000);

  TASK_QUEUE_close(&queue);
}
