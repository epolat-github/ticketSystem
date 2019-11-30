#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXCHAR 500

enum classes {business, economy, standard};
const char* classNames[] = {"business", "economy", "standard"};

//flight struct
struct flight
{
    char flightName[7];
    int businessCount, economyCount, standardCount;

    //for priority queue
    struct queueNode *rootNode;
    int hasRoot;
};

//ticket struct
struct ticket
{
    char *flightName; //check
    enum classes class;
};

//STACK FOR SEATS
struct SeatNode     //represents each seat
{
    char *flightName;
    enum classes class;
    struct SeatNode* next;
};

// add new seat to stack
struct SeatNode* newSeat(struct ticket ticket)
{
    struct SeatNode* seatNode;
    seatNode = (struct SeatNode*)malloc(sizeof(struct SeatNode)); 
    seatNode->class = ticket.class;
    seatNode->flightName = ticket.flightName;
    seatNode->next = NULL;
    return seatNode;
}

//Check Stack Empty
int checkEmpty(struct SeatNode* main)
{
    return !main;
}

//Push Ticket to the Stack
void push(struct SeatNode** main, struct ticket ticket) 
{ 
    struct SeatNode* SeatNode = newSeat(ticket); 
    SeatNode->next = *main; 
    *main = SeatNode; 
    // printf("pushed\n");
}

//Pop Ticket from Stack
struct ticket pop(struct SeatNode** main)
{
    if (checkEmpty(*main)){
        printf("Empty Stack!");
    }
    else{
        struct SeatNode* temp = *main;
        *main = (*main)->next;
        struct ticket poppedItem;
        poppedItem.flightName = temp->flightName;
        poppedItem.class = temp->class;
        free(temp);

        return poppedItem;
    }
}

//Peek the Last Ticket
void peek(struct SeatNode* main){
    if (checkEmpty(main)){
        printf("Empty Stack!");
    }
    else{
        printf("Flight Name: %s, Ticket Class: %s\n", main->flightName, classNames[main->class]);
    }
}


// PRIORITY QUEUE FOR PASSENGERS
//passenger struct
struct passenger
{
    char *passengerName;
    char *flightName;
    enum classes wantedClass;
    enum classes givenClass;

};

struct queueNode
{
    struct passenger passenger;
    int priority;
    struct queueNode *next;
};

struct queueNode *newQueueNode(char *flightName, char *classStr, char *passengerName, int priority)
{
    struct queueNode *temp = malloc(sizeof(struct queueNode));
    temp->priority = priority;
    temp->next = NULL;
    temp->passenger.flightName = flightName;
    temp->passenger.passengerName = passengerName;

    int class;
    if(strcmp(classStr, "business") == 0){
        class = 0;
    }
    else if(strcmp(classStr, "economy") == 0){
        class = 1;
    }
    else if(strcmp(classStr, "standard") == 0){
        class = 2;
    }
    else{
        printf("Undetermined class.\n");
    }

    temp->passenger.wantedClass = class;

    return temp;
}

void pushQueue(struct queueNode **head, char *flightName, char *classStr, char *passengerName, int priority){
    struct queueNode *start = (*head);

    struct queueNode *temp = newQueueNode(flightName, classStr, passengerName, priority);

    if((*head)->priority > priority){
        temp->next = *head;
        (*head)->next = temp;
    }
    else{
        while(start->next != NULL && start->next->priority < priority){
            start = start->next;
        }

        temp->next = start->next;
        start->next = temp;
    }
}

struct passenger peekQueue(struct queueNode **head){
    return (*head)->passenger;
}

void popQueue(struct queueNode** head){
    struct queueNode *temp = *head;
    (*head) = (*head)->next;
    free(temp);
}


