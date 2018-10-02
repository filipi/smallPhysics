/**************************************************************************

  File: Physics3.h

  Prepared by Filipi Vianna for PUCRS grad school anim and CG classes

**************************************************************************/

#include "Physics2.h"     //Physics1.h is a must for Physics3.h simulations
#include <stdio.h>

/**
 * class SoftBodySimulation is derived from class Simulation (see Physics1.h).
 * It simulates a softbody with point-like particles binded with springs.
 * The springs have inner friction and normal length. One tip of the softbody
 * is stabilized at a point in space called "Vector3D softbodyConnectionPos".
 * This point can be moved externally by a method
 * "void setSoftBodyConnectionVel(Vector3D softbodyConnectionVel)". SoftBodySimulation
 * creates air friction and a planer surface (or ground) with a
 * normal in +y direction. SoftBodySimulation implements the force applied
 * by this surface. In the code, the surface is refered as "ground".
 */
class SoftBodySimulation : public Simulation
{       //An object to simulate a softbody interacting with a planer surface and air
public:
  Vector3D** massesPositions; // Masses possitins on the solid geometry

  int numOfSprings;
  int numOfXMasses;
  int numOfYMasses;
  int numOfZMasses;
  int massIndexOfCursor;

  Spring** springs;                //Springs binding the masses (there shall
                                   //be [numOfMasses - 1] of them)

  Vector3D gravitation;            // gravitational acceleration (gravity
                                   // will be applied to all masses)

  Vector3D softbodyConnectionPos;      // A point in space that is used to set
                                   // the position of the first mass in the
                                   // system (mass with index 0)

  Vector3D softbodyConnectionVel;      //a variable to move the softbodyConnectionPos
                                   //(by this, we can swing the softbody)

  float groundRepulsionConstant;  // a constant to represent how much the
                                  // ground shall repel the masses

  float groundFrictionConstant;   // a constant of friction applied to
                                  // masses by the ground (used for the
                                  // sliding of softbody on the ground)

  float groundAbsorptionConstant; // a constant of absorption friction
                                  // applied to masses by the ground
                                  // (used for vertical collisions of
                                  // the softbody with the ground)

  float groundHeight;             //a value to represent the y position
                                  //value of the ground (the ground is a
                                  //planer surface facing +y direction)

  float airFrictionConstant;     //a constant of air friction applied
                                 //to masses

  float springLength;
  void setConnectionPos(Vector3D* pos){
    this->softbodyConnectionPos = *pos;
  }
  float offSetX(void){
    if (this->numOfXMasses>1)
      return (this->numOfXMasses / 2) * springLength;
    else
      return springLength;
  }
  float offSetY(void){
    if (this->numOfYMasses>1)
      return (this->numOfYMasses / 2) * springLength;
    else
      return springLength;
  }
  float offSetZ(void){
    if (this->numOfZMasses>1)
      return (this->numOfZMasses / 2) * springLength;
    else
      return springLength;
  }

