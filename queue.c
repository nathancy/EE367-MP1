
// Name:  <Nathan Lam1>
// EE 367 Spring 2016 MP1
// This program simulates server systems 


#include <stdio.h>
#include <stdlib.h>

#define COSTBLOCKED 10  // Cost of getting blocked
#define COST3       1100 // Cost of Warrior 3
#define COST4       1300 // Cost of Warrior 4

typedef struct req { // A request
    int arrival;     // Arrival time
    int service;     // Service time
    struct req *next;
} Request;

typedef struct { // Processor state 
    int busy;    // Indicates if processor is busy (busy=1, idle=0)
    int depart;  // Time when processor completes service.
} Processor;

typedef struct { //
    int occ;        //Number of items in queue
    int service1;  //Service time of the request 
    int service2;
    int service3;
    int service4;    
} Reqqueue;

typedef struct {
    int occ;
    int service1;  //Service time of the request 
    int service2;
    int service3;
    int service4;    
    int service5; 
    int service6;
    int service7;
    int service8;    
} Warrior4Queue;

typedef struct {
    int busy1;
    int busy2;
    int depart1;
    int depart2;
} WarriorProcessor;

Request * getrequests(void);  // Inputs request data from file 
int warrior3(Request * list); // Returns the # blocked for Warrior 3 
int dblwarrior(Request * list); // Returns the # blocked for Double Warrior 3
int warrior4(Request * list); // Returns the # blocked for Warrior 4 

void main()
{
    Request * reqqueue;
    int blocked3;   // Blocking for Warrior 3
    int blockeddbl; // Blocking for Double Warrior 3
    int blocked4;   // Blocking for Warrior 4

    reqqueue = getrequests();
    if (reqqueue == NULL) return;

    blocked3 = warrior3(reqqueue);
    printf("Warrior 3:\n");
    printf("   -> Daily number blocked = %d\n",blocked3);
    printf("\n");

    blockeddbl = dblwarrior(reqqueue);
    printf("Double Warrior:\n");
    printf("   -> Daily number blocked = %d\n",blockeddbl);
    printf("\n");

    blocked4 = warrior4(reqqueue);
    printf("Warrior 4:\n");
    printf("   -> Daily number blocked = %d\n",blocked4);
    printf("\n");
}

// Returns the number of blocked requests for Double Warrior 3 
// *** Implement this function *** 
int dblwarrior(Request * list)
{
    Processor proc1;
    Processor proc2;
    Reqqueue reqq1;
    Reqqueue reqq2;

    Request * nextreq;

    int ctime;
    int blocked = 0;
    int selector = 0;
    nextreq = list;
    proc1.busy = proc2.busy = 0;
    reqq1.occ = reqq2.occ = 0;

    reqq1.service1 = reqq1.service2 = reqq1.service3 = reqq1.service4 = 0;
    reqq2.service1 = reqq2.service2 = reqq2.service3 = reqq2.service4 = 0;

    for(ctime = 0; nextreq != NULL; ctime++) {
        //Arrival
        if (nextreq->arrival == ctime) {
            //Processor one
            if(selector%2 == 0)
            {
                //Full
                if (reqq1.occ == 4) blocked++;
                //Have spot
                else if (reqq1.occ == 0)
                {
                    reqq1.occ++;
                    reqq1.service1 = nextreq->service;
                }
                else if (reqq1.occ == 1)
                {
                    reqq1.occ++;
                    reqq1.service2 = nextreq->service;
                }
                else if (reqq1.occ == 2)
                {
                    reqq1.occ++;
                    reqq1.service3 = nextreq->service;
                }
                else if (reqq1.occ == 3)
                {
                    reqq1.occ++;
                    reqq1.service4 = nextreq->service;
                }
            }
            //Processor two
            else
            {
                if (reqq2.occ == 4) blocked++;
                else if (reqq2.occ == 0)
                {
                    reqq2.occ++;
                    reqq2.service1 = nextreq->service;
                }
                else if (reqq2.occ == 1)
                {
                    reqq2.occ++;
                    reqq2.service2 = nextreq->service;
                }
                else if (reqq2.occ == 2)
                {
                    reqq2.occ++;
                    reqq2.service3 = nextreq->service;
                }
                else if (reqq2.occ == 3)
                {
                    reqq2.occ++;
                    reqq2.service4 = nextreq->service;
                }
            }
            nextreq = nextreq->next;
            selector++;
        }

        // Transfer request from request queue to idle processor
        if (proc1.busy == 0 && reqq1.occ >0)
        {
            proc1.depart = ctime + reqq1.service1 - 1;
            proc1.busy =1;
            reqq1.occ--;

            reqq1.service1 = reqq1.service2;
            reqq1.service2 = reqq1.service3;
            reqq1.service3 = reqq1.service4;
        }
        if (proc2.busy == 0 && reqq2.occ > 0)
        {
            proc2.depart = ctime + reqq2.service1 - 1;
            proc2.busy = 1;
            reqq2.occ--;

            reqq2.service1 = reqq2.service2;
            reqq2.service2 = reqq2.service3;
            reqq2.service3 = reqq2.service4;
        }

        //End of time slot
        // Remove requests from the processor that have completed service
        if (proc1.busy == 1 && proc1.depart == ctime) proc1.busy = 0;
        if (proc2.busy == 1 && proc2.depart == ctime) proc2.busy = 0;

        // Transfer request from request queue to idle processor
        if (proc1.busy == 0 && reqq1.occ >0)
        {
            proc1.depart = ctime + reqq1.service1;
            proc1.busy =1;
            reqq1.occ--;

            reqq1.service1 = reqq1.service2;
            reqq1.service2 = reqq1.service3;
            reqq1.service3 = reqq1.service4;
        }
        if (proc2.busy == 0 && reqq2.occ > 0)
        {
            proc2.depart = ctime + reqq2.service1;
            proc2.busy = 1;
            reqq2.occ--;

            reqq2.service1 = reqq2.service2;
            reqq2.service2 = reqq2.service3;
            reqq2.service3 = reqq2.service4;
        }
    }
    return blocked;
}

