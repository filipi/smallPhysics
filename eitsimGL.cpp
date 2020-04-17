//*****************************************************
//
// Adapted from CuboRGB.cpp
// which is an example code to use a GLUT window
// with a RGB cube with ilumination allowing
// zoom in and out and rotation
//
// CuboRGB.cpp comes as an example code of the
// book "OpenGL - Uma Abordagem Pratica e Objetiva"
// by Marcelo Cohen e Isabel H. Manssour
//
//*****************************************************

//#include <windows.h>
#include <GL/glut.h>
#include <stdlib.h>
#include "Physics3.h"
#include <stdio.h>

// Navigation control variables
GLfloat angle, fAspect;
GLfloat rotX, rotY, rotX_ini, rotY_ini;
GLfloat obsX, obsY, obsZ, obsX_ini, obsY_ini, obsZ_ini;
int x_ini,y_ini,bot;

int lastTime;

float prox = 1.5;
float force = 0;

static bool SaturdayNightFever = false;
static bool debug = false;

bool fullScreen = false;


/**
 * class SoftBodySimulation is derived from class Simulation (see Physics1.h).
 * It simulates a softbody with point-like particles binded with springs. The
 * springs have inner friction and normal length. One tip of the softbody is
 * stabilized at a point in space called "Vector3D softbodyConnectionPos".
 * This point can be moved externally by a method
 * "void setSoftBodyConnectionVel(Vector3D softbodyConnectionVel)". SoftBodySimulation
 * creates air friction and a planer surface (or ground) with a
 * normal in +y direction. SoftBodySimulation implements the force applied
 * by this surface. In the code, the surface is refered as "ground".
 */
SoftBodySimulation* softbodySimulation;

ForceApplyCursor* cursor;

// Função responsável pela especificação dos parâmetros de iluminação
void DefineIluminacao (void)
{
  GLfloat luzAmbiente[4]={0.25,0.25,0.25,1.0};
  GLfloat luzDifusa[4]={0.7,0.7,0.7,1.0};	   // "cor"
  GLfloat posicaoLuz0[4]={0.0, 100.0, 0.0, 1.0};
  GLfloat posicaoLuz1[4]={0.0, -100.0, 0.0, 1.0};

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

  // Define os parâmetros da luz de número 0
  glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
  glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz0 );

  // Define os parâmetros da luz de número 1
  glLightfv(GL_LIGHT1, GL_AMBIENT, luzAmbiente);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, luzDifusa );
  glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz1 );
}

