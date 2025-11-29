# Problem 1 – Broken Counter (Race, Mutex, Spinlock)

Version (A) without synchronization may give a wrong result because counter++ is not atomic: different threads can interleave the read–modify–write steps and overwrite each other's updates, causing a race condition. In versions (B) and (C), the mutex or spinlock forces only one thread at a time to execute counter++, so increments are serialized and the final result equals N * M.
