#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
HANDLE hStdout;
#define red SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_RED | FOREGROUND_INTENSITY);
#define green SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_GREEN| FOREGROUND_INTENSITY);
#define yellow SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_RED | FOREGROUND_GREEN);
#define blue SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_BLUE | FOREGROUND_INTENSITY);
#define white SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_BLUE | 6);
#define grey SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_INTENSITY | FOREGROUND_INTENSITY);
#define coloring hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

int compare(int numb1, int numb2) {
    if(numb1 > numb2)
        return numb1;
    return numb2;
}

void highscore(int nOfScores, int newScore) {
    coloring
    int i,j,scores[40],temp;
    empty_1D_Array(40,scores);


    FILE *scoreFile;

    //loading file
    scoreFile = fopen("highscores.c4","r");
    for(i = 0; i < nOfScores; i++)
    {
        fscanf(scoreFile,"%d", &scores[i]);
    }
    fclose(scoreFile);
    grey
    printf("Score is : %d\n\n",newScore);

    //Adding new score if it's in range
    if(newScore > scores[nOfScores-1])
        scores[nOfScores-1] = newScore;

    //Bubble Sort
    for(i = 0; i < nOfScores; i++)
    {
        for(j = 0; j < (nOfScores - i); j++)
        {
            if(scores[j] < scores[j+1])
            {
                temp = scores[j];
                scores[j] = scores[j+1];
                scores[j+1] = temp;
            }
        }
    }

    //printing scores
    white
    for(i = 0; i < nOfScores; i++)
    {
        if(scores[i] == newScore)
        {
            green
        }
        printf("%d\n", scores[i]);
        white
    }

    //save file
    scoreFile = fopen("highscores.c4","w");
    for(i = 0; i < nOfScores; i++)
    {
        fprintf(scoreFile,"%d ", scores[i]);
    }
    fclose(scoreFile);

    return 0;
}

void save(char grid[][30], int HEIGHT, int WIDTH) {

    //declaration of variables
    int i,j,k,exist=0;
    BYTE number[40], temp,temp1,temp2,temp3,temp4;
    char name[20];
    char path[30] = "saved";
    BYTE gridToSave[100];

    //Reading File name
    printf("Enter name of save file : ");
    gets(name);
    //Adding path and Extension "saved\<name>.cf"
    i=0,k=6;
    path[5] = 92;
    while(name[i] != '\0')
    {
        path[k]=name[i];
        i++;
        k++;
    }
    path[k] = '.',path[k+1] = 'c',path[k+2] = 'f';


    FILE *savefile;
    savefile = fopen(path, "rb");

    while(savefile) //
    {
        SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("\nThere's some file having the same name \nDo you want to overwrite (y or n) ?? ");
        SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_BLUE | 6);
        char overwrite[4];
        gets(overwrite);
        if(overwrite[0] == 'y')
        {
            break;
        }
        else
        {
            //Reading File name
            printf("Enter name of save file : ");
            gets(name);
            //Adding path and Extension "saved\<name>.cf"
            i=0,k=6;
            path[5] = 92;
            while(name[i] != '\0')
            {
                path[k]=name[i];
                i++;
                k++;
            }
            path[k] = '.',path[k+1] = 'c',path[k+2] = 'f';
            savefile = fopen(path, "rb");
        }
    }
    fclose(savefile);
    //save Height & Weight to 1st Byte
    number[0] = (BYTE)HEIGHT;  // HEIGHT < 16 0000|1111
    temp = (BYTE)WIDTH << 4;    // WIDTH < 16 1111|0000
    number[0] = number[0] | temp;    // 1111|1111
    k = 0;

    //Convert 2D Array To 1D Array >> Easier to save
    for(i = 0; i < HEIGHT; i++)
    {
        for(j = 0; j < WIDTH; j++)
        {
            if(grid[i][j] == 'X')
            {
                gridToSave[k++] = 1;
            }
            else if(grid[i][j] == 'O')
            {
                gridToSave[k++] = 2;
            }
            else
            {
                gridToSave[k++] = 0;
            }
        }
    }

    //Adding every 4 elements to 1 byte
    k = 0;
    for(i = 1; i <= ((HEIGHT*WIDTH)/4)+1; i++)
    {
        temp1 = gridToSave[k++];         // 00000011
        temp2 = gridToSave[k++] << 2;    // 00001100
        temp3 = gridToSave[k++] << 4;    // 00110000
        temp4 = gridToSave[k++] << 6;    // 11000000
        number[i] = (temp1 | temp2) | (temp3 | temp4);
    }

    //Access File
    //FILE *savefile;
    savefile = fopen(path,"wb");
    for(i = 0 ; i <= ((HEIGHT*WIDTH)/4)+1; i++ )
    {
        fwrite(&number[i],1,1,savefile);
    }
    fclose(savefile);

    SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("\nSaved Successfully\n");
    SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_BLUE | 6);

    return 0;
}

