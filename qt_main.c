#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include "randomParticleGenerator.h"
#include "particle.h"
#include "quadtree.h"
//#include"pq_collisionSystem.h"
//#define PQ_PARTICLE pq_sys.sys
// Window size
int width = 1000, height = 1000;

//pq_CollisionSystem pq_sys;
particleSystem* qt_sys;
quadtree_node* QT;

void drawCircle(float r, float x, float y);

// Bouncing ball function
void drawScene() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
	
    // Draw the ball
    Particle particle;
    for(int i = 0 ; i < qt_sys->particleCount ; i++){
    	particle = qt_sys->particleArray[i];
    	glColor3f(particle.color.r, particle.color.g, particle.color.b);
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
void update() {
    // Update the ball position


    propagate_sys(qt_sys, 1.0);
    reverse_at_boundry(qt_sys);
    clearQuadtree(QT);
    QT = createquadtree_node(0, 0, 2, 2);
    for(int i = 0 ; i < qt_sys->particleCount ; i++){
        insertParticleQuadtree(&QT, &(qt_sys->particleArray[i]));
    }
    //updateQuadtree(&QT, &QT);
    if(!correct_quadtree(QT))
        printf("WRONG quadtree\n");
    detectCollisionQuadtree(qt_sys, &QT);  

//    printf("\n");
//    for(int i = 0 ; i < qt_sys->particleCount ; i++){
//        printf("%lf ",(qt_sys->particleArray[i]).x);
//    }
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
	//createRandomSystem(&(pq_sys.sys), 200);
    QT = createquadtree_node(0, 0, 2, 2);
	createRandomSystem(&(qt_sys), 120);
    printf("particle array size id: %d\n",qt_sys->particleCount);
    for(int i = 0 ; i < qt_sys->particleCount ; i++){
    	//particle = qt_sys->particleArray[i];
        insertParticleQuadtree(&QT, &(qt_sys->particleArray[i]));
    }
    printf("here\n");
//    display_QT(QT);


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
 	glutKeyboardFunc(keyboardFunc);
    // Start the main loop
 	glutMainLoop();
 	return 0;
}
