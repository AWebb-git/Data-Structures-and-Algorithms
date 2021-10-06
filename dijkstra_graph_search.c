//Andrew Webb 17/12/20
//Program using Dijkstra's algorithm to find shortest route between two entered bus stops 
#ifdef  _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif //  _MSC_VER
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#define MAX_STRING_SIZE 100

int firstNode, endLine, colNum, rowNum, rec1done, prevPermanent;

typedef struct listnode{
    int data;
    int weight;
    struct listnode* next;
}listnode;

typedef struct list{
    listnode* head;
}list;

typedef struct graph{
    int NodeAmount;
    list* listArray;
    int* permanentArray; //used for storing vertice permanent status
    int* prevArray; //used for finding path
    int* distArray; //storing distances
    int* prevParray; //used for storing previous node made permanent
    char** nameArray;   
    double* latArray;
    double* longArray;
}graph;


graph* makeGraph(int N){
    graph* G = (graph*)malloc(sizeof(graph));
    G->NodeAmount = N;
    G->listArray = (list*)malloc(N*sizeof(list));
    G->permanentArray = (int*)malloc(N*sizeof(int));
    G->prevArray = (int*)malloc(N*sizeof(int));
    G->distArray = (int*)malloc(N*sizeof(int));
    G->prevParray = (int*)malloc(N*sizeof(int));
    G->nameArray = (char**)malloc(N*sizeof(char*));
    G->latArray = (double*)malloc(N*sizeof(double));
    G->longArray = (double*)malloc(N*sizeof(double));

    for(int i = 0; i < N; i++){
        G->listArray[i].head = NULL;
        G->permanentArray[i] = 0;
        G->prevArray[i] = -1;
        G->distArray[i] = INT_MAX;
        G->nameArray[i] = (char*)malloc(MAX_STRING_SIZE*sizeof(char));
        strcpy(G->nameArray[i], "");
        G->latArray[i] = 0;
        G->longArray[i] = 0;
        G->prevParray[i] = -1;
    }
    return G;
}


void addEdge(graph* G, int start, int end, int w){
    listnode* newnode = (listnode*)malloc(sizeof(listnode));
    newnode->next = NULL;
    newnode->data = end;
    newnode->weight = w;

    listnode* oldnode = (listnode*)malloc(sizeof(listnode));
    oldnode->next = NULL;
    oldnode->data = start;
    oldnode->weight = w;

    //undirected graph so add nodes to lists for both vertices
    listnode* temp = G->listArray[start].head;
    if(temp == NULL){G->listArray[start].head = newnode;}
    else{
        while(temp->next != NULL){temp = temp->next;}
        temp->next = newnode;
    }

    temp = G->listArray[end].head;
    if(temp == NULL){G->listArray[end].head = oldnode;}
    else{
        while(temp->next != NULL){temp = temp->next;}
        temp->next = oldnode;
    }
}

listnode* getValidAdj(int src, graph* G){
    listnode* temp = G->listArray[src].head;

    //loop until null is reached or non-permanent adjacent is found
    while(temp != NULL){ 
        if(G->permanentArray[temp->data] != 1){break;}
        temp = temp->next;
    }

    return temp;
}

listnode* getShortestAdj(int a, graph* G){
    
    listnode* shortest = G->listArray[a].head;
    
    //checking to see if default shortest (head) is valid (non-permanant)
    if(G->permanentArray[shortest->data] == 1){
        //if invalid go to next valid in list if available
        shortest = getValidAdj(a, G);
    }

    listnode* temp = shortest;
    //if shortest isn't already null, no need to check it against itself
    if(shortest != NULL){temp = shortest->next;}

    while(temp != NULL){
        if((G->distArray[temp->data] < G->distArray[shortest->data]) && G->permanentArray[temp->data] != 1){
            shortest = temp;
        }
        temp = temp->next;
    }

    return shortest; 
}

listnode* getShortest(int a, graph* G){
    listnode* shortest = getShortestAdj(a, G);
    
    //check if shortest is null
    if(shortest == NULL){
        //go to previous if available
        if(G->prevArray[a] != -1){
            a = G->prevParray[a];
            shortest = getShortestAdj(a, G);
        }
    }

    a = G->prevParray[a];
    
    while(a != -1){
        listnode* potential = getShortestAdj(a, G);
        if(potential != NULL){
            if(shortest == NULL){shortest = potential;}
            else if((G->distArray[shortest->data] > G->distArray[potential->data]) && G->permanentArray[potential->data]!= 1){
            shortest = potential;
            }
        }
        a = G->prevParray[a];
    }

    return shortest;
}

void updateAdj(int src, graph* G){
    int dest;

    listnode* temp = G->listArray[src].head;
    
    while(temp != NULL){
        dest = temp->data;
        if((G->distArray[src] + temp->weight) < G->distArray[dest]){
            G->prevArray[dest] = src;
            G->distArray[dest] = G->distArray[src] + temp->weight;
        }
        temp = temp->next;
    }
}

