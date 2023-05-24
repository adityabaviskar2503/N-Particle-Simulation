#ifndef KD_TREE_H_
#define KD_TREE_H_

typedef struct kdnode{
	Particle ball;
  	struct kdnode *left , *right;
}kdnode;


kdtree insertNode(kdtree root, Particle point, int level);

kdtree createKDTree(Particle points[],int start , int end , int level);


