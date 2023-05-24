#include<stdlib.h>
#include<stdio.h>
#include"collisionSystem.h"
#include<math.h>
#include<time.h>

void createRandomSystem(CollisionSystem** system, int particleCount){
	*system = (CollisionSystem*)malloc(sizeof(CollisionSystem));
	if(!(*system)){
		printf("Malloc in generation of random system failed");
		return;
	}
	(*system)->particleCount = particleCount;
//	printf("value here is %d %d\n",particleCount, system->particleCount);
	(*system)->pq = NULL;
	(*system)->particleArray = (Particle*)malloc(sizeof(Particle)*particleCount);
//	divideAndAssignParticles(particleCount, -1, 1, -1, 1, (*system)->particleArray, 0);
}

double distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

int checkOverlap(Particle* particles, int numParticles, int currentIndex) {
	double currentX = particles[currentIndex].x;
	double currentY = particles[currentIndex].y;
	double currentRadius = particles[currentIndex].radius;

   	for (int i = 0; i < numParticles; i++) {
       	 	if (i == currentIndex) {
			continue;
        	}

        	double otherX = particles[i].x;
        	double otherY = particles[i].y;
        	double otherRadius = particles[i].radius;

        	double dist = distance(currentX, currentY, otherX, otherY);
        	double minDist = currentRadius + otherRadius;

        	if (dist < minDist) {
			return 1; // Overlap detected
        	}
	}
	return 0; // No overlap
}

double getRandomDouble(double min, double max) {
	double range = max - min;
    	double randomValue = ((double)rand() / RAND_MAX) * range + min;
    	return randomValue;
}

void divideAndAssignParticles(int particleCount, double left, double right, double top, double bottom, Particle* particles, int arrayPos){
	if(particleCount < 40){
		for (int i = arrayPos; i < particleCount; i++){
			particles[i].x = getRandomDouble(left,right);
	       		particles[i].y = getRandomDouble(bottom,top);
	       	 	particles[i].vx = getRandomDouble(-0.2,0.2);
	        	particles[i].vy = getRandomDouble(-0.2,0.2);
	        	particles[i].radius = getRandomDouble(0.01,0.02);
	        	particles[i].mass = getRandomDouble(0.1,10);
	        	particles[i].collisions = 0;
		        particles[i].color.r = (float)rand() / RAND_MAX;
		        particles[i].color.g = (float)rand() / RAND_MAX;
			particles[i].color.b = (float)rand() / RAND_MAX;
	
	        // Check for overlaps and reassign random values if necessary
	        	while (checkOverlap(particles, particleCount, i)) {
				particles[i].x = getRandomDouble(left,right);
	            		particles[i].y = getRandomDouble(bottom,top);
	            		particles[i].radius = getRandomDouble(0.01,0.02);
	        	}
		}
	}else{
		//for top left region
		divideAndAssignParticles(particleCount/4, left, (left+right)/2, top, (top+bottom)/2, particles, arrayPos);
		//for top right region
		divideAndAssignParticles(particleCount/4, (left+right)/2, right, top, (top+bottom)/2, particles, arrayPos);
		// for bottom left region
		divideAndAssignParticles(particleCount/4, left, (left+right)/2, (top+bottom)/2, bottom, particles, arrayPos);
		//for bottom right region
		divideAndAssignParticles(particleCount/4, (left+right)/2, right, (top+bottom)/2, bottom, particles, arrayPos);
	}
}

//void displaySystemConfiguration(CollisionSystem system){
//	Particle* particleArray = system->particleArray;
//	int count = system->particleCount;
//	for(int i = 0;i<particleCount;i++){
//		printf("Particle %d - x:")
//	}
//}

int main(){
	CollisionSystem* cs;
	createRandomSystem(&cs, 36);
	printf("%d\n",cs->particleCount);
//	for(int i = 0;i<cs.particleCount;i++){
//		printf("%f ",cs.particleArray[i].radius);
//	}
	return 0;
}



