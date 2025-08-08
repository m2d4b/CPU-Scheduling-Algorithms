#include <iostream>
#include <vector>
#include <fstream>   // For file operations
#include <sstream>   // For string parsing
#include <string>    // For strin
using namespace std ; 

struct process {
    int bt ; //burst time 
    int priority ; 
    int queueID ;
    int wt ; //Waiting time
} ;
 vector <process> pc;

double avrgetime (vector <process> processes){
    double sum = 0;
    for(int i = 0 ; i < processes.size() ;i++ ){
        sum += processes[i].wt;
    }
    double awt = sum / processes.size();
    return awt ; 
}


 void findwitedtime( vector <process> & processes){
    if(processes.empty()){
        return ;
    }
    processes[0].wt = 0 ; //becouse the frist process alwoes is 0 wt 
    for (int i = 1 ; i < processes.size() ; i++){
        processes[i].wt = processes[i-1].wt + processes[i-1].bt;
    }

}

//sjf
void sjf (vector<process>&processes){
    //sort by  Bubble Sort    
    for (int i = 0 ;i < processes.size()-1 ;i++ ){
        for (int j =0 ; j<processes.size()-i-1 ;j++){
            if(processes[j].bt > processes[j+1].bt) {
            
                process temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
        }
        
    }
}
    // Calculate waiting times
    findwitedtime(processes) ;
}

void priority(vector<process>& processes) {
    if(processes.empty()) return;
    
    // Sort 
    for(int i = 0; i < processes.size()-1; i++) {
        for(int j = 0; j < processes.size()-i-1; j++) {
            if(processes[j].priority > processes[j+1].priority) {
                // Swap priority
                process temp = processes[j];
                processes[j] = processes[j+1];
                processes[j+1] = temp;
            }
        }
    }
    // Calculate waiting times
    findwitedtime(processes) ;
 }




void readinputfile(string filename){
       
    ifstream file(filename);  
    
    if (!file.is_open()) {
        cout << "Error: Cannot open input file " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {  
        process p;
        
        
        stringstream ss(line);
        string item;
        
        
        getline(ss, item, ':');
        p.bt = stoi(item);  
        
        
        getline(ss, item, ':');
        p.priority = stoi(item);
        
        
        getline(ss, item, ':');
        p.queueID = stoi(item);
        
        p.wt = 0; 
        
        pc.push_back(p);  
    }
    
    file.close();  

}
void writeOutputFile(string filename,
                     vector<vector<process>> fcfs_all,
                     vector<vector<process>> sjf_all,
                     vector<vector<process>> priority_all) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error writing file!" << endl;
        return;
    }

    for (int i = 0; i < fcfs_all.size(); i++) {
        if (!fcfs_all[i].empty()) {

        // FCFS
        file << i << ":1:";
        for (int j = 0; j < fcfs_all[i].size(); j++) file << fcfs_all[i][j].wt << ":";
        file << avrgetime(fcfs_all[i]) << endl;

        // SJF
        file << i << ":2:";
        for (int j = 0; j < sjf_all[i].size(); j++) file << sjf_all[i][j].wt << ":";
        file << avrgetime(sjf_all[i]) << endl;

        // Priority
        file << i << ":3:";
        for (int j = 0; j < priority_all[i].size(); j++) file << priority_all[i][j].wt << ":";
        file << avrgetime(priority_all[i]) << endl;
    }
}

    file.close();
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "plz put the input and output text file" << endl;
        return 1;
    }

    string input_filename = argv[1];
    string output_filename = argv[2];
    readinputfile(input_filename);

    // Find max queue ID to create enough vectors
    int maxQueueID = 0;
    for (int i = 0; i < pc.size(); i++) {
        if (pc[i].queueID > maxQueueID)
            maxQueueID = pc[i].queueID;
    }

    int totalQueues = maxQueueID + 1;

    // Create queues
    vector<vector<process>> queues(totalQueues);
    for (int i = 0; i < pc.size(); i++) {
        queues[pc[i].queueID].push_back(pc[i]);
    }  

    // Prepare result containers
    vector<vector<process>> fcfs_all(totalQueues);
    vector<vector<process>> sjf_all(totalQueues);
    vector<vector<process>> priority_all(totalQueues);

    for (int i = 0; i < totalQueues; i++) {
        fcfs_all[i] = queues[i];
        sjf_all[i] = queues[i];
        priority_all[i] = queues[i];

        findwitedtime(fcfs_all[i]);
        sjf(sjf_all[i]);
        priority(priority_all[i]);
    }

    writeOutputFile(output_filename, fcfs_all, sjf_all, priority_all);
    return 0;
}
