//
// Created by Yusuf Sherif on 4/29/20.
//

#ifndef PROJECT_2_BATHROOM_H
#define PROJECT_2_BATHROOM_H

#include <semaphore.h>
#include <stdlib.h>

typedef struct bathroom {
    unsigned int capacity;
    int people_present;
    sem_t men_mutex;
    sem_t women_mutex;
    sem_t full_mutex;
    sem_t bathroom_door_mutex; //only one person can enter or exit (i.e. use the door) the bathroom at a time.
    sem_t bathroom_entry_mutex; //only one person can enter the bathroom at a time.
    sem_t bathroom_exit_mutex; //only one person can enter the bathroom at a time.

} bathroom;

void bathroom_init(bathroom* b, unsigned int capacity) {
    b->capacity = capacity;
    b->people_present = 0;

    sem_init(&(b->bathroom_door_mutex), 0, 1);
    sem_init(&(b->bathroom_entry_mutex), 0, 1);
    sem_init(&(b->bathroom_exit_mutex), 0, 1);

    sem_init(&(b->men_mutex),0,1);
    sem_init(&(b->women_mutex),0,1);
    sem_init(&(b->full_mutex),0,capacity);
}

void bathroom_woman_wants_to_enter(bathroom *b, person *next_in_line) {
    sem_wait(&(b->bathroom_entry_mutex));
    sem_wait(&(b->women_mutex));
    sem_wait(&(b->full_mutex));
    sem_wait(&(b->bathroom_door_mutex));
    if(b->people_present==0){
        sem_wait(&(b->men_mutex));
    }
    b->people_present++;
    printf("Woman with id: %d is present\n",next_in_line->id);
    sem_post(&(b->bathroom_door_mutex));
    sem_post(&(b->women_mutex));
    sem_post(&(b->bathroom_entry_mutex));
}

void bathroom_man_wants_to_enter(bathroom* b, person *next_in_line) {
    sem_wait(&(b->bathroom_entry_mutex));
    sem_wait(&(b->men_mutex)); //to wait for women to yield the bathroom.
    sem_wait(&(b->full_mutex)); //Wait until bathroom is not full. If not, signal that it is one more person full.
    sem_wait(&(b->bathroom_door_mutex));
    if(b->people_present==0){ //prevent women from coming in while men exist inside.
        sem_wait(&(b->women_mutex));
    }
    b->people_present++;
    printf("Man with id: %d is present\n",next_in_line->id);
    sem_post(&(b->bathroom_door_mutex));
    sem_post(&(b->men_mutex)); //reached in case of no women, used to undo wait above.
    sem_post(&(b->bathroom_entry_mutex));
}

void bathroom_woman_leaves(bathroom* b, person * p){
    sem_wait(&(b->bathroom_exit_mutex));
    sem_wait(&(b->bathroom_door_mutex));
    b->people_present--;
    if(b->people_present==0) { //allow men to come in if nobody is left.
        sem_post(&(b->men_mutex));
    }
    sem_post(&(b->full_mutex));
    printf("Woman with id: %d left\n",p->id);
    sem_post(&(b->bathroom_door_mutex));
    sem_post(&(b->bathroom_exit_mutex));
}

void bathroom_man_leaves(bathroom* b, person * p){
    sem_wait(&(b->bathroom_exit_mutex));
    sem_wait(&(b->bathroom_door_mutex));
    b->people_present--;
    if(b->people_present==0) {
        sem_post(&(b->women_mutex));
    }
    sem_post(&(b->full_mutex));
    printf("Man with id: %d left\n",p->id);
    sem_post(&(b->bathroom_door_mutex));
    sem_post(&(b->bathroom_exit_mutex));
}

void bathroom_destroy(bathroom* b) {
    sem_destroy(&(b->bathroom_door_mutex));
    sem_destroy(&(b->men_mutex));
    sem_destroy(&(b->women_mutex));
    sem_destroy(&(b->full_mutex));
}
#endif //PROJECT_2_BATHROOM_H
