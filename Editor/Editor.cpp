#include "Editor.h"
#include "algebra3.h"
#include "Ray.h"
#include "VoxelGrid.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <regex>
#include "Camera.h"

Editor::Editor(uint32_t width, uint32_t height)
	:m_window(nullptr),
	m_context(nullptr),
	m_width(width),
	m_height(height),
	m_isRunning(true),
	m_hasTexture(false) {
}

Editor::~Editor() {
	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

bool Editor::Initialize() {
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		std::cout << ("Error: %s\n", SDL_GetError()) << std::endl;
		return false;
	}

	// Setup window
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	m_window = SDL_CreateWindow("Volume Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_OPENGL);
	SDL_GLContext glcontext = SDL_GL_CreateContext(m_window);
	glewInit();

	// Setup ImGui binding
	ImGui_ImplSdlGL3_Init(m_window);
	Process();
	return true; // Return initialization result
}

//! Byte swap short
int16_t swap_int16(int16_t val)
{
	return (val << 8) | ((val >> 8) & 0xFF);
}


void Editor::Run() {
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	while (m_isRunning) {
		// Handle SDL events
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			ImGui_ImplSdlGL3_ProcessEvent(&event);
			HandleSDLEvent(&event);
		}
		ImGui_ImplSdlGL3_NewFrame(m_window);
		// Editor
		{
			ControlPanel(m_width - 720, 720);
			Scene(720, 720);
			// Code sample of ImGui (Remove comment when you want to see it)
			ImGui::ShowTestWindow();
		}
		// Rendering
		glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui::Render();
		SDL_GL_SwapWindow(m_window);
	}
}

void Editor::UpdateTexture(const void * buffer, int width, int height) {
	if (!m_hasTexture) {
		auto err = glGetError();
		glGenTextures(1, &m_textureID);
		if (err != GL_NO_ERROR) {
			throw std::runtime_error("Not able to create texture from buffer" + std::to_string(glGetError()));
		}
		else {
			m_hasTexture = true;
		}
	}
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	// set texture sampling methods
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Editor::Process() {
	ifstream myData;
	const int width = 512;
	const int height = 512;
	const int depth = 56;


	myData.open("C:\\Users\\±èÀç¿ë\\Documents\\tutorials\\volume-rendering-tutorial\\asset\\data\\volume1_512x512x56-short-bigendian.raw", ios::binary);

	if (myData.fail()) {
		cout << "opening file failed.";
		exit(1);
	}

	int16_t value;
	int i = 0;
	char pixels[sizeof(int16_t)];
	

	vector<short> data;
	vector<vec3> dataPos;
	vector<vec3> pixelValue;
	vector<vec3> pixelPos;

	while (myData.read(pixels, sizeof(pixels)))
	{
		memcpy(&value, pixels, sizeof(value));
		i++;
		data.push_back(swap_int16(value));
	}

	myData.close();



	VoxelGrid grid = VoxelGrid(width, height, depth, data);
	vec3 camPosition = vec3(255, 255, -20);
	Camera camera = Camera(camPosition);

	//for each pixel
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			vec3 pixelCoordinate = vec3(width + 0.5, height + 0.5, -10);
			Ray ray = Ray(camera.getPosition(), camera.getPosition() - pixelCoordinate);
			
			/*
			vec3 v1 = vec3(0, 0, 0);
			vec3 v2 = vec3(512, 0, 0);
			vec3 v3 = vec3(0, 512, 0);
			vec3 v4 = vec3(512, 512, 0);
			vec3 v5 = vec3(0, 0, 56);
			vec3 v6 = vec3(512, 0, 56);
			vec3 v7 = vec3(0, 512, 56);
			vec3 v8 = vec3(512, 512, 56);
			*/

			vec3 min = vec3(0, 0, 0);
			vec3 max = vec3(512, 512, 56);


			//compute ray direction 
			//Ray primRay;
			//computePrimRay(i, j, &primray);
			//Point pHit;
			//Normal nHit;
			//float minDis = INFINITY;
			//Object object = NULL;

			/*
			for (int k = 0; k < objects.size(); ++k){
				if(intersect(objects[k], primRay, &pHit, &nHit){
					float distnace = Distance(eyePosition, pHit);
					if(distance < minDistance){
						object = objects[k];
						minDistance = distance;
					}
				}

			}
			
			
			*/

			//Ray ray = Ray(, direction);

			//volume entry position 
			float t = 0.5;
			while (grid.isInsideGrid(ray.getCurrentPos(t))) { //shoot ray
					vec3 samplePoint = ray.getCurrentPos(t);

					//interpolation
					float interpolated = grid.triInterp(samplePoint);
					/*
					//transfer function 
					if (interpolated < 0) {  //black area

					}
					else { //feature of interest 


					}
					*/
				t = t + 0.5; //ray step size
			}



		}
	}
}

void Editor::ControlPanel(uint32_t width, uint32_t height) {
	// Control Panel Window
	ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

	/* TODO : Write UI Functions */

	ImGui::End();
}

void Editor::Scene(uint32_t width, uint32_t height) {
	// Scene Window
	ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));
	ImGui::SetNextWindowPos(ImVec2((float)(m_width - width), 0.f));
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	// Draw texture if there is one
	if (m_hasTexture) {
		ImGui::Image(ImTextureID(m_textureID), ImGui::GetContentRegionAvail());
	}
	ImGui::End();
}

void Editor::OnResize(uint32_t width, uint32_t height) {
	m_width = width;
	m_height = height;
}

void Editor::HandleSDLEvent(SDL_Event * event) {
	// SDL_Event wiki : https://wiki.libsdl.org/SDL_Event
	static bool mouseIsDown = false;
	static bool isDragging = false;
	int degreeStep = 5;
	switch (event->type) {
	case SDL_QUIT:
		m_isRunning = false;
		break;
	case SDL_KEYDOWN:
		break;
	case SDL_MOUSEWHEEL:
		break;
	case SDL_MOUSEMOTION:
		break;
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		mouseIsDown = false;
		break;
	case SDL_WINDOWEVENT:
		switch (event->window.event) {
		case SDL_WINDOWEVENT_RESIZED:
			OnResize(event->window.data1, event->window.data2);
			break;
		default:
			break;
		}
	default:
		break;
	}
}


