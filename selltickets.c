#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXCHAR 500

enum classes
{
    business,
    economy,
    standard
};
const char *classNames[] = {"business", "economy", "standard"};

//flight struct
struct flight
{
    char flightName[8];
    //seat counts
    int businessCount, economyCount, standardCount;

    //for priority queue
    struct queueNode *rootNode;
    int hasRoot;
    int businessQueueCount, economyQueueCount, standardQueueCount;
    int totalPassengerCount;
};

//ticket struct
struct ticket
{
    char flightName[8]; //check
    enum classes class;
};

//STACK FOR SEATS
struct SeatNode //represents each seat
{
    char *flightName;
    enum classes class;
    struct SeatNode *next;
};

// add new seat to stack
struct SeatNode *newSeat(struct ticket ticket)
{
    struct SeatNode *seatNode;
    seatNode = (struct SeatNode *)malloc(sizeof(struct SeatNode));
    seatNode->class = ticket.class;
    seatNode->flightName = ticket.flightName;
    seatNode->next = NULL;
    return seatNode;
}

//Check Stack Empty
int checkEmpty(struct SeatNode *main)
{
    return !main;
}

//Push Ticket to the Stack
void push(struct SeatNode **main, struct ticket ticket)
{
    struct SeatNode *SeatNode = newSeat(ticket);
    SeatNode->next = *main;
    *main = SeatNode;
    // printf("pushed\n");
}

//Pop Ticket from Stack
struct ticket pop(struct SeatNode **main)
{
    if (checkEmpty(*main))
    {
        printf("Empty Stack!");
    }
    else
    {
        struct SeatNode *temp = *main;
        *main = (*main)->next;
        struct ticket poppedItem;
        // poppedItem.flightName = temp->flightName;
        strcpy(poppedItem.flightName, temp->flightName);
        poppedItem.class = temp->class;
        free(temp);

        return poppedItem;
    }
}

//Peek the Last Ticket
void peek(struct SeatNode *main)
{
    if (checkEmpty(main))
    {
        printf("Empty Stack!");
    }
    else
    {
        printf("Flight Name: %s, Ticket Class: %s\n", main->flightName, classNames[main->class]);
    }
}

// PRIORITY QUEUE FOR PASSENGERS
//passenger struct
struct passenger
{
    char passengerName[15];
    char flightName[8];
    enum classes wantedClass;
    enum classes givenClass;
};

struct queueNode
{
    struct passenger passenger;
    int priority;
    struct queueNode *next;
};

struct queueNode *newQueueNode(char flightName[8], int class, char passengerName[15], int priority)
{
    struct queueNode *temp = malloc(sizeof(struct queueNode));
    temp->priority = priority;
    temp->next = NULL;
    // temp->passenger.flightName = flightName;
    strcpy(temp->passenger.flightName, flightName);
    // temp->passenger.passengerName = passengerName;
    strcpy(temp->passenger.passengerName, passengerName);

    temp->passenger.wantedClass = class;

    return temp;
}

void pushQueue(struct queueNode **head, char *flightName, int class, char *passengerName, int priority)
{
    struct queueNode *start = (*head);

    struct queueNode *temp = newQueueNode(flightName, class, passengerName, priority);

    if ((*head)->priority > priority)
    {
        temp->next = *head;
        (*head) = temp;
    }
    else
    {
        if (start->next != NULL && start->next->priority == priority)
        {
            temp->next = start->next->next;
            start->next->next = temp;
        }
        else
        {
            while (start->next != NULL && start->next->priority < priority)
            {
                start = start->next;
            }

            temp->next = start->next;
            start->next = temp;
        }
    }
}

struct passenger peekQueue(struct queueNode **head)
{
    return (*head)->passenger;
}

void popQueue(struct queueNode **head)
{
    struct queueNode *temp = *head;
    (*head) = (*head)->next;
    free(temp);
}

