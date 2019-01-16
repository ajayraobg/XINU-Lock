/*  main.c  - main */

 #include <xinu.h>

al_lock_t lk1;
al_lock_t lk2;
al_lock_t lk3;

void userfunc1(){

	int32 i=0,j=0;
	kprintf("PID %d started executing\n",currpid);

	al_lock(&lk1);

	kprintf("Lock %d held by: %d\n",lk1.lock_id,currpid);

	sleepms(6000);

	kprintf("Trying to hold Lock %d  by pid : %d\n",lk2.lock_id,currpid);

	al_lock(&lk2);

	kprintf("Lock %d held by: %d\n",lk2.lock_id,currpid);

	sleepms(2000);

	//al_unlock(&lk1);
	//al_unlock(&lk2);
}

void userfunc2(){

	int32 i=0,j=0;
	kprintf("PID %d started executing\n",currpid);
	al_lock(&lk2);

	kprintf("Lock %d held by: %d\n",lk2.lock_id,currpid);

	sleepms(10000);

	kprintf("Trying to hold Lock %d  by pid : %d\n",lk3.lock_id,currpid);
	al_lock(&lk3);
	kprintf("Lock %d held by: %d\n",lk3.lock_id,currpid);
	sleepms(2000);

	//al_unlock(&lk2);
	//al_unlock(&lk3);
}

void userfunc3(){

	int32 i=0,j=0;
	kprintf("PID %d started executing\n",currpid);
	al_lock(&lk3);

	kprintf("Lock %d held by: %d\n",lk3.lock_id,currpid);

	sleepms(2000);

	kprintf("Trying to hold Lock %d  by pid : %d\n",lk1.lock_id,currpid);
	al_lock(&lk1);
	kprintf("Lock %d held by: %d\n",lk1.lock_id,currpid);
	sleepms(2000);

	//al_unlock(&lk3);
	//al_unlock(&lk1);
}
 process	main(void)
 {

 	/* Run the Xinu shell */

 	/*sl_lock_t lock;
 	sl_init(&lock);
 	kprintf("New val:%d\n",lock);
 	sl_lock(&lock);
 	kprintf("New val:%d\n",lock);
 	sl_unlock(&lock);
 	kprintf("New val:%d\n",lock);
 	//val = test_and_set(&lock,1);
 	//kprintf("%d\n",val);*/
 	pid32 pr1,pr2,pr3;
 	al_init(&lk1);
 	al_init(&lk2);
 	al_init(&lk3);
 	pr1 = resume(create(userfunc1, 1024, 10, "Proc1",0));
 	pr2 = resume(create(userfunc2, 1024, 9, "Proc2",0));
 	pr3 = resume(create(userfunc3, 1024, 8, "Proc3",0));

 	sleepms(200000);

 	//kprintf("here\n");

 	return OK;
    
 }
