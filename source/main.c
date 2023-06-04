#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<GL/freeglut.h>
#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include"../include/kd_collisionSystem.h"
#define KD_PARTICLE kd_sys.sys
int width = 1000, height = 1000;

KD_CollisionSystem kd_sys;

void drawCircle(float r, float x, float y){
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,y);
	
	for(float i = 0;i<360;i++){
		glVertex2f(x + r*cos(i*180/M_PI),y + r*sin(i*180/M_PI));
	}	
	glEnd();
}

void drawKDTree(kdtree root) {
	if (root == NULL)
		return;
	drawKDTree(root->left);
	glColor3f(root->ball.color.r, root->ball.color.g, root->ball.color.b);
	drawCircle(root->ball.radius, root->ball.x, root->ball.y);
	drawKDTree(root->right);
	return;
}

void move_particles(kdtree root,double k){
	if(root == NULL) return;
	//UPDATING AND COLLISION HANDELLING
	move(&(root->ball),k);
	move_particles(root->left,k);
	move_particles(root->right,k);
}

void  particle_collision_render(kdtree root){
	if(root == NULL) return;

	//P2P Collision
	kdtree nearest;
	nearest = findNN(kd_sys.tree,&(root->ball),0);

	if(collisionCheck_P2P(nearest->ball,root->ball)){
		handle_collision(&(nearest->ball), &(root->ball));
	}

	particle_collision_render(root->left);
	particle_collision_render(root->right);
}

void update_particles_boundary(kdtree root){
	if(!root) return ;
	
	double left_gap = root->ball.x - root->ball.radius + 1;
        double right_gap = 1 - root->ball.x - root->ball.radius;
        double bottom_gap = root->ball.y - root->ball.radius + 1;
        double top_gap = 1 - root->ball.y - root->ball.radius;

	if(left_gap < 0 && root->ball.vx < 0){
            //propagate_sys(qt_sys, left_gap / (root->ball.vx));
            root->ball.vx *= -1;
            //propagate_sys(qt_sys, -left_gap / (root->ball.vx));
        }

        else if(right_gap < 0 && root->ball.vx > 0){
            //propagate_sys(qt_sys, right_gap / (root->ball.vx));
            root->ball.vx *= -1;
            //propagate_sys(qt_sys, -right_gap / (root->ball.vx));
        }

        if(bottom_gap < 0 && root->ball.vy < 0){
            //propagate_sys(qt_sys, bottom_gap / (root->ball.vy));
            root->ball.vy *= -1;
            //propagate_sys(qt_sys, -bottom_gap / (root->ball.vy));
        }

        else if(top_gap < 0 && root->ball.vy > 0){
            //propagate_sys(qt_sys, top_gap / (root->ball.vy));
            root->ball.vy *= -1;
            //propagate_sys(qt_sys, -top_gap / (root->ball.vy));
        }
	
	update_particles_boundary(root->left);
	update_particles_boundary(root->right);

}

void propagate_sys(particleSystem* qt_sys, double dt){
    for(int i = 0; i < qt_sys->particleCount; i++){
        Particle* particle = &(qt_sys->particleArray)[i];
        particle->x += particle->vx * dt;
        particle->y += particle->vy * dt;
    }
}

void reverse_at_boundry(particleSystem* qt_sys){
    for (int i = 0; i < qt_sys->particleCount; i++) {
        Particle* particle = &(qt_sys->particleArray[i]);

        double left_gap = particle->x - particle->radius + 1;
        double right_gap = 1 - particle->x - particle->radius;
        double bottom_gap = particle->y - particle->radius + 1;
        double top_gap = 1 - particle->y - particle->radius;

        if(left_gap < 0 && particle->vx < 0){
            //propagate_sys(qt_sys, left_gap / (particle->vx));
            particle->vx *= -1;
            //propagate_sys(qt_sys, -left_gap / (particle->vx));
        }

        else if(right_gap < 0 && particle->vx > 0){
            //propagate_sys(qt_sys, right_gap / (particle->vx));
            particle->vx *= -1;
            //propagate_sys(qt_sys, -right_gap / (particle->vx));
        }

        if(bottom_gap < 0 && particle->vy < 0){
            //propagate_sys(qt_sys, bottom_gap / (particle->vy));
            particle->vy *= -1;
            //propagate_sys(qt_sys, -bottom_gap / (particle->vy));
        }

        else if(top_gap < 0 && particle->vy > 0){
            //propagate_sys(qt_sys, top_gap / (particle->vy));
            particle->vy *= -1;
            //propagate_sys(qt_sys, -top_gap / (particle->vy));
        }

    }

}

void drawScene() {
    	// Clear the screen
    	glClear(GL_COLOR_BUFFER_BIT);
	
    	// Draw the particle
    	drawKDTree(kd_sys.tree);

    	// Swap buffers
    	glutSwapBuffers();
}

// Timer function for ball movement
void update(int value) {
    	// Update the ball position
	move_particles(kd_sys.tree,0.5);
	update_particles_boundary(kd_sys.tree);
	rebalanceKDTree(kd_sys.tree,KD_PARTICLE->particleCount);
   	particle_collision_render(kd_sys.tree);

    	 // Redraw the scene
    	glutPostRedisplay();

    	// Set the timer for the next update
    	glutTimerFunc(8, update, 0);
}

void keyboardFunc(unsigned char key, int x, int y) {
    	if (key == 'x' || key == 'X') {
        	// Exit the main loop
        	glutLeaveMainLoop();
    	}
}

// Main function
int main(int argc, char** argv) {	
	
	createRandomSystem(&(kd_sys.sys), 10);
	kd_sys.tree = createKDTree(KD_PARTICLE->particleArray,0,KD_PARTICLE->particleCount-1,0);
	
//	displayKDTree(kd_sys.tree);

	// Initialize GLUT and create a window
   	glutInit(&argc, argv);
   	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   	glutInitWindowSize(width, height);
    	glutCreateWindow("SIMULATION : KD-TREE");
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