int load(char grid[][30], int *HEIGHT_, int *WIDTH_, int *stepNumber) {
    //Declaration of variables
    int i,j,k,HEIGHT,WIDTH;
    BYTE number[40], temp1,temp2,temp3,temp4;
    char name[20];
    char path[30] = "saved";
    //int gridToSave[100];
    int gridToOpen[100];

    //Reading File name
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE); //Yellow color
    SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_RED | FOREGROUND_GREEN);
    printf("\nEnter file name : ");
    gets(name);
    //Adding path and Extension "saved\<name>.cf"
    i=0,k=6;
    path[5] = 92;
    while(name[i] != '\0')
    {
        path[k]=name[i];
        i++;
        k++;
    }
    path[k] = '.',path[k+1] = 'c',path[k+2] = 'f';


    //Access File
    FILE *loadfile;
    loadfile = fopen(path, "rb");
    if(!loadfile)
    {
        SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_RED | FOREGROUND_INTENSITY);
        printf("\nFile Not Found !\n");
        return 0;
    }
    fread(&number[0],1,1,loadfile);

    //Read Height & Width for edges
    HEIGHT = (int)number[0]; // 11W1|11H1
    WIDTH = HEIGHT >> 4;  // 0000|11W1
    HEIGHT = HEIGHT & 15; // 0000|11H1

    //Read bytes from file
    for(i = 1 ; i <= ((HEIGHT*WIDTH)/4)+1; i++ )
    {
        fread(&number[i],1,1,loadfile);
    }
    fclose(loadfile);

    //Extract every 1 byte to 4 elements
    k = 0;
    for(i = 1; i <= ((HEIGHT*WIDTH)/4)+1; i++) //
    {
        temp1 = (int)number[i];
        gridToOpen[k++] = temp1&3; // 00000011
        gridToOpen[k++] = (temp1&12) >> 2; //00001100 >> 00000011
        gridToOpen[k++] = (temp1&48) >> 4; //00110000 >> 00000011
        gridToOpen[k++] = (temp1&192) >> 6;//11000000 >> 00000011
    }

    //Convert 1D Array(saved)  To 2D Array
    k = 0;
    *stepNumber = 0;
    for(i = 0; i < HEIGHT; i++)
    {
        for(j = 0; j < WIDTH; j++)
        {
            switch (gridToOpen[k])
            {
            case 0:
                grid[i][j] = '\0';
                break;
            case 1:
                grid[i][j] = 'X';
                *stepNumber += 1;
                break;
            case 2:
                grid[i][j] = 'O';
                *stepNumber += 1;
                break;
            }
            k++;
        }
    }

    //Assign Values to pointers
    *HEIGHT_ = HEIGHT;
    *WIDTH_ = WIDTH;


    SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("\nLoaded successfully\n");
    SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_BLUE | 6);

    return 1;
}

void addToHistory(int stepNumber,int column, int row, char element, int history[][5]){
    history[stepNumber][0] = column;
    history[stepNumber][1] = row;
    history[stepNumber][2] = (int)element;

    return 0;
}
void redo(int *stepNumber, int history[][5], char grid[][30],
          int*scorex,int*scoreo,int numberOfRedo){
    int i,c;
    int r;
    for (i=1;i<=numberOfRedo;i++){
        r = history[*stepNumber][1];
        c =  history[*stepNumber][0];
        grid[r][c] = history[*stepNumber][2];
        *scorex = history[*stepNumber][3];
        *scoreo = history[*stepNumber][4];
        if (numberOfRedo==2){
            *stepNumber+=1;
        }
    }
}
void undo(int *stepNumber, int history[][5], char grid[][30],int *scorex,
          int *scoreo,int numberOfUndo){
    int i,c;
    int r ;
    for (i=1;i<=numberOfUndo;i++){
        c =  history[*stepNumber-1][0];
        r = history[*stepNumber-1][1];
        grid[r][c] = 0;
        *scorex = history[*stepNumber-1][3];
        *scoreo = history[*stepNumber-1][4];
        *stepNumber-=2;
    }
    return 0;
}

