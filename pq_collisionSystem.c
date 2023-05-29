#include<stdlib.h>
#include<stdio.h>
#include"pq_collisionSystem.h"
#include<math.h>
#include<time.h>
#include<limits.h>
//#include"particle.h"
//#include"event.h"
//#include"minPQ.h"

void predict(Particle* p, minPQ* pq, particleSystem* sys){
        if(p == NULL) return;
        double dt;
        for (int i = 0; i < sys->particleCount; i++){
                dt = timeToHit(p, &(sys->particleArray[i]));
                if(dt != INT_MAX){
                        Event* event = newEvent(dt, p, &(sys->particleArray[i]));
                        enqueue(pq, event);
                }
        }
        dt = timeToHitVerticalWall(p);
        if(dt != INT_MAX){
                Event* event = newEvent(dt, p, NULL);
                enqueue(pq, event);
        }
        dt = timeToHitHorizontalWall(p);
        if(dt != INT_MAX){
                Event* event = newEvent(dt, p, NULL);
                enqueue(pq, event);
        }
}

void redraw(minPQ* pq){
        Event* event = newEvent(1.0, NULL, NULL); //1 is the minimum time after which redraw will be called
        enqueue(pq, event);
}

void fillPQ(minPQ* pq, particleSystem* sys){
        for(int i = 0;i < sys->particleCount;i++){
                predict(&(sys->particleArray[i]), pq, sys); //fill PQ at the first render with all possible collisions
        }
	Event* event = newEvent(0, NULL, NULL); //initial redraw event
        enqueue(pq, event); //initial redraw event
}

void updatePQ(minPQ* pq, particleSystem* sys, pq_CollisionSystem* pq_sys){
        Event* event = dequeue(pq);
        //if(!isValid(event)) continue;
	while(!isValid(event)){
		free(event);
		event = dequeue(pq);
	}
        Particle* particle1 = event->particle1;
        Particle* particle2 = event->particle2;

        for(int i = 0;i < sys->particleCount;i++){
                move(&(sys->particleArray[i]), event->time - pq_sys->t);
        }
	pq_sys->t = event->time;
	
	if(particle1 != NULL && particle2 != NULL) bounceOff(particle1, particle2);
	else if (particle1 != NULL && particle2 == NULL) bounceOffVerticalWall(particle1);
	else if (particle1 == NULL && particle2 != NULL) bounceOffHorizontalWall(particle2);
	else if (particle1 == NULL && particle2 == NULL) redraw(pq);
	
	predict(particle1, pq, sys);
	predict(particle2, pq, sys);
	free(event);
}




