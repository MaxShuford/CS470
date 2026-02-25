/*
 * CPU Scheduling Simulator
 * 
 *   - Preemptive Shortest Job First (SRTF)
 *   - Round Robin
 *
 * Author: Max Shuford
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int finished;
} Process;


// Reset all dynamic scheduling values before running an algorithm
// This ensures both SJF and RR start from identical initial states.
void reset_processes(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].remaining_time = p[i].burst_time;
        p[i].completion_time = 0;
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].finished = 0;
    }
}

// Print final scheduling results and compute averages
// Waiting Time = Turnaround Time - Burst Time
// Turnaround Time = Completion Time - Arrival Time
void print_results(Process p[], int n) {
    double total_wait = 0;
    double total_turn = 0;

    printf("\nPID | Arrival | Burst | Waiting | Turnaround\n");
    printf("------------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        printf("%3d | %7d | %5d | %7d | %10d\n",
               p[i].pid,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].waiting_time,
               p[i].turnaround_time);

        total_wait += p[i].waiting_time;
        total_turn += p[i].turnaround_time;
    }

    printf("\nAverage Waiting Time: %.2f\n", total_wait / n);
    printf("Average Turnaround Time: %.2f\n", total_turn / n);
}

// ================= PREEMPTIVE SJF =================
void sjf(Process p[], int n) {
    printf("\n===== Preemptive SJF (SRTF) =====\n");

    reset_processes(p, n);

    int completed = 0;
    int time = 0;
    int prev_pid = -1;

    printf("Execution Order:\n");

    // Continue simulation until all processes complete
    while (completed < n) {
        int shortest = -1;
        int min_remaining = 1000000000;

        // Find the process with the smallest remaining time
        // among all processes that have already arrived
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= time &&
                p[i].finished == 0 &&
                p[i].remaining_time > 0 &&
                p[i].remaining_time < min_remaining) {

                min_remaining = p[i].remaining_time;
                shortest = i;
            }
        }

        // If no process is ready, increment system time (Idle)
        if (shortest == -1) {
            time++;
            continue;
        }

        if (prev_pid != p[shortest].pid) {
            printf("P%d ", p[shortest].pid);
            prev_pid = p[shortest].pid;
        }

        // Execute selected process for one time unit
        p[shortest].remaining_time--;
        time++;


        // calculate its completion, turnaround, and waiting times
        if (p[shortest].remaining_time == 0) {
            p[shortest].finished = 1;
            completed++;

            p[shortest].completion_time = time;
            p[shortest].turnaround_time =
                p[shortest].completion_time - p[shortest].arrival_time;

            p[shortest].waiting_time =
                p[shortest].turnaround_time - p[shortest].burst_time;
        }
    }

    printf("\n");
    print_results(p, n);
}

// ================= ROUND ROBIN ================= 
void rr(Process p[], int n, int quantum) {
    printf("\n===== Round Robin (Quantum = %d) =====\n", quantum);

    reset_processes(p, n);

    // Add all processes that have arrived at current time
    int queue[MAX_PROCESSES];
    int front = 0;
    int rear = 0;

    int time = 0;
    int completed = 0;

    // If no process is in ready queue, advance
    int in_queue[MAX_PROCESSES];
    for (int i = 0; i < n; i++) {
        in_queue[i] = 0;
    }

    printf("Execution Order:\n");

    // Determine how long process will run, If remaining time is less than quantum,
    // run only for remaining time. Otherwise run for full quantum.
    while (completed < n) {

        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= time &&
                in_queue[i] == 0 &&
                p[i].remaining_time > 0) {

                queue[rear] = i;
                rear++;
                in_queue[i] = 1;
            }
        }

        if (front == rear) {
            time++;
            continue;
        }

        int idx = queue[front];
        front++;

        printf("P%d ", p[idx].pid);

        int run_time;

        if (p[idx].remaining_time < quantum) {
            run_time = p[idx].remaining_time;
        } else {
            run_time = quantum;
        }

        // If no process is in ready queue, advance time
        p[idx].remaining_time -= run_time;
        time += run_time;

        // If process is not finished after quantum expires, place it back at the end
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= time &&
                in_queue[i] == 0 &&
                p[i].remaining_time > 0) {

                queue[rear] = i;
                rear++;
                in_queue[i] = 1;
            }
        }

        // If process finishes, calculate performance metrics
        if (p[idx].remaining_time > 0) {
            queue[rear] = idx;
            rear++;
        } else {
            p[idx].finished = 1;
            completed++;

            p[idx].completion_time = time;
            p[idx].turnaround_time =
                p[idx].completion_time - p[idx].arrival_time;

            p[idx].waiting_time =
                p[idx].turnaround_time - p[idx].burst_time;
        }
    }

    printf("\n");
    print_results(p, n);
}


int main() {
    int n;

    // Collect process information from user
    printf("Enter number of processes: ");
    scanf("%d", &n);

    // Ensure number of processes does not exceed system limit
    if (n <= 0 || n > MAX_PROCESSES) {
        printf("Invalid number of processes.\n");
        return 1;
    }

    Process processes[MAX_PROCESSES];

    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;

        printf("\nProcess %d\n", i + 1);
        printf("Arrival Time: ");
        scanf("%d", &processes[i].arrival_time);

        printf("Burst Time: ");
        scanf("%d", &processes[i].burst_time);
    }

    int choice;

    printf("\nChoose Algorithm:\n");
    printf("1. Preemptive SJF\n");
    printf("2. Round Robin\n");
    printf("Choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        sjf(processes, n);
    } 
    else if (choice == 2) {
        int quantum;
        printf("Enter Time Quantum: ");
        scanf("%d", &quantum);
        // Ensure time quantum is a positive integer
        if (quantum <= 0) {
            printf("Quantum must be positive.\n");
                return 1;
        
        }
        rr(processes, n, quantum);
    } 
    else {
        printf("Invalid choice.\n");
    }

    return 0;
}