void empty_2D_Array(int WIDTH, int HEIGHT, char Array[WIDTH][HEIGHT]){
    int i,j;
    for(i=0; i<HEIGHT; i++) // all elements are zeros
    {
        for(j=0; j<WIDTH; j++)
        {
            Array[i][j] = 0;
        }
    }
    return 0;
}
void empty_1D_Array(int Size, char Array[]){
    int i;
    for(i=0; i<Size; i++) // all elements are zeros
    {
        Array[i] = 0;
    }
    return 0;
}
void makeStep(int *stepNumber, int WIDTH, int HEIGHT, char select[],
    char grid[][30], int history[][5], char name1[],
    char name2[], int *row, int *col, int playMode,int*flagToDoCal,int *scorex,int *scoreo){
    int c,r;
    char element;
    char name[20];

    if((*stepNumber+1)%2 == 0){
        element = 'X';
        SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_RED | FOREGROUND_INTENSITY);
        strcpy(name,name1);
    }
    else{
        element = 'O';
        SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        strcpy(name,name2);
    }

   //-----------------------------------------------------------
    if(playMode == 2)  // for 2 players
    {

        printf("\n>[%c]:", element); // prints player's sign
        empty_1D_Array(9, select);
        gets(select);
        SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_BLUE | 6);
        if(select[0] == 'u')
        {
            if (*stepNumber < 2){
                printf("there is no undo");
                *stepNumber-=1;
            }
            else{
                    undo(&*stepNumber,history,grid,&*scorex,&*scoreo,1);
            }
        }
        else if (select[0] == 'r')
        {
            if(history[*stepNumber][2] == 0){
                printf("there is no redo");
                *stepNumber-=1;
            }
            else{
                redo(&*stepNumber,history,grid,&*scorex,&*scoreo,1);
            }
        }
        else if(select[0] == 's') {
            save(grid,HEIGHT,WIDTH);
            *stepNumber = HEIGHT*WIDTH;
        }
        else
        {
            for(c=0; c<WIDTH; c++){ // puts element in grid place
                if(select[c+1] == '\0'){
                    for(r = 0 ; r<HEIGHT; r++) {
                        if(grid[r][c] == 0)
                        {
                            int n,x;
                            grid[r][c] = element;
                            *row = r,*col = c;
                            addToHistory(*stepNumber,c,r,element,history);
                            *flagToDoCal = 1;
                            for (n=*stepNumber+1;n<81;n++){
                                history[n][2]=0;
                            }
                            break;
                        }
                    }
                    if(r == HEIGHT ){
                        printf("\n>>>>>>>>  Not Valid  <<<<<<<<\a");
                        printf("\nHey %s There's No Place in this Column !!",name);
                        (*stepNumber)-= 1;
                        break;
                    }
                    break;
                }
                if(c == WIDTH - 1){
                    printf("\n>>>>>>>>  Not Valid  <<<<<<<<\a");
                    printf("\nThink about it %s You Didn't choose a Column", name);
                    printf("\nPress Tab till you get Column you'll put in !\n");
                    (*stepNumber) -= 1;
                }
            }
        }
    SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_BLUE | 6);
    }
    else if(playMode == 1)  // for 1 player
    {
                if(select[0] == 'u')
        {
            if (*stepNumber < 2){
                printf("there is no undo");
                *stepNumber-=1;
            }
            else{
                    undo(&*stepNumber,history,grid,&*scorex,&*scoreo,1);
            }
        }
        else if (select[0] == 'r')
        {
            if(history[*stepNumber][2] == 0){
                printf("there is no redo");
                *stepNumber-=1;
            }
            else{
                redo(&*stepNumber,history,grid,&*scorex,&*scoreo,2);
            }
        }
        else if(select[0] == 's') {
            save(grid,HEIGHT,WIDTH);
            *stepNumber = HEIGHT*WIDTH;
        }
        else
        {
            if((*stepNumber+1)%2 == 0) // Player Turn
            {
                element = 'X';
                printf("\n>[%c]:", element); // prints player's sign
                empty_1D_Array(9, select);
                gets(select);

                if(select[0] == 'u')
                {
                    undo(stepNumber,history,grid,&*scorex,&*scoreo,2);
                }
                else
                {
                    for(c=0; c<WIDTH; c++){ // puts element in grid place
                        if(select[c+1] == '\0'){
                            for(r = 0 ; r<HEIGHT; r++) {
                                if(grid[r][c] == 0)
                                {
                                    int n,x;
                                    grid[r][c] = element;
                                    *row = r,*col = c;
                                    addToHistory(*stepNumber,c,r,element,history);
                                    *flagToDoCal = 1;
                                    for (n=*stepNumber+1;n<81;n++){
                                        history[n][2]=0;
                                    }
                                    break;
                                }
                            }
                            if(r == HEIGHT ){
                                printf("\n>>>>>>>>  Not Valid  <<<<<<<<\a");
                                printf("\nHey %s There's No Place in this Column !!",name);
                                (*stepNumber)-= 1;
                                break;
                            }
                            break;
                        }
                        if(c == WIDTH - 1){
                            printf("\n>>>>>>>>  Not Valid  <<<<<<<<\a");
                            printf("\nThink about it %s You Didn't choose a Column", name);
                            printf("\nPress Tab till you get Column you'll put in !\n");
                            (*stepNumber) -= 1;
                        }
                    }
                }
            }
            else  // Computer Turn
            {
                int x=0,i=0;
                while(x==0)
                {
                    switch(i) {
                        case 0:
                            x = defenceStr(stepNumber,grid,WIDTH,HEIGHT,row,col,history);
                            printf("Defence..");
                            break;
                        case 1:
                            x = attackStr(stepNumber,grid,WIDTH,HEIGHT,row,col,history);
                            printf("Attack..");
                            break;
                        case 2:
                            x = subAttackStr(stepNumber,grid,WIDTH,HEIGHT,row,col,history);
                            printf("SubAttack..");
                            break;
                        case 3:
                            x = subDefenceStr(stepNumber,grid,WIDTH,HEIGHT,row,col,history);
                            printf("SubDefence..");
                            break;
                        case 4:
                            x = randomStr(stepNumber,grid,WIDTH,HEIGHT,row,col,history);
                            printf("Random..");
                            break;

                    }
                    i++;
                }
                addToHistory(*stepNumber,*col,*row,element,history);
            }
        }
    //return 0;
    }
    return 0;
}
void readNames(char name1[],char name2[], int noOfPlayers){
        printf("Player X's name :");
        scanf("%s",name1);

        if(noOfPlayers == 2)
        {
            printf("Player O's name :");
            scanf("%s",name2);

        }
        else
        {
            name2 = "Computer";
        }
    return 0;
}

