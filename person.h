//
// Created by Yusuf Sherif on 4/29/20.
//

#ifndef PROJECT_2_PERSON_H
#define PROJECT_2_PERSON_H

#include <stdbool.h>

typedef struct person {
    int id;
    bool isMan;
} person;

void person_init(person* p, int id, bool isMan) {
    p->id = id;
    p->isMan = isMan;
}

#endif //PROJECT_2_PERSON_H
