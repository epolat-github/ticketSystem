#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    FILE *fileptr;

    char strippedName[15];
    char priority[10];
    char flight_name[] = "deneme psg_bus_1\r\n";
    char flight_name2[] = "deneme psg_bus_2 veteran";
    char *ptr = strtok(flight_name2, " ");
    printf("%s\n", ptr);
    ptr = strtok(NULL, " ");    //psg_bus_1\r\n
    strcpy(strippedName, ptr);
    char *ptr2 = strtok(strippedName, "\r\n");
    strcpy(strippedName, ptr2);
    ptr2 = strtok(NULL, " ");
    strcpy(priority, ptr);
    printf("%s", strippedName);
    if (priority){
        printf("%s", priority);
    }






    return 0;
}