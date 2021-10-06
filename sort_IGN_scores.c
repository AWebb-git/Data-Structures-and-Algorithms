//Andrew Webb 12/11/20
//Program that uses both mergesort and quicksort to sort a list of IGN video game
//scores from 1996 to 2016 and prints the 10 best games by score and how many consoles
//the game was on
#ifdef  _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif //  _MSC_VER
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#define MAX_STRING_SIZE 100
#define ARRAY_SIZE 18625

int compares, swaps;

struct Data {
	int score, year, perfPorts;
	char title[MAX_STRING_SIZE];
	char platform[MAX_STRING_SIZE];
};

void swap(struct Data* a, struct Data* b){
    struct Data t = *a;
    *a = *b;
    *b = t;
    swaps++;
}

int
next_field( FILE *f, char *buf, int max ) {
	int i=0, end=0, quoted=0;
	
	for(;;) {
		// fetch the next character from file		
		buf[i] = fgetc(f);
		// if we encounter quotes then flip our state and immediately fetch next char
		if(buf[i]=='"') { quoted=!quoted; buf[i] = fgetc(f); }
		// end of field on comma if we're not inside quotes
		if(buf[i]==',' && !quoted) { break; }
		// end record on newline or end of file
		if(feof(f) || buf[i]=='\n') { end=1; break; } 
		// truncate fields that would overflow the buffer
		if( i<max-1 ) { ++i; } 
	}

	buf[i] = 0; // null terminate the string
	return end; // flag stating whether or not this is end of the line
}

int partition_perfPorts(struct Data array[], int start, int end){
    int pivot = array[end].perfPorts;
    int i = start - 1;
    int j = end ;

    for(;;){
        while(i < end){
            i++;
            compares++;
            if(array[i].perfPorts < pivot){break;}
        }

        while(j > start){
            j--;
            compares++;
            if(array[j].perfPorts >= pivot){break;}
        }

        if(j <= i){
            swap(&array[i], &array[end]);
            return i;
        }

        swap(&array[i], &array[j]);
    }
}

void quicksort2(struct Data A[], int p, int r){
    if(p < r){
        int q = partition_perfPorts(A, p, r);
        quicksort2(A, p, q-1);
        quicksort2(A, q+1, r);
    }
}

void merge(struct Data array[], int low, int mid, int high){
    int index1, index2, index3; //indexs for left, right and combined arrays
    int size1 = mid - low + 1;  //left array size
    int size2 = high - mid;     //right array size

    struct Data* l_arr = (struct Data*)malloc(size1*sizeof(struct Data));
    struct Data* r_arr = (struct Data*)malloc(size2*sizeof(struct Data));

    //assign values for left array
    for(index1 = 0; index1 < size1; index1++){
        l_arr[index1] = array[low + index1];
    }
    //assign values for right array
    for(index2 = 0; index2 < size2; index2++){
        r_arr[index2] = array[mid + index2 + 1];
    }

    index1 = 0;
    index2 = 0;
    index3 = low;

    //comparing right and left arrays
    while(index1 < size1 && index2 < size2){
        compares++;
        //if left array value is less than right, value in combined array is left
        if(l_arr[index1].score >= r_arr[index2].score){
            array[index3] = l_arr[index1];
            index1++;
        }
        else{//else its the right value
            array[index3] = r_arr[index2];
            index2++;
        }
        index3++;
    }
    //assign values leftover in left if all of right is already inserted into combined array
    while (index1 < size1){
        compares++;
        array[index3] = l_arr[index1];
        index1++;
        index3++;
    }
    //assign values leftover in right if all of left is already inserted into combined array
    while (index2 < size2){
        compares++;
        array[index3] = r_arr[index2];
        index2++;
        index3++;
    }
    
    //if the last call of merge, free dynamically allocated memory
    if(low == 0 && high == ARRAY_SIZE - 1){
        free(l_arr);
        free(r_arr);
    }
}

void mergesort(struct Data A[], int p, int r){
    if(p < r){
        int q = (p+r)/2;
        mergesort(A, p, q);
        mergesort(A, q + 1, r);
        merge(A, p, q, r);
    }
}

void bestsort(struct Data A[]){
    //i is the lower index for sorting later
    int i = 0;
    //index for array without perfect(same score) duplicates/ports of other games
    int x = -1;
   
    struct Data* arr10 = (struct Data*)malloc(ARRAY_SIZE*sizeof(struct Data));
    //popular by score and amount of ports(same title, different console) with same score
    for(int z = 0; z<(ARRAY_SIZE); z++){
        //if not the first entry and has the same title and score as the last game (is a perfect port)
        if(z > 0 && strcmp(A[z].title, A[z-1].title) == 0 && A[z].score == A[z-1].score){
            arr10[x].perfPorts++;   //its a port, so no new entry, just update amount of ports
        }
        else{       //new entry
            x++;
            arr10[x] = A[z];
            arr10[x].perfPorts = 1;
        }
        //if change in score, quicksort by no. of perfect ports between i (lower index) and x-1, then update i
        //This means all the 10s will be sorted by amount of ports, then all the 9s, etc.
        if(A[z].score != A[z-1].score && z > 0){
            quicksort2(arr10, i, x-1);
            i = x;
        }
        
    }

    //printf("%d\n", arr10[0].year); //prints the year, used if we want to sort into groups of 10 per year
    printf("%5s| %60s|%5s|%s\n", "", "Title", "Score", "Perfect Ports");
    for(int i = 0; i < 10; i ++){
        printf("%5d| %60s|%5d|%5d\n", i+1, arr10[i].title, arr10[i].score, arr10[i].perfPorts);
    }
    printf("\n");

    //used if we want to sort into groups of 10 per year
    //if last year free dynamically allocated memory
    //if(arr10[0].year == 1996){
        free(arr10);
    //}

}

int main(){
    int endLine = 0;
	int rec1done = 0;
	int colNum = 0;
    int rowNum = 0;
	

	FILE* file;
	char fieldBuffer[MAX_STRING_SIZE];
	int maxSize = sizeof(fieldBuffer);

	struct Data* arr = (struct Data*)malloc(ARRAY_SIZE*sizeof(struct Data));

	//open file
	file = fopen("ign.csv", "r");
	
	if (file == NULL) {
		printf("no file opened\n");
	}
	else {
		
		//iterate by record
		while (!feof(file)) {
			//iterate by field
			while (endLine != 1)
			{
				endLine = next_field(file, fieldBuffer, maxSize);
				
				//if past the first record pass values to temp
				if(rec1done == 1){
					switch (colNum)
					{
					case 0:
						strcpy(arr[rowNum].title, fieldBuffer);
						colNum++;
						break;
					case 1:
						strcpy(arr[rowNum].platform, fieldBuffer);
						colNum++;
						break;
					case 2:
						arr[rowNum].score = atoi(fieldBuffer);
						colNum++;
						break;
					case 3:
						arr[rowNum].year = atoi(fieldBuffer);
						colNum = 0;
						break;
					default:
						printf("Error at column: %d", colNum);
						break;
					}

					
				}
			}
			
			if(rec1done == 1){rowNum++;}
			rec1done = 1;
			endLine = 0;
		}


        mergesort(arr, 0, ARRAY_SIZE-1);
        bestsort(arr);
    }

    free(arr);  
	fclose(file);
    return 0;
}
