# Problem 2 – Bank Account (Mutex vs Spinlock, Short vs Long Critical Section)



With short critical sections, spinlocks can perform well because threads spin only briefly before acquiring the lock, avoiding the overhead of kernel-level context switches. With long critical sections, spinning is worse: waiting threads waste CPU cycles while the lock holder keeps the lock for a long time (especially with usleep inside), so mutexes are preferable because they put waiting threads to sleep and let the scheduler run other work.
