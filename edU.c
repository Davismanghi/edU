#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct commandElement{
    bool commandType;
    int startingLine;
    int finishingLine;
    struct commandElement *previousCommand;
    struct commandElement *nextCommand;
    char **firstOldString;
    int dimOldString;
}commandElement;

char ** ListToPrint;
int dimListToPrint = 0, maxCommand = 0, numCommands = 0, actualCommand = 0;
bool ifRedoCalling = false;

commandElement *change(commandElement** currComm, int initNumber, int finishNumber){
    struct commandElement *newCommand, *scrollCommand;
    maxCommand = numCommands;
    actualCommand = maxCommand;
    numCommands++;
    maxCommand++;
    actualCommand++;

    char currInput[500];


    newCommand = malloc(sizeof(struct commandElement));
    newCommand->commandType = 1;
    newCommand->previousCommand = *currComm;
    if(*currComm != NULL) {
        (*currComm)->nextCommand = newCommand;
    }
    newCommand->startingLine = initNumber;
    newCommand->finishingLine = finishNumber;
    newCommand->nextCommand = NULL;
    newCommand->firstOldString = NULL;
    if(dimListToPrint == 0 || dimListToPrint < initNumber){                    
        newCommand->dimOldString = 0;
    }
    else if(dimListToPrint < finishNumber) {
        newCommand->dimOldString = dimListToPrint - initNumber + 1; 
    }
    else if(dimListToPrint >= finishNumber){
        newCommand->dimOldString = finishNumber - initNumber + 1;
    }
    newCommand->firstOldString = malloc(sizeof (char*) * newCommand->dimOldString);

    if(dimListToPrint < finishNumber)
    {
        ListToPrint = (char**)realloc(ListToPrint, sizeof(char*) * finishNumber);
        dimListToPrint =finishNumber;
    }
    int i = 0;
    
    // Read the lines and write them in the list
    for(int lineNum = initNumber; lineNum <= finishNumber; lineNum++){
        fgets(currInput,499,stdin);
        if(i < newCommand->dimOldString) {
            newCommand->firstOldString[i] = ListToPrint[lineNum - 1];
        }
        ListToPrint[lineNum - 1] = strdup(currInput);
        i++;
    }
    //last line, a point.
    fgets(currInput,2,stdin);
    return newCommand;
}

commandElement *delete(struct commandElement** currComm, int initNumber, int finishNumber){
    struct commandElement *newCommand;
    if(ifRedoCalling == false) {
        maxCommand = numCommands;
        actualCommand = maxCommand;
        maxCommand++;
        actualCommand++;
        numCommands++;
    }
       

    newCommand = malloc(sizeof(struct commandElement));
    newCommand->commandType = 0;
    newCommand->previousCommand = *currComm;
    if(*currComm != NULL) {
        (*currComm)->nextCommand = newCommand;
    }
    newCommand->startingLine = initNumber;
    newCommand->finishingLine = finishNumber;
    newCommand->nextCommand = NULL;
    newCommand->firstOldString = NULL;

    if(initNumber == 0 && finishNumber == 0){
        newCommand->dimOldString = 0;    
        return newCommand;
    }

    if(initNumber == 0 && finishNumber != 0){
        newCommand->startingLine++;
        initNumber = 1;
    }

    if(initNumber > dimListToPrint){
        newCommand->dimOldString = 0;   
        return newCommand;
    }

    if(dimListToPrint == 0 || dimListToPrint < initNumber){                    
        newCommand->dimOldString = 0;
    }                                           
    else if(dimListToPrint < finishNumber) {
        newCommand->dimOldString = dimListToPrint - initNumber + 1;
    }
    else if(dimListToPrint >= finishNumber){
        newCommand->dimOldString = finishNumber - initNumber + 1;
    }
    newCommand->firstOldString = malloc(sizeof (char*) * (newCommand->dimOldString) );

    int i = 0;
    // Read the lines and write them in the list
    for(int lineNum = initNumber; lineNum <= finishNumber; lineNum++){
        if(lineNum > dimListToPrint) break;
        if(i < newCommand->dimOldString) {
            newCommand->firstOldString[i] = ListToPrint[lineNum - 1];
        }
        ListToPrint[lineNum - 1] = NULL;
        i++;
    }

    int toMove = initNumber;

    for(int lineNum = finishNumber; lineNum < dimListToPrint; lineNum++){
        ListToPrint[toMove - 1] = ListToPrint[lineNum];
        toMove++;
    }

    if(dimListToPrint < finishNumber){
        dimListToPrint = initNumber - 1;
    }
    else{
        dimListToPrint = dimListToPrint - (finishNumber - initNumber + 1);
    }

    ListToPrint = (char **)realloc(ListToPrint,sizeof(char *)*(toMove - 1));

    return newCommand;
}

