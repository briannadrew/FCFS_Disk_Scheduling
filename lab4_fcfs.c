/*********************************************************************/
/* Name: Disk Scheduling Simulation FCFS (Lab 4)                     */
/* Author: Brianna Drew                                              */
/* Description                                                       */
/*    This program simulates disk scheduling for a disk with 800     */
/*    tracks with an FCFS scheduling algorithm.                      */
/*********************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <values.h>
#include <time.h>
#include <stdbool.h>

// linked list to represent the queue
struct Queue_Node{
  int trackNumber; // track number of the track request in the queue
  struct Queue_Node *next; // pointer to the next node in queue
};

struct Queue_Struct{
  struct Queue_Node *q_head; // pointer to first node in queue
  struct Queue_Node *q_tail; // pointer to last node in queue
};

// create the queue
struct Queue_Struct fcfs;

// function declarations
int trackReqs(void);
int numTrack(void);
void AddToList(struct Queue_Struct *req_queue, int trackNum);
int RemoveFromList(struct Queue_Struct *req_queue);
void ProcessReqs(struct Queue_Struct *req_queue);

// global variable declarations/initializations
unsigned seed;
int fileReqs;
bool first = true;
float diff;
float totalHeadMove = 0;
int currTrack = 0;
int totalReqs = 0;

void main(){
  fcfs.q_head = NULL; // initialize head of queue to null
  fcfs.q_tail = NULL; // initialize tail of queue to null
  printf("*********************************************************************\n");
  printf("*********************************************************************\n");
  printf("**                                                                 **\n");
  printf("**                DISK SCHEDULING SYSTEM SIMULATION                **\n");
  printf("**                          FCFS EDITION                           **\n");
  printf("**                         By Brianna Drew                         **\n");
  printf("**                                                                 **\n");
  printf("*********************************************************************\n");
  printf("*********************************************************************\n\n");
  printf("Seed for the random number generator: ");
  scanf("%d", &seed); // get user input as integer for seed for random number generation
  srand(seed);
  printf("Number of file requests: ");
  scanf("%d", &fileReqs); // get user input as integer for number of file requests

  // local variable declarations/initializations
  int numFileReqs = 0;
  float average;

  do { // do this...
    int numTrackReqs = trackReqs(); // call function to get a random number between 1 and 5 to represent the number of track requests for the current file request
    int i;
    for (i = 0; i < numTrackReqs; i++) { // for each track request for the current file request...
      int trackNum = numTrack(); // call function to get a random number between 0 and 799 to represent the number of the track requested
      AddToList(&fcfs, trackNum); // call function to add the track request to the queue
      first = false;
    }
    int nextTrack = RemoveFromList(&fcfs); // call function to remove the next (first) track request from the queue (signifying that the disk head will be moved to that track) and have that track returned
    diff = abs((float)nextTrack - (float)currTrack); // calculate the head movement for the current track request
    totalHeadMove += diff; // add the head movement for the current track request to the total head movement
    totalReqs++; // increase number of total track requests by 1
    currTrack = nextTrack; // make the current track now the next track
    numFileReqs++; // increase number of file requests by 1
  } while(numFileReqs <= fileReqs); // ...for each file request
  ProcessReqs(&fcfs); // call function to remove the remaining track requests from the queue
  average = totalHeadMove / (float) totalReqs; // calculate the average total head movement for each file request and print the result
  printf("Average head movement: %5.2f\n", average);
}

int trackReqs(void){
  int rand_num = (rand() % (5 - 1 + 1)) + 1; // generate random number from 1 to 5 representing number of track requests for the current file request
  return rand_num;
}

int numTrack(void){
  int rand_num = rand() % 800; // generate random number from 0 to 799 representing
  return rand_num;
}

void AddToList(struct Queue_Struct *req_queue, int trackNum){

  struct Queue_Node *newnode; // create new node to be inserted into the queue linked list
  newnode = (struct Queue_Node *) malloc(sizeof(struct Queue_Node)); // create the required space for the node in the queue linked list
  newnode->next = NULL; // initialize the next pointer of the newly created node to null (as it is being added to the end of the queue)
  newnode->trackNumber = trackNum; // insert given track number data into the new node

  // if the queue is initially empty...
  if(req_queue->q_tail == NULL){
    req_queue->q_head = newnode; // set queue head pointer to the new node
    req_queue->q_tail = newnode; // set queue tail pointer to the new node
    return;
  }
  // if not, then...
  req_queue->q_tail->next = newnode; // link new node to the end of the queue
  req_queue->q_tail = newnode; // set queue tail pointer to the new node
  return;
}

int RemoveFromList(struct Queue_Struct *req_queue){

  struct Queue_Node *loc; // create a new temporary node to hold the first request from the queue to be removed
  int first_req; // integer representing the track number of the first request in the queue

  // if the queue is empty...
  if(req_queue->q_head == NULL){
    printf("***Error - Queue is Empty***\n"); // print error message
    return(NULL); // return null as the track number for first request in the queue
  }
  loc = req_queue->q_head; // store the first request in the queue in the temporary node
  first_req = loc->trackNumber; // get the track number of the first request in the queue

  // if there is only one request in the queue...
  if(req_queue->q_head == req_queue->q_tail){
    req_queue->q_tail = NULL; // set queue tail pointer to null (empty queue)
    req_queue->q_head = NULL; // set queue head pointer to null (empty queue)
    return first_req; // return the track number of the first request in the queue
  }
  req_queue->q_head = loc->next; // set the next request in the queue now as the first request
  free(loc); // free the temporary node
  return first_req; // return the track number of the first request in the queue
}

void ProcessReqs(struct Queue_Struct *req_queue){

  while(req_queue->q_head != NULL){ // while the queue is not empty...
    int nextTrack = RemoveFromList(&fcfs); // call function to remove the next (first) track request from the queue (signifying that the disk head will be moved to that track) and have that track returned
    diff = abs((float)nextTrack - (float)currTrack); // calculate the head movement for the current track request
    totalHeadMove += diff; // add the head movement for the current track request to the total head movement
    totalReqs++; // increase number of total track requests by 1
    currTrack = nextTrack; // make the current track now the next track
  }
  return;
}