// Função que desenha um cubo cujas cores dos vértices
// mostram como é o espaço RGB
void DesenhaCuboRGB(void)
{
  gluLookAt(0, 0, prox, 0, 0, 0, 0, 1, 0);
  glRotatef(-60.0, 1.0, 0.0, 0.0); // rotates X to back 60 degrees
  glRotatef(45.0, 0.0, 0.0, 1.0); // rotates Z counter clockwise 45 degrees
  glTranslatef(0.0, 0.0, -0.3);


  glColor3ub(255, 255, 0);              // Set Color To Yellow
  for (int index_X = 0; index_X < softbodySimulation->numOfXMasses - 1; ++index_X){
    for (int index_Y = 0; index_Y < softbodySimulation->numOfYMasses - 1; ++index_Y){
      if (SaturdayNightFever) glColor3ub(rand()%255, rand()%255, rand()%255);
      int massIndex1 = softbodySimulation->getMassIndex(index_X,     index_Y,     softbodySimulation->numOfZMasses - 1);
      int massIndex2 = softbodySimulation->getMassIndex(index_X + 1, index_Y,     softbodySimulation->numOfZMasses - 1);
      int massIndex3 = softbodySimulation->getMassIndex(index_X + 1, index_Y + 1, softbodySimulation->numOfZMasses - 1);
      int massIndex4 = softbodySimulation->getMassIndex(index_X,     index_Y + 1, softbodySimulation->numOfZMasses - 1);
      Mass* mass1 = softbodySimulation->getMass(massIndex1); Vector3D* pos1 = &mass1->pos;
      Mass* mass2 = softbodySimulation->getMass(massIndex2); Vector3D* pos2 = &mass2->pos;
      Mass* mass3 = softbodySimulation->getMass(massIndex3); Vector3D* pos3 = &mass3->pos;
      Mass* mass4 = softbodySimulation->getMass(massIndex4); Vector3D* pos4 = &mass4->pos;


      glBegin(GL_TRIANGLES);
      GLfloat fvert1[3] = {pos1->x  - softbodySimulation->offSetX(),
			   pos1->y - softbodySimulation->offSetY(),
			   pos1->z  - softbodySimulation->offSetZ()};
      GLfloat fvert2[3] = {pos2->x  - softbodySimulation->offSetX(),
			   pos2->y - softbodySimulation->offSetY(),
			   pos2->z  - softbodySimulation->offSetZ()};
      GLfloat fvert3[3] = {pos4->x  - softbodySimulation->offSetX(),
			   pos4->y - softbodySimulation->offSetY(),
			   pos4->z  - softbodySimulation->offSetZ()};
      GLfloat fNormalX, fNormalY, fNormalZ;
      CalculateVectorNormal(fvert1, fvert2, fvert3, &fNormalX, &fNormalY, &fNormalZ);
      glNormal3f(fNormalX, fNormalY, fNormalZ);
      glVertex3f(pos1->x  - softbodySimulation->offSetX(),
		 pos1->y - softbodySimulation->offSetY(),
		 pos1->z  - softbodySimulation->offSetZ());
      glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		 pos2->y - softbodySimulation->offSetY(),
		 pos2->z  - softbodySimulation->offSetZ());
      glVertex3f(pos4->x  - softbodySimulation->offSetX(),
		 pos4->y - softbodySimulation->offSetY(),
		 pos4->z  - softbodySimulation->offSetZ());
      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex3f(pos4->x  - softbodySimulation->offSetX(),
		 pos4->y - softbodySimulation->offSetY(),
		 pos4->z  - softbodySimulation->offSetZ());
      glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		 pos2->y - softbodySimulation->offSetY(),
		 pos2->z  - softbodySimulation->offSetZ());
      glVertex3f(pos3->x  - softbodySimulation->offSetX(),
		 pos3->y - softbodySimulation->offSetY(),
		 pos3->z  - softbodySimulation->offSetZ());
      glEnd();
    }
  }
  for (int index_Z = 0; index_Z < softbodySimulation->numOfZMasses - 1; ++index_Z){
    for (int index_Y = 0; index_Y < softbodySimulation->numOfYMasses - 1; ++index_Y){
      if (SaturdayNightFever) glColor3ub(rand()%255, rand()%255, rand()%255);
      int massIndex1 = softbodySimulation->getMassIndex(0, index_Y,     index_Z);
      int massIndex2 = softbodySimulation->getMassIndex(0, index_Y + 1, index_Z);
      int massIndex3 = softbodySimulation->getMassIndex(0, index_Y + 1, index_Z + 1);
      int massIndex4 = softbodySimulation->getMassIndex(0, index_Y,     index_Z + 1);
      Mass* mass1 = softbodySimulation->getMass(massIndex1); Vector3D* pos1 = &mass1->pos;
      Mass* mass2 = softbodySimulation->getMass(massIndex2); Vector3D* pos2 = &mass2->pos;
      Mass* mass3 = softbodySimulation->getMass(massIndex3); Vector3D* pos3 = &mass3->pos;
      Mass* mass4 = softbodySimulation->getMass(massIndex4); Vector3D* pos4 = &mass4->pos;

             GLfloat fvert1[3] = {pos1->x  - softbodySimulation->offSetX(),
       			   pos1->y - softbodySimulation->offSetY(),
       			   pos1->z  - softbodySimulation->offSetZ()};
             GLfloat fvert2[3] = {pos2->x  - softbodySimulation->offSetX(),
       			   pos2->y - softbodySimulation->offSetY(),
       			   pos2->z  - softbodySimulation->offSetZ()};
             GLfloat fvert3[3] = {pos4->x  - softbodySimulation->offSetX(),
       			   pos4->y - softbodySimulation->offSetY(),
       			   pos4->z  - softbodySimulation->offSetZ()};
             GLfloat fNormalX, fNormalY, fNormalZ;
             CalculateVectorNormal(fvert1, fvert2, fvert3, &fNormalX, &fNormalY, &fNormalZ);
             glNormal3f(fNormalX, fNormalY, fNormalZ);


      glBegin(GL_TRIANGLES);
      glVertex3f(pos1->x  - softbodySimulation->offSetX(),
		 pos1->y - softbodySimulation->offSetY(),
		 pos1->z  - softbodySimulation->offSetZ());
      glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		 pos2->y - softbodySimulation->offSetY(),
		 pos2->z  - softbodySimulation->offSetZ());
      glVertex3f(pos4->x  - softbodySimulation->offSetX(),
		 pos4->y - softbodySimulation->offSetY(),
		 pos4->z  - softbodySimulation->offSetZ());
      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex3f(pos4->x  - softbodySimulation->offSetX(),
		 pos4->y - softbodySimulation->offSetY(),
		 pos4->z  - softbodySimulation->offSetZ());
      glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		 pos2->y - softbodySimulation->offSetY(),
		 pos2->z  - softbodySimulation->offSetZ());
      glVertex3f(pos3->x  - softbodySimulation->offSetX(),
		 pos3->y - softbodySimulation->offSetY(),
		 pos3->z  - softbodySimulation->offSetZ());
      glEnd();
    }
  }
  for (int index_X = 0; index_X < softbodySimulation->numOfXMasses - 1; ++index_X){
    for (int index_Z = 0; index_Z < softbodySimulation->numOfZMasses - 1; ++index_Z){
      if (SaturdayNightFever) glColor3ub(rand()%255, rand()%255, rand()%255);
      int massIndex1 = softbodySimulation->getMassIndex(index_X,     0, index_Z);
      int massIndex2 = softbodySimulation->getMassIndex(index_X + 1, 0, index_Z);
      int massIndex3 = softbodySimulation->getMassIndex(index_X + 1, 0, index_Z + 1);
      int massIndex4 = softbodySimulation->getMassIndex(index_X,     0, index_Z + 1);
      Mass* mass1 = softbodySimulation->getMass(massIndex1); Vector3D* pos1 = &mass1->pos;
      Mass* mass2 = softbodySimulation->getMass(massIndex2); Vector3D* pos2 = &mass2->pos;
      Mass* mass3 = softbodySimulation->getMass(massIndex3); Vector3D* pos3 = &mass3->pos;
      Mass* mass4 = softbodySimulation->getMass(massIndex4); Vector3D* pos4 = &mass4->pos;

      GLfloat fvert1[3] = {pos1->x  - softbodySimulation->offSetX(),
			   pos1->y - softbodySimulation->offSetY(),
			   pos1->z  - softbodySimulation->offSetZ()};
      GLfloat fvert2[3] = {pos2->x  - softbodySimulation->offSetX(),
			   pos2->y - softbodySimulation->offSetY(),
			   pos2->z  - softbodySimulation->offSetZ()};
      GLfloat fvert3[3] = {pos4->x  - softbodySimulation->offSetX(),
			   pos4->y - softbodySimulation->offSetY(),
			   pos4->z  - softbodySimulation->offSetZ()};
      GLfloat fNormalX, fNormalY, fNormalZ;
      CalculateVectorNormal(fvert1, fvert2, fvert3, &fNormalX, &fNormalY, &fNormalZ);
      glNormal3f(fNormalX, fNormalY, fNormalZ);

      glBegin(GL_TRIANGLES);
      glVertex3f(pos1->x  - softbodySimulation->offSetX(),
		 pos1->y - softbodySimulation->offSetY(),
		 pos1->z  - softbodySimulation->offSetZ());
      glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		 pos2->y - softbodySimulation->offSetY(),
		 pos2->z  - softbodySimulation->offSetZ());
      glVertex3f(pos4->x  - softbodySimulation->offSetX(),
		 pos4->y - softbodySimulation->offSetY(),
		 pos4->z  - softbodySimulation->offSetZ());
      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex3f(pos4->x  - softbodySimulation->offSetX(),
		 pos4->y - softbodySimulation->offSetY(),
		 pos4->z  - softbodySimulation->offSetZ());
      glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		 pos2->y - softbodySimulation->offSetY(),
		 pos2->z  - softbodySimulation->offSetZ());
      glVertex3f(pos3->x  - softbodySimulation->offSetX(),
		 pos3->y - softbodySimulation->offSetY(),
		 pos3->z  - softbodySimulation->offSetZ());
      glEnd();
    }
  }

  for (int index_X = 0; index_X < softbodySimulation->numOfXMasses - 1; ++index_X){
    for (int index_Y = 0; index_Y < softbodySimulation->numOfYMasses - 1; ++index_Y){
      int massIndex1 = softbodySimulation->getMassIndex(index_X,     index_Y,     0);
      int massIndex2 = softbodySimulation->getMassIndex(index_X + 1, index_Y,     0);
      int massIndex3 = softbodySimulation->getMassIndex(index_X + 1, index_Y + 1, 0);
      int massIndex4 = softbodySimulation->getMassIndex(index_X,     index_Y + 1, 0);
      Mass* mass1 = softbodySimulation->getMass(massIndex1); Vector3D* pos1 = &mass1->pos;
      Mass* mass2 = softbodySimulation->getMass(massIndex2); Vector3D* pos2 = &mass2->pos;
      Mass* mass3 = softbodySimulation->getMass(massIndex3); Vector3D* pos3 = &mass3->pos;
      Mass* mass4 = softbodySimulation->getMass(massIndex4); Vector3D* pos4 = &mass4->pos;

             GLfloat fvert1[3] = {pos1->x  - softbodySimulation->offSetX(),
       			   pos1->y - softbodySimulation->offSetY(),
       			   pos1->z  - softbodySimulation->offSetZ()};
             GLfloat fvert2[3] = {pos2->x  - softbodySimulation->offSetX(),
       			   pos2->y - softbodySimulation->offSetY(),
       			   pos2->z  - softbodySimulation->offSetZ()};
             GLfloat fvert3[3] = {pos4->x  - softbodySimulation->offSetX(),
       			   pos4->y - softbodySimulation->offSetY(),
       			   pos4->z  - softbodySimulation->offSetZ()};
             GLfloat fNormalX, fNormalY, fNormalZ;
             CalculateVectorNormal(fvert1, fvert2, fvert3, &fNormalX, &fNormalY, &fNormalZ);
             glNormal3f(fNormalX, fNormalY, fNormalZ);

      glBegin(GL_TRIANGLES);
      glVertex3f(pos1->x  - softbodySimulation->offSetX(),
		 pos1->y - softbodySimulation->offSetY(),
		 pos1->z  - softbodySimulation->offSetZ());
      glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		 pos2->y - softbodySimulation->offSetY(),
		 pos2->z  - softbodySimulation->offSetZ());
      glVertex3f(pos4->x  - softbodySimulation->offSetX(),
		 pos4->y - softbodySimulation->offSetY(),
		 pos4->z  - softbodySimulation->offSetZ());
      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex3f(pos4->x  - softbodySimulation->offSetX(),
		 pos4->y - softbodySimulation->offSetY(),
		 pos4->z  - softbodySimulation->offSetZ());
      glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		 pos2->y - softbodySimulation->offSetY(),
		 pos2->z  - softbodySimulation->offSetZ());
      glVertex3f(pos3->x  - softbodySimulation->offSetX(),
		 pos3->y - softbodySimulation->offSetY(),
		 pos3->z  - softbodySimulation->offSetZ());
      glEnd();
    }
  }

  for (int index_Z = 0; index_Z < softbodySimulation->numOfZMasses - 1; ++index_Z){
    for (int index_Y = 0; index_Y < softbodySimulation->numOfYMasses - 1; ++index_Y){
      if (SaturdayNightFever) glColor3ub(rand()%255, rand()%255, rand()%255);
      int massIndex1 = softbodySimulation->getMassIndex(softbodySimulation->numOfXMasses - 1, index_Y,     index_Z);
      int massIndex2 = softbodySimulation->getMassIndex(softbodySimulation->numOfXMasses - 1, index_Y + 1, index_Z);
      int massIndex3 = softbodySimulation->getMassIndex(softbodySimulation->numOfXMasses - 1, index_Y + 1, index_Z + 1);
      int massIndex4 = softbodySimulation->getMassIndex(softbodySimulation->numOfXMasses - 1, index_Y,     index_Z + 1);
      Mass* mass1 = softbodySimulation->getMass(massIndex1); Vector3D* pos1 = &mass1->pos;
      Mass* mass2 = softbodySimulation->getMass(massIndex2); Vector3D* pos2 = &mass2->pos;
      Mass* mass3 = softbodySimulation->getMass(massIndex3); Vector3D* pos3 = &mass3->pos;
      Mass* mass4 = softbodySimulation->getMass(massIndex4); Vector3D* pos4 = &mass4->pos;

             GLfloat fvert1[3] = {pos1->x  - softbodySimulation->offSetX(),
       			   pos1->y - softbodySimulation->offSetY(),
       			   pos1->z  - softbodySimulation->offSetZ()};
             GLfloat fvert2[3] = {pos2->x  - softbodySimulation->offSetX(),
       			   pos2->y - softbodySimulation->offSetY(),
       			   pos2->z  - softbodySimulation->offSetZ()};
             GLfloat fvert3[3] = {pos4->x  - softbodySimulation->offSetX(),
       			   pos4->y - softbodySimulation->offSetY(),
       			   pos4->z  - softbodySimulation->offSetZ()};
             GLfloat fNormalX, fNormalY, fNormalZ;
             CalculateVectorNormal(fvert1, fvert2, fvert3, &fNormalX, &fNormalY, &fNormalZ);
             glNormal3f(fNormalX, fNormalY, fNormalZ);

      glBegin(GL_TRIANGLES);
      glVertex3f(pos1->x  - softbodySimulation->offSetX(),
		 pos1->y - softbodySimulation->offSetY(),
		 pos1->z  - softbodySimulation->offSetZ());
      glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		 pos2->y - softbodySimulation->offSetY(),
		 pos2->z  - softbodySimulation->offSetZ());
      glVertex3f(pos4->x  - softbodySimulation->offSetX(),
		 pos4->y - softbodySimulation->offSetY(),
		 pos4->z  - softbodySimulation->offSetZ());
      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex3f(pos4->x  - softbodySimulation->offSetX(),
		 pos4->y - softbodySimulation->offSetY(),
		 pos4->z  - softbodySimulation->offSetZ());
      glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		 pos2->y - softbodySimulation->offSetY(),
		 pos2->z  - softbodySimulation->offSetZ());
      glVertex3f(pos3->x  - softbodySimulation->offSetX(),
		 pos3->y - softbodySimulation->offSetY(),
		 pos3->z  - softbodySimulation->offSetZ());
      glEnd();
    }
  }
  for (int index_X = 0; index_X < softbodySimulation->numOfXMasses - 1; ++index_X){
    for (int index_Z = 0; index_Z < softbodySimulation->numOfZMasses - 1; ++index_Z){
      if (SaturdayNightFever) glColor3ub(rand()%255, rand()%255, rand()%255);
      int massIndex1 = softbodySimulation->getMassIndex(index_X,     softbodySimulation->numOfYMasses - 1, index_Z);
      int massIndex2 = softbodySimulation->getMassIndex(index_X + 1, softbodySimulation->numOfYMasses - 1, index_Z);
      int massIndex3 = softbodySimulation->getMassIndex(index_X + 1, softbodySimulation->numOfYMasses - 1, index_Z + 1);
      int massIndex4 = softbodySimulation->getMassIndex(index_X,     softbodySimulation->numOfYMasses - 1, index_Z + 1);
      Mass* mass1 = softbodySimulation->getMass(massIndex1); Vector3D* pos1 = &mass1->pos;
      Mass* mass2 = softbodySimulation->getMass(massIndex2); Vector3D* pos2 = &mass2->pos;
      Mass* mass3 = softbodySimulation->getMass(massIndex3); Vector3D* pos3 = &mass3->pos;
      Mass* mass4 = softbodySimulation->getMass(massIndex4); Vector3D* pos4 = &mass4->pos;

      GLfloat fvert1[3] = {pos1->x  - softbodySimulation->offSetX(),
			   pos1->y - softbodySimulation->offSetY(),
			   pos1->z  - softbodySimulation->offSetZ()};
      GLfloat fvert2[3] = {pos2->x  - softbodySimulation->offSetX(),
			   pos2->y - softbodySimulation->offSetY(),
			   pos2->z  - softbodySimulation->offSetZ()};
      GLfloat fvert3[3] = {pos4->x  - softbodySimulation->offSetX(),
			   pos4->y - softbodySimulation->offSetY(),
			   pos4->z  - softbodySimulation->offSetZ()};
      GLfloat fNormalX, fNormalY, fNormalZ;
      CalculateVectorNormal(fvert1, fvert2, fvert3, &fNormalX, &fNormalY, &fNormalZ);
      glNormal3f(fNormalX, fNormalY, fNormalZ);

      glBegin(GL_TRIANGLES);
      glVertex3f(pos1->x  - softbodySimulation->offSetX(),
		 pos1->y - softbodySimulation->offSetY(),
		 pos1->z  - softbodySimulation->offSetZ());
      glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		 pos2->y - softbodySimulation->offSetY(),
		 pos2->z  - softbodySimulation->offSetZ());
      glVertex3f(pos4->x  - softbodySimulation->offSetX(),
		 pos4->y - softbodySimulation->offSetY(),
		 pos4->z  - softbodySimulation->offSetZ());
      glEnd();
      glBegin(GL_TRIANGLES);
      glVertex3f(pos4->x  - softbodySimulation->offSetX(),
		 pos4->y - softbodySimulation->offSetY(),
		 pos4->z  - softbodySimulation->offSetZ());
      glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		 pos2->y - softbodySimulation->offSetY(),
		 pos2->z  - softbodySimulation->offSetZ());
      glVertex3f(pos3->x  - softbodySimulation->offSetX(),
		 pos3->y - softbodySimulation->offSetY(),
		 pos3->z  - softbodySimulation->offSetZ());
      glEnd();
    }
  }

  // Desenha os contornos da malha
  glColor3ub(0, 0, 0);                // Set Color To Black
  glLineWidth(2);
  //int count = 0;
  int fromMass = 0;
  int toMass = 0;
  if (debug) printf("%d, %d, %d\n", softbodySimulation->numOfXMasses, softbodySimulation->numOfYMasses, softbodySimulation->numOfZMasses);
  //exit(0);
  if (debug) printf ("\nMolas em Z:\n ");


  for(int index_X = 0; index_X < softbodySimulation->numOfXMasses; ++index_X) {
      for(int index_Y = 0; index_Y < softbodySimulation->numOfYMasses; ++index_Y) {
          for(int index_Z = 0; index_Z < softbodySimulation->numOfZMasses; ++index_Z) {
              if(index_X < softbodySimulation->numOfXMasses - 1) {
                  fromMass = softbodySimulation->getMassIndex(index_X, index_Y, index_Z);
                  toMass = softbodySimulation->getMassIndex(index_X + 1, index_Y, index_Z);
                  Mass* mass1 = softbodySimulation->getMass(fromMass);

                  Vector3D* pos1 = &mass1->pos;
                  Mass* mass2 = softbodySimulation->getMass(toMass);
                  Vector3D* pos2 = &mass2->pos;
                  glBegin(GL_LINES);
                    glVertex3f(pos1->x  - softbodySimulation->offSetX(),
                    pos1->y - softbodySimulation->offSetY(),
                    pos1->z  - softbodySimulation->offSetZ());
                    glVertex3f(pos2->x  - softbodySimulation->offSetX(),
                    pos2->y - softbodySimulation->offSetY(),
                    pos2->z  - softbodySimulation->offSetZ());
                  glEnd();
              }
              if(index_Y < softbodySimulation->numOfYMasses - 1) {
                  fromMass = softbodySimulation->getMassIndex(index_X, index_Y, index_Z);
                  toMass = softbodySimulation->getMassIndex(index_X, index_Y + 1, index_Z);
                  Mass* mass1 = softbodySimulation->getMass(fromMass);

                  Vector3D* pos1 = &mass1->pos;
                  Mass* mass2 = softbodySimulation->getMass(toMass);
                  Vector3D* pos2 = &mass2->pos;
                  glBegin(GL_LINES);
                    glVertex3f(pos1->x  - softbodySimulation->offSetX(),
                    pos1->y - softbodySimulation->offSetY(),
                    pos1->z  - softbodySimulation->offSetZ());
                    glVertex3f(pos2->x  - softbodySimulation->offSetX(),
                    pos2->y - softbodySimulation->offSetY(),
                    pos2->z  - softbodySimulation->offSetZ());
                  glEnd();
              }

              if(index_Z < softbodySimulation->numOfZMasses - 1) {
                  fromMass = softbodySimulation->getMassIndex(index_X, index_Y, index_Z);
                  toMass = softbodySimulation->getMassIndex(index_X, index_Y, index_Z + 1);
                  Mass* mass1 = softbodySimulation->getMass(fromMass);

                  Vector3D* pos1 = &mass1->pos;
                  Mass* mass2 = softbodySimulation->getMass(toMass);
                  Vector3D* pos2 = &mass2->pos;
                  glBegin(GL_LINES);
                    glVertex3f(pos1->x  - softbodySimulation->offSetX(),
                    pos1->y - softbodySimulation->offSetY(),
                    pos1->z  - softbodySimulation->offSetZ());
                    glVertex3f(pos2->x  - softbodySimulation->offSetX(),
                    pos2->y - softbodySimulation->offSetY(),
                    pos2->z  - softbodySimulation->offSetZ());
                  glEnd();
              }
          }
      }
  }



  /*for (int index_X = 0; index_X < softbodySimulation->numOfXMasses; ++index_X){
    for (int index_Y = 0; index_Y < softbodySimulation->numOfYMasses; ++index_Y){
      for (int index_Z = 0; index_Z < softbodySimulation->numOfZMasses - 1; ++index_Z){
          if(index_Z == 0 || index_Z == softbodySimulation->numOfZMasses - 1) {
              fromMass = softbodySimulation->getMassIndex(index_X, index_Y, index_Z);
              toMass = softbodySimulation->getMassIndex(index_X, index_Y, index_Z + 1);
              Mass* mass1 = softbodySimulation->getMass(fromMass);

              Vector3D* pos1 = &mass1->pos;
              Mass* mass2 = softbodySimulation->getMass(toMass);
              Vector3D* pos2 = &mass2->pos;
              glBegin(GL_LINES);
                glVertex3f(pos1->x  - softbodySimulation->offSetX(),
                pos1->y - softbodySimulation->offSetY(),
                pos1->z  - softbodySimulation->offSetZ());
                glVertex3f(pos2->x  - softbodySimulation->offSetX(),
                pos2->y - softbodySimulation->offSetY(),
                pos2->z  - softbodySimulation->offSetZ());
              glEnd();
          }

        fromMass = softbodySimulation->getMassIndex(index_X, index_Y, index_Z);
        toMass = softbodySimulation->getMassIndex(index_X, index_Y, index_Z + 1);
        Mass* mass1 = softbodySimulation->getMass(fromMass);
        Vector3D* pos1 = &mass1->pos;
        Mass* mass2 = softbodySimulation->getMass(toMass);
        Vector3D* pos2 = &mass2->pos;
        glBegin(GL_LINES);
	glVertex3f(pos1->x  - softbodySimulation->offSetX(),
		   pos1->y - softbodySimulation->offSetY(),
		   pos1->z  - softbodySimulation->offSetZ());
	glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		   pos2->y - softbodySimulation->offSetY(),
		   pos2->z  - softbodySimulation->offSetZ());
	glEnd();
        if (debug) printf ("(%.2f, %.2f, %.2f)-(%.2f, %.2f, %.2f)\n ",
			   pos1->x  - softbodySimulation->offSetX(),
			   pos1->y - softbodySimulation->offSetY(),
			   pos1->z  - softbodySimulation->offSetZ(),
			   pos2->x  - softbodySimulation->offSetX(),
			   pos2->y - softbodySimulation->offSetY(),
			   pos2->z  - softbodySimulation->offSetZ());
        count++;
      }
    }
  }
  if (debug) printf ("\nMolas em X:\n");
  for (int index_Y = 0; index_Y < softbodySimulation->numOfYMasses; ++index_Y){
    for (int index_Z = 0; index_Z < softbodySimulation->numOfZMasses; ++index_Z){
      for (int index_X = 0; index_X < softbodySimulation->numOfXMasses - 1; ++index_X){
        fromMass = softbodySimulation->getMassIndex(index_X, index_Y, index_Z);
        toMass = softbodySimulation->getMassIndex(index_X + 1, index_Y, index_Z);
        Mass* mass1 = softbodySimulation->getMass(fromMass);
        Vector3D* pos1 = &mass1->pos;
        Mass* mass2 = softbodySimulation->getMass(toMass);
        Vector3D* pos2 = &mass2->pos;
	glBegin(GL_LINES);
	glVertex3f(pos1->x  - softbodySimulation->offSetX(),
		   pos1->y - softbodySimulation->offSetY(),
		   pos1->z  - softbodySimulation->offSetZ());
	glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		   pos2->y - softbodySimulation->offSetY(),
		   pos2->z  - softbodySimulation->offSetZ());
	glEnd();
        if (debug) printf ("(%.2f, %.2f, %.2f)-(%.2f, %.2f, %.2f)\n",
			   pos1->x  - softbodySimulation->offSetX(),
			   pos1->y - softbodySimulation->offSetY(),
			   pos1->z  - softbodySimulation->offSetZ(),
			   pos2->x  - softbodySimulation->offSetX(),
			   pos2->y - softbodySimulation->offSetY(),
			   pos2->z  - softbodySimulation->offSetZ());
        count++;
      }
    }
  }
  if (debug) printf ("\nMolas em Y:\n");
  for (int index_Z = 0; index_Z < softbodySimulation->numOfZMasses; ++index_Z){
    for (int index_X = 0; index_X < softbodySimulation->numOfXMasses; ++index_X){
      for (int index_Y = 0; index_Y < softbodySimulation->numOfYMasses - 1; ++index_Y){
        fromMass = softbodySimulation->getMassIndex(index_X, index_Y, index_Z);
        toMass = softbodySimulation->getMassIndex(index_X, index_Y + 1, index_Z);
        Mass* mass1 = softbodySimulation->getMass(fromMass);
        Vector3D* pos1 = &mass1->pos;
        Mass* mass2 = softbodySimulation->getMass(toMass);
        Vector3D* pos2 = &mass2->pos;
	glBegin(GL_LINES);
	glVertex3f(pos1->x  - softbodySimulation->offSetX(),
		   pos1->y - softbodySimulation->offSetY(),
		   pos1->z  - softbodySimulation->offSetZ());
	glVertex3f(pos2->x  - softbodySimulation->offSetX(),
		   pos2->y - softbodySimulation->offSetY(),
		   pos2->z  - softbodySimulation->offSetZ());
	glEnd();
        if (debug) printf ("(%.2f, %.2f, %.2f)-(%.2f, %.2f, %.2f)\n",
			   pos1->x  - softbodySimulation->offSetX(),
			   pos1->y - softbodySimulation->offSetY(),
			   pos1->z  - softbodySimulation->offSetZ(),
			   pos2->x  - softbodySimulation->offSetX(),
			   pos2->y - softbodySimulation->offSetY(),
			   pos2->z  - softbodySimulation->offSetZ());
        count++;
      }
    }
  }*/
  cursor->draw();
}

