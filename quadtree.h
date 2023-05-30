#ifndef QUADTREE_H_
#define QUADTREE_H_

#include <stdbool.h>
#include <math.h>
#include "particle.h"
#include "randomParticleGenerator.h"
#define MAX_PARTICLES 5

typedef struct quadtree_node{
    double x;
    double y;
    double width;
    double height;
    int particle_count;
    //int level;
    Particle* particles[MAX_PARTICLES];
    struct quadtree_node* children[4];
    // index: 0 = south-west, 1 = south-east, 2 = north-west, 3 = north-east
    //struct quadtree_node* neighbors[8];
}quadtree_node;

// Function to create a new quadtree node
quadtree_node* createquadtree_node(double x, double y, double width, double height) {
    quadtree_node* node = (quadtree_node*)malloc(sizeof(quadtree_node));
    node->x = x;
    node->y = y;
    node->width = width;
    node->height = height;
    //node->level = 0;
    node->particle_count = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {
        node->particles[i] = NULL;
    }
    for (int i = 0; i < 4; i++) {
        node->children[i] = NULL;
    }
    //for (int i = 0; i < 8; i++) {
    //    node->neighbors[i] = NULL;
    //}
    return node;
}

// Function to check if a node is a leaf node (has no children)
bool isLeafNode(quadtree_node** node) {
    return ((*node)->children[0] == NULL);
}

// clamp function
double clamp(double val, double minval, double maxval){
    return fmax(fmin(val, maxval), minval);
}

// Function to check if a point is cutting a region
// param in: center of point (x,y), center of region (x,y), radius of particle, width_region, height_region
bool isCuttingRegion(double Cx, double Cy, double Rx, double Ry, double radius, double width, double height){
    double dist = 0;
    double dx, dy;
    double x1 = Rx - width / 2;
    double y1 = Ry - height / 2;
    double x2 = Rx + width / 2;
    double y2 = Ry + height / 2;
    
    dx = clamp(Cx, x1, x2) - Cx;
    dy = clamp(Cy, y1, y2) - Cy;
    dist = sqrt(dx * dx + dy * dy);
    //printf("distance is %lf\n",distance);
    return (dist  <= radius);

}

void subdivideNode(quadtree_node* node) {
    double subWidth = node->width / 2.0;
    double subHeight = node->height / 2.0;
    //int level = node->level + 1;

    node->children[0] = createquadtree_node(node->x - subWidth / 2, node->y - subHeight / 2, subWidth, subHeight);
    node->children[1] = createquadtree_node(node->x + subWidth / 2, node->y - subHeight / 2, subWidth, subHeight);
    node->children[2] = createquadtree_node(node->x - subWidth / 2, node->y + subHeight / 2, subWidth, subHeight);
    node->children[3] = createquadtree_node(node->x + subWidth / 2, node->y + subHeight / 2, subWidth, subHeight);

}

void insertParticleQuadtree(quadtree_node** node, Particle* particle) {
    //if (!isLeafNode(&node)) {
    if ((*node)->children[0] != NULL) {
        // Find the child node that the particle belongs to and insert it there
        double subWidth = (*node)->width / 2.0;
        double subHeight = (*node)->height / 2.0;

        if(isCuttingRegion(particle->x, particle->y, (*node)->x - subWidth / 2, (*node)->y - subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[0], particle);

        if(isCuttingRegion(particle->x, particle->y, (*node)->x + subWidth / 2, (*node)->y - subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[1], particle);

        if(isCuttingRegion(particle->x, particle->y, (*node)->x - subWidth / 2, (*node)->y + subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[2], particle);

        if(isCuttingRegion(particle->x, particle->y, (*node)->x + subWidth / 2, (*node)->y + subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[3], particle);

        return;
    }

    // Add the particle to the node's particle array
    if ((*node)->particle_count < MAX_PARTICLES) {
        (*node)->particles[(*node)->particle_count++] = particle;
    } else {
        // Subdivide the node if it has reached the maximum number of particles
        subdivideNode(*node);

        // Reinsert the existing particles into the children nodes
        for (int i = 0; i < (*node)->particle_count; i++) {
            Particle* existingParticle = (*node)->particles[i];
            insertParticleQuadtree(node, existingParticle);
        }
        //
        // Clear the particle array in the current node
        for (int i = 0; i < MAX_PARTICLES; i++) {
            (*node)->particles[i] = NULL;
        }

        (*node)->particle_count = 0;

        // Insert the new particle into the appropriate child node
        // Find the child node that the particle belongs to and insert it there
        double subWidth = (*node)->width / 2.0;
        double subHeight = (*node)->height / 2.0;

        //        if(isCuttingRegion(particle->x, particle->y, node->x - subWidth / 2, node->y - subHeight / 2, particle->radius, subWidth, subHeight))
        //            insertParticleQuadtree(node->children[0], particle);
        //        
        //        if(isCuttingRegion(particle->x, particle->y, node->x + subWidth / 2, node->y - subHeight / 2, particle->radius, subWidth, subHeight))
        //            insertParticleQuadtree(node->children[1], particle);
        //
        //        if(isCuttingRegion(particle->x, particle->y, node->x - subWidth / 2, node->y + subHeight / 2, particle->radius, subWidth, subHeight))
        //            insertParticleQuadtree(node->children[2], particle);
        //
        //        if(isCuttingRegion(particle->x, particle->y, node->x + subWidth / 2, node->y + subHeight / 2, particle->radius, subWidth, subHeight))
        //            insertParticleQuadtree(node->children[3], particle);
        //
        if(isCuttingRegion(particle->x, particle->y, (*node)->x - subWidth / 2, (*node)->y - subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[0], particle);

        if(isCuttingRegion(particle->x, particle->y, (*node)->x + subWidth / 2, (*node)->y - subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[1], particle);

        if(isCuttingRegion(particle->x, particle->y, (*node)->x - subWidth / 2, (*node)->y + subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[2], particle);

        if(isCuttingRegion(particle->x, particle->y, (*node)->x + subWidth / 2, (*node)->y + subHeight / 2, particle->radius, subWidth, subHeight))
            insertParticleQuadtree(&(*node)->children[3], particle);
    }
}

void propagate(quadtree_node** node, double dt){
   //if (isLeafNode(&node)) {
   if ((*node)->children[0] != NULL) {
        for (int i = 0; i < (*node)->particle_count; i++) {
            Particle* particle = (*node)->particles[i];
            particle->x += (particle->vx * dt);
            particle->y += (particle->vy * dt);
            
            double left_gap = particle->x - particle->radius + 1;
            double right_gap = 1 - particle->x - particle->radius;
            double bottom_gap = particle->y - particle->radius + 1;
            double top_gap = 1 - particle->y - particle->radius;

            if (left_gap == 0 || right_gap == 0) {
                particle->vx *= -1;
            }

            if (top_gap == 0 || bottom_gap == 0) {
                particle->vy *= -1;
            }

            //if(left_gap < 0){
            //    propagate(node, left_gap / particle->vx);
            //}

            //else if(right_gap < 0){
            //    propagate(node, right_gap / particle->vx);
            //}

            //else if(bottom_gap < 0){
            //    propagate(node, bottom_gap / particle->vy);
            //}

            //else if(top_gap < 0){
            //    propagate(node, top_gap / particle->vy);
            //}

        }
    } 
    else {
        for (int i = 0; i < 4; i++) {
            propagate(&(*node)->children[i], dt);
        }
    }
}

#endif
