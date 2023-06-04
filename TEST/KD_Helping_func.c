#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "../include/particle.h"
#include "../include/KD_Helping_func.h"

kdtree createNode(Particle point){
	kdtree kd;
	kd  = (kdtree)malloc(sizeof(kdnode));
	kd->ball.x = point.x;
	kd->ball.y = point.y;
	kd->ball.mass = point.mass;
	kd->ball.radius = point.radius;
	kd->ball.vx = point.vx;
	kd->ball.vy = point.vy;
	kd->left = NULL;	
	kd->right = NULL;
	return kd;
}

int comparePoints(Particle p1, Particle p2, int level) {
    if (level % 2 == 0) {
        if (p1.x < p2.x)
            return -1;
        else if (p1.x > p2.x)
            return 1;
        else
            return 0;
    } else {
        if (p1.y < p2.y)
            return -1;
        else if (p1.y > p2.y)
            return 1;
        else
            return 0;
    }
}

void swap(Particle *a, Particle *b) {
  Particle t = *a;
  *a = *b;
  *b = t;
}

int partition_xy(Particle array[], int low, int high , int*flag) {
  	int pivot;
 	if(*flag) pivot = array[high].y;
 	else pivot = array[high].x;

	int i = (low - 1);

  	for (int j = low; j < high; j++) {
    		if(*flag){
			if (array[j].y <= pivot) {
     	 			i++;
     				swap(&array[i], &array[j]);
    			}
    		}	
		else{
    			if (array[j].x <= pivot) {
     	 			i++;
     				swap(&array[i], &array[j]);
    			}
		}
  	}	

  	swap(&array[i + 1], &array[high]);
  
  	return (i + 1);
}

void quickSort_xy(Particle array[], int low, int high, int*flag) {
  if (low < high) {

    int pi = partition_xy(array, low, high,flag);
    
    quickSort_xy(array, low, pi - 1,flag);
   
    quickSort_xy(array, pi + 1, high,flag);
  }
}

double calculateDistance(Particle p1,Particle p2){
	return sqrt( pow( (p1.x - p2.x), 2)  + pow( (p1.y - p2.y),2 ) );
}