// Função callback de redesenho da janela de visualização
void Desenha(void)
{
  // Limpa a janela de visualização com a cor
  // de fundo definida previamente
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Chama a função que especifica os parâmetros de iluminação
  DefineIluminacao();

  DesenhaCuboRGB();

  // Executa os comandos OpenGL
  glutSwapBuffers();
}

// Função usada para especificar a posição do observador virtual
void PosicionaObservador(void)
{
  // Especifica sistema de coordenadas do modelo
  glMatrixMode(GL_MODELVIEW);
  // Inicializa sistema de coordenadas do modelo
  glLoadIdentity();

  DefineIluminacao();
  // Posiciona e orienta o observador
  glTranslatef(-obsX,-obsY,-obsZ);
  glRotatef(rotX,1,0,0);
  glRotatef(rotY,0,1,0);
}

// Função usada para especificar o volume de visualização
void EspecificaParametrosVisualizacao(void)
{
  // Especifica sistema de coordenadas de projeção
  glMatrixMode(GL_PROJECTION);
  // Inicializa sistema de coordenadas de projeção
  glLoadIdentity();

  // Especifica a projeção perspectiva
  // (angulo, aspecto, zMin, zMax)
  gluPerspective(angle,fAspect,0.5,1000);

  PosicionaObservador();
}

