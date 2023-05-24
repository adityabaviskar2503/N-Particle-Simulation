#include<stdio.h>
#include<stdlib.h>
#include "particle.h"
#define PART_SIZE 6
typedef struct kdnode{
	Particle ball;
  	struct kdnode *left , *right;
}kdnode;

typedef kdnode* kdtree;

kdtree createNode(Particle point){
	kdtree kd;
	kd  = (kdtree)malloc(sizeof(kdnode));
	kd->ball.x = point.x;
	kd->ball.y = point.y;
	kd->left = NULL;	
	kd->right = NULL;
	return kd;
}

int comparePoints(Particle p1, Particle p2, int level) {
    if (level % 2 == 0) {
        // Compare x-coordinates if level is even
        if (p1.x < p2.x)
            return -1;
        else if (p1.x > p2.x)
            return 1;
        else
            return 0;
    } else {
        // Compare y-coordinates if level is odd
        if (p1.y < p2.y)
            return -1;
        else if (p1.y > p2.y)
            return 1;
        else
            return 0;
    }
}

kdtree insertNode(kdtree root, Particle point, int level) {
    // If the tree is empty, create a new node and return it
    if (root == NULL)
        return createNode(point);
    
    // Calculate the current dimension
    int currentLevel = level % 2;
    
    // Compare the point with the current node's point and decide the direction to traverse
    if (comparePoints(point, root->ball, currentLevel) < 0)
        root->left = insertNode(root->left, point, level + 1);
    else
        root->right = insertNode(root->right, point, level + 1);
    
    return root;
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
    
    // find the pivot element such that
    // elements smaller than pivot are on left of pivot
    // elements greater than pivot are on right of pivot
    int pi = partition_xy(array, low, high,flag);
    
    // recursive call on the left of pivot
    quickSort_xy(array, low, pi - 1,flag);
    
    // recursive call on the right of pivot
    quickSort_xy(array, pi + 1, high,flag);
  }
}



// function to print array elements
void printArray(Particle array[], int size) {
  for (int i = 0; i < size; ++i) {
    printf(" x = %lf \t y = %lf \n", array[i].x,array[i].y);
  }
}


kdtree createKDTree(Particle points[],int start , int end , int level) {
	if (start > end)
        	return NULL;
    
  	 // Calculate the current dimension (level)
  	 int currentLevel = level % 2;
    
   	 // Sort the points array based on the current level
  	 if (currentLevel == 0) {
    		quickSort_xy(points,start,end,&currentLevel);
	 }
	 else {	
    		quickSort_xy(points,start,end,&currentLevel);
	} 		
    
   	 // Find the median index
   	 int medianIndex;
	 if((end - start) % 2) medianIndex = ((end-start)/2) + 1 + start;
	 else medianIndex = start +  (end-start)/2  ;
    
   	 // Create a new node with the median point
   	 kdtree root = createNode(points[medianIndex]);
    
   	 // Recursively construct the left and right subtrees
    	root->left = createKDTree(points, start, medianIndex - 1, level + 1);
    	root->right = createKDTree(points, medianIndex + 1, end, level + 1);
    
    return root;	   
}

void displayKDTree(kdtree root) {
    if (root == NULL)
        return;
    
    displayKDTree(root->left);
    printf("(%.2f, %.2f)\n", root->ball.x, root->ball.y);
    displayKDTree(root->right);
    return;
}

void Pre_order(kdtree root) {
    if (root == NULL)
        return;
    
    printf("(%.2f, %.2f)\n", root->ball.x, root->ball.y);
    displayKDTree(root->left);
    displayKDTree(root->right);
    return;
}

int main(){
	Particle p_arr[6];
	p_arr[0].x = 2;
	p_arr[0].y = 3;
	p_arr[1].x = 5;
	p_arr[1].y = 4;
	p_arr[2].x = 9;
	p_arr[2].y = 6;
	p_arr[3].x = 4;
	p_arr[3].y = 7;
	p_arr[4].x = 8;
	p_arr[4].y = 1;
	p_arr[5].x = 7;
	p_arr[5].y = 2;

	
	kdtree tree;
	tree = createKDTree(p_arr,0,5,0);
	
	displayKDTree(tree);
	printf("-----------------------------------------\n");	
	Pre_order(tree);

	return 0;
}

