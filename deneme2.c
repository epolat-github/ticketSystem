#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* replace_char(char* str, char find){
    // char *current_pos = strchr(str,find);
    // *current_pos = '\0';
    int i;
    for(i = 0; *(str + i) != '\0'; ++i){
        if(str[i] == find){
            str[i] = '\0';
        }
    }
    return str;
}

int main(){

    char line[] = "deneme ahmet\r\n";
    char line2[] = "deneme2 denemedeneme";
    
    char *txt1 = strtok(line, " ");
    printf("%s\n", txt1);
    txt1 = strtok(NULL, " ");
    char *name = strtok(txt1, "\r");
    printf("%s", name);
    strtok(NULL, " ");

    char *txt2 = strtok(line2, " ");
    printf("%s", txt2);
    char *name2 = strtok(NULL, " ");
    printf("%s", name2); 




    return 0;
}