//Andrew Webb 17/12/20
//Program showing Breadth-First and Depth-First Search of a Graph using an Adjacency List
#ifdef  _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif //  _MSC_VER
#include <stdlib.h>
#include <stdio.h>

char queue[6];
int ind;

typedef struct listnode{
    char data;
    listnode* next;
}listnode;

typedef struct list{
    listnode* head;
}list;

typedef struct graph{
    int NodeAmount;
    list* listArray;
    int* visitedArray;
}graph;

int caps2int(char x){
    //converts A-Z to int values 
    //ie A = 0, B = 1, etc.
    int y = (int)(x) - 65;
    return y;
}

graph* makeGraph(int N){
    graph* G = (graph*)malloc(sizeof(graph));
    G->NodeAmount = N;
    G->listArray = (list*)malloc(N*sizeof(list));
    G->visitedArray = (int*)malloc(N*sizeof(int));

    for(int i = 0; i < N; i++){
        G->listArray[i].head = NULL;
        G->visitedArray[i] = 0;
    }
    return G;
}

void addEdge(graph* G, char start, char end){
    int s = caps2int(start);  //s is used to get to get list number i.e A=0, B=1, etc.
    listnode* newnode = (listnode*)malloc(sizeof(listnode));
    newnode->next = NULL;
    newnode->data = end;

    listnode* temp = G->listArray[s].head;
    if(temp == NULL){G->listArray[s].head = newnode;}
    else{
        while(temp->next != NULL){temp = temp->next;}
        temp->next = newnode;
    }
}

void DFS(graph* G, char start){
    int num = caps2int(start);
    G->visitedArray[num] = 1; //mark as visited
    printf("%c", start);
    
    listnode* temp = G->listArray[num].head;
    while(temp!=NULL){
        int x = caps2int(temp->data);
        if(G->visitedArray[x] == 0){DFS(G, temp->data);}//go to adjacent if no visited
        temp = temp->next; //look at next adjacent
    }

}  

void BFS(graph* G, char start){
    int num = caps2int(start);
    G->visitedArray[num] = 2; //mark as visited, 2:visited, 1:printed, 0:neither 
    int initial = ind;

    if(ind == 0){ 
    queue[ind] = start;
    ind++;
    }


    listnode* temp = G->listArray[num].head;
    while(temp!=NULL){ //go through adjacents of start
        if(G->visitedArray[caps2int(temp->data)] == 0){ //unvisited and not printed
            queue[ind] = temp->data; //add to queue
            ind++;
            G->visitedArray[caps2int(temp->data)] = 1; //mark as printed
        }
        temp= temp->next;
    }

    for(int i = initial; i < ind; i++){printf("%c", queue[i]);} //print queue from start to last adjancent of start

    temp = G->listArray[num].head;
    while(temp!=NULL){
        if(G->visitedArray[caps2int(temp->data)] != 2){BFS(G, temp->data);} //call BFS for unvisited 
        temp = temp->next;
    }
    
}

void resetVisited(graph* G){
    for(int i = 0; i < G->NodeAmount; i++){
        G->visitedArray[i] = 0;
    }
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
    }

    free(G->listArray);
    free(G);
}

int main(){
    int N = 6;
    ind = 0;
    graph* G = makeGraph(N);
    addEdge(G, 'A', 'B');
    addEdge(G, 'A', 'D');
    addEdge(G, 'A', 'E');
    addEdge(G, 'B', 'C');
    addEdge(G, 'B', 'D');
    addEdge(G, 'C', 'B');
    addEdge(G, 'C', 'D');
    addEdge(G, 'D', 'F');
    addEdge(G, 'E', 'D');
    addEdge(G, 'F', 'C');
    
    printf("DFS: ");
    DFS(G, 'A');
    
    printf("\nBFS: ");
    resetVisited(G);
    
    BFS(G, 'A');
    printf("\n");

    free(G);
    return 0;
}