#ifndef HELLO1_H
#define HELLO1_H

struct hello_entry {
    struct list_head list;
    ktime_t start_time;
    ktime_t end_time;
};

void print_hello(int count);

#endif
