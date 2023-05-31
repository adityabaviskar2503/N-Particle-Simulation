#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include"pq_collisionSystem.h"
#define PQ_PARTICLE pq_sys->sys
int width = 1000, height = 1000;

pq_CollisionSystem* pq_sys;

void drawCircle(float r, float x, float y){
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,y);
	
	for(float i = 0;i<360;i++){
		glVertex2f(x + r*cos(i*180/M_PI),y + r*sin(i*180/M_PI));
	}	
	glEnd();
}

void drawScene() {
    	// Clear the screen
    	glClear(GL_COLOR_BUFFER_BIT);
	
    	// Draw the particle
    	Particle particle;
    	for(int i = 0 ; i < PQ_PARTICLE->particleCount ; i++){
    		particle = PQ_PARTICLE->particleArray[i];
    		glColor3f(particle.color.r, particle.color.g, particle.color.b);
   		drawCircle(particle.radius, particle.x, particle.y);
    	}
    	// Swap buffers
    	glutSwapBuffers();
}

// Timer function for ball movement
void update(int value) {
	
	updatePQ(pq_sys->pq, pq_sys->sys, pq_sys);
	 
	 // Redraw the scene
    	glutPostRedisplay();

    	// Set the timer for the next update
    	glutTimerFunc(16, update, 0);
}

void keyboardFunc(unsigned char key, int x, int y) {
    	if (key == 'x' || key == 'X') {
        	// Exit the main loop
        	glutLeaveMainLoop();
    	}
}

// Main function
int main(int argc, char** argv) {
	pq_sys = malloc(sizeof(pq_CollisionSystem));
	createRandomSystem(&(pq_sys->sys), 2);
	pq_sys->pq = createPriorityQueue(2000);
	pq_sys->t = 0;
	printf("%d ",pq_sys->pq->capacity);
	fillPQ(pq_sys->pq, pq_sys->sys);
    // Initialize GLUT and create a window
/*   	glutInit(&argc, argv);
   	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   	glutInitWindowSize(width, height);
    	glutCreateWindow("Bouncing Ball");
   	// Set the background color
 	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
 	   
 	// Set the projection
 	glMatrixMode(GL_PROJECTION);
 	glLoadIdentity();
 	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
 	   
 	// Set the display function and timer
 	glutDisplayFunc(drawScene);
	glutTimerFunc(16, update, 0);
 	glutKeyboardFunc(keyboardFunc);
 	// Start the main loop
 	glutMainLoop();*/
int count = 0;
	while (pq_sys->pq->size > 0) {
		Event* dequeuedEvent = dequeue(pq_sys->pq);
             	// Process the event
              	printf("Event time: %f\n", dequeuedEvent->time);
              	// Free memory allocated for the event
              	free(dequeuedEvent);
		count++;
      	}
	printf("%d ", count);
	return 0;
}
