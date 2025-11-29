# Problem 3 – Bounded Buffer (Producer–Consumer with Semaphores + Mutex)


Semaphores empty_slots and full_slots control how many items can be produced or consumed and block when the buffer is full or empty, but they do not protect the shared data structures themselves. The mutex is still needed to make updates to buffer, in_pos, and out_pos atomic, so that concurrent threads do not corrupt indices or overwrite data.
