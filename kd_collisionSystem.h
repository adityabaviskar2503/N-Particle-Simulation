#ifndef PQ_COL_SYS
#define PQ_COL_SYS

#include "particle.h"
#include "KD_Tree.h"
#include "randomParticleGenerator.h"

typedef struct kd_CollisionSystem{
	kdtree kdtree;
    	particleSystem* sys;
}KD_CollisionSystem;

#endif