void showGrid(int HEIGHT,int WIDTH,char grid[][30], char nameX[], int scoreX, char nameO[], int scoreO) {
    int i = 0,j;
    coloring
    grey
    printf("\n\ns:save | m:menu | u:undo | r:redo");
    red
    printf("\n%s:%d",nameX, scoreX);
    green
    printf("\n%s:%d\n", nameO, scoreO);
    printf("\a\n    ");
    white
    for(j=0; j<WIDTH; j++) // prints a line
    {
        printf("________");
    }
    printf("_\n    ");

    for (i = HEIGHT - 1; i >= 0; i--) {
        printf("|");
        for (j = 0; j < WIDTH; j++) {
            printf("       |", grid[i][j]);
        }
        printf("\n    ");
        for (j = 0; j < WIDTH; j++) {
            printf("|");
            if(grid[i][j] == 'X') {
                SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_RED | FOREGROUND_INTENSITY);
                printf("   %c   ", grid[i][j]);
            }
            else if(grid[i][j] == 'O') {
                SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                printf("   %c   ", grid[i][j]);
            }
            else {
                SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                printf("   %c   ", grid[i][j]);
            }
            SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_BLUE | 6);

        }

        printf("|\n    |");
        for (j = 0; j < WIDTH; j++) {
            printf("_______|", grid[i][j]);
        }
        printf("\n    ");
    }
}

