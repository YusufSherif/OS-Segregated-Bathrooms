//
// Created by Yusuf Sherif on 4/29/20.
//

//
// Created by Yusuf Sherif on 4/29/20.
//

#ifndef PROJECT_2_bq_H
#define PROJECT_2_bq_H

#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>

#include "person.h"

typedef struct bathroom_queue {
    person *queue;
    int nextInLine_index;
    int capacity;
    int size;
    sem_t enqueue_mutex;
    sem_t dequeue_mutex;

} bathroom_queue;

void bathroom_queue_init(bathroom_queue* bq, int capacity) {
    bq->queue = calloc(capacity, sizeof(person));
    bq->nextInLine_index = 0;
    bq->capacity = capacity;
    bq->size = 0;
    sem_init(&(bq->enqueue_mutex), 0, 1);
    sem_init(&(bq->dequeue_mutex), 0, 0);
}

void bathroom_queue_enqueue(bathroom_queue* bq, bool isMan, int id) {
    sem_wait(&(bq->enqueue_mutex));
    person_init(&bq->queue[bq->size], id, isMan);
    bq->size++;
    sem_post(&(bq->dequeue_mutex)); //serves the purpose of notifying the dequeue mechanism that a person arrived.
    sem_post(&(bq->enqueue_mutex));

}

person*  bathroom_queue_dequeue(bathroom_queue* bq) {
    //Detect if there is no one in the queue. If so, then hang this function until one person gets enqueued the go on.
    //Otherwise, the function goes on without waiting.
    if(bq->nextInLine_index == bq->size) {
        sem_wait(&(bq->dequeue_mutex));
    }
    return &bq->queue[bq->nextInLine_index++];
}

void bathroom_queue_destroy(bathroom_queue* bq) {
    free(bq->queue);
    sem_destroy(&(bq->enqueue_mutex));
}

#endif //PROJECT_2_bq_H
