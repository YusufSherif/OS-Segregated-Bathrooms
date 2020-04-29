//
// Created by Yusuf Sherif on 4/29/20.
//

#ifndef PROJECT_2_BATHROOM_H
#define PROJECT_2_BATHROOM_H

#include <semaphore.h>
#include <stdlib.h>

typedef struct bathroom {
    int capacity;
    int people_present;
    sem_t men_mutex;
    sem_t women_mutex;
    sem_t full_mutex;
    sem_t bathroom_entry_and_exit_mutex; //only one person can enter or exit (i.e. use the door) the bathroom at a time.
} bathroom;

void bathroom_init(bathroom* b, int capacity) {
    b->capacity = capacity;
    b->people_present = 0;

    sem_init(&(b->bathroom_entry_and_exit_mutex),0,1);
    sem_init(&(b->men_mutex),0,1);
    sem_init(&(b->women_mutex),0,1);
    sem_init(&(b->full_mutex),0,capacity);
}

void bathroom_woman_wants_to_enter(bathroom* b) {
    sem_wait(&(b->bathroom_entry_and_exit_mutex));
    sem_wait(&(b->full_mutex));
    sem_wait(&(b->men_mutex));
    b->people_present++;
    sem_post(&(b->bathroom_entry_and_exit_mutex));
}

void bathroom_man_wants_to_enter(bathroom* b) {
    sem_wait(&(b->bathroom_entry_and_exit_mutex));
    sem_wait(&(b->full_mutex));
    sem_wait(&(b->women_mutex));
    b->people_present++;
    sem_post(&(b->men_mutex));
    sem_post(&(b->bathroom_entry_and_exit_mutex));
}

void bathroom_woman_leaves(bathroom* b){
    sem_wait(&(b->bathroom_entry_and_exit_mutex));
    b->people_present--;
    sem_post(&(b->men_mutex));
    sem_post(&(b->full_mutex));
    sem_post(&(b->bathroom_entry_and_exit_mutex));
}

void bathroom_man_leaves(bathroom* b){
    sem_wait(&(b->bathroom_entry_and_exit_mutex));
    b->people_present--;
    sem_post(&(b->women_mutex));
    sem_post(&(b->full_mutex));
    sem_post(&(b->bathroom_entry_and_exit_mutex));
}

void bathroom_destroy(bathroom* b) {
    sem_destroy(&(b->bathroom_entry_and_exit_mutex));
    sem_destroy(&(b->men_mutex));
    sem_destroy(&(b->women_mutex));
    sem_destroy(&(b->full_mutex));
}
#endif //PROJECT_2_BATHROOM_H