int main(int argc, char *argv[]){

    FILE *fptr;
    char inputLine[MAXCHAR];

    int flightCount = 0;

    struct flight *flightsArray = NULL;

    //check parameter count
    if (argc != 3){
        printf("Wrong parameters.");
        return -1;
    }    

    //open input file
    fptr = fopen(argv[1], "r"); 
    
    //check whether input file is found
    if(fptr == NULL){
        printf("No input file found.");
        return -1;
    }

    //Declare Stack Root Node
    struct SeatNode *main = NULL;

    
    

    //Read Line by Line
    while (fgets(inputLine, MAXCHAR, fptr) != NULL){
        char *paramPtr = strtok(inputLine, " ");
        
        while(paramPtr != NULL){
            // "addseat" command
            if(strcmp(paramPtr, "addseat") == 0){

                struct ticket ticketTemp;

                //Ticket Name
                paramPtr = strtok(NULL, " ");
                ticketTemp.flightName = paramPtr;
               
                //Ticket Class
                paramPtr = strtok(NULL, " ");
                int class_tmp;
                if(strcmp(paramPtr, "business") == 0){
                    ticketTemp.class = 0;
                    class_tmp = 0;
                }
                else if(strcmp(paramPtr, "economy") == 0){
                    ticketTemp.class = 1;
                    class_tmp = 1;
                }
                else if(strcmp(paramPtr, "standard") == 0){
                    ticketTemp.class = 2;
                    class_tmp = 2;
                }
                else{
                    printf("Wrong ticket class!");
                }

                //Ticket Quantity
                paramPtr = strtok(NULL, " ");
                int ticketCount = atoi(paramPtr);

                //Push to the Stack
                for (size_t i = 0; i < ticketCount; i++)
                {
                    push(&main, ticketTemp);    //Add Ticket
                    // peek(main);

                    struct flight flight_temp;

                    if (flightsArray == NULL){ //no other flights
                        flightsArray = malloc(sizeof(*flightsArray));
                        strcpy(flight_temp.flightName, ticketTemp.flightName); //check
                        flight_temp.businessCount = 0;
                        flight_temp.economyCount = 0;
                        flight_temp.standardCount = 0;

                        switch (ticketTemp.class)
                        {
                        case 0: //business
                            flight_temp.businessCount++;
                            break;
                        case 1: //economy
                            flight_temp.economyCount++;
                            break;
                        case 2: //standard
                            flight_temp.standardCount++;
                            break;                        
                        default:
                            printf("wrong class.");
                            break;
                        }
                        *(flightsArray) = flight_temp;
                        
                        
                        flightCount++;
                    }
                    else{ //there are other flights
                        //check if the flight exists already
                        int foundFlag = 0;
                        for (size_t i = 0; i < flightCount; i++)
                        {
                            //flight found
                            if(strcmp(ticketTemp.flightName, (*(flightsArray + i)).flightName) == 0){
                                switch (ticketTemp.class)
                                {
                                case 0: //business
                                    foundFlag = 1;
                                    (*(flightsArray + i)).businessCount++;
                                    break;
                                case 1: //economy
                                    foundFlag = 1;
                                    (*(flightsArray + i)).economyCount++;
                                    break;
                                case 2: //standard
                                    foundFlag = 1;
                                    (*(flightsArray + i)).standardCount++;
                                    break;                        
                                default:
                                    printf("wrong class.");
                                    break;
                                }
                                break;
                            }
                        }
                        //if not found
                        if (foundFlag == 0){
                            //check the realloc part, might be null
                            flightsArray = realloc(flightsArray, (flightCount + 1) * sizeof(*flightsArray));
                            strcpy(flight_temp.flightName, ticketTemp.flightName);
                            flight_temp.businessCount = 0;
                            flight_temp.economyCount = 0;
                            flight_temp.standardCount = 0;

                            switch (ticketTemp.class)
                            {
                            case 0: //business
                                flight_temp.businessCount++;
                                break;
                            case 1: //economy
                                flight_temp.economyCount++;
                                break;
                            case 2: //standard
                                flight_temp.standardCount++;
                                break;                        
                            default:
                                printf("wrong class.");
                                break;
                            }
                            *(flightsArray + flightCount) = flight_temp;
                            flightCount++;                            
                        }
                        
                    }
                }
                
                //find the index of the flight
                int index;
                for (size_t i = 0; i < flightCount; i++)
                {
                    if(strcmp(ticketTemp.flightName, (*(flightsArray + i)).flightName) == 0){
                        index = i;
                    }
                }
                 
                //proper output
                printf("addseats %s %d %d %d\n", ticketTemp.flightName,
                                                    flightsArray[index].businessCount,
                                                    flightsArray[index].economyCount,
                                                    flightsArray[index].standardCount);
            }

            // "enqueue" command
            else if (strcmp(paramPtr, "enqueue") == 0)
            {
                struct passenger passenger_temp;

                //Flight Name
                paramPtr = strtok(NULL, " ");
                char *flightName = paramPtr;

                //Class
                paramPtr = strtok(NULL, " ");
                char *classStr = paramPtr;

                //Passenger Name
                paramPtr = strtok(NULL, " ");
                char *passengerName = paramPtr;

                //Priority
                paramPtr = strtok(NULL, " ");
                char *priority = paramPtr;

                int priorityForQueue;

                if(strcmp(priority, "diplomat\r\n") == 0){
                    priorityForQueue = 0;
                }     
                else if(strcmp(classStr, "business") == 0){
                    priorityForQueue = 1;
                }
                else if(strcmp(priority, "veteran\r\n") == 0){
                    priorityForQueue = 2;
                }
                else if(strcmp(classStr, "economy") == 0){
                    priorityForQueue = 3;
                }
                else if(strcmp(classStr, "standard")){
                    priorityForQueue = 4;
                }

                int flightIndex = -1;
                for (size_t i = 0; i < flightCount; i++)
                {
                    if((strcmp(flightName, flightsArray[i].flightName)) == 0){  //found
                        flightIndex = i;
                        break;
                    }
                }
                
                struct queueNode *queueRoot;

                if(flightIndex == -1){  // flight not found
                    printf("No such flight!\n");
                }
                else{   //flight found
                    if(flightsArray[flightIndex].rootNode != 1){    //if queue has not initialized
                        flightsArray[flightIndex].rootNode = newQueueNode(flightName, classStr, passengerName, priorityForQueue);
                        flightsArray[flightIndex].rootNode = 1;
                    }
                    else{   //if queue has been initialized
                        pushQueue(&(flightsArray[flightIndex].rootNode), flightName, classStr, passengerName, priorityForQueue);
                    }
                    
                    
                }
                

                // printf("%s %s %d %s\n", queueRoot->passenger.flightName, queueRoot->passenger.passengerName,
                //                 queueRoot->priority, classNames[queueRoot->passenger.wantedClass]);

                
            }
            paramPtr = strtok(NULL, " ");
        }
    }

    
    fclose(fptr);

}