void print( int initNumber, int finishNumber)
{

    if(initNumber == 0 && finishNumber == 0){
        puts(".");
        return;
    }
    else
    {
        if (initNumber == 0)
        {
            puts(".");
            initNumber++;
        }
        if(initNumber > dimListToPrint){
            for(int i = initNumber - 1; i < finishNumber; i++){
                puts(".");
            }
        }
        else if (finishNumber > dimListToPrint)
        {
            for (int i = initNumber - 1; i < dimListToPrint; ++i)
            {
                if(*ListToPrint != NULL) {
                    fputs(ListToPrint[i], stdout);
                }
            }
            for(int i = dimListToPrint; i < finishNumber; i++)
            {
                puts(".");
            }
        }
        else
        {
            for (int i = initNumber - 1; i < finishNumber; ++i)
            {
                if(*ListToPrint != NULL) {
                    fputs(ListToPrint[i], stdout);
                }
            }
        }
    }
}

commandElement *undo(struct commandElement** currentCommand, int numberUndo){
    struct commandElement *currComm = *currentCommand;
    int oldDimListToPrint = dimListToPrint;
    if(currComm->previousCommand == NULL){
        return(currComm);
    }
    for(int currUndo = numberUndo; currComm->previousCommand != NULL && currUndo > 0; currUndo--, currComm = currComm->previousCommand){
        oldDimListToPrint = dimListToPrint;
        
        //caso DELETE
        if(currComm->commandType == 0){
            if(currComm->startingLine == 0 && currComm->finishingLine == 0){
                continue;
            }
            if(currComm->startingLine - 1 > dimListToPrint){
                continue;
            }

            dimListToPrint = dimListToPrint + currComm->dimOldString;

            if(oldDimListToPrint != 0 && dimListToPrint == 0) ListToPrint = (char**)realloc(ListToPrint,sizeof(char*)*dimListToPrint);
            else if (dimListToPrint != 0) ListToPrint = (char**)realloc(ListToPrint,sizeof(char*)*dimListToPrint);


            for(int lineNum = dimListToPrint; lineNum > currComm->startingLine; lineNum--){
                if(lineNum - 1 - currComm->dimOldString >= 0) {     
                    ListToPrint[lineNum - 1] = ListToPrint[lineNum - 1 - currComm->dimOldString];
                }
            }

            int j = 0;

            if(dimListToPrint != 0) {
                for (int i = currComm->startingLine; j < currComm->dimOldString; i++) {     
                    ListToPrint[i - 1] = currComm->firstOldString[j];
                    j++;
                }
            }
        }
        
        //case CHANGE
        else{
            //caso tutto FUORI lista
            if(currComm->dimOldString == 0){
                dimListToPrint = currComm->startingLine - 1;
                currComm->firstOldString = (char**)malloc(sizeof(char *)*(currComm->finishingLine - currComm->startingLine + 1));
                int i = 0;
                // Read the lines and write them in the list
                for(int lineNum = currComm->startingLine; lineNum <= currComm->finishingLine; lineNum++){                    
                    currComm-> firstOldString[i] = ListToPrint[lineNum - 1];
                    ListToPrint[lineNum - 1] = NULL;
                    i++;
                }
                ListToPrint = (char **)realloc(ListToPrint,sizeof(char*)*dimListToPrint);
            }
            //caso metà fuori e metà dentro lista
            else if(currComm->dimOldString < (currComm->finishingLine - currComm->startingLine + 1)){
                int currInList = currComm->dimOldString, currLine = currComm->startingLine - 1, i = 0;
                char *toCopy;
                //copio elementi GIA' nella lista
                for(;currInList > 0; currInList--){
                    toCopy = /*strdup*/(ListToPrint[currLine]);
                    ListToPrint[currLine] = currComm->firstOldString[i];
                    currComm->firstOldString[i] = toCopy;
                    currLine++;
                    i++;
                }

                dimListToPrint = currLine;

                //copio in lista vecchia
                currComm->firstOldString = (char**)realloc(currComm->firstOldString,sizeof(char*)*(currComm->finishingLine - currComm->startingLine + 1));
                for(; currLine < currComm->finishingLine; i++){     
                    currComm->firstOldString[i] = ListToPrint[currLine];
                    currLine++;
                }

               

                ListToPrint = (char  **)realloc(ListToPrint,sizeof(char *) * dimListToPrint);
            }
            //caso tutto DENTRO lista
            else if(currComm->dimOldString == (currComm->finishingLine - currComm->startingLine + 1)){
                char *toCopy;
                int i = 0;
                for(int lineNum = currComm->startingLine; lineNum <= currComm->finishingLine; lineNum++){                 
                    toCopy = /*strdup*/(currComm->firstOldString[i]);
                    currComm-> firstOldString[i] = ListToPrint[lineNum - 1];
                    ListToPrint[lineNum - 1] = toCopy;
                    i++;
                }
            }
        }

        //finisco tutti i comandi
        if(currComm->previousCommand->commandType == 1 && currComm->previousCommand->startingLine == 0){
            dimListToPrint = 0;     ////
            currComm = currComm->previousCommand;
            break;
        }
    }
    return currComm;
}