  SoftBodySimulation(  //a long long constructor with 11 parameters starts here
    int numOfXMasses,                 //1. the number of masses
    int numOfYMasses,                 //1. the number of masses
    int numOfZMasses,                 //1. the number of masses
    float m,                          //2. weight of each mass
    float springConstant,             //3. how stiff the springs are
    float springLength,               //4. the length that a spring does not exert any force
    float springFrictionConstant,     //5. inner friction constant of spring
    Vector3D gravitation,             //6. gravitational acceleration
    float airFrictionConstant,        //7. air friction constant
    float groundRepulsionConstant,    //8. ground repulsion constant
    float groundFrictionConstant,     //9. ground friction constant
    float groundAbsorptionConstant,   //10. ground absorption constant
    float groundHeight                //11. height of the ground (y position)
    ) : Simulation(numOfXMasses*numOfYMasses*numOfZMasses, m)    //The super class creates masses with weights m of each
  {
    this->springLength = springLength;
    this->massIndexOfCursor = 0;
    this->numOfXMasses = numOfXMasses;
    this->numOfYMasses = numOfYMasses;
    this->numOfZMasses = numOfZMasses;

    this->gravitation = gravitation;

    this->airFrictionConstant = airFrictionConstant;

    this->groundFrictionConstant = groundFrictionConstant;
    this->groundRepulsionConstant = groundRepulsionConstant;
    this->groundAbsorptionConstant = groundAbsorptionConstant;
    this->groundHeight = groundHeight;


    //printf("Creating masses\n");
    // Create an array of pointers
    massesPositions = new Vector3D*[numOfMasses];
    //printf("Passei\n");

    //printf("Num of masses: %d\n", numOfMasses);
    int count = 0;
    for (int index_X = 0; index_X < numOfXMasses; ++index_X){
      for (int index_Y = 0; index_Y < numOfYMasses; ++index_Y){
        for (int index_Z = 0; index_Z < numOfZMasses; ++index_Z){
          // Create a Mass as a pointer and put it in the array
          massesPositions[count] = new Vector3D(index_X, index_Y, index_Z);
          //massMap.setPos(index_X, index_Y, index_Z, count);
          //printf("%d\t%d\t%d\t%d\n", index_X, index_Y, index_Z, count);
          masses[count]->pos.x = index_X * springLength;
          masses[count]->pos.y = index_Y * springLength;
          masses[count]->pos.z = index_Z * springLength;
          ////printf("%.2f\t%.2f\t%.2f\t\n", masses[count]->pos.x, masses[count]->pos.y, masses[count]->pos.z);
          count++;
        }
      }
    }

    /*  Pra que fazer as iteracoes se pode fazer o calculo direto??
    //printf("Counting springs\n");
    count = 0;
    for (int index_X = 0; index_X < numOfXMasses; ++index_X){
      for (int index_Y = 0; index_Y < numOfYMasses; ++index_Y){
        for (int index_Z = 0; index_Z < numOfZMasses - 1; ++index_Z){
          count++;
        }
      }
    }
    //printf("%d springs until now\n", count);
    for (int index_Y = 0; index_Y < numOfYMasses; ++index_Y){
      for (int index_Z = 0; index_Z < numOfZMasses; ++index_Z){
        for (int index_X = 0; index_X < numOfXMasses - 1; ++index_X){
          count++;
        }
      }
    }
    //printf("%d springs until now\n", count);
    for (int index_Z = 0; index_Z < numOfZMasses; ++index_Z){
      for (int index_X = 0; index_X < numOfXMasses; ++index_X){
        for (int index_Y = 0; index_Y < numOfYMasses - 1; ++index_Y){
          count++;
        }
      }
    }*/

    int count2 = ((numOfXMasses-1)*numOfYMasses + (numOfYMasses-1)*numOfXMasses)*numOfZMasses + //Springs do topo vezes a altura
                    (numOfZMasses-1)*numOfXMasses*numOfYMasses; //Springs verticais da frente vezes o comprimento

    //printf("%d springs until now\n", count);
    //printf("%d springs I calculated", count2);
    numOfSprings = count2;

    springs = new Spring*[numOfSprings];      //create [numOfMasses - 1] pointers for springs
                            //([numOfMasses - 1] springs are necessary for numOfMasses)
    //exit(0);

    //printf("Creating springs\n");
    count = 0;
    int fromMass = 0;
    int toMass = 0;
    //printf("Springs on Z\n");

    for(int indeX = 0; indeX < numOfXMasses; ++indeX) {
        for(int indeY = 0; indeY < numOfYMasses; ++indeY) {
            for(int indeZ = 0; indeZ < numOfZMasses; ++indeZ) {
                // Calcula Z
                if(indeZ < numOfZMasses - 1) {
                    fromMass = this->getMassIndex(indeX, indeY, indeZ);
                    toMass = this->getMassIndex(indeX, indeY, indeZ + 1);
                    springs[count] = new Spring(masses[fromMass], masses[toMass],springConstant, springLength, springFrictionConstant);
                    count++;
                }
                // Calcula X
                if(indeX < numOfXMasses - 1) {
                    fromMass = this->getMassIndex(indeX, indeY, indeZ);
                    toMass = this->getMassIndex(indeX + 1, indeY, indeZ);
                    springs[count] = new Spring(masses[fromMass], masses[toMass],springConstant, springLength, springFrictionConstant);
                    count++;
                }
                // Calcula Y
                if(indeY < numOfYMasses - 1) {
                    fromMass = this->getMassIndex(indeX, indeY, indeZ);
                    toMass = this->getMassIndex(indeX, indeY + 1, indeZ);
                    springs[count] = new Spring(masses[fromMass], masses[toMass],springConstant, springLength, springFrictionConstant);
                    count++;
                }
            }
        }
    }
    /*printf("Num of Springs: %i", count);
    count = 0;

    for (int index_X = 0; index_X < numOfXMasses; ++index_X){
      for (int index_Y = 0; index_Y < numOfYMasses; ++index_Y){
        for (int index_Z = 0; index_Z < numOfZMasses - 1; ++index_Z){
          fromMass = this->getMassIndex(index_X, index_Y, index_Z);
          toMass = this->getMassIndex(index_X, index_Y, index_Z + 1);
          springs[count] = new Spring(masses[fromMass], masses[toMass],
	    springConstant, springLength, springFrictionConstant);
          //printf("Spring from mass %d to mass %d\n", fromMass, toMass);
          count++;
        }
      }
    }
    //printf("Springs on X\n");
    for (int index_Y = 0; index_Y < numOfYMasses; ++index_Y){
      for (int index_Z = 0; index_Z < numOfZMasses; ++index_Z){
        for (int index_X = 0; index_X < numOfXMasses - 1; ++index_X){
          fromMass = this->getMassIndex(index_X, index_Y, index_Z);
          toMass = this->getMassIndex(index_X + 1, index_Y, index_Z);
          springs[count] = new Spring(masses[fromMass], masses[toMass],
	  springConstant, springLength, springFrictionConstant);
          //printf("Spring from mass %d to mass %d\n", fromMass, toMass);
          count++;
        }
      }
    }
    //printf("Springs on Y\n");
    for (int index_Z = 0; index_Z < numOfZMasses; ++index_Z){
      for (int index_X = 0; index_X < numOfXMasses; ++index_X){
        for (int index_Y = 0; index_Y < numOfYMasses - 1; ++index_Y){
          fromMass = this->getMassIndex(index_X, index_Y, index_Z);
          toMass = this->getMassIndex(index_X, index_Y + 1, index_Z);
          springs[count] = new Spring(masses[fromMass], masses[toMass],
	    springConstant, springLength, springFrictionConstant);
          //printf("Spring from mass %d to mass %d\n", fromMass, toMass);
          count++;
          }
      }
    }
    printf("\nNum of Springs: %i", count);*/
  }

