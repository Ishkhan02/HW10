# Problem 5 – Limited Resources: Printer Pool (Counting Semaphore)

If the semaphore is initialized to 1, only one thread can print at a time, so the system behaves like a single printer. If it is initialized to a value that is too large, more threads will enter the printing section simultaneously than there are real printers, so the semaphore no longer enforces the real resource constraint.
