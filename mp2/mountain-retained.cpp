#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

float sealevel;

int res = 257;
int nFPS = 30;
float velocity = 0.0020;
float fRotateAngle1 = 0.0f, fRotateAngle2 = 0.0f;
float pos[3] = {-2.0f, 0.0f, 1.0f};
float target[4] = {1.0f, 0.0f, 0.0f, 1.0f};
float up[4] = {0.0f, 0.0f, 1.0f, 1.0f};
float v[3], u[3], r[3];
float rotation[4][4];
float result[4][4];
float R[4][4]; 
float RT[4][4];
float oneof[4][4];

#define ADDR(i,j,k) (3*((j)*res + (i)) + (k))


GLfloat *verts = 0;
GLfloat *norms = 0;
GLuint *faces = 0;


float frand(float x, float y) {
    static int a = 1588635695, b = 1117695901;
    int xi = *(int *)&x;
	int yi = *(int *)&y;
    srand(((xi * a) % b) - ((yi * b) % a));
	return 2.0*((float)rand()/(float)RAND_MAX) - 1.0;
}

void mountain(int i, int j, int s)
{
	if (s > 1) {

		float x0,y0,z0,x1,y1,z1,x2,y2,z2,x3,y3,z3,x01,y01,z01,x02,y02,z02,x13,y13,z13,x23,y23,z23,x0123,y0123,z0123;

		x0 = verts[ADDR(i,j,0)];
		y0 = verts[ADDR(i,j,1)];
		z0 = verts[ADDR(i,j,2)];

		x1 = verts[ADDR(i+s,j,0)];
		y1 = verts[ADDR(i+s,j,1)];
		z1 = verts[ADDR(i+s,j,2)];

		x2 = verts[ADDR(i,j+s,0)];
		y2 = verts[ADDR(i,j+s,1)];
		z2 = verts[ADDR(i,j+s,2)];

		x3 = verts[ADDR(i+s,j+s,0)];
		y3 = verts[ADDR(i+s,j+s,1)];
		z3 = verts[ADDR(i+s,j+s,2)];

		x01 = 0.5*(x0 + x1);
		y01 = 0.5*(y0 + y1);
		z01 = 0.5*(z0 + z1);

		x02 = 0.5*(x0 + x2);
		y02 = 0.5*(y0 + y2);
		z02 = 0.5*(z0 + z2);

		x13 = 0.5*(x1 + x3);
		y13 = 0.5*(y1 + y3);
		z13 = 0.5*(z1 + z3);

		x23 = 0.5*(x2 + x3);
		y23 = 0.5*(y2 + y3);
		z23 = 0.5*(z2 + z3);

		x0123 = 0.25*(x0 + x1 + x2 + x3);
		y0123 = 0.25*(y0 + y1 + y2 + y3);
		z0123 = 0.25*(z0 + z1 + z2 + z3);

		z01 += 0.5*((float)s/res)*frand(x01,y01);
		z02 += 0.5*((float)s/res)*frand(x02,y02);
		z13 += 0.5*((float)s/res)*frand(x13,y13);
		z23 += 0.5*((float)s/res)*frand(x23,y23);
		z0123 += 0.5*((float)s/res)*frand(x0123,y0123);

		verts[ADDR(i+s/2,j,0)] = x01;
		verts[ADDR(i+s/2,j,1)] = y01;
		verts[ADDR(i+s/2,j,2)] = z01;

		verts[ADDR(i,j+s/2,0)] = x02;
		verts[ADDR(i,j+s/2,1)] = y02;
		verts[ADDR(i,j+s/2,2)] = z02;

		verts[ADDR(i+s,j+s/2,0)] = x13;
		verts[ADDR(i+s,j+s/2,1)] = y13;
		verts[ADDR(i+s,j+s/2,2)] = z13;

		verts[ADDR(i+s/2,j+s,0)] = x23;
		verts[ADDR(i+s/2,j+s,1)] = y23;
		verts[ADDR(i+s/2,j+s,2)] = z23;

		verts[ADDR(i+s/2,j+s/2,0)] = x0123;
		verts[ADDR(i+s/2,j+s/2,1)] = y0123;
		verts[ADDR(i+s/2,j+s/2,2)] = z0123;

		mountain(i,j,s/2);
		mountain(i+s/2,j,s/2);
		mountain(i,j+s/2,s/2);
		mountain(i+s/2,j+s/2,s/2);

	} else {

		float dx,dy,dz;

		if (i == 0) {
			dx = verts[ADDR(i+1,j,2)] - verts[ADDR(i,j,2)];
		} else if (i == res-1) {
			dx = verts[ADDR(i,j,2)] - verts[ADDR(i-1,j,2)];
		} else {
			dx = (verts[ADDR(i+1,j,2)] - verts[ADDR(i-1,j,2)])/2.0;
		}

		if (j == 0) {
			dy = verts[ADDR(i,j+1,2)] - verts[ADDR(i,j,2)];
		} else if (j == res-1) {
			dy = verts[ADDR(i,j,2)] - verts[ADDR(i,j-1,2)];
		} else {
			dy = (verts[ADDR(i,j+1,2)] - verts[ADDR(i,j-1,2)])/2.0;
		}

		dx *= res;
		dy *= res;
		dz = 1.0/sqrt(dx*dx + dy*dy + 1.0);
		dx *= dz;
		dy *= dz;

		norms[ADDR(i,j,0)] = dx;
		norms[ADDR(i,j,1)] = dy;
		norms[ADDR(i,j,2)] = dz;
	}
}

