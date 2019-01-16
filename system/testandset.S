/* testandset.S - test_and_set (for x86) */

		.text
		.globl	test_and_set

/*------------------------------------------------------------------------
 * testandset -  X86 test and set; the call is test_and_set(&lock_var, lock_val)
 *------------------------------------------------------------------------
 */

 test_and_set:
 					pushl	%ebp			/* Push ebp onto stack		*/	 
					movl	%esp,%ebp		/* Record current SP in ebp	*/	 
 					pushl	%ebx			/* Push EBX onto stack as we are going to modify its contents */
					movl	8(%ebp),%ebx 	/* Get memory location of the lock pointer */ 
					movl	12(%ebp),%eax	/* Get memory location of the new lock value */
					xchg	%eax,(%ebx)		/* Exchange the dereferenced value of the lock pointer and the new lock value */
					popl	%ebx 			/* Restore ebx */
					popl	%ebp			/* Restore ebp */
					ret 					/* Return the contents of EAX which is the old lock value */
