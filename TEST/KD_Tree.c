#include<stdio.h>
#include<stdlib.h>
#include "../include/particle.h"
#include <math.h>
#include "../include/KD_Tree.h"
#include "../include/KD_Helping_func.h"

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

// function to print array elements
void printArray(Particle array[], int size) {
  for (int i = 0; i < size; ++i) {
    printf(" x = %lf \t y = %lf \n", array[i].x,array[i].y);
  }
}


kdtree createKDTree(Particle points[],int start , int end , int level) {
	if (start > end) return NULL;
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
	 else medianIndex = start +  (end-start)/2 ;
    
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
    printf("%.2f\t%.2f\n", root->ball.x, root->ball.y);
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

int collisionCheck(Particle p1,Particle p2){
	double dist = calculateDistance(p1,p2);
	if(dist <= (p1.radius + p2.radius)) return 1;
	return 0;
}

kdtree findNearestNeighbour(kdtree root,Particle target,kdtree* nearest, double* minDistance , int axis){
	if(!root) return NULL;

	double distance = calculateDistance(root->ball, target);

	if (distance == 0) {
        	return NULL;
    	}

	if( distance < *minDistance){
		*minDistance = distance;
		*nearest = root;
	}
	
	int nextAxis = (axis+1)%2;
	
	if(!axis){
		if(target.x < root->ball.x){
			findNearestNeighbour(root->left,target,nearest,minDistance,nextAxis);
			if(target.x + sqrt(*minDistance) >= root->ball.x){
				findNearestNeighbour(root->left,target,nearest,minDistance,nextAxis);
			}
		}
		else{
			findNearestNeighbour(root->left,target,nearest,minDistance,nextAxis);
			if(target.x - sqrt(*minDistance) <= root->ball.x){
				findNearestNeighbour(root->left,target,nearest,minDistance,nextAxis);
			}
		}
	}
	else{
		if(target.y < root->ball.y){
			findNearestNeighbour(root->left,target,nearest,minDistance,nextAxis);
			if(target.y + sqrt(*minDistance) >= root->ball.y){
				findNearestNeighbour(root->left,target,nearest,minDistance,nextAxis);
			}
		}
		else{
			findNearestNeighbour(root->left,target,nearest,minDistance,nextAxis);
			if(target.y - sqrt(*minDistance) <= root->ball.y){
				findNearestNeighbour(root->left,target,nearest,minDistance,nextAxis);
			}
		}

	}

}

int isInBoundingBox(Particle target, Particle minPoint, Particle maxPoint) {
    if (target.x >= minPoint.x && target.x <= maxPoint.x && target.y >= minPoint.y && target.y <= maxPoint.y) {
        return 1;
    }
    return 0;
}

// Function to find the minimum and maximum points of the bounding box for a given subtree
void findBoundingBox(kdtree root, Particle *minPoint, Particle *maxPoint) {
    if (root == NULL) {
        return;
    }

    if (root->ball.x < minPoint->x) {
        minPoint->x = root->ball.x;
    }
    if (root->ball.y < minPoint->y) {
        minPoint->y = root->ball.y;
    }

    if (root->ball.x > maxPoint->x) {
        maxPoint->x = root->ball.x;
    }
    if (root->ball.y > maxPoint->y) {
        maxPoint->y = root->ball.y;
    }

    findBoundingBox(root->left, minPoint, maxPoint);
    findBoundingBox(root->right, minPoint, maxPoint);
}

kdtree update_tree(kdtree root,Particle points[],int start , int end , int level) {
	if (start > end) return NULL;
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
	 else medianIndex = start +  (end-start)/2 ;
    
   	 // Create a new node with the median point
   	 root->ball = points[medianIndex]; 
    
   	 // Recursively construct the left and right subtrees
    	root->left = update_tree(root->left,points, start, medianIndex - 1, level + 1);
    	root->right = update_tree(root->right,points, medianIndex + 1, end, level + 1);
    
    return root;	   
}

kdtree nearest_neighbor_bb(kdtree root, Particle target ){
	kdtree nearest = NULL;
	double bestDistance = INFINITY;
	Particle minPoint,maxPoint;
	minPoint.x = INFINITY;
	minPoint.y = INFINITY;
	maxPoint.x = INFINITY;
	maxPoint.y = INFINITY;
	findBoundingBox(root, &minPoint, &maxPoint);
	if (isInBoundingBox(target, minPoint, maxPoint)) {
        // Find the nearest neighbor
        	findNearestNeighbour(root, target, &nearest, &bestDistance, 0); // Starting with axis 0
    	}
   	printf("%.2f\t%.2f", nearest->ball.x,nearest->ball.y);
	return nearest;
}



void destroyKDTree(kdtree root) {
    if (root == NULL)
        return;

    destroyKDTree(root->left);
    destroyKDTree(root->right);

    free(root);
}

int main(){
	Particle p_arr[5];
	p_arr[0].x = 2.5;
	p_arr[0].y = 3.5;
	p_arr[1].x = 1.2;
	p_arr[1].y = 4.7;
	p_arr[2].x = 3.9;
	p_arr[2].y = 2.1;
	p_arr[3].x = 5.5;
	p_arr[3].y = 1.8;
	p_arr[4].x = 4.1;
	p_arr[4].y = 5.2;

		
	kdtree tree;
	tree = createKDTree(p_arr,0,4,0);	
	displayKDTree(tree);
	printf("\n----------------\n");
//	printf("x:%.2f , y:%.2f\n", p_arr[0].x,p_arr[0].y);
//	nearest_neighbor_bb(tree,p_arr[0]);
	p_arr[0].x = 2.2;
	p_arr[0].y = 3.9;
	p_arr[1].x = 3.2;
	p_arr[1].y = 1.9;
	p_arr[2].x = 5.2;
	p_arr[2].y = 3.9;
	p_arr[3].x = 4.2;
	p_arr[3].y = 2.9;
	p_arr[4].x = 4.2;
	p_arr[4].y = 5.9;
	
	tree = update_tree(tree,p_arr,0,4,0);
	displayKDTree(tree);
	
	return 0;
}

