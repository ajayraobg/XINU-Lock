#include <xinu.h>

void deadlock_detector(void){

	/*
	Algorithm used:

	Available:  A vector of length m indicates the number of available resources of each type.
	Allocation:  An n x m matrix defines the number of resources of each type currently allocated to each process.
	Request:  An n x m matrix indicates the current request of each process. 
				If Request [ij] = k, then process Pi is requesting k more instances of resource type. Rj.
				
	1.  Let Work and Finish be vectors of length m and n, respectively
		Initialize:
			Work = Available
			For i = 1,2, …, n, if Allocationi ¹ 0, then 
 				Finish[i] = false; otherwise, Finish[i] = true 

	2.  Find an index i such that both:
		Finish[i] == false AND Requesti £ Work
		If no such i exists, go to step 4 

	3.  Work = Work + Allocationi
		Finish[i] = true
		go to step 2 

	4.  If Finish[i] == false, for some i, 1 £ i £  n, then the system is in deadlock state.

	Source for algorithm : https://www.cs.odu.edu/~cs471w/spring10/lectures/Deadlocks.htm

	*/

	uint32 i,j,k;
	for(i=0;i<MAXLOCK;i++){
		work[i] = available[i];
	}
	for(i=0;i<NPROC;i++){
		for(j=0;j<MAXLOCK;j++){
			if(allocation[i][j]==1){
				finish[i] = 0;
				break;
			}
			else
				finish[i] = 1;
		}
	}
	while(1){
		uint8 flag2 = 0;
		for(i=0;i<NPROC;i++){
			uint8 flag = 0;
			if(finish[i]==0){
				for(j=0;j<MAXLOCK;j++){
					if(request[i][j]>work[j]){
						flag=1;
						break;
					}
				}
				if(flag==0){
					for(k=0;k<MAXLOCK;k++){
							work[k] += allocation[i][k];
						}
						finish[i] = 1;
						flag2 = 1;
				}
			}
			if(flag2==1)
				break;
		}
		if(i==NPROC)
			break;
	}

	for(i=0;i<NPROC;i++){
		if(finish[i]==0){
			kprintf("WARNING: DEADLOCK DETECTED !!!!!\n");
			break;
		}
	}
}


void al_init(al_lock_t *l){

	l->flag = 0;
	l->guard = 0;
	l->lock_id = lock_counter;
	lock_counter++;
	l->queue = newqueue();
}

void al_lock(al_lock_t *l){

	while(test_and_set(&l->guard,1)==1)
		;
	if(l->flag==0){
		l->flag = 1;
		l-> guard = 0;
	}
	else{
		//since the lock is unavalable, make the corresponding entry in request matrix as 1
		request[currpid][l->lock_id] = 1;
		//call deadlock detector before queueing the process for lock
		deadlock_detector();
		//add thread to queue
		enqueue(currpid,l->queue);
		l->guard = 0;
		//park
		proctab[currpid].prstate = PR_WAIT;
		resched();				
	}
	//if here then lock is allocated to the process, so update the corresponding entries in allocation and request matrix
	request[currpid][l->lock_id] = 0;
	allocation[currpid][l->lock_id] = 1;
	available[l->lock_id] = 0;
}

void al_unlock(al_lock_t *l){

	while(test_and_set(&l->guard,1)==1)
		;

	//update the allocation matrix to indicate that the currpid is no longer holding th lock
	allocation[currpid][l->lock_id] = 0;

	if(isempty(l->queue)){
		available[l->lock_id] = 1;
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

bool8 al_trylock(al_lock_t *l){
	while(test_and_set(&l->guard,1)==1)
		;
	return l->flag;
}