void dijkstra(int src, graph* G){

    //if first vertice set distance to 0
    if(firstNode == 0){
        G->distArray[src] = 0;
    }

    firstNode++;

    G->permanentArray[src] = 1;
    G->prevParray[src] = prevPermanent;
    prevPermanent = src;

    updateAdj(src, G);
    
    //choosing next
    listnode* next = getShortest(src, G);
    //check that it's not null (if it is there are no non-permanent adjacents)
    if(next != NULL){
    //if shortest is non-permanent go to it
        if(G->permanentArray[next->data] != 1){
            dijkstra(next->data, G);
        }
    }
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

void readVerts(graph* G, FILE* file){
endLine = 0;
rec1done = 0;
colNum = 0;
int index;
char fieldBuffer[MAX_STRING_SIZE];
int maxSize = sizeof(fieldBuffer);

if (file == NULL) {
		printf("no file opened\n");
	}
	else {
		
		//iterate by record
		while (!feof(file)) {
			//iterate by field
			while (endLine != 1){
				endLine = next_field(file, fieldBuffer, maxSize);
				//if past the first record pass values to temp
				if(rec1done == 1){
					switch (colNum){
					case 0:
						index = atoi(fieldBuffer) - 1;
						colNum++;
						break;
					case 1:
                        strcpy(G->nameArray[index], fieldBuffer);
						colNum++;
						break;
					case 2:
						G->latArray[index] = atof(fieldBuffer);
						colNum++;
						break;
					case 3:
						G->longArray[index] = atof(fieldBuffer);
						colNum = 0;
						break;
					default:
						printf("Error at column: %d", colNum);
						break;
					}
				}
			}
			
			rec1done = 1;
			endLine = 0;
		}
    }
}

void readEdges(graph* G, FILE* file){
endLine = 0;
rec1done = 0;
colNum = 0;
int x, y, z;
char fieldBuffer[MAX_STRING_SIZE];
int maxSize = sizeof(fieldBuffer);

if (file == NULL) {
		printf("no file opened\n");
	}
	else {
		
		//iterate by record
		while (!feof(file)) {
			//iterate by field
			while (endLine != 1){
				endLine = next_field(file, fieldBuffer, maxSize);
				//if past the first record pass values to temp
				if(rec1done == 1){
					switch (colNum){
					case 0:
						x = atoi(fieldBuffer) - 1;
						colNum++;
						break;
					case 1:
                        y = atoi(fieldBuffer) - 1;
						colNum++;
						break;
					case 2:
						z = atoi(fieldBuffer);
						colNum = 0;
						break;
					default:
						printf("Error at column: %d", colNum);
						break;
					}
				}
			}
			
			if(rec1done == 1){addEdge(G, x, y, z);}
			rec1done = 1;
			endLine = 0;
		}
    }
}

void printRoute(graph* G, int dest){
    int *stops = (int*)malloc(G->NodeAmount*sizeof(int));
    int index = 0;
    int a = dest;

    while (a!=-1){
        stops[index] = a;
        a = G->prevArray[a];
        index++;
    }

    index = index - 1;
    printf("%6s|%30s|%10s|%10s\n", "StopID", "Stop Name", "Latitude", "Longitude");
    while(index != -1){
        printf("%6d|%30s|%10lf|%10lf\n", (stops[index] + 1), G->nameArray[stops[index]], G->latArray[stops[index]], G->longArray[stops[index]]);
        index = index - 1;
    }
    free(stops);
}

void deleteList(listnode* head){
    listnode* curr = head;
    listnode* next;

    while(curr != NULL){
        next = curr->next;
        free(curr);
        curr = next;
    }

    head = NULL;
}

void deleteGraph(graph* G){
    for(int i = 0; i < G->NodeAmount; i++){
        deleteList(G->listArray[i].head);
        free(G->nameArray[i]);
    }

    free(G->permanentArray);
    free(G->prevParray);
    free(G->prevArray);
    free(G->distArray);
    free(G->latArray);
    free(G->longArray);

    free(G);
}

int main(){
    int N = 7667;
    firstNode = 0;
    prevPermanent = -1;
    graph* G = makeGraph(N);

    FILE* file;

	//open file
	file = fopen("vertices.csv", "r");
	
	readVerts(G, file);
    fclose(file);

    file = fopen("edges.csv", "r");
    readEdges(G, file);
    fclose(file);

    int src, dest;
    
    printf("Enter starting Stop ID: ");
    scanf("%d", &src);
    printf("\nEnter destination Stop ID: ");
    scanf("%d", &dest);
    printf("\n");
    dijkstra(src - 1, G);
    printRoute(G, dest - 1);
    
    deleteGraph(G);
    return 0;
}