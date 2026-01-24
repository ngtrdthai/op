#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int priority;
    int remaining;
    int start = -1;
    int finish = 0;
    int completion = 0;
    int turnaround = 0;
    int waiting = 0;
};

void printMenu() {
    cout << "\n===== CPU SCHEDULING DEMO =====\n";
    cout << "1. FCFS (First Come First Serve)\n";
    cout << "2. SJF (Non-preemptive)\n";
    cout << "3. Priority (Non-preemptive)\n";
    cout << "4. Round Robin\n";
    cout << "0. Exit\n";
    cout << "Choose algorithm: ";
}

void inputProcesses(vector<Process>& p, int n) {
    for (int i = 0; i < n; i++) {
        Process pr;
        pr.pid = i + 1;
        cout << "Process P" << pr.pid << " Arrival Time: ";
        cin >> pr.arrival;
        cout << "Process P" << pr.pid << " Burst Time: ";
        cin >> pr.burst;
        cout << "Priority (lower = higher priority): ";
        cin >> pr.priority;
        pr.remaining = pr.burst;
        p.push_back(pr);
    }
}

void printGantt(const vector<int>& gantt) {
    cout << "\nGantt Chart:\n|";
    for (int pid : gantt)
        cout << " P" << pid << " |";
    cout << "\n";
}

void FCFS(vector<Process>& p) {
    int time = 0;
    vector<int> gantt;

    
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.arrival < b.arrival;
        });

    for (auto& pr : p) {
        if (time < pr.arrival)
            time = pr.arrival;

        for (int i = 0; i < pr.burst; i++) {
            gantt.push_back(pr.pid);
            time++;
        }

        
        pr.completion = time;

      
        pr.turnaround = pr.completion - pr.arrival;

        pr.waiting = pr.turnaround - pr.burst;
    }

    
    cout << "\nGantt Chart:\n|";
    for (int pid : gantt)
        cout << " P" << pid << " |";
    cout << "\n";

   
    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto pr : p) {
        cout << "P" << pr.pid << "\t"
            << pr.arrival << "\t"
            << pr.burst << "\t"
            << pr.completion << "\t"
            << pr.turnaround << "\t"
            << pr.waiting << "\n";
    }
}


void RoundRobin(vector<Process>& p, int quantum) {
    int n = p.size();
    int time = 0;
    int completed = 0;

    vector<int> gantt;
    queue<int> q;
    vector<bool> inQueue(n, false);

    // Sort theo Arrival Time
    sort(p.begin(), p.end(), [](Process a, Process b) {
        return a.arrival < b.arrival;
        });

    // Thêm tiến trình đến tại time = 0
    for (int i = 0; i < n; i++) {
        if (p[i].arrival == 0) {
            q.push(i);
            inQueue[i] = true;
        }
    }

    while (completed < n) {

        // Nếu queue rỗng → CPU idle
        if (q.empty()) {
            gantt.push_back(-1); // idle
            time++;

            // kiểm tra tiến trình mới đến
            for (int i = 0; i < n; i++) {
                if (!inQueue[i] && p[i].arrival <= time) {
                    q.push(i);
                    inQueue[i] = true;
                }
            }
            continue;
        }

        int i = q.front();
        q.pop();

        int exec = min(quantum, p[i].remaining);

        for (int t = 0; t < exec; t++) {
            gantt.push_back(p[i].pid);
            time++;

            // thêm tiến trình mới đến
            for (int j = 0; j < n; j++) {
                if (!inQueue[j] && p[j].arrival <= time) {
                    q.push(j);
                    inQueue[j] = true;
                }
            }
        }

        p[i].remaining -= exec;

        if (p[i].remaining > 0) {
            q.push(i);
        }
        else {
            p[i].completion = time;
            p[i].turnaround = p[i].completion - p[i].arrival;
            p[i].waiting = p[i].turnaround - p[i].burst;
            completed++;
        }
    }

    
    cout << "\nGantt Chart:\n|";
    for (int pid : gantt) {
        if (pid == -1) cout << " IDLE |";
        else cout << " P" << pid << " |";
    }
    cout << "\n";

    cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto pr : p) {
        cout << "P" << pr.pid << "\t"
            << pr.arrival << "\t"
            << pr.burst << "\t"
            << pr.completion << "\t"
            << pr.turnaround << "\t"
            << pr.waiting << "\n";
    }
}

int main() {
    int choice;
    do {
        printMenu();
        cin >> choice;

        if (choice == 0) break;

        int n;
        cout << "Number of processes: ";
        cin >> n;

        vector<Process> processes;
        inputProcesses(processes, n);

        switch (choice) {
        case 1:
            FCFS(processes);
            break;
        case 4:
            int q;
            cout << "Time Quantum: ";
            cin >> q;
            RoundRobin(processes, q);
            break;
        default:
            cout << "Algorithm not implemented yet.\n";
        }
    } while (true);

    return 0;
}
