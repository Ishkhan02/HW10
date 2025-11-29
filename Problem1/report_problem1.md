# Problem 1 – Broken Counter (Race, Mutex, Spinlock)

## How to compile

cd HW10/Problem1
clang -Wall -Wextra -pthread -o broken_counter broken_counter.c

## How to run

./broken_counter no_sync
./broken_counter mutex
./broken_counter spin

Take screenshots of each run showing Expected vs Actual.

## Short answer

Version (A) without synchronization may give a wrong result because counter++ is not atomic: different threads can interleave the read–modify–write steps and overwrite each other's updates, causing a race condition. In versions (B) and (C), the mutex or spinlock forces only one thread at a time to execute counter++, so increments are serialized and the final result equals N * M.
