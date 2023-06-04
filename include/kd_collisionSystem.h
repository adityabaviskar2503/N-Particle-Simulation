#ifndef PQ_COL_SYS
#define PQ_COL_SYS

#include "particle.h"
#include "randomParticleGenerator.h"
#include "KD_Helping_func.h"

typedef kdnode* kdtree;

typedef struct kd_CollisionSystem{
	kdtree tree;
    	particleSystem* sys;
}KD_CollisionSystem;


#endif

kdtree createKDTree(Particle points[],int start , int end , int level);

void displayKDTree(kdtree root);

void Pre_order(kdtree root);

void printArray(Particle array[], int size);

int collisionCheck_P2P(Particle p1,Particle p2);

kdtree update_tree(kdtree root,Particle points[],int start , int end , int level);

void destroyKDTree(kdtree root);

kdtree findNN(kdtree root,Particle* particle,  int depth) ;

void handle_collision(Particle* p1, Particle* p2);
void rebalanceKDTree(kdtree root ,int numParticles);