// Returns the number of blocked requests for Warrior 4 
// *** Implement this function ***
int warrior4(Request * list)
{
    WarriorProcessor proc;
    Request * nextreq;
    Warrior4Queue reqq;

    int ctime;
    int blocked = 0;

    nextreq = list;
    proc.busy1 = proc.busy2 = 0;
    reqq.occ = 0;

    reqq.service1 = 0;
    reqq.service2 = 0;
    reqq.service3 = 0;
    reqq.service4 = 0;
    reqq.service5 = 0;
    reqq.service6 = 0;
    reqq.service7 = 0;
    reqq.service8 = 0;

    for (ctime = 0; nextreq != NULL; ctime++) {
        // Beginning of the time slot
        // If there is an arrival then put into request queue 
        if (nextreq->arrival == ctime) { // Arrival
            //If full
            if (reqq.occ == 8) blocked++;
            //If have a spot
            else if (reqq.occ == 0)
            {
                reqq.occ++;
                reqq.service1 = nextreq->service;
            }
            else if (reqq.occ == 1)
            {
                reqq.occ++;
                reqq.service2 = nextreq->service;
            }
            else if (reqq.occ == 2)
            {
                reqq.occ++;
                reqq.service3 = nextreq->service;
            }
            else if (reqq.occ == 3)
            {
                reqq.occ++;
                reqq.service4 = nextreq->service;
            }
            else if (reqq.occ == 4)
            {
                reqq.occ++;
                reqq.service5 = nextreq->service;
            }
            else if (reqq.occ == 5)
            {
                reqq.occ++;
                reqq.service6 = nextreq->service;
            }
            else if (reqq.occ == 6)
            {
                reqq.occ++;
                reqq.service7 = nextreq->service;
            }
            else if (reqq.occ == 7)
            {
                reqq.occ++;
                reqq.service8 = nextreq->service;
            }
            nextreq = nextreq->next; // Point to the next arrival 
        }

        // Transfer a request from request queue to idle processor 
        // If processor idle and stuff in queue
        if (proc.busy1==0 && reqq.occ>0) {
            proc.depart1 = ctime + reqq.service1 - 1;
            proc.busy1=1;
            reqq.occ--;

            //Shift queue 
            reqq.service1 = reqq.service2;
            reqq.service2 = reqq.service3;
            reqq.service3 = reqq.service4;
            reqq.service4 = reqq.service5;
            reqq.service5 = reqq.service6;
            reqq.service6 = reqq.service7;
            reqq.service7 = reqq.service8;
        } 
        // If processor idle and stuff in queue
        if (proc.busy2==0 && reqq.occ>0) {
            proc.depart2 = ctime + reqq.service1 - 1;
            proc.busy2=1;
            reqq.occ--;

            //Shift queue 
            reqq.service1 = reqq.service2;
            reqq.service2 = reqq.service3;
            reqq.service3 = reqq.service4;
            reqq.service4 = reqq.service5;
            reqq.service5 = reqq.service6;
            reqq.service6 = reqq.service7;
            reqq.service7 = reqq.service8;
        } 

        // End of the time slot 
        // Remove requests from the processor that have completed service 
        if (proc.busy1==1 && proc.depart1==ctime) proc.busy1=0;
        if (proc.busy2==1 && proc.depart2==ctime) proc.busy2=0;

        if (proc.busy1==0 && reqq.occ>0) {
            proc.depart1 = ctime + reqq.service1;
            proc.busy1=1;
            reqq.occ--;

            //Shift queue 
            reqq.service1 = reqq.service2;
            reqq.service2 = reqq.service3;
            reqq.service3 = reqq.service4;
            reqq.service4 = reqq.service5;
            reqq.service5 = reqq.service6;
            reqq.service6 = reqq.service7;
            reqq.service7 = reqq.service8;
        } 
        // If processor idle and stuff in queue
        if (proc.busy2==0 && reqq.occ>0) {
            proc.depart2 = ctime + reqq.service1;
            proc.busy2=1;
            reqq.occ--;

            //Shift queue 
            reqq.service1 = reqq.service2;
            reqq.service2 = reqq.service3;
            reqq.service3 = reqq.service4;
            reqq.service4 = reqq.service5;
            reqq.service5 = reqq.service6;
            reqq.service6 = reqq.service7;
            reqq.service7 = reqq.service8;
        } 
    }
    return blocked;
} 

