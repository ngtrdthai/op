#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int pid, arrival, burst, priority;
    int start = -1, completion = -1, waiting = 0, turnaround = 0;
};

struct GanttBlock {
    int pid, start, finish;
};

int chooseNextProcess(const vector<Process>& ps, int time, const vector<bool>& done) {
    int idx = -1;
    for (int i = 0; i < (int)ps.size(); ++i) {
        if (done[i] || ps[i].arrival > time) continue;
        if (idx == -1 || ps[i].priority < ps[idx].priority ||
            (ps[i].priority == ps[idx].priority && (ps[i].arrival < ps[idx].arrival ||
                (ps[i].arrival == ps[idx].arrival && ps[i].pid < ps[idx].pid))))
            idx = i;
    }
    return idx;
}

void schedule(vector<Process>& ps, vector<GanttBlock>& gantt) {
    int n = (int)ps.size();
    vector<bool> done(n, false);
    int completed = 0;
    int time = ps[0].arrival;
    while (completed < n) {
        int idx = chooseNextProcess(ps, time, done);
        if (idx == -1) {
            int nextArrival = INT_MAX;
            for (int i = 0; i < n; ++i)
                if (!done[i]) nextArrival = min(nextArrival, ps[i].arrival);
            time = max(time, nextArrival);
            continue;
        }
        ps[idx].start = time;
        ps[idx].completion = time + ps[idx].burst;
        ps[idx].turnaround = ps[idx].completion - ps[idx].arrival;
        ps[idx].waiting = ps[idx].start - ps[idx].arrival;
        gantt.push_back({ ps[idx].pid, ps[idx].start, ps[idx].completion });
        time = ps[idx].completion;
        done[idx] = true;
        completed++;
    }
}

void printResults(const vector<Process>& ps, const vector<GanttBlock>& gantt) {
    cout << "PID Arrival Burst Priority Start Complete Waiting Turnaround" ;
        for (auto& p : ps) {
            cout << p.pid << ' ' << p.arrival << ' ' << p.burst << ' ' << p.priority << ' '
                << p.start << ' ' << p.completion << ' ' << p.waiting << ' ' << p.turnaround << ' ';
        }
    cout << "Gantt Chart:";
        for (auto& g : gantt) {
            cout << "P" << g.pid << ": [" << g.start << ", " << g.finish << ' ';
        }
}

int main() {
    ifstream fin("input.txt");
    int n; fin >> n;
    vector<Process> ps(n);
    for (int i = 0; i < n; ++i) {
        fin >> ps[i].pid >> ps[i].arrival >> ps[i].burst >> ps[i].priority;
    }
    vector<GanttBlock> gantt;
    schedule(ps, gantt);
    printResults(ps, gantt);
    return 0;
}

