# Problem 4 – Ordered Printing A → B → C (Semaphores)

## How to compile

cd HW10/Problem4
clang -Wall -Wextra -pthread -o ordered_print ordered_print.c

## How to run

Example:

./ordered_print 5

Expected pattern:

Thread A: A 0
Thread B: B 0
Thread C: C 0
Thread A: A 1
Thread B: B 1
Thread C: C 1
...

Take a screenshot of the ordered output.

## Short answer

Using only sleep calls is not reliable for ordering because the OS scheduler is nondeterministic: the actual wake-up times and scheduling decisions can vary, so threads may print in different orders. Semaphores provide strict ordering by forcing each thread to wait for a specific signal before proceeding.
