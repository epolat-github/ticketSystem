#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXCHAR 500

enum classes {business, economy, standard};
const char* classNames[] = {"business", "economy", "standard"};

//ticket struct
struct ticket
{
    char flightName[6];
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
    printf("pushed\n");
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





int main(int argc, char *argv[]){

    FILE *fptr;
    char inputLine[MAXCHAR];

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
            if(strcmp(paramPtr, "addseat") == 0){

                struct ticket ticketTemp;

                //Ticket Name
                paramPtr = strtok(NULL, " ");
                strcpy(ticketTemp.flightName, paramPtr);
               
                //Ticket Class
                paramPtr = strtok(NULL, " ");
                if(strcmp(paramPtr, "business") == 0){
                    ticketTemp.class = 0;
                }
                else if(strcmp(paramPtr, "economy") == 0){
                    ticketTemp.class = 1;
                }
                else if(strcmp(paramPtr, "standard") == 0){
                    ticketTemp.class = 2;
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
                    push(&main, ticketTemp);
                    peek(main);
                }
                
            }
            else if (strcmp(paramPtr, "enqueue") == 0)
            {
                printf("enqueue input entered\n");
                paramPtr = strtok(NULL, " ");
                printf("Flight name: %s\n", paramPtr);
                paramPtr = strtok(NULL, " ");
                printf("Passenger Class: %s\n", paramPtr);
                paramPtr = strtok(NULL, " ");
                printf("Passenger name: %s\n", paramPtr);
                paramPtr = strtok(NULL, " ");
                

            }
            // else
            // {
            //     printf("error\n");
            // }
            paramPtr = strtok(NULL, " ");
        }



    }

    fclose(fptr);

}