// Função callback chamada para gerenciar eventos de teclas normais (ESC)
void Teclado (unsigned char tecla, int x, int y)
{
  switch ( tecla ) {
  case 27: // ESC !
    exit(0);
    break;
  case 'F':
  case 'f':
    if (fullScreen){
      glutPositionWindow (50,50);
      glutReshapeWindow ( 700, 500 );
    }
    else
      glutFullScreen ( );   // Go Into Full Screen Mode
    fullScreen = !fullScreen;
    break;
  }
}

// Função callback para tratar eventos de teclas especiais
void TeclasEspeciais (int tecla, int x, int y)  {
  Vector3D softbodyConnectionVel;  // Create A Temporary Vector3D
  switch (tecla){
  case GLUT_KEY_HOME:	//if(angle>=10)  angle -=5;
    softbodyConnectionVel.z += 150.0f;  // Add Velocity In +Y Direction
    break;
  case GLUT_KEY_END:	//if(angle<=150) angle +=5;
    softbodyConnectionVel.z -= 150.0f;  // Add Velocity In +Y Direction
    break;
  case GLUT_KEY_F1:
    softbodyConnectionVel.x += 150.0f;  // Add Velocity In +Y Direction
    break;
  case GLUT_KEY_F2:
    softbodyConnectionVel.x -= 150.0f;  // Add Velocity In +Y Direction
    break;
  case GLUT_KEY_UP:
    if (cursor->y < softbodySimulation->numOfYMasses - 1)
      cursor->incY();
    break;
  case GLUT_KEY_DOWN:
    if (cursor->y  > 0)
      cursor->decY();
    break;
  case GLUT_KEY_RIGHT:
    if (cursor->x < softbodySimulation->numOfXMasses - 1)
      cursor->incX();
    break;
  case GLUT_KEY_LEFT:
    if (cursor->x > 0)
      cursor->decX();
    break;
  case GLUT_KEY_PAGE_UP:
    if(cursor->z < softbodySimulation->numOfZMasses - 1)
    cursor->incZ();
    break;
  case GLUT_KEY_INSERT:
    if(cursor->z > 0)
    cursor->decZ();
    break;
  default:
    softbodyConnectionVel.z = 0.0f;  // Add Velocity In +Y Direction
  }
  // Set The Obtained softbodyConnectionVel In The Simulation
  softbodySimulation->setSoftBodyConnectionVel(softbodyConnectionVel);
}

