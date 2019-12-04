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

    //sold ticket counts
    int soldBusiness, soldEconomy, soldStandard;

    //sold flag
    int soldFlag;

    int closed;
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
    free(SeatNode);
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
    strcpy(temp->passenger.flightName, flightName);
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
        while (start->next != NULL && start->next->priority <= priority)
        {
            start = start->next;
        }

        temp->next = start->next;
        start->next = temp;
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
void sellTicket(struct flight *flightsArray, int index)
{
    printf("%s\n", flightsArray[index].flightName);
}

int main(int argc, char *argv[])
{

    FILE *inputFile;
    FILE *outputFile;

    char inputLine[MAXCHAR];

    int flightCount = 0;

    struct flight *flightsArray = NULL;

    //holds passengers who could buy ticket
    int totalSoldCount = 0;
    struct passenger *soldPassengers = NULL;

    //holds passengers who couldn't buy ticket
    int totalUnsoldCount = 0;
    struct passenger *totalUnsolds = NULL;
    int totalUnsoldBusinessCount = 0;
    struct passenger *unsoldBusiness = NULL;
    int totalUnsoldEconomyCount = 0;
    struct passenger *unsoldEconomy = NULL;
    int totalUnsoldStandardCount = 0;
    struct passenger *unsoldStandard = NULL;

    //check parameter count
    if (argc != 3)
    {
        printf("Wrong parameters.");
        return -1;
    }

    //open input file
    inputFile = fopen(argv[1], "r");

    //open output file
    outputFile = fopen(argv[2], "w");

    //check whether input file is found
    if (inputFile == NULL)
    {
        printf("No input file found.");
        return -1;
    }

    //Declare Stack Root Node
    struct SeatNode *main = NULL;

    //Read Line by Line
    while (fgets(inputLine, MAXCHAR, inputFile) != NULL)
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
                    fprintf(outputFile, "error\n");
                }

                //Ticket Quantity
                paramPtr = strtok(NULL, " ");
                int ticketCount = atoi(paramPtr);

                //Push to the Stack
                for (size_t i = 0; i < ticketCount; i++)
                {
                    push(&main, ticketTemp); //Add Ticket

                    struct flight flight_temp;

                    // not to return garbage value in the future
                    flight_temp.businessQueueCount = 0;
                    flight_temp.economyQueueCount = 0;
                    flight_temp.standardQueueCount = 0;
                    flight_temp.totalPassengerCount = 0;

                    flight_temp.soldBusiness = 0;
                    flight_temp.soldEconomy = 0;
                    flight_temp.soldStandard = 0;
                    flight_temp.soldFlag = 0;

                    flight_temp.closed = 0;

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
                            printf("wrong class.\n");
                            fprintf(outputFile, "error\n");
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
                                if (flightsArray[i].closed == 1)
                                {
                                    fprintf(outputFile, "error\n");
                                    foundFlag = 1;
                                }
                                else
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
                                        printf("wrong class.\n");
                                        fprintf(outputFile, "error\n");
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                        //if not found
                        if (foundFlag == 0)
                        {
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
                                printf("wrong class.\n");
                                fprintf(outputFile, "error\n");
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
                fprintf(outputFile, "addseats %s %d %d %d\n", ticketTemp.flightName,
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
                    fprintf(outputFile, "error\n");
                }

                //Passenger Name
                char *passengerName;
                paramPtr = strtok(NULL, " ");
                passengerName = paramPtr;

                //Priority
                paramPtr = strtok(NULL, " ");
                char *priority = paramPtr;

                int priorityForQueue; //-1 represents error

                if (priority)
                {
                    if (strcmp(priority, "diplomat\r\n") == 0)
                    {
                        if (class != 0)
                        {
                            priorityForQueue = -1;
                        }
                        else
                        {
                            priorityForQueue = 0;
                        }
                    }
                    else if (strcmp(priority, "veteran\r\n") == 0)
                    {
                        if (class != 1)
                        {
                            priorityForQueue = -1;
                        }
                        else
                        {
                            priorityForQueue = 2;
                        }
                    }
                    else
                    {
                        priorityForQueue = -1;
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

                if (priorityForQueue == -1)
                {
                    fprintf(outputFile, "error\n");
                }
                else if (flightIndex == -1)
                { // flight not found
                    fprintf(outputFile, "error\n");
                }
                else if (flightsArray[flightIndex].closed == 1)
                {
                    fprintf(outputFile, "error\n");
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
                    switch (class)
                    {
                    case 0:
                        flightsArray[flightIndex].businessQueueCount++;
                        flightsArray[flightIndex].totalPassengerCount++;
                        fprintf(outputFile, "queue %s %s %s %d\n", flightName, passengerName, classNames[class],
                                flightsArray[flightIndex].businessQueueCount);
                        break;

                    case 1:
                        flightsArray[flightIndex].economyQueueCount++;
                        flightsArray[flightIndex].totalPassengerCount++;
                        fprintf(outputFile, "queue %s %s %s %d\n", flightName, passengerName, classNames[class],
                                flightsArray[flightIndex].economyQueueCount);
                        break;

                    case 2:
                        flightsArray[flightIndex].standardQueueCount++;
                        flightsArray[flightIndex].totalPassengerCount++;
                        fprintf(outputFile, "queue %s %s %s %d\n", flightName, passengerName, classNames[class],
                                flightsArray[flightIndex].standardQueueCount);
                        break;
                    }
                }
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

                int totalSeatCount;

                //find flight
                int index = -1;
                for (size_t i = 0; i < flightCount; i++)
                {
                    if (strcmp(flightsArray[i].flightName, flightName) == 0)
                    {
                        index = i;
                    }
                }

                if (index == -1)
                {
                    fprintf(outputFile, "error\n");
                }
                else if (flightsArray[index].closed == 1)
                {
                    fprintf(outputFile, "error\n");
                }
                else
                {

                    flightsArray[index].soldFlag = 1;
                    totalSeatCount = (flightsArray[index].businessCount) + (flightsArray[index].economyCount) + (flightsArray[index].standardCount);

                    while (flightsArray[index].hasRoot == 1)
                    {
                        struct passenger temp = peekQueue(&(flightsArray[index].rootNode));

                        switch (temp.wantedClass)
                        {
                        case 0: //business
                            if (flightsArray[index].businessCount != 0)
                            {
                                temp.givenClass = 0;

                                flightsArray[index].businessCount--;
                                flightsArray[index].businessQueueCount--;
                                totalSeatCount--;

                                flightsArray[index].soldBusiness++;

                                popQueue(&(flightsArray[index].rootNode));
                                soldPassengers = realloc(soldPassengers, (totalSoldCount + 1) * sizeof(struct passenger));
                                soldPassengers[totalSoldCount] = temp;
                                totalSoldCount++;
                            }
                            else
                            {
                                popQueue(&(flightsArray[index].rootNode));
                                totalUnsolds = realloc(totalUnsolds, (totalUnsoldCount + 1) * sizeof(struct passenger));
                                totalUnsolds[totalUnsoldCount] = temp;
                                totalUnsoldCount++;
                            }
                            flightsArray[index].totalPassengerCount--;

                            if (flightsArray[index].totalPassengerCount == 0)
                            {
                                flightsArray[index].hasRoot = 0;
                            }
                            break;

                        case 1: //economy
                            if (flightsArray[index].economyCount != 0)
                            {
                                temp.givenClass = 1;

                                flightsArray[index].economyCount--;
                                flightsArray[index].economyQueueCount--;
                                totalSeatCount--;
                                flightsArray[index].soldEconomy++;

                                popQueue(&(flightsArray[index].rootNode));
                                soldPassengers = realloc(soldPassengers, (totalSoldCount + 1) * sizeof(struct passenger));
                                soldPassengers[totalSoldCount] = temp;
                                totalSoldCount++;
                            }
                            else
                            {
                                popQueue(&(flightsArray[index].rootNode));
                                totalUnsolds = realloc(totalUnsolds, (totalUnsoldCount + 1) * sizeof(struct passenger));
                                totalUnsolds[totalUnsoldCount] = temp;
                                totalUnsoldCount++;
                            }
                            flightsArray[index].totalPassengerCount--;
                            if (flightsArray[index].totalPassengerCount == 0)
                            {
                                flightsArray[index].hasRoot = 0;
                            }

                            break;
                        case 2: //standard
                            if (flightsArray[index].standardCount != 0)
                            {
                                temp.givenClass = 2;

                                flightsArray[index].standardCount--;
                                flightsArray[index].standardQueueCount--;

                                totalSeatCount--;
                                flightsArray[index].soldStandard++;

                                popQueue(&(flightsArray[index].rootNode));
                                soldPassengers = realloc(soldPassengers, (totalSoldCount + 1) * sizeof(struct passenger));
                                soldPassengers[totalSoldCount] = temp;
                                totalSoldCount++;
                            }
                            else
                            {
                                popQueue(&(flightsArray[index].rootNode));
                                totalUnsolds = realloc(totalUnsolds, (totalUnsoldCount + 1) * sizeof(struct passenger));
                                totalUnsolds[totalUnsoldCount] = temp;
                                totalUnsoldCount++;
                            }
                            flightsArray[index].totalPassengerCount--;
                            if (flightsArray[index].totalPassengerCount == 0)
                            {
                                flightsArray[index].hasRoot = 0;
                            }

                            break;
                        default:
                            printf("Wrong class!\n");
                            fprintf(outputFile, "error\n");
                            break;
                        }
                    }
                    //sell left standard tickets to business
                    for (size_t i = 0; i < totalUnsoldCount; i++)
                    {
                        if (strcmp(totalUnsolds[i].flightName, flightsArray[index].flightName) == 0)
                        {
                            if (flightsArray[index].standardCount != 0)
                            {
                                totalUnsolds[i].givenClass = 2;

                                soldPassengers = realloc(soldPassengers, (totalSoldCount + 1) * sizeof(struct passenger));
                                soldPassengers[totalSoldCount] = totalUnsolds[i];
                                totalSoldCount++;

                                flightsArray[index].soldStandard++;
                                flightsArray[index].standardCount--;
                                flightsArray[index].totalPassengerCount--;
                                totalSeatCount--;

                                switch (totalUnsolds[i].wantedClass)
                                {
                                case 0:
                                    flightsArray[index].businessQueueCount--;
                                    break;
                                case 1:
                                    flightsArray[index].economyQueueCount--;
                                    break;
                                }
                            }
                            else
                            {
                                switch (totalUnsolds[i].wantedClass)
                                {
                                case 0:
                                    unsoldBusiness = realloc(unsoldBusiness, (totalUnsoldBusinessCount + 1) * sizeof(struct passenger));
                                    unsoldBusiness[totalUnsoldBusinessCount] = totalUnsolds[i];
                                    totalUnsoldBusinessCount++;
                                    break;
                                case 1:
                                    unsoldEconomy = realloc(unsoldEconomy, (totalUnsoldEconomyCount + 1) * sizeof(struct passenger));
                                    unsoldEconomy[totalUnsoldEconomyCount] = totalUnsolds[i];
                                    totalUnsoldEconomyCount++;
                                    break;
                                }
                            }
                        }
                    }

                    //Proper Output
                    fprintf(outputFile, "sold %s %d %d %d\n", flightsArray[index].flightName, flightsArray[index].soldBusiness, flightsArray[index].soldEconomy, flightsArray[index].soldStandard);
                }
            }

            //"close" command
            else if (strcmp(paramPtr, "close") == 0)
            {

                //flight name
                char *flightName;
                paramPtr = strtok(NULL, " ");
                flightName = paramPtr;
                flightName = strtok(flightName, "\r");
                strtok(NULL, " ");

                int index = -1;
                for (size_t i = 0; i < flightCount; i++)
                {
                    if (strcmp(flightsArray[i].flightName, flightName) == 0)
                    {
                        index = i;
                    }
                }

                if (index == -1)
                {
                    printf("Flight not found!\n");
                    fprintf(outputFile, "error\n");
                }
                else
                {
                    //close the flight
                    flightsArray[index].closed = 1;

                    int totalSoldCount_t = flightsArray[index].soldBusiness + flightsArray[index].soldEconomy + flightsArray[index].soldStandard;
                    int waitingCount = flightsArray[index].businessQueueCount + flightsArray[index].economyQueueCount + flightsArray[index].standardQueueCount;

                    fprintf(outputFile, "closed %s %d %d\n", flightsArray[index].flightName, totalSoldCount_t, waitingCount);

                    if (flightsArray[index].soldFlag != 1)
                    {
                        while (flightsArray[index].hasRoot == 1)
                        {
                            struct passenger temp = peekQueue(&(flightsArray[index].rootNode));
                            popQueue(&(flightsArray[index].rootNode));

                            switch (temp.wantedClass)
                            {
                            case 0:
                                unsoldBusiness = realloc(unsoldBusiness, (totalUnsoldBusinessCount + 1) * sizeof(struct passenger));
                                unsoldBusiness[totalUnsoldBusinessCount] = temp;
                                totalUnsoldBusinessCount++;
                                break;
                            case 1:
                                unsoldEconomy = realloc(unsoldEconomy, (totalUnsoldEconomyCount + 1) * sizeof(struct passenger));
                                unsoldEconomy[totalUnsoldEconomyCount] = temp;
                                totalUnsoldEconomyCount++;
                                break;
                            case 2:
                                unsoldStandard = realloc(unsoldStandard, (totalUnsoldStandardCount + 1) * sizeof(struct passenger));
                                unsoldStandard[totalUnsoldStandardCount] = temp;
                                totalUnsoldStandardCount++;
                                break;
                            }

                            fprintf(outputFile, "waiting %s\n", temp.passengerName);

                            flightsArray[index].totalPassengerCount--;

                            if (flightsArray[index].totalPassengerCount == 0)
                            {
                                flightsArray[index].hasRoot = 0;
                            }
                        }
                    }
                    //if there are passengers left unsold
                    else
                    {
                        //for unsold business tickets
                        for (size_t i = 0; i < totalUnsoldBusinessCount; i++)
                        {
                            if (strcmp(unsoldBusiness[i].flightName, flightName) == 0)
                            {
                                fprintf(outputFile, "waiting %s\n", unsoldBusiness[i].passengerName);
                            }
                        }

                        //for unsold economy tickets
                        for (size_t i = 0; i < totalUnsoldEconomyCount; i++)
                        {
                            if (strcmp(unsoldEconomy[i].flightName, flightName) == 0)
                            {
                                fprintf(outputFile, "waiting %s\n", unsoldEconomy[i].passengerName);
                            }
                        }

                        //for unsold standard tickets
                        for (size_t i = 0; i < totalUnsoldStandardCount; i++)
                        {
                            if (strcmp(unsoldStandard[i].flightName, flightName) == 0)
                            {
                                fprintf(outputFile, "waiting %s\n", unsoldStandard[i].passengerName);
                            }
                        }
                    }
                }
            }

            //"report" command
            else if (strcmp(paramPtr, "report") == 0)
            {

                //flight name
                char *flightName;
                paramPtr = strtok(NULL, " ");
                flightName = paramPtr;
                flightName = strtok(flightName, "\r");
                strtok(NULL, " ");

                //find flight index
                int index = -1;
                for (size_t i = 0; i < flightCount; i++)
                {
                    if (strcmp(flightsArray[i].flightName, flightName) == 0)
                    {
                        index = i;
                    }
                }

                if (index == -1)
                {
                    fprintf(outputFile, "error\n");
                }
                else
                {
                    fprintf(outputFile, "report %s\n", flightName);
                    fprintf(outputFile, "business %d\n", flightsArray[index].soldBusiness);
                    for (size_t i = 0; i < totalSoldCount; i++)
                    {
                        if (strcmp(flightName, soldPassengers[i].flightName) == 0 && soldPassengers[i].givenClass == 0)
                        {
                            fprintf(outputFile, "%s\n", soldPassengers[i].passengerName);
                        }
                    }
                    fprintf(outputFile, "economy %d\n", flightsArray[index].soldEconomy);
                    for (size_t i = 0; i < totalSoldCount; i++)
                    {
                        if (strcmp(flightName, soldPassengers[i].flightName) == 0 && soldPassengers[i].givenClass == 1)
                        {
                            fprintf(outputFile, "%s\n", soldPassengers[i].passengerName);
                        }
                    }
                    fprintf(outputFile, "standard %d\n", flightsArray[index].soldStandard);
                    for (size_t i = 0; i < totalSoldCount; i++)
                    {
                        if (strcmp(flightName, soldPassengers[i].flightName) == 0 && soldPassengers[i].givenClass == 2)
                        {
                            fprintf(outputFile, "%s\n", soldPassengers[i].passengerName);
                        }
                    }
                    fprintf(outputFile, "end of report %s\n", flightName);
                }
            }

            //"info" command
            else if (strcmp(paramPtr, "info") == 0)
            {

                //passenger name
                char *passengerName;
                paramPtr = strtok(NULL, " ");
                passengerName = paramPtr;
                passengerName = strtok(passengerName, "\r");
                strtok(NULL, " ");

                //First, traversing the sold list
                int foundFlag = 0;
                for (size_t i = 0; i < totalSoldCount; i++)
                {
                    if (strcmp(passengerName, soldPassengers[i].passengerName) == 0)
                    {
                        foundFlag = 1;
                        fprintf(outputFile, "info %s %s %s %s\n", soldPassengers[i].passengerName, soldPassengers[i].flightName,
                                classNames[soldPassengers[i].wantedClass], classNames[soldPassengers[i].givenClass]);
                    }
                }

                //Traversing unsold lists
                for (size_t i = 0; i < totalUnsoldBusinessCount; i++)
                {
                    if (strcmp(passengerName, unsoldBusiness[i].passengerName) == 0)
                    {
                        foundFlag = 1;
                        fprintf(outputFile, "info %s %s %s none\n", unsoldBusiness[i].passengerName, unsoldBusiness[i].flightName,
                                classNames[unsoldBusiness[i].wantedClass]);
                    }
                }

                for (size_t i = 0; i < totalUnsoldEconomyCount; i++)
                {
                    if (strcmp(passengerName, unsoldEconomy[i].passengerName) == 0)
                    {
                        foundFlag = 1;
                        fprintf(outputFile, "info %s %s %s none\n", unsoldEconomy[i].passengerName, unsoldEconomy[i].flightName,
                                classNames[unsoldEconomy[i].wantedClass]);
                    }
                }

                for (size_t i = 0; i < totalUnsoldStandardCount; i++)
                {
                    if (strcmp(passengerName, unsoldStandard[i].passengerName) == 0)
                    {
                        foundFlag = 1;
                        fprintf(outputFile, "info %s %s %s none\n", unsoldStandard[i].passengerName, unsoldStandard[i].flightName,
                                classNames[unsoldStandard[i].wantedClass]);
                    }
                }

                //if passenger not found
                if (foundFlag == 0)
                {
                    fprintf(outputFile, "error\n");
                }
            }

            paramPtr = strtok(NULL, " ");
        }
    }
    free(flightsArray);
    free(unsoldBusiness);
    free(unsoldEconomy);
    free(unsoldStandard);
    free(soldPassengers);
    free(totalUnsolds);

    fclose(inputFile);
    fclose(outputFile);
}