commandElement *redo(struct commandElement** currentCommand, int numberRedo){
    struct commandElement *currComm = *currentCommand;

    if(currComm->nextCommand == NULL){
        return currComm;
    }

    for(int currRedo = numberRedo; currRedo > 0 ; currRedo--){
        currComm = currComm->nextCommand;
        //caso DELETE
        if (currComm->commandType == 0) {
            struct commandElement *nextCommand = currComm->nextCommand;
            currComm = delete(&currComm->previousCommand,currComm->startingLine,currComm->finishingLine);   ///
            currComm->nextCommand = nextCommand;
            if(currComm->nextCommand != NULL) {
                currComm->nextCommand->previousCommand = currComm;
            }
        }
        //case CHANGE
        else {
            if(dimListToPrint < currComm->finishingLine)
            {
                ListToPrint = (char**)realloc(ListToPrint, sizeof(char*) * currComm->finishingLine);
                dimListToPrint = currComm->finishingLine;
            }
            int i = 0;
            char *toCopy;
            // Read the lines and write them in the list
            for(int lineNum = currComm->startingLine; lineNum <= currComm->finishingLine; lineNum++){
                //if(dimListToPrint - currComm->finishingLine <= 0) {
                    toCopy = /*strdup*/ListToPrint[/*currComm->startingLine*/lineNum - 1];
                //}
                ListToPrint[lineNum - 1] = (currComm->firstOldString[i]);
                currComm->firstOldString[i] = toCopy;
                i++;
            }
        }
        if(currComm->nextCommand == NULL){
            break;
        }
    }
    return currComm;
}

int breakString(char *Command){
    for(int i = 0; i < strlen(Command); i++){
        if(Command[i] == ','){
            return i;
        }
    }
    return -1;
}

