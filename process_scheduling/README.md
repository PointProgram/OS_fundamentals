<h1>Priority Process Scheduling</h1>

## Problem overview

Priority Scheduling with increasing priorities:

1. queue 1 – FCFS algorithm
2. stage 2 – FCFS algorithm
3. turn 3 – RR algorithm

## Technologies
Project is created with:
* C++
* gcc: version 6.3.0

	
## Setup
git clone and run a program with the following makefile:

<code>$ make</code>

# FCFS

FCFS (First Come First Serve) - first come - first
is served
This is an irreplaceable strategy. Processes are executed in the order of their entry into the system, that is, the i-th process can be started for execution only after the completion of the (i-1)-th process. If the (i-1)th process has not yet finished when the i-th process arrives, then the i-th process goes into the waiting state until the processor is released.

# RR

RR (Round Robin) – circular (carousel) planning
According to this strategy, each process alternately gets to use a small interval (quantum) of processor time. Of course, it is about 100 ms. Thus, processes should not wait for their quantum longer than the length of the quantum multiplied by the number of processes present in the system. Since one quantum may not be enough for execution, further processes will alternately receive a quantum again, etc. until their completion.

# Priority Planning

Each process can be assigned a certain priority, and the scheduler must always select the process with the highest priority among the processes. We will assume that there are several queues of processes ready for execution. Queues are ordered by decreasing priority (one priority within the queue).
When selecting a process, the scheduler starts with queue Q0, which has the highest priority. If the queue is not empty, the process is chosen according to some strategy, for example, FCFS. If the queue is empty, then queue Q1 is considered, etc.

# Reduction priorities

One way to favor short processes is to penalize long-running processes. Here is how it can be achieved. Displacement (by time quanta) scheduling is performed using a dynamic mechanism. Upon arrival, the process enters the Q0 queue. After the first execution (when the quantum is finished and the process returns to the ready state), it already enters the Q1 queue. Then, with each ouster, it enters the queue with a lower and lower priority. Thus, long processes gradually lose their initial priority, and short ones do not go very far in the hierarchy of processes and gain an advantage over long ones. In each queue, you can, for example, use the FCFS algorithm. When reaching the lowest queue, the process no longer leaves it until its completion, getting to it again and again after using the quantum provided to it (that is, the RR algorithm actually works here).

# Increasing priorities

The disadvantage of a rather simple preliminary scheme of reducing priorities is the stretching of the execution time of long processes. As a result, this can lead to starvation, even while increasing the execution time for each downstream queue. To prevent this, it is possible to provide for the transfer of a process to a queue with a higher priority if its maintenance has not been completed within a certain threshold period of time.