  void release()                    //release() is overriden because we have springs to delete
  {
    Simulation::release();              //Have the super class release itself

    for (int index = 0; index < numOfSprings; ++index)    //to delete all springs, start a loop
    {
      delete(springs[index]);
      springs[index] = NULL;
    }

    delete(springs);
    springs = NULL;
  }

  void solve()//solve() is overriden because we have forces to be applied
  {
    for (int index = 0; index < numOfSprings; ++index)    //apply force of all springs
    {
      springs[index]->solve();            //Spring with index "a" should apply its force
    }

    for (int index = 0; index < numOfMasses; ++index)        //Start a loop to apply forces which are common for all masses
    {
      masses[index]->applyForce(gravitation * masses[index]->m);        //The gravitational force

      masses[index]->applyForce(-masses[index]->vel * airFrictionConstant);  //The air friction

      if (masses[index]->pos.y < groundHeight)    //Forces from the ground are applied if a mass collides with the ground
      {
        Vector3D v;                //A temporary Vector3D

        v = masses[index]->vel;            //get the velocity
        v.y = 0;                //omit the velocity component in y direction

        //The velocity in y direction is omited because we will apply a friction force to create
        //a sliding effect. Sliding is parallel to the ground. Velocity in y direction will be used
        //in the absorption effect.
        masses[index]->applyForce(-v * groundFrictionConstant);    //ground friction force is applied

        v = masses[index]->vel;            //get the velocity
        v.x = 0;                //omit the x and z components of the velocity
        v.z = 0;                //we will use v in the absorption effect

        //above, we obtained a velocity which is vertical to the ground and it will be used in
        //the absorption force

        if (v.y < 0)              //let's absorb energy only when a mass collides towards the ground
          masses[index]->applyForce(-v * groundAbsorptionConstant);    //the absorption force is applied

        //The ground shall repel a mass like a spring.
        //By "Vector3D(0, groundRepulsionConstant, 0)" we create a vector in the plane normal direction
        //with a magnitude of groundRepulsionConstant.
        //By (groundHeight - masses[a]->pos.y) we repel a mass as much as it crashes into the ground.
        Vector3D force = Vector3D(0, groundRepulsionConstant, 0) *
          (groundHeight - masses[index]->pos.y);

        masses[index]->applyForce(force);      //The ground repulsion force is applied
      }
    }
  }

