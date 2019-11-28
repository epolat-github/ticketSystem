#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// char ** initFlightList(char **flights_array, char flightName[7], int *flightCount){
//     //init flight names array
//     flights_array = malloc(1 * sizeof(*flights_array));
//     *(flights_array) = malloc(7);
//     strcpy(*flights_array, flightName);
//     (*flightCount)++;

//     return flights_array;
// }

// char ** addFlight(char **flights_array, char flightName[7], int *flightCount){
//     char **flightNames_t = realloc(flights_array, (*flightCount+1) * sizeof(*flights_array));
//     if(flightNames_t == NULL){
//         printf("No memory. Can't Add New Flight.\n");
//         return flights_array;
//     }
//     else{
//         flights_array = flightNames_t;
//         *(flights_array + *flightCount) = malloc(7 * sizeof(char));
//         strcpy(*(flights_array + *flightCount), flightName);
//         (*flightCount)++;

//         return flights_array;
//     }
// }

// int findFlight(char **flights_array, char flightName[7], int count){
//     for (size_t i = 0; i < count; i++)
//     {
//         if(strcmp(*(flights_array + i), flightName) == 0){
//             return i;
//         }
//     }
//     return -1;
// }

// void printArray(char **array, int count){
//     for (size_t i = 0; i < count; i++)
//     {
//         printf("%s\n", *(array + i));
//     }
    
// }

// int ** initFlightSeats(int **seats_array, int count){

//     seats_array = malloc(count * sizeof(*seats_array));
//     *(seats_array + (count - 1)) = malloc(3 * sizeof(int));
// }

struct deneme{
    char name[7];
    int id;
};


int main(){

    char name_temp[7];
    
    struct deneme foo;
    struct deneme *array = NULL;
    for (size_t i = 0; i < 3; i++)
    {
        array = realloc(array, (i + 1) * sizeof(*array));
        printf("Name: ");
        scanf("%s", name_temp);
        foo.id = i;
        strcpy(foo.name, name_temp);
        *(array + i) = foo;
    }

    for (size_t i = 0; i < 3; i++)
    {
        printf("%d %s\n", array[i].id, array[i].name);
    }
    
    

    // int flightCount = 0;

    // char **flightNames = NULL;

    // int **flightSeats = NULL;
    
    // //f name tk1212
    // if (flightNames == NULL){
    //     flightNames = initFlightList(flightNames, "tk1212", &flightCount);

    //     flightSeats = initFlightSeats(flightSeats, flightCount);       

    // }   
    
    // //f name tk1213
    // flightNames = addFlight(flightNames, "tk1213", &flightCount);
    // flightNames = addFlight(flightNames, "tk1213", &flightCount);


    // //f name tk1214
    // flightNames = addFlight(flightNames, "tk1214", &flightCount);


    // int index = findFlight(flightNames, "tk1218", flightCount);
    // printf("%d\n", index);

    // //KOLTUK SAYILARINI TUTAN ARRAYİ DE EKLE
    // printArray(flightNames, flightCount);

    // // printArray(flightNames, flightCount);    
    

    
    
    return 0;

    // flightCount++;

    // //flight name = tk1234
    // char **flightNames_t = realloc(flightNames, (flightCount+1)*sizeof(*flightNames));
    // if(flightNames_t == NULL){
    //     printf("No memory. Can't expand.\n");
    // }
    // else{
    //     free(*(flightNames));
    //     flightNames = flightNames_t;
    // }



    // *(flightNames + 1) = malloc(7);
    // strcpy(*(flightNames + 1), "tk1234");
    // flightCount++;

    // for (size_t i = 0; i < flightCount; i++)
    // {
    //     printf("%s\n", *(flightNames + i));
    // }
    
    // return 0;
}