// Função callback para eventos de botões do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
  if(state==GLUT_DOWN)
    {
      // Salva os parâmetros atuais
      x_ini = x;
      y_ini = y;
      obsX_ini = obsX;
      obsY_ini = obsY;
      obsZ_ini = obsZ;
      rotX_ini = rotX;
      rotY_ini = rotY;
      bot = button;
    }
  else bot = -1;
}

// Função callback para eventos de movimento do mouse
#define SENS_ROT	5.0
#define SENS_OBS	10.0
#define SENS_TRANSL	10.0
void GerenciaMovim(int x, int y)
{
  // Botão esquerdo ?
  if(bot==GLUT_LEFT_BUTTON)
    {
      // Calcula diferenças
      int deltax = x_ini - x;
      int deltay = y_ini - y;
      // E modifica ângulos
      rotY = rotY_ini - deltax/SENS_ROT;
      rotX = rotX_ini - deltay/SENS_ROT;
    }
  // Botão direito ?
  else if(bot==GLUT_RIGHT_BUTTON)
    {
      // Calcula diferença
      int deltaz = y_ini - y;
      // E modifica distância do observador
      obsZ = obsZ_ini + deltaz/SENS_OBS;
    }
  // Botão do meio ?
  else if(bot==GLUT_MIDDLE_BUTTON)
    {
      // Calcula diferenças
      int deltax = x_ini - x;
      int deltay = y_ini - y;
      // E modifica posições
      obsX = obsX_ini + deltax/SENS_TRANSL;
      obsY = obsY_ini - deltay/SENS_TRANSL;
    }
  PosicionaObservador();
  glutPostRedisplay();
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
  // Para previnir uma divisão por zero
  if ( h == 0 ) h = 1;

  // Especifica as dimensões da viewport
  glViewport(0, 0, w, h);

  // Calcula a correção de aspecto
  fAspect = (GLfloat)w/(GLfloat)h;

  EspecificaParametrosVisualizacao();
}

