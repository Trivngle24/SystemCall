// Structure to hold values returned by process_ancestors sys-call

#ifndef _PROCESS_ANCESTORS_H
#define _PROCESS_ANCESTORS_H

#define ANCESTOR_NAME_LEN 16
struct process_info{
	long pid;						//Process ID
	char name[ANCESTOR_NAME_LEN];	//Program name of process
	long state;						//Current process state
	long uid;						//User ID of process owner
	long nvcsw;						//number of voluntary context switches
	long nivcsw;					//number of involuntary context switches
	long num_children;				//number of children a process has
	long num_siblings;				//number of sibling a process has
};

#endif