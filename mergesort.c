//Andrew Webb 18326385 5/11/20
//Program demonstrating mergesort in effect
#ifdef  _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif //  _MSC_VER
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define big_s 10000

int compares;

void merge(int array[], int low, int mid, int high){
    int index1, index2, index3; //indexs for left, right and combined arrays
    int size1 = mid - low + 1;  //left array size
    int size2 = high - mid;     //right array size

    int l_arr[size1];
    int r_arr[size2];
    
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
        if(l_arr[index1] <= r_arr[index2]){
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
    
}

void mergesort(int A[], int p, int r){
    if(p < r){
        int q = (p+r)/2;
        mergesort(A, p, q);
        mergesort(A, q + 1, r);
        merge(A, p, q, r);
    }
}

void printArray(int A[], int size){
   for(int i = 0; i < size; i++){
        printf("%d ", A[i]);
    } 
    printf("    Comparsions: %10d\n", compares);
    compares = 0;
    
}

void printStats(){
    printf("Comparsions: %10d\n\n", compares);
    compares = 0;
    
}


int main(){
    compares = 0;

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
    mergesort(arr1, 0, s - 1);
    printArray(arr1, s);
    mergesort(arr2, 0, s - 1);
    printArray(arr2, s);
    mergesort(arr3, 0, s - 1);
    printArray(arr3, s);
    mergesort(arr4, 0, s - 1);
    printArray(arr4, s);
    mergesort(arr5, 0, s - 1);
    printArray(arr5, s);

    printf("\n\nTests w/ 10000 elements\n\n");
    printf("Unique Random Values\n");
    mergesort(arr6, 0, big_s - 1);
    printStats();
    printf("Random Values\n");
    mergesort(arr7, 0, big_s - 1);
    printStats();
    printf("Ascending Sorted List\n");
    mergesort(arr8, 0, big_s - 1);
    printStats();
    printf("Descending Sorted List\n");
    mergesort(arr9, 0, big_s - 1);
    printStats();
    printf("Uniform List\n");
    mergesort(arr10, 0, big_s - 1);
    printStats();
    return 0;
}
