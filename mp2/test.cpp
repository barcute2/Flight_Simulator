#include<stdlib.h>
#include<math.h>
#include<stdio.h>


double v[4], u[4], r[4], target[4] = {1, 0, 0, 1}, up[4] = {0, 0, 1, 1}, R[4][4], RT[4][4], rotation[4][4], fRotateAngle2 = 0.0f;
double result[4][4], oneof[4][4];

void normalize(double* x){
	double length = sqrt(x[0]*x[0] + x[1]*x[1] + x[2]*x[2]);
	x[0] /= length;
	x[1] /= length;
	x[2] /= length;
}

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
	for(int i = 0; i < 4; i++)
	for(int j = 0; j < 4; j++)
		rotation[i][j] = 0;
	rotation[0][0] = 1;
	rotation[3][3] = 1;
	rotation[1][1] = cos(fRotateAngle2);
	rotation[1][2] = -sin(fRotateAngle2);
	rotation[2][1] = sin(fRotateAngle2);
	rotation[2][2] = cos(fRotateAngle2);
}

void multiplyMatrices(double a[][4], double b[][4], double inter[][4]){
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++){
			inter[i][j] = 0;
			for(int k=0; k <4; k++){
				inter[i][j] += a[i][k] * b[k][j];
			}			
		}		
}

void multMatrixWithVector(double a[][4], double vec[]){
	for(int i = 0; i < 4; i++){
		double temp = 0.0;
		for(int j = 0; j < 4; j++)
			temp += a[i][j]*vec[j];
		vec[i] = temp;
	}
	vec[0] /= vec[3];
	vec[1] /= vec[3];
	vec[2] /= vec[3];
	vec[3] /= vec[3];
}

int main(){
	getRollOrthoBasis();
	multiplyMatrices(RT, rotation, oneof);
	multiplyMatrices(oneof, R, result);
	multMatrixWithVector(result, up);
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++)
			printf("%f ",R[i][j]);
		printf("\n");
	}
	printf("\n");
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++)
			printf("%f ",RT[i][j]);
		printf("\n");
	}
	printf("\n");
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++)
			printf("%f ",rotation[i][j]);
		printf("\n");
	}
	printf("\n");
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++)
			printf("%f ",result[i][j]);
		printf("\n");
	}
	printf("\n");
	printf("%f %f %f %f", up[0], up[1], up[2], up[3], up[4]);
	printf("\n");
	return 0;
}

