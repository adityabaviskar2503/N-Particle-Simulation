#ifndef KD_TREE_H_
#define KD_TREE_H_

#include "particle.h"

typedef struct kdnode{
	Particle ball;
  	struct kdnode *left , *right;
}kdnode;

typedef kdnode* kdtree;

#endif

kdtree createNode(Particle point);

int comparePoints(Particle p1, Particle p2, int level);

void swap(Particle *a, Particle *b);

int partition_xy(Particle array[], int low, int high , int*flag);

void quickSort_xy(Particle array[], int low, int high, int*flag);

double calculateDistance(Particle p1,Particle p2);
 
