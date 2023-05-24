#include "particle.h"
#include "MinPriorityQueue.h"

typedef struct CollisionSystem{
	MinPriorityQueue* pq;
	Particle* particleArray;
	int particleCount;
	double t;
}CollisionSystem;

void createRandomSystem(CollisionSystem** system, int particleCount);

double distance(double x1, double y1, double x2, double y2);

int checkOverlap(Particle* particles, int numParticles, int currentIndex);

double getRandomDouble(double min, double max);

void divideAndAssignParticles(int particleCount, double left, double right, double top, double bottom, Particle* particles, int arrayPos);
