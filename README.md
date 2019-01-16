# XINU-Lock
This project implements locks in Xinu based on the test_and_set hardware instruction. 

1.  The project provides an assembly implementation of atomic test_and_set function. 
    The function has the following declaration and implement the following code atomically:
    uint32 test_and_set(uint32 *ptr, uint32 new_value) {
      uint32 old_value = *ptr;
      *ptr = new_value;
      return old_value;
    }

2.  The project implements a spinlock based on test_and_set function. The spinlock has an initialization, a lock and an unlock 
    functions declared as specified below. 
    void sl_init(sl_lock_t *l);
    void sl_lock(sl_lock_t *l);
    void sl_unlock(sl_lock_t *l);
    
    The spinlock implementation is in system/spinlock.c file.
    
3.  The project implements a lock that limits busy waiting by putting the current process to sleep. 
    The lock is based on the test_and_set function, and has an initialization, a lock and an unlock
    functions declared as specified below. 
    void init(lock_t *l);
    void lock(lock_t *l);
    void unlock(lock_t *l);
    
    The lock implementation is in system/lock.c file
    
 4. The project implements deadlock detecting capabilities for 3. The implementation notifies the presence of a deadlock 
    (without performing any corrective actions). The initialization, lock and unlock functions should now be declared as follows.
    void al_init(al_lock_t *l);
    void al_lock(al_lock_t *l);
    void al_unlock(al_lock_t *l);
    bool8 al_trylock(al_lock_t *l);
    
    al_trylock tries to obtain a lock and it returns immediately to the caller if the lock is already held. 
    The function returns true if it has obtained the lock, and false if it hasn’t.
    
    The lock implementation should be in system/active_lock.c file.
    
 5. Modifies lock implementation (3) so to avoid priority inversion. 
    Uses the “Basic Priority Inheritance Protocol” described in the following paper:
    L. Sha, R. Rajkumar and J. P. Lehoczky, "Priority inheritance protocols: an approach to real-time
    synchronization," in IEEE Transactions on Computers, vol. 39, no. 9, pp. 1175-1185, Sep 1990.
    
    The initialization, lock and unlock functions should now be declared as follows.
    void pi_init(pi_lock_t *l);
    void pi_lock(pi_lock_t *l);
    void pi_unlock(pi_lock_t *l);

    This lock’s implementation should is in system/pi_lock.c file.
