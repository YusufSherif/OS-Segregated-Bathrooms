//
// Created by Yusuf Sherif on 4/29/20.
//

#ifndef PROJECT_2_DISPATCH_UTILS_H
#define PROJECT_2_DISPATCH_UTILS_H

#include <unistd.h>

#define MAX_WAIT_TIME 1000000

typedef struct dispatch_fns_args_struct {
    int count;
    bathroom_queue * bq;
} dispatch_fns_args_struct;

void dispatch_fns_args_struct_init(dispatch_fns_args_struct* args, bathroom_queue* q, int count) {
    args->bq = q;
    args->count = count;
}

void* dispatchMen_fn(void* pointer) {
    dispatch_fns_args_struct* args = (dispatch_fns_args_struct*) pointer;
    for (int i = 0; i < args->count; ++i) {
        bathroom_queue_enqueue(args->bq, true, i);
        usleep((random() % MAX_WAIT_TIME));
    }
    return NULL;
}

void* dispatchWomen_fn(void* pointer) {
    dispatch_fns_args_struct* args = (dispatch_fns_args_struct*) pointer;
    for (int i = 0; i < args->count; ++i) {
        bathroom_queue_enqueue(args->bq, false, i);
        usleep((random() % MAX_WAIT_TIME));
    }
    return NULL;
}

#endif //PROJECT_2_DISPATCH_UTILS_H