// Returns the number of blocked request for Warrior 3
// Currently it assumes a request queue size of 1.  
// *** Rewrite it so it has a queue size of 4. *** 
int warrior3(Request * list)
{
    Processor proc; // Processor
    Request * nextreq;  // Next arriving request.
    Reqqueue  reqq;

    int ctime;
    int blocked=0; // Number of requests blocked

    nextreq=list;
    proc.busy=0;   // Initialize processor
    reqq.occ = 0;  // Initialize the request queue. 

    reqq.service1 = 0;
    reqq.service2 = 0;
    reqq.service3 = 0;
    reqq.service4 = 0;

    for (ctime=0; nextreq!= NULL; ctime++) {

        // Beginning of the time slot
        // If there is an arrival then put into request queue 
        if (nextreq->arrival == ctime) { // Arrival
            //If full
            if (reqq.occ == 4) blocked++;
            //If have a spot
            else if (reqq.occ == 0)
            {
                reqq.occ++;
                reqq.service1 = nextreq->service;
            }
            else if (reqq.occ == 1)
            {
                reqq.occ++;
                reqq.service2 = nextreq->service;
            }
            else if (reqq.occ == 2)
            {
                reqq.occ++;
                reqq.service3 = nextreq->service;
            }
            else if (reqq.occ == 3)
            {
                reqq.occ++;
                reqq.service4 = nextreq->service;
            }
            nextreq = nextreq->next; // Point to the next arrival 
        }

        // Transfer a request from request queue to idle processor 
        // If processor idle and stuff in queue
        if (proc.busy==0 && reqq.occ>0) {
            proc.depart = ctime + reqq.service1 - 1;
            proc.busy=1;
            reqq.occ--;

            //Shift queue 
            reqq.service1 = reqq.service2;
            reqq.service2 = reqq.service3;
            reqq.service3 = reqq.service4;
        } 

        // End of the time slot 
        // Remove requests from the processor that have completed service 
        if (proc.busy==1 && proc.depart==ctime) proc.busy=0;

        // Transfer a request from request queue to idle processor 
        if (proc.busy == 0 && reqq.occ>0) {
            proc.depart = ctime + reqq.service1;
            proc.busy=1;
            reqq.occ--;

            reqq.service1 = reqq.service2;
            reqq.service2 = reqq.service3;
            reqq.service3 = reqq.service4;
        } 
    } 
    return blocked;
}


// Queries user for a data file that has a list of requests.
// The data is read and put into a linked list.
// The link to the link list is returned.  If the return value
// is NULL then there are no requests or the file couldn't be open.

Request * getrequests(void)
{
    FILE *fp;
    char filename[200];
    int i;
    int num;       // Number of requests
    int arrival;   // Arrival time of request
    int service;   // Service time of request 
    Request *head;    // Head of the queue
    Request *tail;    // Tail of queue
    Request *new;

    printf("Enter file with arriving customer data: ");
    scanf("%s",filename);
    fp = fopen(filename,"r");
    if (fp==NULL) {
        printf("File '%s' not found or can't be opened\n",filename);
        return(NULL); 
    }
    fscanf(fp,"%d",&num);
    printf("Number of arriving customers = %d\n",num);

    // Fill queue with customer data
    head=tail=NULL;  // Initialize queue
    for (i=0; i<num; i++) {
        // Load a new customer and create an item for it
        fscanf(fp,"%d %d",&arrival,&service);
        new = (Request*) malloc(sizeof(Request));
        new->arrival = arrival;
        new->service = service;
        new->next = NULL;
        // Insert into queue
        if (head==NULL) { // Insert into empty queue
            head=new;
            tail=new;
        }
        else {
            tail->next = new; // Insert at the tail
            tail=new;         
        }
        //   printf("Customer %i: %d %d\n",i,tail->arrival,tail->service);
        //   Above is used to debug -- check if everything was loaded properly
    }

    fclose(fp);

    if (head==NULL) printf("No requests in the file\n");
    return(head);
}


