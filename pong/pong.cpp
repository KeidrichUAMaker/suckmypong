// Created by Keidrich Sellati for UAMAKER VGP Class 2017-2018
#include "stdafx.h"

#include <string>
#include <Windows.h>
#include <iostream>
#include <conio.h>
#include <sstream>
#include <math.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "GL/freeglut.h"

#pragma comment(lib, "OpenGL32.lib")

//extra shit ;)

	//Makes int2str work
std::string int2str(int x) {
	// converts int to string
	std::stringstream ss;
	ss << x;
	return ss.str();
}

//makes vect2norm work
void vec2_norm(float& x, float &y) {
	// sets a vectors length to 1 (which means that x + y == 1)
	float length = sqrt((x * x) + (y * y));
	if (length != 0.0f) {
		length = 1.0f / length;
		x *= length;
		y *= length;
	}
}

//keycodes
#define VK_W 0x57
#define VK_S 0x53

//extra shit ;)




//Windows size and shit
int width = 750; //Width of Console
int height = 450; //Height of Console
int interval = 1000 / 60; //FPS

//Score
int score_left = 0;
int score_right = 0;

void drawText(float x, float y, std::string text) {
	glRasterPos2f(x, y);
	glutBitmapString(GLUT_BITMAP_8_BY_13, (const unsigned char*)text.c_str());
}

//Racket Shit
int racket_width = 10;
int racket_height = 80;
int racket_speed = 5;

//Left Racket bs
float racket_left_x = 10.0f;
float racket_left_y = 50.0f;

//Right Racket bs
float racket_right_x = width - racket_width - 10;
float racket_right_y = 50;

//Drawing Rackets
void drawRect(float x, float y, float width, float height) {
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}

//Controls for this garbage
void keyboard() {
	//left racket
	if (GetAsyncKeyState(VK_W)) racket_left_y += racket_speed;
	if (GetAsyncKeyState(VK_S)) racket_left_y -= racket_speed;

	//right racket
	if (GetAsyncKeyState(VK_UP)) racket_right_y += racket_speed;
	if (GetAsyncKeyState(VK_DOWN)) racket_right_y -= racket_speed;
}

//Testicle
float ball_pos_x = width / 2;
float ball_pos_y = height / 2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;
int ball_size = 8;
int ball_speed = 4;

//Make the testicle fly
void updateTesti() {
	// fly
	ball_pos_x += ball_dir_x * ball_speed;
	ball_pos_y += ball_dir_y * ball_speed;

	// hit by left racket?
	if (ball_pos_x < racket_left_x + racket_width &&
		ball_pos_x > racket_left_x &&
		ball_pos_y < racket_left_y + racket_height &&
		ball_pos_y > racket_left_y) {
		// set fly direction depending on where it hit the racket
		// (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
		float t = ((ball_pos_y - racket_left_y) / racket_height) - 0.5f;
		ball_dir_x = fabs(ball_dir_x); // force it to be positive
		ball_dir_y = t;
	}

	// hit by right racket?
	if (ball_pos_x > racket_right_x &&
		ball_pos_x < racket_right_x + racket_width &&
		ball_pos_y < racket_right_y + racket_height &&
		ball_pos_y > racket_right_y) {
		// set fly direction depending on where it hit the racket
		// (t is 0.5 if hit at top, 0 at center, -0.5 at bottom)
		float t = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
		ball_dir_x = -fabs(ball_dir_x); // force it to be negative
		ball_dir_y = t;
	}

	// hit left wall?
	if (ball_pos_x < 0) {
		++score_right;
		ball_pos_x = width / 2;
		ball_pos_y = height / 2;
		ball_dir_x = fabs(ball_dir_x); // force it to be positive
		ball_dir_y = 0;
	}

	// hit right wall?
	if (ball_pos_x > width) {
		++score_left;
		ball_pos_x = width / 2;
		ball_pos_y = height / 2;
		ball_dir_x = -fabs(ball_dir_x); // force it to be negative
		ball_dir_y = 0;
	}

	// hit top wall?
	if (ball_pos_y > height) {
		ball_dir_y = -fabs(ball_dir_y); // force it to be negative
	}

	// hit bottom wall?
	if (ball_pos_y < 0) {
		ball_dir_y = fabs(ball_dir_y); // force it to be positive
	}

	// make sure that length of dir stays at 1
	vec2_norm(ball_dir_x, ball_dir_y);
}


//Drawing shit
void draw() {
	//clear (has to be blank to start)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//ToDo: Draw scene
	
	//draw testicle
	drawRect(ball_pos_x - ball_size / 2, ball_pos_y - ball_size / 2, ball_size, ball_size);
	
	//draw rackets on screen
	drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
	drawRect(racket_right_x, racket_right_y, racket_width, racket_height);

	//draw score on screen
	drawText(width / 2 - 7, height - 20, int2str(score_left) + ":" + int2str(score_right));

	//swap buffers
	glutSwapBuffers();
}

//updates everything in this shitty game
void update(int value) {
	//input crap
	keyboard();
	
	//update testicle
	updateTesti();

	//Call update() again in "x" seconds
	glutTimerFunc(interval, update, 0);

	//redisplay frame
	glutPostRedisplay();
}

void enable2D(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int _tmain(int argc, char** argv) {
    //intialize opengl (GLUT)
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutCreateWindow("Pong - Keidrich UAMAKER");

	//Callback Functions
	glutDisplayFunc(draw);
	glutTimerFunc(interval, update, 0);

	//Setup scene in 2d and draw white color
	enable2D(width, height);
	glColor3f(1.0f, 1.0f, 1.0f);

	//start everything
	glutMainLoop();
	return 0;
}



