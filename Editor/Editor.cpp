#include "Editor.h"
#include "algebra3.h"
#include "Ray.h"
#include "VoxelGrid.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <regex>
#include "Camera.h"
#include "AABBox.h"

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
	float angles[4] = { 0, 0, 0, 0.0f };
	// Setup ImGui binding
	ImGui_ImplSdlGL3_Init(m_window);
	Process(angles);
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
			ControlPanel(m_width - 512, 512);
			Scene(512, 512);
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

void Editor::Process(float angles[4]) {
	ifstream myData;
	const int width = 512;
	const int height = 512;
	const int depth = 56;

	//open raw data and read to store in vector
	myData.open("C:\\Users\\김재용\\Documents\\tutorials\\volume-rendering-tutorial\\asset\\data\\volume1_512x512x56-short-bigendian.raw", ios::binary);

	if (myData.fail()) {
		cout << "opening file failed.";
		exit(1);
	}

	int16_t value;
	char pixels[sizeof(int16_t)];


	vector<short> data;
	vector<unsigned char> processed;
	processed.reserve(width * height * 4);

	while (myData.read(pixels, sizeof(pixels)))
	{
		memcpy(&value, pixels, sizeof(value));
		data.push_back(swap_int16(value));
	}

	int max = 0;
	int min = INFINITY;

	myData.close();

	for (int i = 0; i < data.size(); i++) {
		if (data[i] > max) {
			max = data[i];
		}

		if (data[i] < min) {
			min = data[i];
		}
	}

	mat4 rotationX = rotation3D(vec3(1, 0, 0), angles[0]);
	mat4 rotationY = rotation3D(vec3(0, 1, 0), angles[1]);
	mat4 rotationZ = rotation3D(vec3(0, 0, 1), angles[2]);

	vec3 volumeCenter = vec3((width - 1) / 2, (height - 1) / 2, depth / 2);
	vec3 volumeVertex1 = vec3(0, 0, 0);
	vec3 volumeVertex2 = vec3(width - 1, height - 1, depth - 1);
	AABBox volumeBound = AABBox(volumeVertex1, volumeVertex2);

	vec3 rayDirection = vec3(0, 0, 1);

	//cout << volumeVertex1 << "\n";
	vec3 planeStart = vec3(0, 0, -300);
	vec3 widthVector = vec3(1, 0, 0);
	vec3 heightVector = vec3(0, 1, 0);
	//vec3 translatedCenter = volumeCenter * translation3D(volumeCenter);
	vec3 translatedPlane = translation3D(-1* volumeCenter) * planeStart;
	

	//vec3 reCenter = translation3D(volumeCenter) * translatedCenter;
	

	vec3 finalDirection = rotationX * rotationY * rotationZ * rayDirection;
	vec3 finalWidthVector = rotationX * rotationY * rotationZ * widthVector;
	vec3 finalHeightVector = rotationX * rotationY * rotationZ * heightVector;

	vec3 widthVectorLength = finalWidthVector.normalize();
	vec3 heightVectorLength = finalHeightVector.normalize();
	vec3 finalDirectionVector = finalDirection.normalize();

	
	cout << widthVectorLength << " " << heightVectorLength << "\n";
	cout << finalDirectionVector << "\n";

	vec3 finalPlane = translation3D(volumeCenter) * rotationX * rotationY * rotationZ * translatedPlane;
	cout << finalPlane << "\n";
	VoxelGrid grid = VoxelGrid(width, height, depth, data);




	
	//Camera camera = Camera(vec3(255, 255, -10));
	//for each pixel
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			vec3 pixelCoordinate = finalPlane + (j * widthVectorLength)+ (i * heightVectorLength);
			if (i == height / 2 && j == width / 2) {
				cout << pixelCoordinate;
			}
			else if (j == width - 1 && i == height - 1) {
				cout << pixelCoordinate;
			}
			else {

			}
	

			Ray ray = Ray(pixelCoordinate, finalDirectionVector);
			float t = INFINITY;
			float color = 0;
			float A = 1;
			if (volumeBound.intersect(ray, t)) {

				double step_size = 1;
				for (int m = t * step_size; m < depth * 10000 * step_size; m++) {
					float increment = m / step_size;					//sampling
					if (!grid.isInsideGrid(ray.getCurrentPos(increment))) {
						break;					//sample only if ray is still inside grid
					}
					vec3 samplePoint = ray.getCurrentPos(increment);
					float pointA = 0;
					float pointColor = 0;
					//interpolation
					double interpolated = grid.triInterp(samplePoint);


					//transfer function
					//Cdes = Cdes + ( 1 - Opacityd) * Csource
					if (interpolated < 0) {
						pointColor = 0;
					}
					else {
						pointColor = interpolated / max;  // 0 과 255 사이 
					}

					if (interpolated < 200) {
						pointA = 0;
					}
					else {
						pointA = (interpolated - 200) / (max - 200);
					}

					color = color + pointA * pointColor * A;
					A = A * (1 - pointA);

				}
				
			}
			processed.push_back((unsigned char)(color * 255));
			processed.push_back((unsigned char)(color * 255));
			processed.push_back((unsigned char)(color * 255));
			processed.push_back((unsigned char)255);
		}

	}

	UpdateTexture(processed.data(), 512, 512);

}

void Editor::ControlPanel(uint32_t width, uint32_t height) {
	// Control Panel Window
	ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Control Panel", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
	ImGuiIO& io = ImGui::GetIO();

	if (ImGui::CollapsingHeader("Scene Control")) {
		if (ImGui::Button("Load Data")) {


		}
	}

	if (ImGui::CollapsingHeader("Transition"))
	{
		static float angles[4] = { 0, 0, 0, 0.5f };
		ImGui::InputFloat3("input float3", angles);
		ImGui::SliderFloat3("slider float3", angles, 0.0f, 360.0f);
		if (ImGui::IsItemActive()) {
			Process(angles);
		}
		ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
	}

	if (ImGui::CollapsingHeader("Volume Data")) {
		static int vec4i[3] = { 512, 512, 58 };
		const char* items[] = { "Ray Cast" };
		static int item = -1;
		static int window = 123;
		static int level = 123;
		ImGui::Combo("Display Mode", &item, items, IM_ARRAYSIZE(items));
		ImGui::InputInt3("Dimensions", vec4i);
		ImGui::InputInt("window", &window);
		ImGui::InputInt("level", &level);
		ImGui::SameLine();
	}

	//if (ImGui::IsMouseDragging()) {
	//	cout << "mouse moved this much : ";
	//	
	//}



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
	//maybe implement arcball later when using gpu
	/*
	if (ImGui::IsWindowHovered()) {
		if (ImGui::IsMouseDragging()) {
			Process(ImGui::GetMouseDragDelta(0));
		}
	}
	*/
	
	

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