void makemountain()
{
	int i,j;

	if (verts) free(verts);
	if (norms) free(norms);
	if (faces) free(faces);

	verts = (GLfloat *)malloc(res*res*3*sizeof(GLfloat));
	norms = (GLfloat *)malloc(res*res*3*sizeof(GLfloat));
	faces = (GLuint *)malloc((res-1)*(res-1)*6*sizeof(GLuint));

	verts[ADDR(0,0,0)] = -5.0;
	verts[ADDR(0,0,1)] = -5.0;
	verts[ADDR(0,0,2)] = 0.0;

	verts[ADDR(res-1,0,0)] = 5.0;
	verts[ADDR(res-1,0,1)] = -5.0;
	verts[ADDR(res-1,0,2)] = 0.0;

	verts[ADDR(0,res-1,0)] = -5.0;
	verts[ADDR(0,res-1,1)] = 5.0;
	verts[ADDR(0,res-1,2)] = 0.0;

	verts[ADDR(res-1,res-1,0)] = 5.0;
	verts[ADDR(res-1,res-1,1)] = 5.0;
	verts[ADDR(res-1,res-1,2)] = 0.0;

	mountain(0,0,res-1);

	GLuint *f = faces;
	for (j = 0; j < res-1; j++) {
		for (i = 0; i < res-1; i++) {
			*f++ = j*res + i;
			*f++ = j*res + i + 1;
			*f++ = (j+1)*res + i + 1;
			*f++ = j*res + i;
			*f++ = (j+1)*res + i + 1;
			*f++ = (j+1)*res + i;
		}
	}

}

void init(void) 
{
	GLfloat amb[] = {0.2,0.2,0.2};
	GLfloat diff[] = {1.0,1.0,1.0};
	GLfloat spec[] = {1.0,1.0,1.0};

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

	glClearColor (0.5, 0.5, 1.0, 0.0);	// sky
	glEnable(GL_DEPTH_TEST);

	sealevel = 0.0;

	makemountain();	
	
}