void calculation(int r, int c, char grid[][30],int *scorex, int *scoreo){
    int n1, n2, signi, signj,signi1, signj1,signi2, signj2, score1, i=r,j=c ;
    if (grid[i][j] == 'X'){
        score1=*scorex;
    }
    else if (grid[i][j] == 'O'){
        score1=*scoreo;
    }

    for(n1=0;n1<7;n1++)
    {
        switch(n1){
            case 0:
                signi=-1,signj=1;
                break;
            case 1:
                signi=0,signj=1;
                break;
            case 2:
                signi=1,signj=1;
                break;
            case 3:
                signi=-1,signj=0;
                break;
            case 4:
                signi=1,signj=-1;
                break;
            case 5:
                signi=0,signj=-1;
                break;
            case 6:
                signi=-1,signj=-1;
                break;
        }
        if(grid[i][j]==grid[i+signi][j+signj]                              &&
           grid[i+(signi)][j+(signj)]==grid[i+(2*signi)][j+(2*signj)]      &&
           grid[i+(2*signi)][j+(2*signj)]==grid[i+(3*signi)][j+(3*signj)]){
            score1++;
        }
    }
    for(n1=0;n1<3;n1++){
        switch(n1){
            case 0:
                signi1=1,signj1=1;
                signi2=-1,signj2=-1;
                break;
            case 1:
                signi1=1,signj1=-1;
                signi2=-1,signj2=1;
                break;
            case 2:
                signi1=0,signj1=1;
                signi2=0,signj2=-1;
                break;
        }
        if(grid[i][j] == grid[i+signi1][j+signj1]  &&
           grid[i][j] == grid[i+signi2][j+signj2]  &&
           grid[i][j] == grid[i+2*signi1][j+2*signj1]){
            score1++;
           }
        if(grid[i][j] == grid[i+signi1][j+signj1]  &&
           grid[i][j] == grid[i+signi2][j+signj2]  &&
           grid[i][j] == grid[i+2*signi2][j+2*signj2]){
            score1++;
           }
    }
    if (grid[i][j] == 'X'){
        *scorex = score1;
    }
    else if (grid[i][j] == 'O'){
        *scoreo = score1;
    }
}
void randomStr(int *stepNumber, char grid[][9],int WIDTH, int HEIGHT,int *row,int *col,
                char history[][5]) {
    int c,r,k;
    char element = 'O';
    srand(time(NULL));
    k = rand()%WIDTH;

    for(c=k; c<WIDTH; c++){
        for(r=0; r<HEIGHT; r++) {
            if(grid[r][c] == 0)
            {
                grid[r][c] = element;
                *row = r,*col = c;
                //addToHistory(stepNumber,c,r,element,history);
                return 0;
            }
        }
    }
    for(c=0; c<WIDTH; c++){
        for(r = 0 ; r<HEIGHT; r++) {
            if(grid[r][c] == 0)
            {
                grid[r][c] = element;
                *row = r,*col = c;
                //addToHistory(stepNumber,c,r,element,history);
                return 0;
            }
        }
    }
    return 1;
}
int attackStr(int *stepNumber, char grid[][30],int WIDTH, int HEIGHT,int *row,int *col,
               char history[][5]) {
    int n1, n2, signi, signj, score1, i, j, r, c;
    char element = 'O';

    for(c=0; c<WIDTH; c++){
        for(r = 0 ; r<HEIGHT; r++) {
            if(grid[r][c] == 0)
            {
                i=r;
                j=c;
                for(n1=0;n1<7;n1++)
                {
                    switch(n1){
                        case 0:
                            signi=-1,signj=1;
                            break;
                        case 1:
                            signi=0,signj=1;
                            break;
                        case 2:
                            signi=1,signj=1;
                            break;
                        case 3:
                            signi=-1,signj=0;
                            break;
                        case 4:
                            signi=1,signj=-1;
                            break;
                        case 5:
                            signi=0,signj=-1;
                            break;
                        case 6:
                            signi=-1,signj=-1;
                            break;
                    }
                    if(grid[i+(signi)][j+(signj)]== 'O'                         && // O >> X for Defence
                       grid[i+(signi)][j+(signj)]==grid[i+(2*signi)][j+(2*signj)] &&
                       grid[i+(2*signi)][j+(2*signj)]==grid[i+(3*signi)][j+(3*signj)])
                    {
                        if(r < HEIGHT){
                            grid[i][j] = element;
                            *row = i,*col = j;
                            //addToHistory(*stepNumber,j,i,element,history);
                            return 1;
                        }
                    }
                    else if(grid[i+(signi)][j+(signj)]== 'O'                         && // O >> X for Defence
                       grid[i+(signi)][j+(signj)]==grid[i+(2*signi)][j+(2*signj)] &&
                       grid[i+(2*signi)][j+(2*signj)]==grid[i-(1*signi)][j-(1*signj)])
                    {
                        if(r < HEIGHT){
                            grid[i][j] = element;
                            *row = i,*col = j;
                            //addToHistory(*stepNumber,j,i,element,history);
                            return 1;
                        }
                    }
                }
                break;
            }
        }
    }
    return 0;
}
int defenceStr(int *stepNumber, char grid[][30],int WIDTH, int HEIGHT,int *row,int *col,
                char history[][5]) {
    int n1, n2, signi, signj, score1, i, j, r, c;
    char element = 'O';

    for(c=0; c<WIDTH; c++){
        for(r = 0 ; r<HEIGHT; r++) {
            if(grid[r][c] == 0)
            {
                i=r;
                j=c;
                for(n1=0;n1<7;n1++)
                {
                    switch(n1){
                        case 0:
                            signi=-1,signj=1;
                            break;
                        case 1:
                            signi=0,signj=1;
                            break;
                        case 2:
                            signi=1,signj=1;
                            break;
                        case 3:
                            signi=-1,signj=0;
                            break;
                        case 4:
                            signi=1,signj=-1;
                            break;
                        case 5:
                            signi=0,signj=-1;
                            break;
                        case 6:
                            signi=-1,signj=-1;
                            break;
                    }
                    if(grid[i+(signi)][j+(signj)]== 'X'                         && // O >> X for Defence
                       grid[i+(signi)][j+(signj)]==grid[i+(2*signi)][j+(2*signj)] &&
                       grid[i+(2*signi)][j+(2*signj)]==grid[i+(3*signi)][j+(3*signj)])
                    {
                        if(r < HEIGHT){
                            grid[i][j] = element;
                            *row = i,*col = j;
                            //addToHistory(*stepNumber,j,i,element,history);
                            return 1;
                        }
                    }
                    else if(grid[i+(signi)][j+(signj)]== 'X'                         && // O >> X for Defence
                       grid[i+(signi)][j+(signj)]==grid[i+(2*signi)][j+(2*signj)] &&
                       grid[i+(2*signi)][j+(2*signj)]==grid[i-(1*signi)][j-(1*signj)])
                    {
                        if(r < HEIGHT){
                            grid[i][j] = element;
                            *row = i,*col = j;
                            //addToHistory(*stepNumber,j,i,element,history);
                            return 1;
                        }
                    }
                }
                break;
            }
        }
    }
    return 0;
}
int subAttackStr(int *stepNumber, char grid[][30],int WIDTH, int HEIGHT,int *row,int *col,
                  char history[][5]) {
    int n1, n2, signi, signj, score1, i, j, r, c;
    char element = 'O';

    for(c=0; c<WIDTH; c++){
        for(r = 0 ; r<HEIGHT; r++) {
            if(grid[r][c] == 0)
            {
                i=r;
                j=c;
                for(n1=0;n1<7;n1++)
                {
                    switch(n1){
                        case 0:
                            signi=-1,signj=1;
                            break;
                        case 1:
                            signi=0,signj=1;
                            break;
                        case 2:
                            signi=1,signj=1;
                            break;
                        case 3:
                            signi=-1,signj=0;
                            break;
                        case 4:
                            signi=1,signj=-1;
                            break;
                        case 5:
                            signi=0,signj=-1;
                            break;
                        case 6:
                            signi=-1,signj=-1;
                            break;
                    }
                    if(grid[i+(signi)][j+(signj)]== 'O'                         && // O >> X for Defence
                       grid[i+(signi)][j+(signj)]==grid[i+(2*signi)][j+(2*signj)])
                    {
                        if(r < HEIGHT){
                            grid[i][j] = element;
                            *row = i,*col = j;
                            //addToHistory(*stepNumber,j,i,element,history);
                            return 1;
                        }
                    }
                }
                break;
            }
        }
    }
    return 0;
}
int subDefenceStr(int *stepNumber, char grid[][30],int WIDTH, int HEIGHT,int *row,int *col,
                   char history[][5]) {
    int n1, n2, signi, signj, score1, i, j, r, c;
    char element = 'O';

    for(c=0; c<WIDTH; c++){
        for(r = 0 ; r<HEIGHT; r++) {
            if(grid[r][c] == 0)
            {
                i=r;
                j=c;
                for(n1=0;n1<7;n1++)
                {
                    switch(n1){
                        case 0:
                            signi=-1,signj=1;
                            break;
                        case 1:
                            signi=0,signj=1;
                            break;
                        case 2:
                            signi=1,signj=1;
                            break;
                        case 3:
                            signi=-1,signj=0;
                            break;
                        case 4:
                            signi=1,signj=-1;
                            break;
                        case 5:
                            signi=0,signj=-1;
                            break;
                        case 6:
                            signi=-1,signj=-1;
                            break;
                    }
                    if(grid[i+(signi)][j+(signj)]== 'X'                         && // O >> X for Defence
                       grid[i+(signi)][j+(signj)]==grid[i+(2*signi)][j+(2*signj)])
                    {
                        if(r < HEIGHT){
                            grid[i][j] = element;
                            *row = i,*col = j;
                            //addToHistory(*stepNumber,j,i,element,history);
                            return 1;
                        }
                    }
                }
                break;
            }
        }
    }
    return 0;
}
int readDim(char str1[],char str2[],char str3[]){
    int i,j;
    char s[200];// to not change in the main string str1
    strcpy(s,str1);
    for (i=findOccurStr2InStr1(s,str2,2),j=0;s[i]!=NULL;i++,j++){
        s[j]=s[i+strlen(str2)];//16 indicate length of
    }
    //put NULL
    s[j]=NULL;
    //find th first occur of </Configurations>
    s[findOccurStr2InStr1(s,str3,1)]=NULL;
    for (i=0;s[i]!=NULL;i++){
        if (isdigit(s[i]) ==0)
            return -1;
    }
    return atoi(s);
}
int findOccurStr2InStr1(char str1[],char str2[],int flagForFisrtOrLast)//to fist 1 to last 2
{
    int counter , counter1 = 0 ,lastOcuer, counter2;
    while(str1[counter1] != '\0')
    {
        counter2 = 0;

        if(str1[counter1] == str2[counter2])
        {
            counter = counter1;

            while(str1[counter] == str2[counter2] && str2[counter2] != '\0')
            {
                counter++;
                counter2++;
            }
            if (str2[counter2] == '\0')
            {
                lastOcuer=counter1;
                if (flagForFisrtOrLast== 1){
                  return lastOcuer;
                }
            }
        }
        counter1++;
    }
    return lastOcuer;
}


