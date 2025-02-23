#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_FLIGHTS 10
#define MAX_PASSENGERS 100

// Structure for Flight
typedef struct Flight {
    char flightID[10];
    char from[20];
    char to[20];
    int cost;
} Flight;

// Graph structure for routes
typedef struct Graph {
    int vertices;
    int adjMatrix[MAX_FLIGHTS][MAX_FLIGHTS];
    Flight flights[MAX_FLIGHTS];
} Graph;

// Min Heap Node for Priority Booking
typedef struct MinHeapNode {
    int ticketID;
    char passengerName[30];
    int priority;
} MinHeapNode;

// Min Heap for Priority Queue Booking
typedef struct MinHeap {
    int size;
    MinHeapNode* array[MAX_PASSENGERS];
} MinHeap;

// Passenger Linked List
typedef struct Passenger {
    int ticketID;
    char name[30];
    struct Passenger* next;
} Passenger;

Passenger* head = NULL;

// Circular Queue for Boarding Management
typedef struct BoardingQueue {
    int front, rear, size;
    int capacity;
    int array[MAX_PASSENGERS];
} BoardingQueue;

// Create Boarding Queue
BoardingQueue* createQueue(int capacity) {
    BoardingQueue* queue = (BoardingQueue*)malloc(sizeof(BoardingQueue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    return queue;
}

// Check if Queue is Full
int isFull(BoardingQueue* queue) {
    return (queue->size == queue->capacity);
}

// Check if Queue is Empty
int isEmpty(BoardingQueue* queue) {
    return (queue->size == 0);
}

// Enqueue Passenger for Boarding
void enqueue(BoardingQueue* queue, int item) {
    if (isFull(queue)) {
        printf("Boarding queue is full!\n");
        return;
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size++;
}

// Dequeue Passenger from Boarding Queue
int dequeue(BoardingQueue* queue) {
    if (isEmpty(queue))
        return -1;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return item;
}

// Add Passenger to Linked List
void addPassenger(int ticketID, char name[]) {
    Passenger* newPassenger = (Passenger*)malloc(sizeof(Passenger));
    newPassenger->ticketID = ticketID;
    strcpy(newPassenger->name, name);
    newPassenger->next = head;
    head = newPassenger;
}

// Display Passengers
void displayPassengers() {
    Passenger* temp = head;
    printf("Passenger List:\n");
    while (temp) {
        printf("Ticket ID: %d, Name: %s\n", temp->ticketID, temp->name);
        temp = temp->next;
    }
}

// Min Heap Functions for Priority Booking
MinHeap* createMinHeap() {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->size = 0;
    return heap;
}

void insertMinHeap(MinHeap* heap, int ticketID, char* passengerName, int priority) {
    MinHeapNode* newNode = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    newNode->ticketID = ticketID;
    strcpy(newNode->passengerName, passengerName);
    newNode->priority = priority;
    heap->array[heap->size++] = newNode;
}

// Graph Initialization for Flight Routes
Graph* createGraph(int vertices) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->vertices = vertices;
    for (int i = 0; i < vertices; i++)
        for (int j = 0; j < vertices; j++)
            graph->adjMatrix[i][j] = INT_MAX;
    return graph;
}

// Add Flight Route to Graph
void addFlight(Graph* graph, int src, int dest, int cost, char flightID[], char from[], char to[]) {
    graph->adjMatrix[src][dest] = cost;
    strcpy(graph->flights[src].flightID, flightID);
    strcpy(graph->flights[src].from, from);
    strcpy(graph->flights[src].to, to);
    graph->flights[src].cost = cost;
}

// Dijkstra's Algorithm for Shortest Path
void dijkstra(Graph* graph, int src) {
    int dist[MAX_FLIGHTS];
    int visited[MAX_FLIGHTS] = {0};
    for (int i = 0; i < graph->vertices; i++)
        dist[i] = INT_MAX;
    dist[src] = 0;

    for (int count = 0; count < graph->vertices - 1; count++) {
        int min = INT_MAX, minIndex;
        for (int v = 0; v < graph->vertices; v++)
            if (!visited[v] && dist[v] <= min)
                min = dist[v], minIndex = v;
        visited[minIndex] = 1;

        for (int v = 0; v < graph->vertices; v++)
            if (!visited[v] && graph->adjMatrix[minIndex][v] && dist[minIndex] != INT_MAX && dist[minIndex] + graph->adjMatrix[minIndex][v] < dist[v])
                dist[v] = dist[minIndex] + graph->adjMatrix[minIndex][v];
    }

    printf("Shortest Routes from Source:\n");
    for (int i = 0; i < graph->vertices; i++)
        printf("Flight %s: Cost %d\n", graph->flights[i].flightID, dist[i]);
}

// Main Function
int main() {
    Graph* graph = createGraph(MAX_FLIGHTS);
    addFlight(graph, 0, 1, 300, "AI101", "NYC", "LAX");
    addFlight(graph, 1, 2, 400, "BA202", "LAX", "SFO");
    
    MinHeap* bookingHeap = createMinHeap();
    insertMinHeap(bookingHeap, 1, "John Doe", 2);
    insertMinHeap(bookingHeap, 2, "Alice Smith", 1);

    addPassenger(101, "John Doe");
    addPassenger(102, "Alice Smith");

    BoardingQueue* boardingQueue = createQueue(5);
    enqueue(boardingQueue, 101);
    enqueue(boardingQueue, 102);

    printf("\n--- Flight Booking System ---\n");
    dijkstra(graph, 0);
    displayPassengers();
    
    return 0;
}
