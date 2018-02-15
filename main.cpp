#include <Windows.h>
#include <iostream>
#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "inputManager.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 800;
Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
const vec3 pos = vec3(0, 0, -50);
const vec3 forward = glm::vec3(0.0f, 0.0f, 1.0f);
const vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
mat4 P = glm::perspective(45.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 100.0f) *glm::lookAt(pos, pos + forward, up);
mat4 scale_s = scale(mat4(1), vec3(1, 2, 1));
mat4 MVP;
Shader shader("./res/shaders/basicShader");
Shader shader2("./res/shaders/basicShader2");
bool clicked_left;
bool clicked_right;
Vertex vertices[] =
{
	Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
	Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
	Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),
	Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1),glm::vec3(0, 0, 1)),

	Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
	Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
	Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),
	Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1),glm::vec3(0, 1, 1)),

	Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
	Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
	Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),
	Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0),glm::vec3(0, 1, 0)),

	Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
	Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
	Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),
	Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0),glm::vec3(1, 1, 0)),

	Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
	Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
	Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),
	Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0),glm::vec3(1, 0, 0)),

	Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
	Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
	Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1)),
	Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0),glm::vec3(1, 0, 1))
};

unsigned int indices[] = { 0, 1, 2, 0, 2, 3, 6, 5, 4, 7, 6, 4, 10, 9, 8, 11, 10, 8, 12, 13, 14, 12, 14, 15, 16, 17, 18, 16, 18, 19, 22, 21, 20, 23, 22, 20};
Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	mat4 trans1;
	if (yoffset == 1)
		trans1 = translate(mat4(1), vec3(0, 0, 1));
	else
		trans1 = translate(mat4(1), vec3(0, 0, -1));
	if (myscene.pickedOBJ < 4) //chain
	{	
		myscene.arm_trans = myscene.arm_trans*trans1;
	}
	else 
	{
		allCubes[4].transMatrix = allCubes[4].transMatrix*trans1;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int state2 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_RELEASE || state2 == GLFW_RELEASE)
	{
		clicked_right = false;
		clicked_left = false;
	}

	if (((button == GLFW_MOUSE_BUTTON_LEFT) || (button == GLFW_MOUSE_BUTTON_RIGHT)) && action == GLFW_PRESS) {
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwGetCursorPos(window, &myscene.clicked_x_pos, &myscene.clicked_y_pos);
		
		
		mat4 cubeRot = mat4(1);
		for (int i = 0; i < 5; i++) {
			if (i == 4) {
				MVP = P*myscene.global_trans*myscene.global_rot*allCubes[i].rotYinverse*allCubes[i].rotY*allCubes[i].rotX*allCubes[i].transMatrix;
			}
			else
			{
				cubeRot = mat4(1);
				for (int j = 0; j <= i; j++) {
					cubeRot = cubeRot*allCubes[j].rotYX*allCubes[j].rotYinverse*allCubes[j].rotX*allCubes[j].rotY;
				}
				MVP = P*myscene.global_trans*myscene.arm_trans*myscene.global_rot*cubeRot*allCubes[i].transMatrix*scale_s;
			}
			mesh.Draw();
			shader2.Bind();
			shader2.Update(MVP, MVP, i, -1);
		}

		double xpos, ypos;
		//getting cursor position
		glfwGetCursorPos(window, &xpos, &ypos);
		glFlush();
		glFinish();
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		unsigned char data[4];
		glReadPixels(xpos, 800 - ypos, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// Convert the color back to an integer ID
		int pickedID =
			data[0] +
			data[1] * 256 +
			data[2] * 256 * 256;

		if (pickedID == 0x00ffffff) { // Full white, must be the background !
			myscene.pickedOBJ = -1;
		}
		else {
			if (pickedID>3)
				myscene.pickedOBJ = 4;
			else
			myscene.pickedOBJ = pickedID;
		}
	}

	glfwGetCursorPos(window, &myscene.clicked_x_pos, &myscene.clicked_y_pos);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		clicked_left = true;
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		clicked_right = true;
}
void drawCoordinates() {

	// draw some lines
	glColor3f(1.0, 0.0, 0.0); // red x
	glBegin(GL_LINES);
	// x aix

	glVertex3f(-4.0, 0.0f, 0.0f);
	glVertex3f(4.0, 0.0f, 0.0f);

	glVertex3f(4.0, 0.0f, 0.0f);
	glVertex3f(3.0, 1.0f, 0.0f);

	glVertex3f(4.0, 0.0f, 0.0f);
	glVertex3f(3.0, -1.0f, 0.0f);
	glEnd();

	// y 
	glColor3f(0.0, 1.0, 0.0); // green y
	glBegin(GL_LINES);
	glVertex3f(0.0, -4.0f, 0.0f);
	glVertex3f(0.0, 4.0f, 0.0f);

	glVertex3f(0.0, 4.0f, 0.0f);
	glVertex3f(1.0, 3.0f, 0.0f);

	glVertex3f(0.0, 4.0f, 0.0f);
	glVertex3f(-1.0, 3.0f, 0.0f);
	glEnd();

}
int main(int argc, char** argv)
{

	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	vec3 sceneObjects[5] = { vec3(0,1,0),vec3(0,5,0),vec3(0,9,0),vec3(0,13,0), vec3(5,0,0) };

	for (int z = 0; z <5; z++) {
		allCubes[z] = Cube(z);
		allCubes[z].transMatrix = translate(mat4(1), sceneObjects[z]);
	}
	glfwSetKeyCallback(display.m_window, key_callback);
	glfwSetMouseButtonCallback(display.m_window, mouse_button_callback);
	glfwSetScrollCallback(display.m_window, scroll_callback);

	

	while (!glfwWindowShouldClose(display.m_window))
	{
		Sleep(1);
		display.Clear(1.0f, 1.0f, 1.0f, 1.0f);
		mat4 MVP(1);
		mat4 cubeRot(1);
		mat4 mvp_whithoutp(1);

		for (int i = 0; i < 5; i++) {
			if (i == 4) {
				MVP = P*myscene.global_trans*myscene.global_rot*allCubes[i].rotYinverse*allCubes[i].rotY*allCubes[i].rotX*allCubes[i].transMatrix;
				mat4 trans_3 = translate(mat4(1), vec3(-1, -1, -1));
				mvp_whithoutp = myscene.global_trans*myscene.global_rot*allCubes[i].rotYinverse*allCubes[i].rotY*allCubes[i].rotX*trans_3*allCubes[i].transMatrix;
				solo_cobe_position =mvp_whithoutp* vec4(1);
				cubes_pos[i] = vec3(solo_cobe_position.x, solo_cobe_position.y,solo_cobe_position.z);
			}
			else
			{
				cubeRot = mat4(1);
				for (int j = 0; j <= i; j++) {
					cubeRot = cubeRot*allCubes[j].rotYX*allCubes[j].rotYinverse*allCubes[j].rotX*allCubes[j].rotY;
				}
				MVP = P*myscene.global_trans*myscene.arm_trans*myscene.global_rot*cubeRot*allCubes[i].transMatrix*scale_s;
				mat4 trans_2 = translate(mat4(1), vec3(-1, 0, -1));
				mvp_whithoutp = myscene.global_trans*myscene.arm_trans*myscene.global_rot*cubeRot*trans_2*allCubes[i].transMatrix*scale_s;
				vec4 temp = mvp_whithoutp * vec4(1);
				cubes_pos[i] = vec3(temp.x, temp.y, temp.z);
				if(i == 0)
				{
					mat4 trans_for_base = translate(mat4(1), vec3(-1, -2, -1));
					mat4 mvp_whithoutp_base = myscene.global_trans*myscene.arm_trans*myscene.global_rot*cubeRot*trans_for_base*allCubes[i].transMatrix*scale_s;
				    temp = mvp_whithoutp_base * vec4(1);
					cubes_pos[5] = vec3(temp.x, temp.y, temp.z);
				}
			}
			if(myscene.pickedOBJ > -1)
			{
				mat4 mvp_whithoutp2 = myscene.arm_trans*cubeRot*allCubes[myscene.pickedOBJ].transMatrix*scale_s;
				myscene.click_depth = (mvp_whithoutp2 *vec4(1)).z;
			}
			glLineWidth(2.5);
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_LINES);
			glVertex3f(-3, -1, 0.0);
			glVertex3f(3, -1, 0);
			glEnd();
			glLineWidth(2.5);
			glColor3f(1.0, 0.0, 0.0);
			glBegin(GL_LINES);
			glVertex3f(0, -1, 3);
			glVertex3f(0, -1, -3);
			glEnd();

			if (myscene.look_for) {
				// cubes_pos[0] is top of base, cubes_pos[4] is target, cubes_pos[5] is base.
				float distance = length(cubes_pos[5] - cubes_pos[4]);
				if (distance > 16) {
					myscene.look_for = false;
					std::cout << "cannot reach , IK is now off." << std::endl;
				}
				else if (length(cubes_pos[3] - cubes_pos[4]) > 0.1) {
					if (myscene.IK_index < 0)
						myscene.IK_index = 3;
					
						vec3 parent_pos;
						vec3 last_tip_pos = cubes_pos[3]; 
						if (myscene.IK_index ==0)
							parent_pos = cubes_pos[5];
						else
							parent_pos = cubes_pos[myscene.IK_index -1];

						vec3 tip_to_parent = normalize(last_tip_pos - parent_pos);
						vec3 last_to_cube = normalize(cubes_pos[4] - last_tip_pos);
						std::cout << "dot " << myscene.IK_index << glm::dot(tip_to_parent, last_to_cube) << std::endl;
							float dot = clamp(glm::dot(tip_to_parent, last_to_cube), -1.0f, 1.0f);
							vec3 c = glm::cross(tip_to_parent, last_to_cube);
							float angle = glm::acos(dot);
							rotate_mat(myscene.IK_index, angle, c, 3);
						myscene.IK_index--;
				}
				if (length(cubes_pos[3] - cubes_pos[4]) <= 0.1)
				{
					std::cout << "distance:" << length(cubes_pos[3] - cubes_pos[4]) << std::endl;
				}
			}

			mesh.Draw();
			shader.Bind();
			shader.Update(MVP, MVP, -1, i);
		}
		//clicking and dragging movement

		glfwGetCursorPos(display.m_window, &myscene.cursor_x_pos, &myscene.cursor_y_pos);
		if (clicked_left && myscene.pickedOBJ<4 && myscene.pickedOBJ>-1)
		{
			//around the arm
				rotate_mat(myscene.pickedOBJ, double (myscene.clicked_x_pos - myscene.cursor_x_pos)/2,vec3(0,1,0),2);
				myscene.clicked_x_pos = myscene.cursor_x_pos;

			//up and down the last link connection
				rotate_mat(myscene.pickedOBJ, double(myscene.clicked_y_pos - myscene.cursor_y_pos)/2, vec3(1, 0, 0),1);
				myscene.clicked_y_pos = myscene.cursor_y_pos;
		}
		if (clicked_right && myscene.pickedOBJ>=0)
		{

			float amount_move_x = ((myscene.clicked_x_pos - myscene.cursor_x_pos) / 17) + ((myscene.clicked_x_pos - myscene.cursor_x_pos)*myscene.click_depth) / (800);
			float amount_move_y = (myscene.clicked_y_pos - myscene.cursor_y_pos) / 17 + ((myscene.clicked_y_pos - myscene.cursor_y_pos)*myscene.click_depth) / (800);
			if (myscene.pickedOBJ == 4)
			{
				mat4 trans = translate(mat4(1), vec3(amount_move_x, 0, 0));
				allCubes[myscene.pickedOBJ].transMatrix = allCubes[myscene.pickedOBJ].transMatrix*trans;
				myscene.clicked_x_pos = myscene.cursor_x_pos;
				mat4 trans2 = translate(mat4(1), vec3(0, amount_move_y, 0));
				allCubes[myscene.pickedOBJ].transMatrix = allCubes[myscene.pickedOBJ].transMatrix*trans2;
				myscene.clicked_y_pos = myscene.cursor_y_pos;
			}
			else
			{
				mat4 trans = translate(mat4(1), vec3(amount_move_x, 0, 0));
				myscene.arm_trans = myscene.arm_trans*trans;
				myscene.clicked_x_pos = myscene.cursor_x_pos;
				mat4 trans2 = translate(mat4(1), vec3(0, amount_move_y, 0));
				myscene.arm_trans = myscene.arm_trans*trans2;
				myscene.clicked_y_pos = myscene.cursor_y_pos;
			}
		}
		display.SwapBuffers();
		glfwPollEvents();
	}

	return 0;
}