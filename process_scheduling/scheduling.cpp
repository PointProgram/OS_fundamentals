#include "scheduling.hpp"

#include <iostream>
#include <vector>
#include <array>
#include <iomanip>
#include <algorithm>

int delay, quant;

// implementing RR scheduling on the queue list
void getRR(std::vector<process_in> queue, int &time_delay, int &clock, std::array<std::vector<process_rr>, 3> &q, int pos, int &time_quantum) {

	unsigned int i = 0;
	std::cout << "clock: " << clock << "| ";

	while (i < q[pos].size() && q[pos][i].remain_btime == 0)
		i++;
	if (q[pos][i].remain_btime > 0) {
		if (q[pos][i].remain_btime == q[pos][i].burst_time)
			q[pos][i].start_time = clock;

		std::cout << "P" << q[pos][i].proc_num << " rem_time: " << q[pos][i].remain_btime << " queue: " << pos + 1;

		if (q[pos][i].remain_btime >= quant) {
			q[pos][i].remain_btime--;
			if (time_quantum == quant && (i + 1) < q[pos].size()) {
				std::cout << " | rr_quantum divide | ";
				rotate(q[pos].begin() + i, q[pos].begin() + i + 1, q[pos].end());
				time_quantum = 0;
				if(delay > quant && (i+1) < q[pos].size())
					time_delay = 0;
			}
		}
		else {
			q[pos][i].remain_btime--;
			time_quantum = 0;
			if (delay > quant && (i + 1) < q[pos].size())
				time_delay = 0;
		}


		if (q[pos][i].remain_btime == 0) {
			q[pos][i].finish_time = clock + 1;
			q[pos][i].full_time = q[pos][i].finish_time - q[pos][i].start_time;
			q[pos][i].waiting_time = q[pos][i].full_time - q[pos][i].burst_time;
		}
	}

	if (time_delay == delay && pos > 0) {
		q[pos - 1].push_back(q[pos][i]);
		q[pos].erase(q[pos].begin() + i);
		std::cout << "  | threshold division | ";
	}
	std::cout << std::endl;
}

// implementing SCFS scheduling on the queue list
void getSCFS(std::vector<process_in> queue, int &time_delay, int& clock, std::array<std::vector<process_rr>, 3> &q, int pos) {
	unsigned int i = 0;
	std::cout << "clock: " << clock << "| ";

	while (i < q[pos].size() && q[pos][i].remain_btime == 0)
		i++;
	if (q[pos][i].remain_btime > 0) {
		if (q[pos][i].remain_btime == q[pos][i].burst_time)
			q[pos][i].start_time = clock;
		std::cout << "P" << q[pos][i].proc_num << " rem_time: " << q[pos][i].remain_btime << " queue: " << pos + 1;
		q[pos][i].remain_btime--;
		
		if (q[pos][i].remain_btime == 0) {
			q[pos][i].finish_time = clock + 1;
			q[pos][i].full_time = q[pos][i].finish_time - q[pos][i].start_time;
			q[pos][i].waiting_time = q[pos][i].full_time - q[pos][i].burst_time;
		}
	}
	
	if (time_delay == delay && pos > 0) {
		q[pos - 1].push_back(q[pos][i]);
		q[pos].erase(q[pos].begin() + i);
		std::cout << " | threshold division |";
		time_delay = 0;
	}
	std::cout << std::endl;
}

// displaying table with details about process execution
void table(int width, int num, int arriving_time, int expected_time, int start_time, int finish_time, int delay_time, int full_time) {
	std::cout << "|"  << std::setw(width) << num << "|" << std::setw(width) << arriving_time <<
		"|" << std::setw(width) << expected_time << "|" << std::setw(width) << start_time <<
		"|" << std::setw(width) << finish_time << "|" << std::setw(width) << delay_time <<
		"|" << std::setw(width) << full_time << "|" << std::endl;
	std::cout << std::string(width * 7 + 8, '-') << std::endl;
}


