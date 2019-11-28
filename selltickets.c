#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXCHAR 500

enum classes {business, economy, standard};
const char* classNames[] = {"business", "economy", "standard"};

//ticket struct
struct ticket
{
    char flightName[7]; //check
    enum classes class;
};

//STACK FOR SEATS
struct SeatNode     //represents each seat
{
    char flightName[6];
    enum classes class;
    struct SeatNode* next;
};

// add new seat to stack
struct SeatNode* newSeat(struct ticket ticket)
{
    struct SeatNode* seatNode;
    seatNode = (struct SeatNode*)malloc(sizeof(struct SeatNode)); 
    seatNode->class = ticket.class;
    strcpy(seatNode->flightName, ticket.flightName);
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
        strcpy(poppedItem.flightName, temp->flightName);
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

// initialize list that holds flight names
char ** initFlightList(char **flights_array, char flightName[7], int *flightCount){
    flights_array = malloc(1 * sizeof(*flights_array));
    *(flights_array) = malloc(7 * sizeof(char));
    strcpy(*flights_array, flightName);
    (*flightCount)++;

    return flights_array;
}

// expand array and add new flight name
char ** addFlight(char **flights_array, char flightName[7], int *flightCount){
    char **flightNames_t = realloc(flights_array, (*flightCount+1) * sizeof(*flights_array));
    // free(flights_array);
    if(flightNames_t == NULL){
        printf("No memory. Can't Add New Flight.\n");
        return flights_array;
    }
    else{
        flights_array = flightNames_t;
        *(flights_array + *flightCount) = malloc(7 * sizeof(char));
        strcpy(*(flights_array + *flightCount), flightName);
        (*flightCount)++;
        // free(flightNames_t);

        return flights_array;
    }
}

// find the index of the given flight name
int findFlight(char **flights_array, char flightName[7], int count){
    for (size_t i = 0; i < count; i++)
    {
        if(strcmp(*(flights_array + i), flightName) == 0){
            return i;
        }
    }
    return -1;
}

//print any array
void printArray(char **array, int count){
    for (size_t i = 0; i < count; i++)
    {
        printf("%s\n", *(array + i));
    }
    
}

//init. the array that holds seat counts per flight
int ** initFlightSeats(int **seats_array, int count){

    seats_array = malloc(count * sizeof(*seats_array));
    *(seats_array + (count - 1)) = malloc(3 * sizeof(int));

    memset(*(seats_array + (count - 1)), 0, 3 * sizeof(int));    //memset kullanım izni check et

    return seats_array;
}

//add new seat counts for a new flight
int ** addSeats(int **seats_array, int flight_count){
    int **seats_t = realloc(seats_array, (flight_count) * sizeof(*seats_array));
        if(seats_t == NULL){
            printf("No memory. Can't Add New Flight Seats.\n");
            return seats_array;
        }
        else{
            seats_array = seats_t;
            *(seats_array + (flight_count - 1)) = malloc(3 * sizeof(int));

            return seats_array;
        }
}

// Priority Queue for Passengers

// Passenger
struct passenger{
    char *passengerName;
    enum classes desiredClass;
    enum classes actualClass;

    char flightName[7];

    int priority;   //Lower = higher priority
};

// Passenger Node
struct passengerNode{
    struct passenger passInfo;

