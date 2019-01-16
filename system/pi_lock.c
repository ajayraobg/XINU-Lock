#include <xinu.h>

bool8 is_inready(pid32 i){
	pid32 curr;
	curr = firstid(readylist);
	while(curr!=queuetail(readylist)){
		if(curr == i)
			return 1;
		curr = queuetab[curr].qnext;
	}
	return 0;
}

void pi_init(pi_lock_t *l){

	l->flag = 0;
	l->guard = 0;
	l->queue = newqueue();
	l->lock_held_proc = 0;
	l->lock_held_proc_o_prio = -1;
}

pi_lock(pi_lock_t *l){

	while(test_and_set(&l->guard,1)==1)
		;
	if(l->flag==0){
		l->flag = 1;
		l-> guard = 0;
		l->lock_held_proc = currpid;
		l->lock_held_proc_o_prio = proctab[currpid].prprio;
	}
	else{
		//add thread to queue
		enqueue(currpid,l->queue);
		l->guard = 0;
		//park
		proctab[currpid].prstate = PR_WAIT;

		if(proctab[l->lock_held_proc].prprio<proctab[currpid].prprio){
			if(is_inready(l->lock_held_proc)){
				getitem(l->lock_held_proc);
				proctab[l->lock_held_proc].prprio =  proctab[currpid].prprio;
				insert(l->lock_held_proc, readylist, proctab[l->lock_held_proc].prprio);
			}
			else
				proctab[l->lock_held_proc].prprio =  proctab[currpid].prprio;
		}
		resched();				
	}
	l->lock_held_proc = currpid;
	l->lock_held_proc_o_prio = proctab[currpid].prprio;

}

pi_unlock(pi_lock_t *l){

	while(test_and_set(&l->guard,1)==1)
		;
	if(isempty(l->queue)){
		l->flag = 0;
		l->lock_held_proc = 0;
		l->lock_held_proc_o_prio = -1;
	}
	else{
		//unpark
		int32 id;
		id = dequeue(l->queue);
		proctab[id].prstate = PR_READY;
		insert(id, readylist, proctab[id].prprio);
		if(proctab[currpid].prprio != l->lock_held_proc_o_prio){
			proctab[currpid].prprio = l->lock_held_proc_o_prio;
			l->guard = 0;
			resched();
		}
	}

	l->guard = 0;
}