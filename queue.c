
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
    int busy1;
    int busy2;
    int depart1;
    int depart2;
} TwoProcessor;

typedef struct {
    int occ1;
    int occ2;
    int one_service1;
    int one_service2;
    int one_service3;
    int one_service4;
    int two_service1;
    int two_service2;
    int two_service3;
    int two_service4;
} TwoRequeue;



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
   TwoProcessor proc;
   Request * nextreq;
   TwoRequeue reqq;

   int ctime;
   int blocked = 0;
   int proc_selector;

   nextreq = list;
   proc.busy1 = proc.busy2 = 0;
   reqq.occ1 = reqq.occ2 = 0;

   reqq.one_service1 = reqq.one_service2 = reqq.one_service3 = reqq.one_service4 = 0;
   reqq.two_service1 = reqq.two_service2 = reqq.two_service3 = reqq.two_service4 = 0;

    for(ctime = 0; nextreq != NULL; ctime++) {
        if(nextreq->arrival == ctime) {
            proc_selector = ctime % 2; //0 for even, 1 for odd

            //Arrival
            //Even number, so processor 1
            if(proc_selector == 0) {
                if(reqq.occ1 == 4) blocked++;
                else if (reqq.occ1 == 0)
                {
                    reqq.occ1++;
                    reqq.one_service1 = nextreq->service;
                }
                else if (reqq.occ1 == 1)
                { 
                    reqq.occ1++;
                    reqq.one_service2 = nextreq->service;
                }
                else if (reqq.occ1 == 2)
                {
                    reqq.occ1++;
                    reqq.one_service3 = nextreq->service;
                }
                else if (reqq.occ1 == 3)
                {
                    reqq.occ1++;
                    reqq.one_service4 = nextreq->service;
                }
            }
            //Odd number, so processor 2
            else {
                if(reqq.occ2 == 4) blocked++;
                else if (reqq.occ2 == 0)
                {
                    reqq.occ2++;
                    reqq.two_service1 = nextreq->service;
                }
                else if (reqq.occ2 == 1)
                {
                    reqq.occ2++;
                    reqq.two_service2 = nextreq->service;
                }
                else if (reqq.occ2 == 2)
                {
                    reqq.occ2++;
                    reqq.two_service3 = nextreq->service;
                }
                else if (reqq.occ2 == 3)
                {
                    reqq.occ2++;
                    reqq.two_service4 = nextreq->service;
                }
            }
            nextreq = nextreq->next; // Point to the next arrival 
        }

        //Transfer request from request queue to idle processor
        //Processor one
        if (proc.busy1 == 0 && reqq.occ1 >0)
        {
            proc.depart1 = ctime + reqq.one_service1 - 1;
            proc.busy1 = 1;
            reqq.occ1--;

            reqq.one_service1 = reqq.one_service2;
            reqq.one_service2 = reqq.one_service3;
            reqq.one_service3 = reqq.one_service4;
        }
        //Processor two
        if (proc.busy2 == 0 && reqq.occ2 > 0)
        {
            proc.depart2 = ctime + reqq.two_service1 -1;
            proc.busy2 = 1;
            reqq.occ2--;

            reqq.two_service1 = reqq.two_service2;
            reqq.two_service2 = reqq.two_service3;
            reqq.two_service3 = reqq.two_service4;
        }

        //End of time slot
        //Remove requests from the processor that have completed service
        //Processor one
        if (proc.busy1 == 1 && proc.depart1 == ctime) proc.busy1 = 0;
        //Processor two
        if (proc.busy2 == 1 && proc.depart2 == ctime) proc.busy2 = 0;

        //Transfer request from request queue to idle processor
        //Processor one
        if (proc.busy1 == 0 && reqq.occ1 >0)
        {
            proc.depart1 = ctime + reqq.one_service1;
            proc.busy1 = 1;
            reqq.occ1--;

            reqq.one_service1 = reqq.one_service2;
            reqq.one_service2 = reqq.one_service3;
            reqq.one_service3 = reqq.one_service4;
        }
        //Processor two
        if (proc.busy2 == 0 && reqq.occ2 > 0)
        {
            proc.depart2 = ctime + reqq.two_service1;
            proc.busy2 = 1;
            reqq.occ2--;

            reqq.two_service1 = reqq.two_service2;
            reqq.two_service2 = reqq.two_service3;
            reqq.two_service3 = reqq.two_service4;
        }
    }
    return blocked;
}

        
                

            

 

// Returns the number of blocked requests for Warrior 4 
// *** Implement this function ***
int warrior4(Request * list)
{
    return 0;
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