int main() {
    commandElement *currentCommand = malloc(sizeof(struct commandElement));
    currentCommand->commandType = 1;
    currentCommand->startingLine = 0;
    currentCommand->finishingLine = 0;
    currentCommand->previousCommand = NULL;
    currentCommand->nextCommand = NULL;
    //numCommands++;

    char Command[500] = "\0";
    int initNumber, finishNumber,numberUndo,numberRedo = 0;

    ListToPrint = (char**)malloc (sizeof(char*));

    while(1) {
        fgets(Command, 499, stdin);        
        if(strlen(Command) > 2) {
            if (Command[strlen(Command) - 2] == 'c') {
                initNumber = atoi(Command);
                finishNumber = atoi(Command + breakString(Command) + 1);
                currentCommand = change(&currentCommand, initNumber, finishNumber);
            } else if (Command[strlen(Command) - 2] == 'd') {
                initNumber = atoi(Command);
                finishNumber = atoi(Command + breakString(Command) + 1);
                currentCommand = delete(&currentCommand, initNumber, finishNumber);
            } else if (Command[strlen(Command) - 2] == 'p') {
                initNumber = atoi(Command);
                finishNumber = atoi(Command + breakString(Command) + 1);
                print(initNumber, finishNumber);
            }
            else if (Command[strlen(Command) - 2] == 'u' || Command[strlen(Command) - 2] == 'r') {
                //se rientro dalla print, actualCommand != numCommands
                actualCommand = numCommands;
                numberUndo = 0;
                while (Command[strlen(Command) - 2] == 'u' || Command[strlen(Command) - 2] == 'r') {
                    //UNDO case
                    if (Command[strlen(Command) - 2] == 'u') {
                        numberUndo = numberUndo + atoi(Command);
                        numCommands = numCommands - atoi(Command);
                        // case when I'm at the end of command list
                        if (numCommands <= 0 && actualCommand == maxCommand) {
                            numCommands = 0;
                            numberUndo = maxCommand;
                        }
                        //case when I'm in mid of command List
                        else if (numCommands <= 0 && actualCommand < maxCommand) {
                            numCommands = 0;
                            if(actualCommand == 0) {
                                numberUndo = actualCommand;
                            }
                            else{
                                if(actualCommand - numberUndo < 0){
                                    numberUndo = actualCommand /*- 1*/;
                                }
                            }
                        }

                    }
                    //REDO case
                    else if (Command[strlen(Command) - 2] == 'r') {
                        numberUndo = numberUndo - atoi(Command);
                        numCommands = numCommands + atoi(Command);
                        // case when I'm at the end of commandList
                        if (numCommands >= maxCommand && actualCommand == maxCommand) {
                            numCommands = maxCommand;
                            numberUndo = 0;
                        }
                            // case when I'm in the mid of command List
                        else if (numCommands >= maxCommand && actualCommand < maxCommand) {
                            numCommands = maxCommand;
                            numberUndo = actualCommand - maxCommand;
                        }
                    }
                    //memset(Command,0,sizeof(Command));
                    //input
                    fgets(Command, 499, stdin);

                    //if command is different from undo/redo

                    if (Command[strlen(Command) - 2] == 'c') {
                        if (numberUndo >= 0) {
                            currentCommand = undo(&currentCommand, numberUndo);
                        } else {
                            ifRedoCalling = true;
                            currentCommand = redo(&currentCommand, -numberUndo);
                            ifRedoCalling = false;
                        }
                        initNumber = atoi(Command);
                        finishNumber = atoi(Command + breakString(Command) + 1);
                        currentCommand = change(&currentCommand, initNumber, finishNumber);
                        break;
                    }
                    else if (Command[strlen(Command) - 2] == 'd') {
                        if (numberUndo >= 0) {
                            currentCommand = undo(&currentCommand, numberUndo);
                        } else {
                            ifRedoCalling = true;
                            currentCommand = redo(&currentCommand, -numberUndo);
                            ifRedoCalling = false;
                        }
                        initNumber = atoi(Command);
                        finishNumber = atoi(Command + breakString(Command) + 1);
                        currentCommand = delete(&currentCommand, initNumber, finishNumber);
                        break;
                    }
                    else if (Command[strlen(Command) - 2] == 'p') {
                        //devo muovermi fino al punto dove stampare!
                        actualCommand = numCommands;

                        if (numberUndo >= 0) {
                            currentCommand = undo(&currentCommand, numberUndo);
                        } else {
                            ifRedoCalling = true;
                            currentCommand = redo(&currentCommand, -numberUndo);
                            ifRedoCalling = false;
                        }
                        initNumber = atoi(Command);
                        finishNumber = atoi(Command + breakString(Command) + 1);
                        print(initNumber, finishNumber);
                        break;
                    }
                    else if (Command[0] == 'q') {
                        return 0;
                    }
                }
            }
        }
        else if(Command[0] == 'q'){          
            return 0;
        }
    }
}
