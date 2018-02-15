#pragma once   //maybe should be static class
#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "GL/glew.h"
#include <Windows.h>
#include <iostream>
#include "display.h"
#include "mesh.h"
using namespace glm;

vec3 moveVector(vec3 point, mat4 mat, float w) {
	//remember w = 1 for move in space; w = 0 rotate in space;
	vec3 res = vec3(0, 0, 0);
	res.x = mat[0].x*point.x + mat[1].x*point.y + mat[2].x*point.z + mat[3].x*w;
	res.y = mat[0].y*point.x + mat[1].y*point.y + mat[2].y*point.z + mat[3].y*w;
	res.z = mat[0].z*point.x + mat[1].z*point.y + mat[2].z*point.z + mat[3].z*w;
	return res;
}

vec3 cubes_pos[6] = { vec3(0,1,0),vec3(0,5,0),vec3(0,9,0),vec3(0,13,0), vec3(5,0,0) ,vec3(0,0,0)};
vec4 solo_cobe_position;
float PI = 3.1415926535897932384626433832795f;

class Scene {
public:

	mat4 global_trans;
	mat4 global_rot;
	mat4 arm_trans;
	double cursor_x_pos;
	double cursor_y_pos;
	double clicked_x_pos;
	double clicked_y_pos;
	int pickedOBJ;
	float maxAngle = 270.0f;
	float click_depth=0;
	bool look_for = false;
	int IK_index = 3;

	Scene() {
		mat4 global_trans = mat4(1);
		mat4 global_rot = mat4(1);
		mat4 arm_trans = mat4(1);
		pickedOBJ = -1;
		cursor_x_pos = 0;
		cursor_y_pos = 0;
		clicked_x_pos = 0;
		clicked_y_pos = 0;
	}
};

Scene myscene = Scene();

class Cube {
public:
	int index;
	mat4 rotX;
	mat4 rotY;
	mat4 rotYinverse;
	mat4 rotYX;
	
	mat4 transMatrix;
	float xAngle = 0.0f;
	float yAngle = 0.0f;

	Cube(int ind) {
		index = ind;
		rotX = mat4(1);
		rotY = mat4(1);
		rotYinverse = mat4(1);
		rotYX = mat4(1);
		transMatrix = mat4(1);
	}

	Cube() {
		index = 0;
	}
};

 Cube allCubes[5];
 vec3 tip_position;

inline bool compare_floats(float A, float B)
 {
	 float epsilon = 0.5f;
	 return (fabs(A - B) < epsilon);
 }

void rotate_mat(int index, const float angle, const vec3 axis, const int  axis_id) //axis_id= 1 ->x , axis_id= 2 ->y
{
	mat4 rot = rotate(angle, axis);
	mat4 rot_inverse = rotate(-angle, axis);
	mat4 translate_to = translate(vec3(0, (index * 4) - 1, 0));
	mat4 translate_from = translate(vec3(0, (index * -4) + 1, 0));

	if (axis_id == 1) { // x exis

		float temp_angle = allCubes[index].xAngle +angle;
		if (abs(temp_angle) <= myscene.maxAngle) {
			allCubes[index].xAngle = temp_angle;
			allCubes[index].rotX = allCubes[index].rotX*translate_to*rot*translate_from;
		}
		else {
			allCubes[index].xAngle = myscene.maxAngle*temp_angle / abs(temp_angle); //what is this?
		}
	}
	else if (axis_id == 2)  // y
	{
		allCubes[index].rotY = rot_inverse*allCubes[index].rotY*rot;
		allCubes[index].rotYinverse = rot_inverse*allCubes[index].rotYinverse;
	}
	else
	{
			allCubes[index].rotYX = allCubes[index].rotYX*translate_to*rot*translate_from;
		
	}
}

mat4 rotate1 = rotate(7.0f, vec3(1, 0, 0));
mat4 rotate2 = rotate(-7.0f, vec3(1, 0, 0));
mat4 rotate3 = rotate(7.0f, vec3(0, 1, 0));
mat4 rotate4 = rotate(-7.0f, vec3(0, 1, 0));
inline void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case GLFW_KEY_UP:
		if ((myscene.pickedOBJ < 4)) {
			if (myscene.pickedOBJ == -1)
				myscene.global_rot = rotate1 * myscene.global_rot;
			else
				rotate_mat(myscene.pickedOBJ, 7, vec3(1, 0, 0), 1);
		}
		break;
	case GLFW_KEY_DOWN:
		if ((myscene.pickedOBJ < 4)) {
			if (myscene.pickedOBJ == -1)
				myscene.global_rot = myscene.global_rot*rotate2;
			else
				rotate_mat(myscene.pickedOBJ, -7, vec3(1, 0, 0), 1);
		}
		break;

	case GLFW_KEY_RIGHT:
		if ((myscene.pickedOBJ < 4)) {
			if (myscene.pickedOBJ == -1)
				myscene.global_rot = rotate3 * myscene.global_rot;
			else
				rotate_mat(myscene.pickedOBJ, 7, vec3(0, 1, 0), 2);
		}
		break;
	case GLFW_KEY_LEFT:
		if ((myscene.pickedOBJ < 4)) {
			if (myscene.pickedOBJ == -1)
				myscene.global_rot = rotate4 * myscene.global_rot;
			else
				rotate_mat(myscene.pickedOBJ, -7, vec3(0, 1, 0), 2);
			break;
		}
	case GLFW_KEY_SPACE:
		if (action == GLFW_PRESS)
		{
			myscene.IK_index = 3;
			if (!myscene.look_for) 
				myscene.look_for = true;
			else
				myscene.look_for = false;
		}
					
		break;
			default:
				break;
			}
}