    struct passengerNode* next;
};

// Create New passengerNode
struct passengerNode* newCustomer(enum classes desClass, int priority){

}




int main(int argc, char *argv[]){

    FILE *fptr;
    char inputLine[MAXCHAR];

    int flightCount = 0;

    char **flightNames = NULL;
    int **flightSeats = NULL;

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

    //Initialize Stack
    struct SeatNode* main = NULL;

    //Read Line by Line
    while (fgets(inputLine, MAXCHAR, fptr) != NULL){
        char *paramPtr = strtok(inputLine, " ");
        
        while(paramPtr != NULL){
            // "addseat" command
            if(strcmp(paramPtr, "addseat") == 0){

                struct ticket ticketTemp;

                //Ticket Name
                paramPtr = strtok(NULL, " ");
                strcpy(ticketTemp.flightName, paramPtr);
               
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
                    //peek(main);

                    //Add flight name and increase seat count
                    if (flightNames == NULL && flightSeats == NULL){    //no previous flights
                        flightNames = initFlightList(flightNames, ticketTemp.flightName, &flightCount);
                        flightSeats = initFlightSeats(flightSeats, flightCount); 

                        flightSeats[flightCount - 1][class_tmp]++;
                    }   
                    else if (flightNames != NULL){      //there are previous flights
                        //search for the flight
                        int index = findFlight(flightNames, ticketTemp.flightName, flightCount);

                        if (index != -1){   //flight found
                            flightSeats[index][class_tmp]++;
                        }
                        else{   //flight not found
                            flightNames = addFlight(flightNames, ticketTemp.flightName, &flightCount);

                            flightSeats = addSeats(flightSeats, flightCount);

                            //increase corresponding flight class
                            flightSeats[flightCount - 1][class_tmp]++;

                        }
                    }
                }
                //print proper output
                int index = findFlight(flightNames, ticketTemp.flightName, flightCount);
                printf("addseats %s %d %d %d\n", ticketTemp.flightName, 
                                                    flightSeats[index][0], 
                                                    flightSeats[index][1], 
                                                    flightSeats[index][2]);         

            }

            // "enqueue" command
            else if (strcmp(paramPtr, "enqueue") == 0)
            {

                struct passenger passenger_temp;
                //Flight Name
                paramPtr = strtok(NULL, " ");
                strcpy(passenger_temp.flightName, paramPtr);

                //Class
                paramPtr = strtok(NULL, " ");
                int class_tmp;
                if(strcmp(paramPtr, "business") == 0){
                    passenger_temp.desiredClass = 0;
                    class_tmp = 0;
                }
                else if(strcmp(paramPtr, "economy") == 0){
                    passenger_temp.desiredClass = 1;
                    class_tmp = 1;
                }
                else if(strcmp(paramPtr, "standard") == 0){
                    passenger_temp.desiredClass = 2;
                    class_tmp = 2;
                }
                else{
                    printf("Wrong ticket class!");
                }

                //Passenger Name
                paramPtr = strtok(NULL, " ");
                char *name = paramPtr;  //check
                passenger_temp.passengerName = name;

                
                //Priority
                paramPtr = strtok(NULL, " ");
                
                //pdf enum diyor check et
                //Determine priority of the passenger
                if(strcmp(paramPtr, "diplomat\r\n") == 0)   //'\r\n' is necessary to compare last token of the line
                {
                    passenger_temp.priority = 0;
                }
                else if (strcmp(classNames[class_tmp], "business") == 0)
                {
                    passenger_temp.priority = 1;                    
                }
                else if (strcmp(paramPtr, "veteran\r\n") == 0)
                {
                    passenger_temp.priority = 2;
                }
                else if (strcmp(classNames[class_tmp], "economy") == 0)
                {
                    passenger_temp.priority = 3;
                }
                else if (strcmp(classNames[class_tmp], "standard") == 0)
                {
                    passenger_temp.priority = 4;
                }
                else
                {
                    printf("Undetermined Priority!");
                }

                printf("%s %s %s %d\n", passenger_temp.passengerName, classNames[passenger_temp.desiredClass], passenger_temp.flightName, passenger_temp.priority);
                
            }
            // else
            // {
            //     printf("error\n");
            // }
            paramPtr = strtok(NULL, " ");
        }

        
                


    }

    //print flight names and seat counts,,,,,,,, can be deleted.
    // for (size_t i = 0; i < flightCount; i++)
    //     {
    //         printf("Flight Name: %s Seat Counts: %d %d %d\n", *(flightNames + i), flightSeats[i][0], flightSeats[i][1], flightSeats[i][2]);            
    //     }

    fclose(fptr);

}

