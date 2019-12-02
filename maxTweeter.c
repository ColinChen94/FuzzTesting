//
//  main.c
//  P4Code
//
//  Created by ZhangVito on 11/30/19.
//  Copyright © 2019 Joe. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


struct User {
        char username[1000];
        int numTweets;
};


bool toNextColumn(FILE *fptr);
void insertNewUser(int *maxSize, int *userCount, char* name, struct User **users);
int findMaxTweetUser(int userCount, struct User *users);


int main(int argc, const char * argv[]) {
        
        FILE *fptr;
        char c;
        int userCount = 0;
        int maxSize = 100;
        bool isEOF = false;
        struct User *users;
        char input[1000];
        
        users = malloc(maxSize*sizeof(struct User));
        
        fptr = fopen(argv[1],"r");
        
        if (fptr == NULL) {
                printf("Invalid Input Format\n");
                exit(0);
        }
        
        
        //----------------------------------------------------------------------------
        //find the column # of the "name" header in the header line.
        char *line = (char*)malloc(1024*sizeof(char));
        size_t lineSize = 1024;
        
        getline(&line, &lineSize, fptr);
        
        char *token;
        token = strtok(line, ",");
        
        int tokenNum = 1;
        int namePosition = -1;
        
        while ( (token = strtok(0, ",")) ) {
                if (strcmp(token, "\"name\"") == 0
                        || strcmp(token, "name") == 0) {
                        namePosition = tokenNum;
                }
                tokenNum += 1;
        }
        if (namePosition == -1) {
                printf("Invalid Input Format\n");
                exit(0);
        }
        
        //----------------------------------------------------------------------------
        //go to the position of the first name.
        
        for (int i = 0; i < namePosition; ++i) {
                if (!isEOF) {
                        isEOF = !toNextColumn(fptr);
                }
        }
        
        //----------------------------------------------------------------------------
        //loop through the whole file to get names
        while (!isEOF) {
                
                //read the user name until next comma into input.
                
                c = fgetc(fptr);
                int i = 0;
                while (c != EOF) {
                        if (c == ',') {
                                fseek(fptr, -1, SEEK_CUR);
                                break;
                        } else if (c == EOF) {
                                isEOF = true;
                        }
                        input[i] = c;
                        i += 1;
                        c = fgetc(fptr);
                }
                input[i] = '\0';
                //----------------------------------------------

                //put the name in the vector of all tweeted users.
                //If that user has tweeted before, the number of times he tweeted will plus one.
                insertNewUser(&maxSize, &userCount, input, &users);
                
                //----------------------------------------------
                //go to the next line.
                for (int i = 0; i < tokenNum-1; ++i) {
                        if (!isEOF) {
                                isEOF = !toNextColumn(fptr);
                        }
                }
                
        }
        
        if (userCount == 0) {
                printf("Invalid Input Format\n");
                exit(0);
        }
        
        //printf("userCount is : %d\n", userCount);
        
        for (int i = 0; i < 10 && i < userCount; ++i) {
                int maxUser = findMaxTweetUser(userCount, users);
                printf("%s: %d\n", users[maxUser].username, users[maxUser].numTweets);
                users[maxUser].numTweets = -1;

        }
        
        
        //printf("%s\n", input);
        fclose(fptr);
        free(line);
        free(users);
        return 0;
}

bool toNextColumn(FILE *fptr) {
        char c;
        
        c = fgetc(fptr);
        
        while (c != EOF)
        {
                if (c == ',') {
                        break;
                }
                c = fgetc(fptr);
        }
        
        if(c == EOF){
                return false;
        } else {
                return true;
        }
        
}

void insertNewUser(int *maxSize, int *userCount, char* name, struct User **users){
        for (int i = 0; i < *userCount; ++i) {
                if (strcmp(name, (*users)[i].username) == 0) {
                        (*users)[i].numTweets += 1;
                        return;
                }
        }
        if (*maxSize == *userCount) {
                *maxSize = *maxSize * 2;
                *users = realloc(*users, *maxSize*sizeof(struct User));
        }
        strcpy((*users)[*userCount].username, name);
        (*users)[*userCount].numTweets = 1;
        *userCount += 1;
}

int findMaxTweetUser(int userCount, struct User *users){
        int maxUser = -1;
        int maxCount = -1;
        for (int i = 0; i < userCount; ++i) {
                if (users[i].numTweets > maxCount) {
                        maxUser = i;
                        maxCount = users[i].numTweets;
                }
        }
        return maxUser;
}