void readXML(int *width,int *height,int *highscore){
	char c,n[50],u[900];
    int j,i=0,k=9,counter;
    FILE *file= fopen("m.xml", "r");
    while(1){
        c=fgetc(file);
        u[i++]=c;
        if(feof(file)){
            counter=i-1;
            u[i]=NULL;
            break;
        }
    }
    i=0;
    while(u[i]!=NULL){
        if (u[i]=='<'){
            for(j=i+1;u[j]!='>'&&u[j]!=NULL;j++);
            i=j;
        }
        if (u[i]==' ' || u[i]=='\n'){
            for(j=i;u[j]!=NULL;j++){
                u[j]=u[j+1];
            }
            u[counter]=NULL;
            counter-=1;
            i--;
        }
        i++;
    }
    fclose(file);
    /*that's make shift for last <Configurations> if more than one <Configurations>
    in xml */
    for (i=findOccurStr2InStr1(u,"<Configurations>",2),j=0;u[i]!=NULL;i++,j++){
        u[j]=u[i+strlen("<Configurations>")];//16 indicate length of
    }
    //put NULL
    u[j]=NULL;
    //find th first occur of </Configurations>
    u[findOccurStr2InStr1(u,"</Configurations>",1)]=NULL;
    *width=readDim(u,"<Width>","</Width>");
    *height=readDim(u,"<Height>","</Height>");
    *highscore=readDim(u,"<Highscores>","</Highscores>");
    if (*width== -1){
        *width = 7;
    }
    if (*highscore==-1){
        *highscore=5;
    }
    if(*height==-1){
        *height=6;
    }
}


