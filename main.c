#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<math.h>
#include<stdlib.h>
#include"pq_collisionSystem.h"
#define PQ_PARTICLE pq_sys.sys
// Window size
int width = 1000, height = 1000;

pq_CollisionSystem pq_sys;

void drawCircle(float r, float x, float y);

// Bouncing ball function
void drawScene() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
	
    // Draw the ball
    Particle particle;
    for(int i = 0 ; i < PQ_PARTICLE->particleCount ; i++){
    	particle = PQ_PARTICLE->particleArray[i];
    	glColor3f(1.0f, 1.0f, 0.0f);
   	drawCircle(particle.radius, particle.x, particle.y);
    }
    // Swap buffers
    glutSwapBuffers();
}

void drawCircle(float r, float x, float y){
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,y);
	
	for(float i = 0;i<360;i++){
		glVertex2f(x + r*cos(i*180/M_PI),y + r*sin(i*180/M_PI));
	}	
	glEnd();
}

// Timer function for ball movement
void update(int value) {
    // Update the ball position
    Particle particle;
    double ballRadius;// ballX, ballY, ballXSpeed, ballYSpeed
    for(int i = 0 ; i < PQ_PARTICLE->particleCount ; i++){
    	particle = PQ_PARTICLE->particleArray[i];
    	 ballRadius = particle.radius;
    	PQ_PARTICLE->particleArray[i].x += particle.vx;
   	PQ_PARTICLE->particleArray[i].y += particle.vy;
   	 // Check for collision with window edges
	if (PQ_PARTICLE->particleArray[i].x > 1.0f - ballRadius || PQ_PARTICLE->particleArray[i].x < -1.0f + ballRadius) {
       		PQ_PARTICLE->particleArray[i].vx = -particle.vx;
   	 }
    	if (PQ_PARTICLE->particleArray[i].y > 1.0f - ballRadius || PQ_PARTICLE->particleArray[i].y < -1.0f + ballRadius) {
       		PQ_PARTICLE->particleArray[i].vy = -particle.vy;
    	} 	
   	
    }
     // Redraw the scene
    glutPostRedisplay();

    // Set the timer for the next update
    glutTimerFunc(16, update, 0);
}


// Main function
int main(int argc, char** argv) {
	
	createRandomSystem(&(pq_sys.sys), 1000);

    // Initialize GLUT and create a window
    glutInit(&argc, argv);
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

    // Start the main loop
    glutMainLoop();
    return 0;
}
