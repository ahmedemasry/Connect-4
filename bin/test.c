#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void addToHistory(int stepNumber,int column, int row, char element, char history[][3]){
    history[stepNumber][0] = column;
    history[stepNumber][1] = row;
    history[stepNumber][2] = element;

    return 0;
}
void undo(int *stepNumber, char emptyRowNumber[], char history[][3], char grid[][9]){
    int c =  history[*stepNumber][0];
    int r = history[*stepNumber][1];
    grid[c][r] = 0;
    emptyRowNumber[c]--;
    *stepNumber--;
    return 0;
}
void printGrid(int WIDTH, int HEIGHT, char grid[][HEIGHT]){
    int i,j;
    printf("\a\n    ");
    for(j=0; j<WIDTH; j++) // prints a line
    {
        printf("________");
    }
    printf("_\n    ");

    for (i = HEIGHT - 1; i >= 0; i--) {
        for (j = 0; j < WIDTH; j++) {
            printf("|   %c   ", grid[i][j]);
        }
        printf("|\n    |");
        for (j = 0; j < WIDTH; j++) {
            printf("_______|", grid[i][j]);
        }
        printf("\n    ");
    }

/*
    int ii = 0;
    int jj = 0;
    for (ii = HEIGHT - 1; ii >= 0; ii--) {
        for (jj = 0; jj < WIDTH; jj++) {
            printf("|   %c   ", grid[ii][jj]);
        }
        printf("|\n    |");
        for(j=0; j < WIDTH; j++) { // prints a line
            printf("_______|");
        }
        printf("\n    ");
    }
    printf("\n");



    /*for(i=(HEIGHT-1); i>=0; i--) // i FROM height-1 TO 0 to print first row in bottom
    {
        for(j=0; j<WIDTH; j++) // prints elements
        {
            printf("|   %c   ",grid[i][j]);
        }

        printf("|\n    |");
        for(j=0; j<WIDTH; j++) // prints a line
        {
            printf("_______|");
        }
        printf("\n    ");
    }
    printf("\n");

    */
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
void makeStep(int *stepNumber, int WIDTH, int HEIGHT, char select[], int emptyRowNumber[], char grid[][9], char history[][3], char name1[], char name2[], int *row, int *col){
    /*printf("printing array ===========\n");
    int ii = 0;
    int jj = 0;
    for (ii = HEIGHT - 1; ii >= 0; ii--) {
        for (jj = 0; jj < WIDTH; jj++) {
            printf(" %c", grid[ii][jj]);
        }
        printf("\n");
    }
    printf("done printing array ===========\n"); */

    int c;
    char element;
    char name[20];

    if((*stepNumber+1)%2 == 0){
        element = 'X';
        strcpy(name,name1);
    }
    else{
        element = 'O';
        strcpy(name,name2);
    }
    printf("\n>[%c]:", element); // prints player's sign
    empty_1D_Array(9, select);
    gets(select);

    if(select[0] == 'u')
    {
        undo(stepNumber,emptyRowNumber,history,grid);
    }
    else
    {
        for(c=0; c<WIDTH; c++){ // puts element in grid place
            if(select[c+1] == '\0'){
                if(emptyRowNumber[c] >= HEIGHT){
                    printf("\n>>>>>>>>  Not Valid  <<<<<<<<\a");
                    printf("\nHey %s There's No Place in this Column !!",name);
                    *stepNumber-=1;
                    break;
                }
                else if(emptyRowNumber[c] < HEIGHT){
                    int r = emptyRowNumber[c];
                    grid[r][c] = element;
                    *row = r,*col = c;
                    addToHistory(*stepNumber,c,r,element,history);
                    emptyRowNumber[c]++;
                    break;
                }
            }
            if(c == WIDTH - 1){
                printf("\n>>>>>>>>  Not Valid  <<<<<<<<\a");
                printf("\nThink about it %s You Didn't choose a Column", name);
                printf("\nPress Tab till you get Column you'll put in !\n");
                *stepNumber -= 1;
            }
        }
    }


//PRINTING GRID ----------------------------------------------------------------------------------------
    int i,j;
    printf("\a\n    ");
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
            if(grid[i][j] == 'X') {
                printf("|   %c   ", grid[i][j]);
            }
            else if(grid[i][j] == 'O') {
                printf("|   %c   ", grid[i][j]);
            }
            else {
                printf("|   %c   ", grid[i][j]);
            }
        }
        printf("|\n    |");
        for (j = 0; j < WIDTH; j++) {
            printf("_______|", grid[i][j]);
        }
        printf("\n    ");
    }

// ------------------------------------------------------------------------------------------------------
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
void calculation(int r, int c, char grid[][9],int *scorex, int *scoreo){
    int n1, n2, signi, signj, score1, i=r,j=c ;
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
        if(grid[i][j]==grid[i+signi][j+signj]                         &&
           grid[i+(signi)][j+(signj)]==grid[i+(2*signi)][j+(2*signj)] &&
           grid[i+(2*signi)][j+(2*signj)]==grid[i+(3*signi)][j+(3*signj)])
        {
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


int main(){
    int i,playMODE,stepNumber,STEPS,HEIGHT,WIDTH, scorex = 0, scoreo = 0,r,c;
    printf("for 1 player enter 1 \n");
    printf("for 2 players enter 2 \n");
    printf("How many players (2 or 1) :");
    scanf("%d",&playMODE);


    // declarations of arrays
    char name1[20], name2[20]; // to read names
    char grid[9][9]; // to add elements and calculate on
    char select[40]; // to select column to add element to
    int emptyRowNumber[9]; // to memorize last empty row for each column {empty row number in column [N]}
    char history[81][3]; // steps for undo & redo

    //
    if(playMODE == 2){ // for 2 Players
        readNames(name1,name2,2);
        printf("Width & Height   (Max : 9x9)\n");
        scanf("%d %d", &WIDTH, &HEIGHT);
        STEPS = WIDTH * HEIGHT;
        // empty arrays
        empty_2D_Array(9, 9, grid);
        for(i=0;i<9;i++) { // to make all values in emptyRowNumber = 0
            emptyRowNumber[i] = 0;
        }


        // for the first time only
        gets(select);
        printGrid(WIDTH,HEIGHT,grid);

        for(stepNumber=1; stepNumber<=STEPS; stepNumber++) // loop until all steps is done
        {
            makeStep(&stepNumber,WIDTH,HEIGHT,select,emptyRowNumber,grid,history,name1,name2,&r,&c);
            calculation(r, c, grid, &scorex, &scoreo);
        }
    printf("%d %d", scorex,scoreo);
    }
    else if(playMODE == 1){ // for 1 Player
        readNames(name1,name2,1);
        printf("Wait For Us %s , Work in progress .... ;\) \n", name1);
    }
    else{ // for a strange input
        printf("think Again you entered %d although you've to enter 1 or 2 !! \a\a \n",playMODE);
    }

    return 0;
}
