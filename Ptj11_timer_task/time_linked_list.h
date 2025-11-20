struct task{
	void (*fun)(char *);
	char arg[8];
};
struct timer{
	int time;
	struct task task;
	struct timer *link;
};
extern struct timer *Thead;
struct timer *get_timer();
void insert_timer(struct task *tskp,int ms);
void tour_timer();
void free_timer();
void app_timer(char *ap);
void task_cmd(char *arg);
void timer_expire(void);
int task_insert(struct task *tskp);
int Task_delete();
void task_init();
void timer_init();