int checkEmptyQueue(struct queueNode **head)
{
    if (head == NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//sell tickets
void sellTicket(struct flight *flightsArray, int index){
        printf("%s\n", flightsArray[index].flightName);
}

int main(int argc, char *argv[])
{

    FILE *fptr;
    char inputLine[MAXCHAR];

    int flightCount = 0;

    struct flight *flightsArray = NULL;

    //check parameter count
    if (argc != 3)
    {
        printf("Wrong parameters.");
        return -1;
    }

    //open input file
    fptr = fopen(argv[1], "r");

    //check whether input file is found
    if (fptr == NULL)
    {
        printf("No input file found.");
        return -1;
    }

    //Declare Stack Root Node
    struct SeatNode *main = NULL;

    //Read Line by Line
    while (fgets(inputLine, MAXCHAR, fptr) != NULL)
    {
        char *paramPtr = strtok(inputLine, " ");

        while (paramPtr != NULL)
        {
            // "addseat" command
            if (strcmp(paramPtr, "addseat") == 0)
            {

                struct ticket ticketTemp;

                //Flight Name
                paramPtr = strtok(NULL, " ");
                // ticketTemp.flightName = paramPtr;
                strcpy(ticketTemp.flightName, paramPtr);

                //Ticket Class
                paramPtr = strtok(NULL, " ");
                int class_tmp;
                if (strcmp(paramPtr, "business") == 0)
                {
                    ticketTemp.class = 0;
                    class_tmp = 0;
                }
                else if (strcmp(paramPtr, "economy") == 0)
                {
                    ticketTemp.class = 1;
                    class_tmp = 1;
                }
                else if (strcmp(paramPtr, "standard") == 0)
                {
                    ticketTemp.class = 2;
                    class_tmp = 2;
                }
                else
                {
                    printf("Wrong ticket class!");
                }

                //Ticket Quantity
                paramPtr = strtok(NULL, " ");
                int ticketCount = atoi(paramPtr);

                //Push to the Stack
                for (size_t i = 0; i < ticketCount; i++)
                {
                    push(&main, ticketTemp); //Add Ticket
                    // peek(main);

                    struct flight flight_temp;

                    flight_temp.businessQueueCount = 0;
                    flight_temp.economyQueueCount = 0;
                    flight_temp.standardQueueCount = 0;
                    flight_temp.totalPassengerCount = 0;

                    if (flightsArray == NULL)
                    { //no other flights
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
                    else
                    { //there are other flights
                        //check if the flight exists already
                        int foundFlag = 0;
                        for (size_t i = 0; i < flightCount; i++)
                        {
                            //flight found
                            if (strcmp(ticketTemp.flightName, (*(flightsArray + i)).flightName) == 0)
                            {
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
                        if (foundFlag == 0)
                        {
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
                    if (strcmp(ticketTemp.flightName, (*(flightsArray + i)).flightName) == 0)
                    {
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
                char flightName[8];
                paramPtr = strtok(NULL, " ");
                strcpy(flightName, paramPtr);
                // char *flightName = paramPtr;

                //Class
                int class;
                paramPtr = strtok(NULL, " ");
                if (strcmp(paramPtr, "business") == 0)
                {
                    class = 0;
                }
                else if (strcmp(paramPtr, "economy") == 0)
                {
                    class = 1;
                }
                else if (strcmp(paramPtr, "standard") == 0)
                {
                    class = 2;
                }
                else
                {
                    printf("Undetermined class!\n");
                }

                //Passenger Name
                char *passengerName;
                paramPtr = strtok(NULL, " ");
                // strcpy(passengerName, paramPtr);
                passengerName = paramPtr;

                //Priority
                paramPtr = strtok(NULL, " ");
                char *priority = paramPtr;

                int priorityForQueue;

                if (priority)
                {
                    if (strcmp(priority, "diplomat\r\n") == 0)
                    {
                        // printf("diplomat\n");
                        priorityForQueue = 0;
                    }
                    else if (strcmp(priority, "veteran\r\n") == 0)
                    {
                        // printf("veteran\n");
                        priorityForQueue = 2;
                    }
                }
                else
                {
                    if (class == 0)
                    { //just business
                        priorityForQueue = 1;
                    }
                    else if (class == 1)
                    { //just economy
                        priorityForQueue = 3;
                    }
                    else if (class == 2)
                    { //just standard
                        priorityForQueue = 4;
                    }
                }

                passengerName = strtok(passengerName, "\r");
                strtok(NULL, " "); //   for \n
                // char *passengerName = paramPtr;

                int flightIndex = -1;
                for (size_t i = 0; i < flightCount; i++)
                {
                    if ((strcmp(flightName, flightsArray[i].flightName)) == 0)
                    { //found
                        flightIndex = i;
                        break;
                    }
                }

                struct queueNode *queueRoot;

                if (flightIndex == -1)
                { // flight not found
                    printf("No such flight!\n");
                }
                else
                { //flight found
                    if (flightsArray[flightIndex].hasRoot != 1)
                    { //if queue has not initialized
                        flightsArray[flightIndex].rootNode = newQueueNode(flightName, class, passengerName, priorityForQueue);
                        flightsArray[flightIndex].hasRoot = 1;
                    }
                    else
                    { //if queue has been initialized
                        pushQueue(&(flightsArray[flightIndex].rootNode), flightName, class, passengerName, priorityForQueue);
                    }
                    //Proper Outputs and increase passenger counts
                    switch(class){
                        case 0:
                        flightsArray[flightIndex].businessQueueCount++;
                        flightsArray[flightIndex].totalPassengerCount++;
                        printf("queue %s %s %s %d\n", flightName, passengerName, classNames[class],
                               flightsArray[flightIndex].businessQueueCount);
                        break;

                        case 1:
                        flightsArray[flightIndex].economyQueueCount++;
                        flightsArray[flightIndex].totalPassengerCount++;
                        printf("queue %s %s %s %d\n", flightName, passengerName, classNames[class],
                               flightsArray[flightIndex].economyQueueCount);
                        break;

                        case 2:
                        flightsArray[flightIndex].standardQueueCount++;
                        flightsArray[flightIndex].totalPassengerCount++;
                        printf("queue %s %s %s %d\n", flightName, passengerName, classNames[class],
                               flightsArray[flightIndex].standardQueueCount);
                        break;
                    }
                }

                // printf("%s %s %d %s\n", queueRoot->passenger.flightName, queueRoot->passenger.passengerName,
                //                 queueRoot->priority, classNames[queueRoot->passenger.wantedClass]);
            }

            // "sell" command
            else if (strcmp(paramPtr, "sell") == 0)
            {

                //flight name
                char *flightName;
                paramPtr = strtok(NULL, " ");
                flightName = paramPtr;
                flightName = strtok(flightName, "\r");
                strtok(NULL, " ");

                //find flight
                int index = -1;
                for (size_t i = 0; i < flightCount; i++)
                {
                    // char flightName_temp[8];
                    // strcpy(flightName_temp, flightsArray[i].flightName);
                    // strcat(flightName_temp, "\r\n");

                    if (strcmp(flightsArray[i].flightName, flightName) == 0)
                    {
                        index = i;
                    }
                }

                if (index == -1)
                {
                    printf("No such flight.\n");
                }
                else
                {
                    sellTicket(flightsArray, index);    //bunu yaz
                    return 1;
                    while (flightsArray[index].hasRoot == 1)
                    {
                        struct passenger temp = peekQueue(&(flightsArray[index].rootNode));

                        switch (temp.wantedClass)
                        {
                        case 0: //business
                            if(flightsArray[index].businessCount != 0){
                                temp.givenClass = 0;

                                flightsArray[index].businessCount--;
                                flightsArray[index].businessQueueCount--;
                                flightsArray[index].totalPassengerCount--;

                                popQueue(&(flightsArray[index].rootNode));
                            }
                            else{
                                printf("No business class left. Trying economy.\n");

                            }
                            break;
                        case 1: //economy

                            break;
                        case 2: //standard

                            break;
                        default:
                            break;
                        }


                        flightsArray[index].totalPassengerCount--;
                        if (flightsArray[index].totalPassengerCount == 0)
                        {
                            flightsArray[index].hasRoot = 0;
                        }
                    }
                }
            }

            //"close" command
            else if (strcmp(paramPtr, "close") == 0){

                //flight name
                char *flightName;
                paramPtr = strtok(NULL, " ");
                flightName = paramPtr;
                flightName = strtok(flightName, "\r");
                strtok(NULL, " ");

            }

            //"report" command
            else if (strcmp(paramPtr, "report") == 0){

                //flight name
                char *flightName;
                paramPtr = strtok(NULL, " ");
                flightName = paramPtr;
                flightName = strtok(flightName, "\r");
                strtok(NULL, " ");

            }

            //"info" command
            else if (strcmp(paramPtr, "info") == 0){
                
                //passenger name
                char *passengerName;
                paramPtr = strtok(NULL, " ");
                passengerName = paramPtr;
                passengerName = strtok(passengerName, "\r");
                strtok(NULL, " ");

            }




            paramPtr = strtok(NULL, " ");
        }
    }
    printf("LAST FORM OF QUEUE:\n");
    for (size_t i = 0; i < flightCount; i++)
    {
        printf("Queue of %s: \n", flightsArray[i].flightName);
        while (flightsArray[i].hasRoot == 1)
        {
            struct passenger temp = peekQueue(&(flightsArray[i].rootNode));
            printf("%s %s %s\n", temp.passengerName, temp.flightName, classNames[temp.wantedClass]);
            popQueue(&(flightsArray[i].rootNode));
            flightsArray[i].totalPassengerCount--;
            if (flightsArray[i].totalPassengerCount == 0)
            {
                flightsArray[i].hasRoot = 0;
            }
        }
    }

    fclose(fptr);
}
