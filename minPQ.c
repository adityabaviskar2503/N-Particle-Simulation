#include <stdio.h>
#include <stdlib.h>
#include"event.h"
#include"minPQ.h"

minPQ* createPriorityQueue(int initialCapacity) {
    	minPQ* queue = (minPQ*)malloc(sizeof(minPQ));
    	queue->events = (Event**)malloc(initialCapacity * sizeof(Event*));
    	queue->capacity = initialCapacity;
   	queue->size = 0;
	return queue;
}

void destroyPriorityQueue(minPQ* queue) {
    	free(queue->events);
    	free(queue);
}

void swap(Event** event1, Event** event2) {
    	Event* temp = *event1;
    	*event1 = *event2;
    	*event2 = temp;
}

void enqueue(minPQ* queue, Event* event) {
	if (queue->size == queue->capacity) {
		// Double the capacity of the queue
		int newCapacity = queue->capacity * 2;
		Event** newEvents = (Event**)realloc(queue->events, newCapacity * sizeof(Event*));
		
		if (newEvents == NULL) {
		    	printf("Failed to resize the priority queue.\n");
		    	return;
		}
	
		queue->events = newEvents;
		queue->capacity = newCapacity;
	}
	
	int currentIndex = queue->size;
	int parentIndex = (currentIndex - 1) / 2;
	
	// Percolate up
	while (currentIndex > 0 && event->time < queue->events[parentIndex]->time) {
		swap(&queue->events[currentIndex], &queue->events[parentIndex]);
	    	currentIndex = parentIndex;
	    	parentIndex = (currentIndex - 1) / 2;
	}
	
	queue->events[currentIndex] = event;
	queue->size++;
}

Event* dequeue(minPQ* queue) {
	if (queue->size == 0) {
       		printf("Priority queue is empty. Cannot dequeue.\n");
          	return NULL;
	}

	Event* dequeuedEvent = queue->events[0];
	queue->size--;
	
	// Move the last event to the root
	queue->events[0] = queue->events[queue->size];
	
	int currentIndex = 0;
	int leftChildIndex = 2 * currentIndex + 1;
	int rightChildIndex = 2 * currentIndex + 2;
	
	// Percolate down
	while (leftChildIndex < queue->size) {
		int minIndex = currentIndex;
		
		if (queue->events[leftChildIndex]->time < queue->events[minIndex]->time)
			minIndex = leftChildIndex;
		
		if (rightChildIndex < queue->size && queue->events[rightChildIndex]->time < queue->events[minIndex]->time)
		    	minIndex = rightChildIndex;
		
		if (minIndex == currentIndex)
		    	break;
		
		swap(&queue->events[currentIndex], &queue->events[minIndex]);
		
		currentIndex = minIndex;
		leftChildIndex = 2 * currentIndex + 1;
		rightChildIndex = 2 * currentIndex + 2;
	}
	
	return dequeuedEvent;
}

//int main() {
//	minPQ* eventQueue = createPriorityQueue(10);
//	
//	// Create some sample events
//	Event* event1 = (Event*)malloc(sizeof(Event));
//	event1->time = 1.5;
//	enqueue(eventQueue, event1);
//	
//	Event* event2 = (Event*)malloc(sizeof(Event));
//	event2->time = 0.5;
//	enqueue(eventQueue, event2);
//	
//	Event* event3 = (Event*)malloc(sizeof(Event));
//	event3->time = 10;
//	enqueue(eventQueue, event3);
//	
//	Event* event4 = (Event*)malloc(sizeof(Event));
//	event4->time = 1.5;
//	enqueue(eventQueue, event4);
//	
//	Event* event5 = (Event*)malloc(sizeof(Event));
//	event5->time = 5.5;
//	enqueue(eventQueue, event5);
//	
//	Event* event6 = (Event*)malloc(sizeof(Event));
//	event6->time = 2.5;
//	enqueue(eventQueue, event6);
//	
//	// Access events in the priority queue
//	while (eventQueue->size > 0) {
//	    	Event* dequeuedEvent = dequeue(eventQueue);
//	    	// Process the event
//	    	printf("Event time: %f\n", dequeuedEvent->time);
//	    	// Free memory allocated for the event
//	    	free(dequeuedEvent);
//	}
//	
//	destroyPriorityQueue(eventQueue);
//	
//	return 0;
//}

