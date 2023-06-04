#include"particle.h"
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>


void move(Particle* p, double dt){
	p->x += p->vx * dt;
	p->y += p->vy * dt;
}

double timeToHit(Particle* this, Particle* that){
	if(this == that) return INT_MAX;
	double dx = that->x - this->x;
	double dy = that->y - this->y;
	double dvx = that->vx - this->vx;
	double dvy = that->vy - this->vy;
	double dvdr = dx*dvx + dy*dvy;
	if(dvdr > 0) return INT_MAX;
	double dvdv = dvx*dvx + dvy*dvy;
	double drdr = dx*dx + dy*dy;
	double sigma = this->radius + that->radius;
	double d = (dvdr*dvdr) - dvdv * (drdr - sigma*sigma);
	if(d < 0) return INT_MAX;
	return -(dvdr + sqrt(d))/dvdv;
}

double timeToHitVerticalWall(Particle* this){
	double time;
	if(this->vx > 0){
		time = (1.0 - this->x)/this->vx;
	}else if(this->vx < 0){
		time = (this->x + 1.0f)/this->vx;
	}else{
		time = INT_MAX;
	}
	return time;
}

double timeToHitHorizontalWall(Particle* this){
	double time;
	if(this->vy > 0){
		time = (1.0 - this->y)/this->vy;
	}else if(this->vy < 0){
		time = (this->y + 1.0)/this->vy;
	}else{
		time = INT_MAX;
	}
	return time;
}

void bounceOff(Particle* this, Particle* that){
	double dx = that->x - this->x; 
	double dy = that->y - this->y;
	double dvx = that->vx - this->vx;
	double dvy = that->vy - this->vy;
	double dvdr = dx*dvx + dy*dvy;
	double dist = this->radius + that->radius;
	double J = 2 * this->mass * that->mass * dvdr / ((this->mass + that->mass) * dist);
	double Jx = J * dx / dist;
	double Jy = J * dy / dist;
	this->vx += Jx / this->mass;
	this->vy += Jy / this->mass;
	that->vx -= Jx / that->mass;
	that->vy -= Jy / that->mass;
	this->collisions++;
	that->collisions++;
}

void bounceOffVerticalWall(Particle* this){
	this->vx *= -1;
	this->collisions++;
}

void bounceOffHorizontalWall(Particle* this){
	this->vy *= -1;
	this->collisions++;
}


