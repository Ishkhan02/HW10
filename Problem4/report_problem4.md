# Problem 4 – Ordered Printing A → B → C (Semaphores)



## Short answer

Using only sleep calls is not reliable for ordering because the OS scheduler is nondeterministic: the actual wake-up times and scheduling decisions can vary, so threads may print in different orders. Semaphores provide strict ordering by forcing each thread to wait for a specific signal before proceeding.