void display(void)
{
	GLfloat tanamb[] = {0.2,0.15,0.1,1.0};
	GLfloat tandiff[] = {0.4,0.3,0.2,1.0};
	GLfloat tanspec[] = {0.0,0.0,0.0,1.0};	// dirt doesn't glisten

	GLfloat seaamb[] = {0.0,0.0,0.2,1.0};
	GLfloat seadiff[] = {0.0,0.0,0.8,1.0};
	GLfloat seaspec[] = {0.5,0.5,1.0,1.0};	// Single polygon, will only have highlight if light hits a vertex just right

	GLfloat lpos[] = {0.0,0.0,10.0,0.0};	// sun, high noon


	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);
	glLoadIdentity ();             /* clear the matrix */
	
	
		
	/* viewing transformation, look at the origin  */
	gluLookAt (pos[0], pos[1], pos[2], pos[0] + target[0], pos[1] + target[1], pos[2] + target[2], up[0], up[1], up[2]);	
	static GLfloat angle = 0.0;
	//glRotatef(angle, 0.0, 0.0, 1.0);
	angle += 0.01;
	
	
	
	

	// send the light position down as if it was a vertex in world coordinates
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);

	// load terrain material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, tanamb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tandiff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, tanspec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);

	// Send terrain mesh through pipeline
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,verts);
	glNormalPointer(GL_FLOAT,0,norms);
	glDrawElements(GL_TRIANGLES, 6*(res-1)*(res-1), GL_UNSIGNED_INT, faces);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	// load water material
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, seaamb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, seadiff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, seaspec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 10.0);

	// Send water as a single quad
	glNormal3f(0.0,0.0,1.0);
	glBegin(GL_QUADS);
		glVertex3f(-5,-5,sealevel);
		glVertex3f(5,-5,sealevel);
		glVertex3f(5,5,sealevel);
		glVertex3f(-5,5,sealevel);
	glEnd();

	glutSwapBuffers();
	glFlush ();

	glutPostRedisplay();
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0,(float)w/h,0.01,10.0);
	glMatrixMode (GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
		case '-':
			sealevel -= 0.01;
			break;
		case '+':
		case '=':
			sealevel += 0.01;
			break;
		case 'f':
			res = (res-1)*2 + 1;
			makemountain();
			break;
		case 'c':
			res = (res-1)/2 + 1;
			makemountain();
			break;
		case 27:
			exit(0);
			break;
   }
}

/**
 * This function takes a vector as an argument and then it makes its size 1
 */
void normalize(float* x){
	float length = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
	x[0] /= length;
	x[1] /= length;
	x[2] /= length;
}


/**
 * This function performs the multiplication of two matrices into a third matrix
 */
void multiplyMatrices(float a[][4], float b[][4], float inter[][4]){
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++){
			float temp = 0;
			for(int k=0; k <4; k++){
				temp += a[i][k] * b[k][j];
			}
			inter[i][j] = temp;
		}		
}


/**
 * This function takes a matrix and a vector. The vector gets multiplied with the matrix and gets the new values
 */
void multMatrixWithVector(float a[][4], float vec[]){
	for(int i = 0; i < 4; i++){
		float temp = 0.0;
		for(int j = 0; j < 4; j++)
			temp += a[i][j]*vec[j];
		vec[i] = temp;
	}
	vec[0] /= vec[3];
	vec[1] /= vec[3];
	vec[2] /= vec[3];
	vec[3] /= vec[3];
}


/**
 * This function gets an orthonormal basis for the roll operation.
 */
void getRollOrthoBasis(){
	v[0] = target[0];
	v[1] = target[1];
	v[2] = target[2];
	normalize(v);
	r[0] = up[1]*target[2] - target[1]*up[2];
	r[1] = up[2]*target[0] - up[0]*target[2];
	r[2] = up[0]*target[1] - up[1]*target[0];
	normalize(r);
	u[0] = v[1]*r[2] - v[2]*r[1];
	u[1] = v[2]*r[0] - v[0]*r[2];
	u[2] = v[0]*r[1] - v[1]*r[0];
	
	for(int i = 0; i < 3; i++){
		R[0][i] = r[i];
		R[1][i] = u[i];
		R[2][i] = v[i];
		R[3][i] = 0;
		RT[i][0] = r[i];
		RT[i][1] = u[i];
		RT[i][2] = v[i];
		RT[i][3] = 0;
		R[i][3] = 0;
		RT[3][i] = 0;
	}
	R[3][3] = 1;
	RT[3][3] = 1;	
	
	//the rotation matrix to rotate relative to x;
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			rotation[i][j] = 0;
	rotation[1][1] = 1;
	rotation[3][3] = 1;
	rotation[0][0] = cos(fRotateAngle2);
	rotation[0][1] = -sin(fRotateAngle2);
	rotation[2][1] = sin(fRotateAngle2);
	rotation[2][2] = cos(fRotateAngle2);
}

