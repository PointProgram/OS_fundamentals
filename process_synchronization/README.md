<h1>Synchroniztion of processes on ATM</h1>

## Problem overview

<i>Model object</i>: ATM.
The ATM accepts an application for payment of money in the amount of X, compares it with
by the amount of money in the client's Y account and, if X/<Y, makes a withdrawal attempt
certain banknotes in denominations of 1, 2, 5, 10, 20, 50, 100 dollars. The values ​​of X and Y are
random numbers The initial number of bills of different denominations in the ATM
given by the receiver. If amount X is to be disbursed, the program determines whether
you can pay this amount with currently available bills. If so -
money is issued (the number of bills available in the ATM is also adjusted
of the corresponding denomination), if not - the ATM reports a refusal.
Requests to withdraw money arrive at the ATM one after another
customer service (or failure).

<i>The number of terminals and processes</i>: The machine model should be presented in the form two interacting processes A and B.
 - Process A determines the receipt of a payment claim and the opportunity payment of the required amount, fixes the possibility of payment.
 - Process B waits for the moment when it becomes necessary to make a payment and, if
payment is possible, the required number of bills of each denomination is determined
for payment, the client's account is adjusted and the amount of cash in the ATM goes
payout If payment is not possible, the client is notified
message.
<i>Synchronization tools</i>: Check and Install‖ operation (or its model) for access to the shared resources.


## Technologies
Project is created with:
* C++
* gcc: version 6.3.0

	
## Setup
git clone and run a program with the following makefile:

<code>$ make</code>

## Synchronization process model

This machine operation greatly simplifies the solution of the critical section problem. The ―Check and Install‖ operation is accessed with two parameters: Local and Common. The operation takes the value of the Common parameter and assigns it to the Local variable, and then sets the Common variable to 1. This operation is one of the hardware solutions to the critical section problem. It was first implemented in IBM-360 and was called TS (Test and Set). The Common variable will be common to all processes. It will be equal to 1 when

one of the processes is in a critical region. If the process wants to enter the critical region, its local variable takes the value 1. The main property of this operation is its indivisibility. When a process performs an operation―Check and Install, no other actions can occur during its execution.
The Common variable is shared by processes that are to be synchronized with respect to some critical resource. Each process has its own Local variable. If Common = 1, it means that some process is in its critical section. The initial value of the variable Common = 0.
We will provide a solution to the problem using the ―Check and installation‖ method. This solution assumes that the intended memory lock, that is, the operation Common := 0 is non-divisible. 

The algorithm is as follows:
```Var Сommon, Locall, Local2: integer;
begin
    Сommon:= 1;
Рarbegin
    Process_1: while true do 
        begin
            Local1:= 1;
            while Local1=1 do TS(Local1, Сommon);
            < Critical section of the Process_1>;
            common:=0;
            < Remainder of the Process_1>;
        end;
    end;
    Process_2: while true do 
        begin
            Local2:=1;
            while Local2=1 do TS(Local2, Сommon);
            < Critical section of the Process_2 >; 
            common:=0;
            < Remainder of the Process_2 >;
        end
    end;
Рarend; 
Еnd.
```

Let the process Process_1 be the first to enter its critical section. In this case, Local1 will initially take the value 1, and after a cycle of verification using the TS command (Local1, Сommon) will become equal to 0. At the same time, the value of Сommon will be equal to 1. Process_1 will enter the critical section. After exiting the critical section, the Common:=0 operation is performed, which will enable Process_2 to enter its critical section. At the same time, we believe that the operation Сommon:=0 is indivisible due to the operation of the memory lock.
The disadvantage of this method is that the cycle of checking the value of the Сommon variable uses processor time unproductively. The situation is even worse if, while Process_1 is running, an interrupt occurs in its critical section, and Process_1 starts executing. It will enter the check loop and will not exit until Process_1 exits the interrupt.
By the way, Intel microprocessors, starting with the 80386, have special commands called BTC (Bit Test and Convert), BTS, BTR, which are prototypes of the ―Check and Install‖ command. Both of the discussed synchronization methods can be quite inefficient, because every time one process executes its critical section, any other process that also wants to enter the critical section enters a check loop and must wait there for permission. With such waiting in a cycle, called active waiting, processor time is wasted. One of the methods that allow you to avoid active waiting is the use of semaphores.