#include <xinu.h>

void init_lock(lock_t *l){

	l->flag = 0;
	l->guard = 0;
	l->queue = newqueue();
}


void lock(lock_t *l){

	while(test_and_set(&l->guard,1)==1)
		;
	if(l->flag==0){
		l->flag = 1;
		l-> guard = 0;
	}
	else{
		//add thread to queue
		enqueue(currpid,l->queue);
		l->guard = 0;
		//park
		proctab[currpid].prstate = PR_WAIT;
		resched();				
	}
}

void unlock(lock_t *l){

	while(test_and_set(&l->guard,1)==1)
		;
	if(isempty(l->queue)){
		l->flag = 0;
	}
	else{
		//unpark
		int32 id;
		id = dequeue(l->queue);
		proctab[id].prstate = PR_READY;
		insert(id, readylist, proctab[id].prprio);
	}

	l->guard = 0;
}