/**
 * This function sets up the matrix for the pitch transformation.
 * We already have an orthonormal basis for this transformation, we just need the multiplication matrix
 */
void getPitchOrthoBasis(){
	
	// the one vector that needs to change
	u[0] = -up[1]*target[2] + target[1]*up[2];
	u[1] = -up[2]*target[0] + up[0]*target[2];
	u[2] = -up[0]*target[1] + up[1]*target[0];
	
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
			R[i][j] = 0;
	R[3][3] = 1;
	
	R[0][0] = cos(fRotateAngle1) + u[0]*u[0]*(1 - cos(fRotateAngle1));
	R[0][1] = u[0]*u[1]*(1-cos(fRotateAngle1)) - u[2]*sin(fRotateAngle1);
	R[0][2] = u[0]*u[2]*(1-cos(fRotateAngle1)) + u[1] * sin(fRotateAngle1);
	
	R[1][0] = u[1]*u[0]*(1-cos(fRotateAngle1)) + u[2]*sin(fRotateAngle1);
	R[1][1] = cos(fRotateAngle1) + u[1]*u[1]*(1 - cos(fRotateAngle1));
	R[1][2] = u[1]*u[2]*(1-cos(fRotateAngle1)) - u[0]*sin(fRotateAngle1);
	
	R[2][0] = u[2]*u[0]*(1-cos(fRotateAngle1)) - u[1]*sin(fRotateAngle1);
	R[2][1] = u[2]*u[1]*(1-cos(fRotateAngle1)) + u[2]*sin(fRotateAngle1);
	R[2][2] = cos(fRotateAngle1) + u[2]*u[2]*(1 - cos(fRotateAngle1));
}


/**
 * This function will determine the movement of the plane depending on which key is pressed
 */
void movement(int key, int x, int y){	
	switch (key){		
		case GLUT_KEY_UP:
			fRotateAngle1 = -0.03f;
			getPitchOrthoBasis();
			multMatrixWithVector(R, target);
			multMatrixWithVector(R, up);
			//printf("%f %f %f\n", target[0], target[1], target[2]);			
			break;
		case GLUT_KEY_DOWN:
			fRotateAngle1 = 0.03f;
			getPitchOrthoBasis();		
			multMatrixWithVector(R, target);
			multMatrixWithVector(R, up);
			break;
		case GLUT_KEY_RIGHT:
			fRotateAngle2 = 0.03f;		
			getRollOrthoBasis();			
			multiplyMatrices(RT, rotation, oneof);
			multiplyMatrices(oneof, R, result);
			multMatrixWithVector(result, up);			
			//printf("%f %f %f\n", up[0], up[1], up[2]);
			break;
		case GLUT_KEY_LEFT:
			fRotateAngle2 = -0.03f;			
			getRollOrthoBasis();				
			multiplyMatrices(RT, rotation, oneof);
			multiplyMatrices(oneof, R, result);
			multMatrixWithVector(result, up);
			//printf("%f %f %f\n", up[0], up[1], up[2]);
			break;
	}
}

/**
 * Function provides a timer for the animation. This is how the velocity of the plane is implemented
 */
void timer(int v)
{	
	pos[0] += velocity * target[0];
	pos[1] += velocity * target[1];
	pos[2] += velocity * target[2];
	
	glutTimerFunc(1000/nFPS,timer,v); // restart timer again
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(movement);
   glutTimerFunc(100,timer,nFPS);
   glutMainLoop();
   return 0;
}
