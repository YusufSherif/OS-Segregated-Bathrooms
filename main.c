#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#include "bathroom_queue.h"
#include "dispatch_utils.h"
#include "bathroom.h"

typedef struct goto_bathroom_args {
    bathroom* b; person* p;
} goto_bathroom_args;

void * goto_bathroom(void *pointer){
    goto_bathroom_args* args = (goto_bathroom_args*) pointer;
    args->p->isMan?bathroom_man_wants_to_enter(args->b):bathroom_woman_wants_to_enter(args->b);
    printf(args->p->isMan?"Man with id: %d is present\n":"Woman with id: %d is present\n",args->p->id);
    args->p->isMan?bathroom_man_leaves(args->b):bathroom_woman_leaves(args->b);
    return NULL;
}

int main() {
    srandom(time(NULL));

    int m = 100, n = 100;
    int bathroom_capacity = 5;

    bathroom b; bathroom_init(&b, bathroom_capacity);

    bathroom_queue q; bathroom_queue_init(&q, n+m);
    //size = n+m because worst case is that all people are spontaneously waiting in line. Which realistically makes
    //sense as a bathroom queue in real life is not limited to a set number, rather if the circumstances allow for
    //some weird reason, all of humanity may queue at one bathroom.

    pthread_t women[n];
    pthread_t men[m];

    pthread_t dispatchMen, dispatchWomen;

    dispatch_fns_args_struct men_dispatch_args;
    dispatch_fns_args_struct_init(&men_dispatch_args, &q, m);

    dispatch_fns_args_struct women_dispatch_args;
    dispatch_fns_args_struct_init(&women_dispatch_args, &q, n);

    pthread_create(&dispatchMen,NULL,dispatchMen_fn,&men_dispatch_args);
    pthread_create(&dispatchWomen,NULL,dispatchWomen_fn,&women_dispatch_args);

    goto_bathroom_args gotoBathroom_args; gotoBathroom_args.b = &b;
    for (int i = 0; i < n+m; ++i) {
        person* next_in_line = bathroom_queue_dequeue(&q);
        gotoBathroom_args.p = next_in_line;
        pthread_create(
                next_in_line->isMan?&men[next_in_line->id]:&women[next_in_line->id],
                NULL,
                goto_bathroom,
                &gotoBathroom_args);
    }

    pthread_join(dispatchMen,NULL);
    pthread_join(dispatchWomen,NULL);

    for (int j = 0; j < m; ++j) {
        pthread_join(men[j],NULL);
    }
    for (int j = 0; j < n; ++j) {
        pthread_join(women[j],NULL);
    }

    bathroom_queue_destroy(&q);
    return 0;
}
