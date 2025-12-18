//task.h
#define MAX_TASK 16

struct task {
    void (*fun)(char *);
    char arg[64];
};

extern volatile int Task_f, Task_r;

void task_init();
int task_insert(struct task *tskp);
int task_delete(struct task *tskp);
void task_cmd(char *arg);
void task_tc1047(char *arg);
void task_prime(char *ap);
