This project was aimed to develop the skill and understanding of semaphores use for synchronization problems.

In this project we were tasked with creating 5 processes that live independently of each other, and communicate and synchronize with other using semaphores.
The program was to produce 5 processes, that together count to 100, where each process prints their number in the looping order, ie:
process number 1 prints 1, process number 2 prints 2, 3->3, 4->4, 5->5 and then process number 1 wakes up again and prints number 6, then process number prints 7, ...
And so on...


Essentially, every semaphore has a wait() and signal() functionalities.

While the most simplified use is a binary semaphore that switches between two tasks, here we needed to balance 5, and in a praticular order.
So, while usually wait() would decrease the semaphore value by 1, and signal would increase the value by 1 of it's own semaphore, here it's a bit different.

Every semaphore had the usual wait() function, where it decreases it's value by {the number assigned to semaphore}, but the signal() function operates a bit differently:
instead of signaling it's own semaphore, the process will signal **all the other processes** by a value of 1.

We initialize the semaphore values where semaphore1 = 4, semaphore2 = 3 ... semaphore5 = 0. 
When we decrease the values by 4, only the first semaphore will be able to operate, since the other semaphores get stuck when trying to cross to a negative integer.
after semaphore1.wait(): semaphore1=0, semaphore2=3, semaphore3=2, ... semaphore5 = 0.

We then increase all the values of other semaphores, excluding the semaphore assosicated with the calling process
after semaphore1.signal(): semaphore1=0, semaphore2=4, semaphore3=3, ... semaphore5 = 1.

This ensure synchronization between the processes.
