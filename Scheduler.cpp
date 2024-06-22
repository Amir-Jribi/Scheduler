#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
using namespace std;

struct Task{
	int id;
	int burstTime;  //Total execution time required by a process on the CPU.
	int arrivalTime; //Time when a process enters the ready queue.
	Task(int id_,int burstTime_,int arrivalTime_ ):id(id_),burstTime(burstTime_),arrivalTime(arrivalTime_){}
	bool operator<(const Task& other)const  {
        return burstTime > other.burstTime; // For the comparison in priority queue.
        //prioritize the tasks with high burst time
    }
};

bool comp(const Task& t1,const Task& t2){
	if (t1.arrivalTime==t2.arrivalTime){
		return t1.burstTime<t2.burstTime;
	}
	return t1.arrivalTime<t2.arrivalTime;
}


class Scheduler{
public:
	vector<Task> Tasks;
	int currentTime=0;
	int quantum=2;

public:

	Scheduler(vector<Task> Tasks_):Tasks(Tasks_){
		sort(Tasks.begin(),Tasks.end(),comp);
	}
	void addTask(Task task){
		Tasks.push_back(task);
	}

	void executeFIFO(){
		// first in first out
		// Processes are scheduled in the order they arrive.
		for(Task task:Tasks){
			executeTask(task);
		}
		currentTime=0;

	}
	void executeSJF(){
		// shortest job first
		//  Processes with the shortest burst time are scheduled first.
		priority_queue<Task> Queue;

		int idx=0;

		while(!Queue.empty() || idx<Tasks.size()){
			while(idx<Tasks.size() && Tasks[idx].arrivalTime<=currentTime){
				Queue.push(Tasks[idx++]);
			}
			if (!Queue.empty()){
				Task currentTask=Queue.top();
				Queue.pop();
				executeTask(currentTask);
			}
			else if (idx<Tasks.size()){
				currentTime+=(Tasks[idx].arrivalTime-currentTime);
				Queue.push(Tasks[idx++]);
			}
		}
		currentTime=0;		
	}
	void executeRoundRobin(){
		// Round Robin
		// Each process gets a small unit of CPU time (time quantum) in a cyclic order.
		// Starvation occurs when a process is perpetually delayed and never gets executed.
		queue<Task>Queue;
		int idx=0;
		while(!Queue.empty() || idx<Tasks.size()){
			// cout<<idx<<endl;
			while(idx<Tasks.size() && Tasks[idx].arrivalTime<=currentTime){
				Queue.push(Tasks[idx++]);
			}

			if (!Queue.empty()){
				Task currentTask=Queue.front();
				Queue.pop();
				if (currentTask.burstTime<=quantum){
					executeTask(currentTask,true);
				}
				else {
					executeTask(currentTask,false);
					currentTask.burstTime-=quantum;
					Queue.push(currentTask);
				}
			}
			else if (idx<Tasks.size()){
				currentTime+=(Tasks[idx].arrivalTime-currentTime);
				Queue.push(Tasks[idx++]);
			}
		}
		currentTime=0;
		
	}

	private:
    void executeTask(const Task& task) {
        cout << "Executing Task ID " << task.id << " at time " << currentTime << endl;
        currentTime += task.burstTime;
        cout << "Task ID " << task.id << " completed at time " << currentTime << endl;
    }
    void executeTask(const Task& task,bool ok) { //method overloading
        cout << "Executing Task ID " << task.id << " at time " << currentTime << endl;
        if (ok==1)currentTime += task.burstTime;
        else currentTime+= quantum;
        cout << "Task ID " << task.id << " completed at time " << currentTime << endl;
    }
};

int main(){
	vector<Task> tasks = {
        {1, 8, 0}, // Task ID, Burst Time, Arrival Time
        {2, 4, 0},
        {3, 9, 18},
        {4, 5, 3}
    };
    // priority_queue<Task> q;
    // q.push(tasks[0]);q.push(tasks[1]);q.push(tasks[2]);q.push(tasks[3]);

    // while(!q.empty()){
    // 	Task t=q.top();
    // 	q.pop();
    // 	cout<<t.id<<" "<<t.burstTime<<" "<<t.arrivalTime<<endl;
    // }

    Scheduler scheduler(tasks);

    // for(int i=0;i<scheduler.Tasks.size();i++){
    // 	cout<<scheduler.Tasks[i].arrivalTime<<endl;
    // }
    // scheduler.executeFIFO();

    // scheduler.executeSJF();

    vector<Task> tasks1 = {
        {1, 8, 0}, // Task ID, Burst Time, Arrival Time
        {2, 4, 0},
        {3, 9, 0},
        {4, 5, 0}
    };
    Scheduler scheduler1(tasks1);
    scheduler1.executeRoundRobin();

	return 0;
}