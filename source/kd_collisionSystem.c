#include<stdlib.h>
#include<stdio.h>
#include"../include/kd_collisionSystem.h"
#include<math.h>
#include<time.h>
#include "../include/KD_Helping_func.h"
#include "../include/particle.h"

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

	kdtree root;
	root = NULL;	
	// Create a new node with the median point
	root = createNode(points[medianIndex]);

	// Recursively construct the left and right subtrees
	root->left = createKDTree(points, start, medianIndex - 1, level + 1);
	root->right = createKDTree(points, medianIndex + 1, end, level + 1);

	return root;	   
}

void displayKDTree(kdtree root) {
	if (root == NULL)
		return;

	displayKDTree(root->left);
	printf("%.2f\t%.2f\t%.2f\n", root->ball.x, root->ball.y,root->ball.color.r);
	displayKDTree(root->right);
	return;
}

void Pre_order(kdtree root) {
	if (root == NULL)
		return;

	printf("(%.2f, %.2f)\n", root->ball.x, root->ball.y);
	Pre_order(root->left);
	Pre_order(root->right);
	return;
}

int collisionCheck_P2P(Particle p1,Particle p2){
	double dist = sqrt(calculateDistance(p1,p2));
	if(dist <= (p1.radius + p2.radius)) return 1;
	return 0;
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

kdtree closest(kdtree node1, kdtree node2, Particle* particle){
	if( node1 == NULL) return node2;
	
	if( node2 == NULL) return node1;

	double d1 = calculateDistance(node1->ball,*particle);
	
	double d2 = calculateDistance(node2->ball,*particle);

	if (d1 == 0) return node2;

	if (d2 == 0) return node1;


	if( d1 < d2) return node1;
	else return node2;
}

// Function to find the nearest neighbor in the KD-tree
kdtree findNN(kdtree root,Particle* particle, int depth) {
	if(root == NULL) return NULL;
	kdtree temp = NULL;
	kdtree best = NULL;
	kdtree nextsubtree = NULL;
	kdtree othersubtree = NULL;
	int axis = depth%2;
	if(axis == 0){
		if(particle->x < root->ball.x){
			nextsubtree = root->left;
			othersubtree = root->right;
		}
		else{
			nextsubtree = root->right;
			othersubtree = root->left;
		}
	}
	else{
		if(particle->y < root->ball.y){
			nextsubtree = root->left;
			othersubtree = root->right;
		}
		else{
			nextsubtree = root->right;
			othersubtree = root->left;
		}
	}

	temp = findNN(nextsubtree,particle,depth+1);
	best = closest(temp,root,particle);

	double radius_square = calculateDistance(*particle,best->ball);	

	double dist;
	if (axis == 0) {
		dist = fabs(root->ball.x - particle->x);
	} else {
		dist= fabs(root->ball.y - particle->y);
	}

	if( radius_square >= dist * dist){
		temp = findNN(othersubtree,particle,depth+1);
		best = closest(temp,best,particle);
	}

	return best;

}
	


void destroyKDTree(kdtree root) {
	if (root == NULL)
		return;

	destroyKDTree(root->left);
	destroyKDTree(root->right);

	free(root);
}


void handle_collision(Particle* p1, Particle* p2){
    double dx = p1->x - p2->x; 
    double dy = p1->y - p2->y; 
    double distance = sqrt(calculateDistance(*p1, *p2));
    double collision_normal_x = dx / distance;
    double collision_normal_y = dy / distance;
    double vx_rel = p2->vx - p1->vx;
    double vy_rel = p2->vy - p1->vy;
    double impact_vel = vx_rel * collision_normal_x + vy_rel * collision_normal_y;
    if(impact_vel > 0){
         double M = p2->mass + p1->mass;
         //double impulse = (2 * impact_vel) / (mass_ratio + 1);
         p1->vx += (2 * p2->mass / M) * impact_vel * collision_normal_x ;
         p1->vy += (2 * p2->mass / M) * impact_vel * collision_normal_y ;
         p2->vx -= (2 * p1->mass / M) * impact_vel * collision_normal_x ;
         p2->vy -= (2 * p1->mass / M) * impact_vel * collision_normal_y ;
    }
}
     
void collectPointsFromKDTree(kdtree  node, Particle* collectedPoints, int* index) {
    if (node == NULL) {
        return;
    }

    // Traverse left subtree
    collectPointsFromKDTree(node->left, collectedPoints, index);

    // Add current point to the collected points array
    collectedPoints[*index] = node->ball;
    (*index)++;

    // Traverse right subtree
    collectPointsFromKDTree(node->right, collectedPoints, index);
}
void rebalanceKDTree(kdtree root ,int numParticles) {
    // Step 1: Traverse the KD tree and collect points in an array
    Particle* collectedPoints = (Particle*)malloc(sizeof(Particle) * numParticles);
    int index = 0;
    collectPointsFromKDTree(root, collectedPoints, &index);

    root = update_tree(root,collectedPoints, 0,numParticles-1,0);

    // Free the collected points array
    free(collectedPoints);
}


/*
int main(){
	Particle p_arr[6];
	p_arr[0].x = 5;
	p_arr[0].y = 4;
	p_arr[1].x = 2;
	p_arr[1].y = 6;
	p_arr[2].x = 13;
	p_arr[2].y = 3;
	p_arr[3].x = 3;
	p_arr[3].y = 1;
	p_arr[4].x = 10;
	p_arr[4].y = 2;
	p_arr[5].x = 8;
	p_arr[5].y = 7;


	kdtree tree;
	kdtree nearest;
	tree = createKDTree(p_arr,0,5,0);	
	displayKDTree(tree);
	printf("\n----------------\n");
	printf("x:%.2f , y:%.2f\n", p_arr[0].x,p_arr[0].y);
	nearest = findNN(tree,&p_arr[0],0);
	printf("x:%.2f , y:%.2f\n",nearest->ball.x,nearest->ball.y);
	Pre_order(tree);

	return 0;
}
*/
