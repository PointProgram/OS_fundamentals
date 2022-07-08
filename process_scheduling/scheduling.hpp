#pragma once

typedef struct process_rr {
	int proc_num;
	int arriving_time;
	int burst_time;
	int start_time;
	int finish_time;
	int waiting_time;
	int full_time;
	int remain_btime;
} process_rr;

typedef struct process_in {
	int process_num;
	int arriving_t;
	int expected_t;
	int priority;
} process_in;

void queue_processing();