  void simulate(float dt) //simulate(float dt) is overriden because we want to simulate
                          //the motion of the softbodyConnectionPos
  {
    Simulation::simulate(dt);            //the super class shall simulate the masses

    softbodyConnectionPos += softbodyConnectionVel * dt;  //iterate the positon of softbodyConnectionPos

    if (softbodyConnectionPos.y < groundHeight)      //softbodyConnectionPos shall not go under the ground
    {
      softbodyConnectionPos.y = groundHeight;
      //softbodyConnectionVel.y = 0;
    }

    //masses[0]->pos = softbodyConnectionPos;        //mass with index "0" shall position at softbodyConnectionPos
/*     int i = 0; */
/*     Vector3D* pos = &softbodyConnectionPos; */
/*     i = this->getMassIndex(pos); */
/*     //i = this->getMassIndex(softbodyConnectionPos->x, softbodyConnectionPos->y, softbodyConnectionPos->z); */
/*     printf("%d\n", i); */

    masses[massIndexOfCursor]->vel = softbodyConnectionVel;        //the mass's velocity is set to be equal to softbodyConnectionVel
  }

  void setSoftBodyConnectionVel(Vector3D softbodyConnectionVel)  //the method to set softbodyConnectionVel
  {
    this->softbodyConnectionVel = softbodyConnectionVel;
  }

  int getMassIndex(int x, int y, int z){
    int i = 0;
    while ( this->massesPositions[i]->x != x ||
            this->massesPositions[i]->y != y ||
            this->massesPositions[i]->z != z )
     i++;
    return i;
  }
  int getMassIndex(Vector3D* pos){
    int i = -1;
    Mass* mass;
    Vector3D* massPos;
    do{
      i++;
      mass = getMass(i);
      massPos = &mass->pos;
      //printf("%f, %f, %f, %d\n", pos->x, pos->y, pos->z, i);
      //printf("%f, %f, %f, %d. %d\n", massPos->x, massPos->y, massPos->z, i, numOfMasses);
    } while( (pos->x != massPos->x ||
              pos->y != massPos->y ||
              pos->z != massPos->z  ) && i < (numOfMasses - 1));
    //exit(0);
    return i;
  }
};

GLvoid CalculateVectorNormal(GLfloat fVert1[], GLfloat fVert2[],
                             GLfloat fVert3[], GLfloat *fNormalX,
                             GLfloat *fNormalY, GLfloat *fNormalZ){
  GLfloat Qx, Qy, Qz, Px, Py, Pz;

  Qx = fVert2[0]-fVert1[0];
  Qy = fVert2[1]-fVert1[1];
  Qz = fVert2[2]-fVert1[2];
  Px = fVert3[0]-fVert1[0];
  Py = fVert3[1]-fVert1[1];
  Pz = fVert3[2]-fVert1[2];

  *fNormalX = Py*Qz - Pz*Qy;
  *fNormalY = Pz*Qx - Px*Qz;
  *fNormalZ = Px*Qy - Py*Qx;
  return;
}


class ForceApplyCursor{
public:
  int x, y, z, lastX, lastY, lastZ, massIndex;
  SoftBodySimulation* simulation;
  ForceApplyCursor(int x, int y, int z, SoftBodySimulation* simulation){
    this->x = x;
    this->y = y;
    this->z = z;
    this->simulation = simulation;
    updatePos();        // agora inicializa certo =D
  }
  void updatePos(){
    this->massIndex = simulation->getMassIndex(this->x, this->y, this->z);
    Mass* mass = simulation->getMass(this->massIndex);
    Vector3D* pos = &mass->pos;
    this->simulation->setConnectionPos(pos);
    simulation->massIndexOfCursor = simulation->getMassIndex(pos);
    //i = this->getMassIndex(softbodyConnectionPos->x, softbodyConnectionPos->y, softbodyConnectionPos->z);
    ////printf("%d\n", i);

  }
  void incX(){
    this->lastX = this->x;
    this->x++;
    this->updatePos();
  };
  void decX(){
    this->lastX = this->x;
    this->x--;
    this->updatePos();
  };
  void incY(){
    this->lastY = this->y;
    this->y++;
    this->updatePos();
  };
  void decY(){
    this->lastY = this->y;
    this->y--;
    this->updatePos();
  };
  void incZ(){
    this->lastZ = this->z;
    this->z++;
    this->updatePos();
  };
  void decZ(){
      this->lastZ = this->z;
      this->z--;
      this->updatePos();
  };
  void draw(){
    this->massIndex = simulation->getMassIndex(this->x, this->y, this->z);
    Mass* mass = simulation->getMass(this->massIndex);
    Vector3D* pos = &mass->pos;

    glColor3ub(255,0,0);

    glPushMatrix();
    glTranslated(pos->x - simulation->offSetX(), pos->y - simulation->offSetY(), pos->z - simulation->offSetZ());
    glutSolidSphere(1.0, 10, 10);
    glPopMatrix();
  }
};
