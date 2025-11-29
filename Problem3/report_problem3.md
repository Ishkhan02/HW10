# Problem 3 – Bounded Buffer (Producer–Consumer with Semaphores + Mutex)

## How to compile

cd HW10/Problem3
clang -Wall -Wextra -pthread -o bounded_buffer bounded_buffer.c

## How to run

Example run:

./bounded_buffer 2 2 10

This creates 2 producers and 2 consumers, each producer generating 10 items. The consumers terminate when they receive the special value -1.

Take a screenshot showing producers and consumers running.

## Short answer

Semaphores empty_slots and full_slots control how many items can be produced or consumed and block when the buffer is full or empty, but they do not protect the shared data structures themselves. The mutex is still needed to make updates to buffer, in_pos, and out_pos atomic, so that concurrent threads do not corrupt indices or overwrite data.
