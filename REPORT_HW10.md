# HW10 – Operating Systems (Pthreads and Synchronization)

## What I implemented

1. Problem 1 – Broken Counter
   - Implemented a global long long counter incremented by multiple threads.
   - Added three modes: no synchronization, mutex, and spinlock.
   - The program prints both the expected value and actual value so the race condition is visible.
   - Short conceptual explanation is in Problem1/report_problem1.md.

2. Problem 2 – Bank Account
   - Implemented a shared balance updated by deposit and withdraw threads.
   - Lock type is selectable via command line: mutex or spinlock.
   - Critical section length (short vs long) is also selectable; the long case uses usleep inside the lock.
   - Measured approximate runtime using gettimeofday and optionally /usr/bin/time.
   - Explanation of mutex vs spinlock behavior is in Problem2/report_problem2.md.

3. Problem 3 – Bounded Buffer (Producer–Consumer)
   - Implemented a circular buffer with size 8 and separate indices in_pos and out_pos.
   - Used semaphores empty_slots and full_slots to limit producers and consumers.
   - Used a mutex to protect buffer and indices.
   - Producers generate a fixed number of items each; consumers terminate when they receive a special -1 poison pill.
   - Explanation of why the mutex is needed in addition to semaphores is in Problem3/report_problem3.md.

4. Problem 4 – Ordered Printing A → B → C
   - Created three threads A, B, and C, each printing N lines.
   - Used three semaphores to enforce strict order: A prints, then B, then C, for each index.
   - Used a mutex only to keep prints on separate lines without mixing.
   - Explanation of why sleep alone is not sufficient is in Problem4/report_problem4.md.

5. Problem 5 – Printer Pool
   - Simulated K identical printers with a counting semaphore initialized to K.
   - Created N threads representing print jobs.
   - Each job waits on the semaphore, prints, then posts back.
   - Used a mutex-protected counter to track current and maximum concurrent printing jobs.
   - Explanation of different semaphore initializations (1 or too large) is in Problem5/report_problem5.md.

## What still needs to be done manually

- Compile each program using the provided commands:

  - Problem 1:
    - cd HW10/Problem1
    - clang -Wall -Wextra -pthread -o broken_counter broken_counter.c

  - Problem 2:
    - cd HW10/Problem2
    - clang -Wall -Wextra -pthread -o bank_account bank_account.c

  - Problem 3:
    - cd HW10/Problem3
    - clang -Wall -Wextra -pthread -o bounded_buffer bounded_buffer.c

  - Problem 4:
    - cd HW10/Problem4
    - clang -Wall -Wextra -pthread -o ordered_print ordered_print.c

  - Problem 5:
    - cd HW10/Problem5
    - clang -Wall -Wextra -pthread -o printer_pool printer_pool.c

- Run the executables with appropriate arguments and take screenshots of the outputs:

  - Problem 1:
    - ./broken_counter no_sync
    - ./broken_counter mutex
    - ./broken_counter spin

  - Problem 2:
    - /usr/bin/time -p ./bank_account mutex short
    - /usr/bin/time -p ./bank_account spin short
    - /usr/bin/time -p ./bank_account mutex long
    - /usr/bin/time -p ./bank_account spin long

  - Problem 3:
    - For example: ./bounded_buffer 2 2 10

  - Problem 4:
    - For example: ./ordered_print 5

  - Problem 5:
    - For example: ./printer_pool 10 3

- Insert the screenshots into the per-problem reports or into a single combined PDF, depending on the instructor's submission requirements.
- Finally, add, commit, and push everything to GitHub:

  - cd ~/HW10
  - git status
  - git add .
  - git commit -m "Complete HW10 pthreads and synchronization problems"
  - git push