// Função responsável por inicializar parâmetros e variáveis
void Inicializa (void)
{
  // Define a cor de fundo da janela de visualização como branca
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // Habilita a definição da cor do material a partir da cor corrente
  glEnable(GL_COLOR_MATERIAL);
  //Habilita o uso de iluminação
  glEnable(GL_LIGHTING);
  // Habilita a luz de número 0
  glEnable(GL_LIGHT0);
  // Habilita a luz de número 1
  glEnable(GL_LIGHT1);
  // Habilita o depth-buffering
  glEnable(GL_DEPTH_TEST);

  // Habilita o modelo de colorização de Gouraud
  glShadeModel(GL_SMOOTH);

  // Inicializa a variável que especifica o ângulo da projeção
  // perspectiva
  angle=45;

  // Inicializa as variáveis usadas para alterar a posição do
  // observador virtual
  rotX = 0;
  rotY = 0;
  obsX = obsY = 0;
  obsZ = 150;
}


void printTime(void){
  int time, milliseconds;
  Vector3D softbodyConnectionVel;  // Create A Temporary Vector3D
  time = glutGet(GLUT_ELAPSED_TIME);
  milliseconds = time - lastTime;
  if (milliseconds){
    //printf("%d\n", time - lastTime);

    float dt = milliseconds / 1000.0f;// Let's Convert Milliseconds To Seconds
    float maxPossible_dt = 0.002f; // Maximum Possible dt Is 0.002 Seconds
    // This Is Needed To Prevent Pass Over Of A Non-Precise dt Value
    // Calculate Number Of Iterations To Be Made At This Update Depending On maxPossible_dt And dt
    int numOfIterations = (int)(dt / maxPossible_dt) + 1;
    if (numOfIterations != 0)    // Avoid Division By Zero
      dt = dt / numOfIterations; // dt Should Be Updated According To numOfIterations
    for (int a = 0; a < numOfIterations; ++a){ // We Need To Iterate Simulations "numOfIterations" Times
      softbodySimulation->operate(dt);
    }
    EspecificaParametrosVisualizacao();
    //DesenhaCuboRGB();
    //glutSwapBuffers();
    glutPostRedisplay();
    softbodyConnectionVel.z = 0.0f;  // Add Velocity In +Y Direction
    softbodySimulation->setSoftBodyConnectionVel(softbodyConnectionVel);
  }
  lastTime = time;
}