int main(){

    //Declarations of Variables
    int i,playMODE,stepNumber,HIGHSCORE,STEPS,HEIGHT,WIDTH, scorex = 0, scoreo = 0,r,c,flagToDoCal,loaded = 0;

    //Declarations of arrays
    char name1[20], name2[20]; // to read names
    char grid[30][30]; // to add elements and calculate on
    char select[40]; // to select column to add element to
    int history[81][5]; // steps for undo & redo


    while(playMODE != 4) {
        // empty arrays
        empty_2D_Array(30, 30, grid);

        //Colors
        hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY|FOREGROUND_BLUE | 6);
        //Menu
        printf("\n    ---------------------\n");
        printf("    |1- play vs Computer |\n");
        printf("    |2- player vs player |\n");
        printf("    |3- load             |___\n");
        printf("    |4- exit                 |\n");
        printf("    -------------|Choose : ");
        scanf("%d",&playMODE);
        gets(select);
        loaded = 0;
        if(playMODE == 3) { //Load game
            while(loaded == 0)
            {
                loaded = load(grid,&HEIGHT,&WIDTH,&stepNumber);
            }
                printf("    ---------------------\n");
                printf("    |1- play vs Computer |___\n");
                printf("    |2- player vs player     |\n");
                printf("    |____________|Choose : ");
                scanf("%d",&playMODE);

        }


        if(playMODE == 2){ //2 Players MODE
            readNames(name1,name2,2);
            if(loaded == 0) //if loaded Width and Height is read from file
            {
                readXML(&WIDTH,&HEIGHT,&HIGHSCORE);
            }
            STEPS = WIDTH * HEIGHT;

            // for the first time only
            gets(select);
            showGrid(HEIGHT,WIDTH,grid,name1,scorex,name2,scoreo);
            for(stepNumber=1; stepNumber<=STEPS; stepNumber++) // loop until all steps are done
            {
                flagToDoCal = 0;
                makeStep(&stepNumber,WIDTH,HEIGHT,select,grid,history,name1,
                         name2,&r,&c,2,&flagToDoCal,&scorex,&scoreo);
                if (flagToDoCal == 1){
                    calculation(r, c, grid, &scorex, &scoreo);
                    history[stepNumber][3]=scorex,history[stepNumber][4]=scoreo;
                }
                showGrid(HEIGHT,WIDTH,grid,name1,scorex,name2,scoreo);
            }
            scorex = compare(scoreo,scorex);
            highscore(HIGHSCORE,scorex);
        }
        else if(playMODE == 1){ //1 Player MODE

            readNames(name1,name2,1);
            if(loaded == 0) //if loaded Width and Height is read from file
            {
                readXML(&WIDTH,&HEIGHT,&HIGHSCORE);
            }
            STEPS = WIDTH * HEIGHT;


            showGrid(HEIGHT,WIDTH,grid,name1,scorex,"Computer",scoreo);
            gets(select);

            for(stepNumber=1; stepNumber<=STEPS; stepNumber++) // loop until all steps are done
            {

                makeStep(&stepNumber,WIDTH,HEIGHT,select,grid,history,name1,
                         name2,&r,&c,1,&flagToDoCal,&scorex, &scoreo);
                calculation(r, c, grid, &scorex, &scoreo);
                showGrid(HEIGHT,WIDTH,grid,name1,scorex,"Computer",scoreo);
            }
            highscore(HIGHSCORE,scorex);
        }
        else if(playMODE == 4) {
            exit(0);
        }
        else{ // for a strange input
            printf("think Again you entered %d although you've to enter from 1 to 4 !! \a\a \n",playMODE);
        }
    }

    return 0;
}
