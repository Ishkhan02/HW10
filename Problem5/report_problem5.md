# Problem 5 – Limited Resources: Printer Pool (Counting Semaphore)

## How to compile

cd HW10/Problem5
clang -Wall -Wextra -pthread -o printer_pool printer_pool.c

## How to run

Example:

./printer_pool 10 3

This creates 10 print job threads and a pool of 3 printers. Check the log and the final line:

Maximum concurrent printing jobs observed: X (limit K = 3)

X should never exceed K. Take a screenshot of a run.

## Short answer

If the semaphore is initialized to 1, only one thread can print at a time, so the system behaves like a single printer. If it is initialized to a value that is too large, more threads will enter the printing section simultaneously than there are real printers, so the semaphore no longer enforces the real resource constraint.