// Programa Principal
int main(int argc, char **argv){

  softbodySimulation = new
    SoftBodySimulation(
		       10,         // 3 Particles (Masses) on X
		       10,         // 3 Particles (Masses) on Y
		       2,          // 2 Particles (Masses) on Z
		       0.05f,      // Each Particle Has A Weight Of 50 Grams
		       40.0f,      // springConstant In The SoftBody
		       10.0f,       // Normal Length Of Springs In The SoftBody
		       3.0f,       // Spring Inner Friction Constant
		       Vector3D(0, 0, 0), // Gravitational Acceleration
		       3.f,        // Air Friction Constant
		       0.0f,       // Ground Repel Constant
		       0.0f,       // Ground Slide Friction Constant
		       0.0f,       // Ground Absoption Constant
		       50.0f);     // Height Of Ground

  cursor = new ForceApplyCursor(0, 0, softbodySimulation->numOfZMasses - 1, softbodySimulation);

  glutInit(&argc, argv);
  // Define do modo de operação da GLUT
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // Especifica a posição inicial da janela GLUT
  glutInitWindowPosition(50,50);

  // Especifica o tamanho inicial em pixels da janela GLUT
  glutInitWindowSize(700, 500);

  // Cria a janela passando como argumento o título da mesma
  glutCreateWindow("Simulador de sensor por TIE");

  // Registra a função callback de redesenho da janela de visualização
  glutDisplayFunc(Desenha);

  // Registra a função callback de redimensionamento da janela de visualização
  glutReshapeFunc(AlteraTamanhoJanela);

  // Registra a função callback para tratamento das teclas normais
  glutKeyboardFunc (Teclado);

  // Registra a função callback para tratamento das teclas especiais
  glutSpecialFunc (TeclasEspeciais);

  // Registra a função callback para eventos de botões do mouse
  glutMouseFunc(GerenciaMouse);

  // Registra a função callback para eventos de movimento do mouse
  glutMotionFunc(GerenciaMovim);

  glutIdleFunc(printTime);

  // Chama a função responsável por fazer as inicializações
  Inicializa();

  // Inicia o processamento e aguarda interações do usuário
  glutMainLoop();

  return 0;
}