void display_table(std::array<std::vector<process_rr>, 3> q) {
	int size = 1, width = 10, average_waiting = 0, average_full = 0, max = q[0].size() + q[1].size() + q[2].size();
	std::cout << std::endl << std::string(width * 7 + 8, '-') << std::endl;
	std::cout << "|" << std::setw(width) << "Process N" << "|" << std::setw(width) << "Arrive t" <<
		"|" << std::setw(width) << "Expected t" << "|" << std::setw(width) << "Start t" <<
		"|" << std::setw(width) << "Finish t" << "|" << std::setw(width) << "Delay t" <<
		"|" << std::setw(width) << "Full t" << "|" << std::endl;
	std::cout << std::string(width * 7 + 8, '-') << std::endl;

	while (size <= max) {
		for (int j = 0; j < 3; j++) {
			for (unsigned int i = 0; i < q[j].size(); i++) {
				if (q[j][i].proc_num == size) {
					table(width, q[j][i].proc_num, q[j][i].arriving_time, q[j][i].burst_time, q[j][i].start_time, q[j][i].finish_time, q[j][i].waiting_time, q[j][i].full_time);
					average_waiting += q[j][i].waiting_time;
					average_full += q[j][i].full_time;
				}
			}
		}
		size++;
	}

	std::cout << "Average time for waiting processes in the queue: " << (float)average_waiting / (float)max << std::endl;
	std::cout << "Average time for full processes in the queue: " << (float)average_full / (float)max << std::endl;

}

void process_init2(std::vector<process_in>& pr) {
	delay = 5;
	quant = 3;

	process_in proc;
	proc.arriving_t = 0;
	proc.expected_t = 5;
	proc.priority = 1;
	proc.process_num = 1;
	pr.push_back(proc);

	proc.expected_t = 12;
	proc.arriving_t = 2;
	proc.priority = 2;
	proc.process_num = 2;
	pr.push_back(proc);

	proc.arriving_t = 3;
	proc.expected_t = 7;
	proc.priority = 3;
	proc.process_num = 3;
	pr.push_back(proc);

	proc.arriving_t = 11;
	proc.expected_t = 2;
	proc.priority = 1;
	proc.process_num = 4;
	pr.push_back(proc);

	proc.arriving_t = 7;
	proc.expected_t = 8;
	proc.priority = 2;
	proc.process_num = 5;
	pr.push_back(proc);
}

void process_init(std::vector<process_in> &pr) {
	delay = 6;
	quant = 2;

	process_in proc;
	proc.arriving_t = 0;
	proc.expected_t = 4;
	proc.priority = 3;
	proc.process_num = 1;
	pr.push_back(proc);

	proc.expected_t = 8;
	proc.arriving_t = 0;
	proc.priority = 3;
	proc.process_num = 2;
	pr.push_back(proc);

	proc.arriving_t = 6;
	proc.expected_t = 7;
	proc.priority = 3;
	proc.process_num = 3;
	pr.push_back(proc);

	proc.arriving_t = 11;
	proc.expected_t = 25;
	proc.priority = 3;
	proc.process_num = 4;
	pr.push_back(proc);

	proc.arriving_t = 12;
	proc.expected_t = 2;
	proc.priority = 1;
	proc.process_num = 5;
	pr.push_back(proc);
}

bool check_p(std::vector<process_rr> pp) {
	for (unsigned int i = 0; i < pp.size(); i++) {
		if (pp[i].remain_btime > 0) {
			return true;
		}
	}
	return false;
}

void counter(int &time_quantum, int &t2, int &t3, int type) {
	if (type == 2) {
		t3 = 0;
		t2++;
	}
	else if (type == 3) {
		t2 = 0;
		t3++;
		time_quantum++;
	}
}

void queue_processing() {

	int clock = 0, t_1 = 0, t_2 = 0, t_3 = 0, num = 0, max_num, t_quantum = 0;
	std::array<std::vector<process_rr>, 3> queue;
	process_rr proc;
	std::vector<process_in> process;
	process_init(process);
	max_num = process.size();
	bool finish = false;
	while (!finish) {
		for (unsigned int i = 0; i < process.size(); i++) {
			if (process[i].arriving_t == clock) {
				proc.burst_time = process[i].expected_t;
				proc.waiting_time = 0;
				proc.proc_num = process[i].process_num;
				proc.remain_btime = process[i].expected_t;
				proc.arriving_time = process[i].arriving_t;
				queue[process[i].priority - 1].push_back(proc);
				
				num++;
				process.erase(process.begin() + i);
				i--;
			}
		}

		if (check_p(queue[0])) {
			getSCFS(process, t_1, clock, queue, 0);
		}
		else if (check_p(queue[1])) {
			counter(t_quantum, t_2, t_3, 2);
			getSCFS(process, t_2, clock, queue, 1);
		}
		else if (check_p(queue[2])) {
			counter(t_quantum, t_2, t_3, 3);
			getRR(process, t_3, clock, queue, 2, t_quantum);
		}
		else {
			if (num == max_num)
				finish = true;
		}
		clock++;
	}

	display_table(queue);
}
