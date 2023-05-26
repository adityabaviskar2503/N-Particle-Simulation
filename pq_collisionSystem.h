#ifndef PQ_COL_SYS
#define PQ_COL_SYS

#include "particle.h"
#include "minPQ.h"
#include "randomParticleGenerator.h"

typedef struct pq_CollisionSystem{
	minPQ* pq;
	particleSystem* sys;
	double t;
}pq_CollisionSystem;

#endif
