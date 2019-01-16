extern uint32 test_and_set(uint32 *ptr, uint32 new_value) ; 

typedef uint32 sl_lock_t;

struct queue_node{
	int32 pid;
	struct queue_node *next;
};

typedef struct lock_t{
	uint32	flag;
	uint32	guard;
	qid16	queue;
}lock_t;

typedef struct al_lock_t{
	uint32	flag;
	uint32	guard;
	uint32	lock_id;
	qid16	queue;
}al_lock_t;

typedef struct pi_lock_t{
	uint32	flag;
	uint32	guard;
	qid16	queue;
	pid32	lock_held_proc;
	pri16	lock_held_proc_o_prio;


}pi_lock_t;

extern uint8 allocation[NPROC][MAXLOCK];
extern uint8 request[NPROC][MAXLOCK];
extern uint8 available[MAXLOCK];
extern uint8 work[MAXLOCK];
extern uint8 finish[NPROC];
extern uint8 lock_counter;



extern void sl_init(sl_lock_t *l);
extern void sl_lock(sl_lock_t *l);
extern void sl_unlock(sl_lock_t *l);

extern void init_lock(lock_t *l);
extern void lock(lock_t *l);
extern void unlock(lock_t *l);

extern void al_init(al_lock_t *l);
extern void al_lock(al_lock_t *l);
extern void al_unlock(al_lock_t *l);
extern bool8 al_trylock(al_lock_t *l);
extern void deadlock_detector(void);

extern void pi_init(pi_lock_t *l);
extern void pi_lock(pi_lock_t *l);
extern void pi_unlock(pi_lock_t *l);

