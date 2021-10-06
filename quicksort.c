//Andrew Webb 5/11/20
//program showing quicksort in effect
#ifdef  _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif //  _MSC_VER
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define big_s 10000

int compares, swaps;

void swap(int *a, int  *b){
    int t = *a;
    *a = *b;
    *b = t;
    swaps++;
}

//pivot at first element
int partition1(int array[], int start, int end){
    int pivot = array[start];
    int i = start;
    int j = end + 1;

    for(;;){
        while(i < end){
            i++;
            compares++;
            if(array[i] > pivot){break;}
        }

        while(j > start){
            j--;
            compares++;
            if(array[j] <= pivot){break;}
        }

        if(j <= i){
            swap(&array[j], &array[start]);
            return j;
        }

        swap(&array[i], &array[j]);
    }
}
//pivot at last element
int partition2(int array[], int start, int end){
   
    int pivot = array[end];
    int i = start - 1;
    int j = end ;

    for(;;){
        while(i < end){
            i++;
            compares++;
            if(array[i] > pivot){break;}
        }

        while(j > start){
            j--;
            compares++;
            if(array[j] <= pivot){break;}
        }

        if(j <= i){
            swap(&array[i], &array[end]);
            return i;
        }

        swap(&array[i], &array[j]);
    }

   
}
//pivot at random element
int partition3(int array[], int start, int end){
    int r = rand()%(end + 1 - start) + start;
    swap(&array[r], &array[end]);
    return partition1(array, start, end);
}

void quicksort1(int A[], int p, int r){
    if(p < r){
        int q = partition2(A, p, r);
        quicksort1(A, p, q-1);
        quicksort1(A, q+1, r);
    }
}

void printArray(int A[], int size){
   for(int i = 0; i < size; i++){
        printf("%d ", A[i]);
    } 
    printf("    Comparsions: %10d Swaps: %10d\n", compares, swaps);
    compares = 0;
    swaps = 0;
}

void printStats(){
    printf("Comparsions: %10d Swaps: %10d\n\n", compares, swaps);
    compares = 0;
    swaps = 0;
}

int main(){
    compares = 0;
    swaps = 0;
    srand(time(0));

    int arr1[] = {4, 3, 5, 1, 0, 2};
    int arr2[] = {3, 3, 2, 1, 1, 4};
    int arr3[] = {0, 1, 2, 3, 4, 5};
    int arr4[] = {5, 4, 3, 2, 1, 0};
    int arr5[] = {3, 3, 3, 3, 3, 3};
    
    int arr6[big_s];
    for(int i = 0; i < big_s; i++){
        int taken = 0;
        for(;;){
            int x = rand();
            //check for duplicates
            for(int y = 0; y < i; y++){
                if(arr6[y] == x){
                    taken = 1;
                }
            }
            if(taken != 1){
                arr6[i] = x;
                break;
            }
        }
    }
    
    int arr7[big_s];
    for(int i = 0; i < big_s; i++){
        arr7[i] = rand();
    }

    int arr8[big_s];
    for(int i = 0; i < big_s; i++){
        arr8[i] = i;
    }

    int arr9[big_s];
    for(int i = 0; i < big_s; i++){
        arr9[i] = 10000 - i;
    }

    int arr10[big_s];
    for(int i = 0; i < big_s; i++){
        arr10[i] = 3;
    }

    int s = sizeof(arr1)/sizeof(arr1[0]); //amount of array elements = size/size of single elemtent
    quicksort1(arr1, 0, s - 1);
    printArray(arr1, s);
    quicksort1(arr2, 0, s - 1);
    printArray(arr2, s);
    quicksort1(arr3, 0, s - 1);
    printArray(arr3, s);
    quicksort1(arr4, 0, s - 1);
    printArray(arr4, s);
    quicksort1(arr5, 0, s - 1);
    printArray(arr5, s);
    
    printf("\n\nTests w/ 10000 elements\n\n");
    printf("Unique Random Values\n");
    quicksort1(arr6, 0, big_s - 1);
    printStats();
    printf("Random Values\n");
    quicksort1(arr7, 0, big_s - 1);
    printStats();
    printf("Ascending Sorted List\n");
    quicksort1(arr8, 0, big_s - 1);
    printStats();
    printf("Descending Sorted List\n");
    quicksort1(arr9, 0, big_s - 1);
    printStats();
    printf("Uniform List\n");
    quicksort1(arr10, 0, big_s - 1);
    printStats();

    return 0;
}