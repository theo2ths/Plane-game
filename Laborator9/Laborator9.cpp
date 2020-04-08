#include "Laborator9.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include <ctime>

using namespace std;
int sign = 0;
float curve = 0.f;
float ang_nor = 0.f;

float cloud1 = 0.f; 
float cloud2 = 0.f;
float cloud3 = 0.f;
int vieti = 3;
int benzina = -1;
float umplere = 0.05;
bool umple = false;
float cazi = 0;
float unghi = 0;

float pozbenz = 7;
float pozviata = 4;



float cubeX = 12;
float cubeX1 = 22;
float cubeX3 = 17;
float cubeX4 = 15;
float cubeX5 = 27;
glm::mat4 planeModelMatrix;
float threshold;

Laborator9::Laborator9()
{
}

Laborator9::~Laborator9()
{
}


float formula(float x, float y, float z, float x2, float y2, float z2)
{

	return sqrt(pow((x - x2), 2) + pow((y - y2), 2) + pow((z - z2), 2));

}

void Laborator9::Init()
{
	srand(static_cast <unsigned> (time(0)));
	camera = new Laborator::Camera();
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	// Load textures
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::MODELS + "playerTexture.png").c_str(), GL_REPEAT);
		mapTextures["plane"] = texture;
	}
	// Load textures
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::MODELS + "w1.png").c_str(), GL_REPEAT);
		mapTextures["w1"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::MODELS + "white.png").c_str(), GL_REPEAT);
		mapTextures["white"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::MODELS + "smoke.png").c_str(), GL_REPEAT);
		mapTextures["smoke"] = texture;
	}
	// Load textures
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::MODELS + "w2.png").c_str(), GL_REPEAT);
		mapTextures["w2"] = texture;
	}
	// Load textures
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::MODELS + "red.png").c_str(), GL_REPEAT);
		mapTextures["red"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::MODELS + "cyan.png").c_str(), GL_REPEAT);
		mapTextures["cyan"] = texture;
	}
	// Load textures
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::MODELS + "sky.jpg").c_str(), GL_CLAMP_TO_BORDER);
		mapTextures["sky"] = texture;
	}
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((RESOURCE_PATH::MODELS + "heart.png").c_str(), GL_REPEAT);
		mapTextures["inima"] = texture;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS , "sfera.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("quad");
		mesh->LoadMesh(RESOURCE_PATH::MODELS, "quad.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("cube");
		mesh->LoadMesh(RESOURCE_PATH::MODELS, "cube.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		Mesh* mesh = new Mesh("plane");
		mesh->LoadMesh(RESOURCE_PATH::MODELS, "plane.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("hair");
		mesh->LoadMesh(RESOURCE_PATH::MODELS, "hair.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("helix");
		mesh->LoadMesh(RESOURCE_PATH::MODELS, "helix.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderLab9");
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}


	{
		Shader* shader = new Shader("Water");
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/WaterVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/WaterFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader* shader = new Shader("Smoke");
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/Smoke.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		Shader* shader = new Shader("light");
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/LightVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/LightFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
	{
		lightPosition = glm::vec3(0, 0, 0);
		lightDirection = glm::vec3(0, 1, 0);
		materialShininess = 30;
		materialKd = 0.5;
		materialKs = 0.5;
		cutoff_angle = -89.2866;
		angleOX = 47.9959; 
		angleOY = 0.499126;
	}
	
}

void Laborator9::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);	
}
void Laborator9::DrawWaterBlob(glm::vec3 pos, glm::vec3 scale, int numBlobs,std::string fig)
{
	
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1), pos);
		modelMatrix = glm::scale(modelMatrix, scale);
		modelMatrix = glm::rotate(modelMatrix, (float)Engine::GetElapsedTime() * -.5f, glm::vec3(1, 0, 0));
		RenderSimpleMesh(meshes[fig], shaders["Water"], modelMatrix, mapTextures["w1"]);
		if (numBlobs < 2) return;
		modelMatrix = glm::translate(glm::mat4(1), pos);
		modelMatrix = glm::scale(modelMatrix, scale);
		modelMatrix = glm::rotate(modelMatrix, (float)Engine::GetElapsedTime() * -0.75f, glm::normalize(glm::vec3(1, 0, 0.25)));
		RenderSimpleMesh(meshes[fig], shaders["Water"], modelMatrix, mapTextures["w2"]);
		if (numBlobs < 3) return;
		modelMatrix = glm::translate(glm::mat4(1), pos);
		modelMatrix = glm::scale(modelMatrix, scale);
		modelMatrix = glm::rotate(modelMatrix, (float)Engine::GetElapsedTime() * -0.6f, glm::normalize(glm::vec3(1, 0, 0.15)));
		RenderSimpleMesh(meshes[fig], shaders["Water"], modelMatrix, mapTextures["w1"]);
		if (numBlobs < 4) return;
		modelMatrix = glm::translate(glm::mat4(1), pos);
		modelMatrix = glm::scale(modelMatrix, scale);
		modelMatrix = glm::rotate(modelMatrix, (float)Engine::GetElapsedTime() * -0.45f, glm::normalize(glm::vec3(1, 0, 0.33f)));
		RenderSimpleMesh(meshes[fig], shaders["Water"], modelMatrix, mapTextures["w2"]);


		glDisable(GL_CULL_FACE);
	
}

float deterministicRandom(float input)
{
	return glm::fract(sin(input * 3523532.f + 1151.f) * 9325238.f);
}

void Laborator9::Update(float deltaTimeSeconds)
{
	cubeX -=10*deltaTimeSeconds;
	cubeX1 -= 12*deltaTimeSeconds;
	cubeX3 -= 12.5*deltaTimeSeconds;
	cubeX4 -= 13*deltaTimeSeconds;
	cubeX5 -= 8*deltaTimeSeconds;
	if (cubeX < -12)
	{
		cubeX = 12;
	}
	if (cubeX1 < -22)
	{
		cubeX1 = 22;
	}
	if (cubeX3 < -17)
	{
		cubeX3 = 17;
	}
	if (cubeX4 < -15)
	{
		cubeX4 = 15;
	}
	if (cubeX5 < -27)
	{
		cubeX5 = 27;
	}
	cloud1++;
	
	//cerul
	{
		glm::mat4 modelMatrix = glm::mat4(1);

		modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 10, 0));
		modelMatrix = glm::rotate(modelMatrix, 90.f, glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(100));
		
		
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, mapTextures["sky"]);
		
	}
	DrawWaterBlob(glm::vec3(0, -2, 0), glm::vec3(13,4,13), 4, "sphere");


	if (sign == -1 && curve <= 20)
		curve += 1.5 * deltaTimeSeconds + 1;

	else
		if (sign == 1 && curve >= -15)
			curve = curve - 1.7 * deltaTimeSeconds - 2;
	ang_nor += 10 * deltaTimeSeconds;
	cloud2 += deltaTimeSeconds*20;
	cloud3 += deltaTimeSeconds * 26;
	//pilot
	if(viatza>0 && vieti !=0)
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 7, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, piloty, 0));
		modelMatrix = glm::rotate(modelMatrix,RADIANS(curve), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.50));
		glDisable(GL_BLEND);
		RenderSimpleMesh(meshes["plane"], shaders["ShaderLab9"], modelMatrix, mapTextures["plane"]);
		RenderSimpleMesh(meshes["hair"], shaders["Water"], modelMatrix, mapTextures["plane"]);

		cazi = piloty;

		planeModelMatrix = modelMatrix;
	}
	else
	{
		cazi -= deltaTimeSeconds * 2;
		if (unghi < 45)
			unghi += deltaTimeSeconds * 30;
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 7, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, cazi, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghi), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.50));
		glDisable(GL_BLEND);
		RenderSimpleMesh(meshes["plane"], shaders["ShaderLab9"], modelMatrix, mapTextures["plane"]);
		RenderSimpleMesh(meshes["hair"], shaders["Water"], modelMatrix, mapTextures["plane"]);


	}

		helixang += 50 * deltaTimeSeconds;
		
	//elice

		if (viatza>0 && vieti != 0)
	{	
		glm::mat4 modelMatrixH = glm::mat4(1);
		modelMatrixH = glm::translate(modelMatrixH, glm::vec3(0, 7, 0));
		modelMatrixH = glm::translate(modelMatrixH, glm::vec3(0, piloty, 0));

		modelMatrixH = glm::rotate(modelMatrixH, RADIANS(curve), glm::vec3(0, 0, 1));
		modelMatrixH = glm::scale(modelMatrixH, glm::vec3(0.50));
		modelMatrixH = glm::rotate(modelMatrixH, helixang ,glm::vec3(0,0,1));
		glDisable(GL_BLEND);
		RenderSimpleMesh(meshes["helix"], shaders["ShaderLab9"], modelMatrixH, mapTextures["plane"]);
	}
		else
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 7, 0));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, cazi, 0));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(unghi), glm::vec3(0, 0, 1));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.50));
			modelMatrix = glm::rotate(modelMatrix, helixang, glm::vec3(0, 0, 1));
			glDisable(GL_BLEND);
			RenderSimpleMesh(meshes["helix"], shaders["ShaderLab9"], modelMatrix, mapTextures["plane"]);


		}
	//benzinometru
	{
		if (umple == true) {
			umple = false;
			if(viatza<1-0.4)
			viatza += 0.4;
		}
		if(viatza >0 && vieti >0)
		viatza += deltaTimeSeconds *0.09f*benzina;
		glm::mat4 modelMatrixH = glm::translate(glm::mat4(1), glm::vec3(0.3, 0.8, 0)) *
								 glm::scale(glm::mat4(1), glm::vec3(viatza* 0.3, 0.08, 1)) *
								 glm::translate(glm::mat4(1), glm::vec3(1, 0, 0));

		RenderSimpleMesh(meshes["quad"], shaders["ShaderLab9"], modelMatrixH, mapTextures["cyan"], NULL, false);

		modelMatrixH = glm::translate(glm::mat4(1), glm::vec3(0.3, 0.8, 0)) *
					   glm::scale(glm::mat4(1), glm::vec3( 0.3, 0.1, 1)) *
					   glm::translate(glm::mat4(1), glm::vec3(1, 0, 0));

		RenderSimpleMesh(meshes["quad"], shaders["ShaderLab9"], modelMatrixH, mapTextures["red"], NULL, false);

	}
	//fum

	if (viatza>0 && vieti != 0)
	{
		
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 7,-2));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, piloty, 0));
	//	modelMatrix = glm::rotate(modelMatrix, RADIANS(curve), glm::vec3(0, 0, 1));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0,1,0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.50));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		RenderSimpleMesh(meshes["quad"], shaders["Smoke"], modelMatrix, mapTextures["smoke"]);
		glDisable(GL_BLEND);		
	}
	else
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 7, -2));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, cazi, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(90), glm::vec3(0, 1, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(unghi), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.50));
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		RenderSimpleMesh(meshes["quad"], shaders["Smoke"], modelMatrix, mapTextures["smoke"]);
		glDisable(GL_BLEND);


	}
	sign = 0;
	float r1 = 1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (5 - 4)));
	{
		

		glm::mat4 modelMatrix = glm::mat4(1);

		modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloud2*3), glm::vec3(3, 0,0 ));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-4, 10, -5));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloud1), glm::vec3(1, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.50));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderLab9"], modelMatrix, mapTextures["white"]);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloud2 *3), glm::vec3(3 , 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-4.2, 10.5, -5.3));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloud1), glm::vec3(-1, 1, -1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.50));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderLab9"], modelMatrix, mapTextures["white"]);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloud2*3), glm::vec3(3, 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-4, 11, -5));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloud1), glm::vec3(-1, 1, -2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.30));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderLab9"], modelMatrix, mapTextures["white"]);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloud3*3), glm::vec3(3, 0,0 ));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 10, 5));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloud1), glm::vec3(1, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.50));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderLab9"], modelMatrix, mapTextures["white"]);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloud3 *3), glm::vec3(3 , 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 10.5,5.3));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloud1), glm::vec3(-1, 1, -2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.50));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderLab9"], modelMatrix, mapTextures["white"]);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloud3*3), glm::vec3(3, 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 11, 5));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloud1), glm::vec3(-1, 1, -2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.30));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderLab9"], modelMatrix, mapTextures["white"]);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloud2*3), glm::vec3(1, 0,0 ));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2,-1, 12));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloud1), glm::vec3(1, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.50));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderLab9"], modelMatrix, mapTextures["white"]);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloud2 *3), glm::vec3(1 , 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2, -1.5, 12));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloud1), glm::vec3(-1, 1, -2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.50));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderLab9"], modelMatrix, mapTextures["white"]);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloud2*3), glm::vec3(1, 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2,-2, 12));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloud1), glm::vec3(-1, 1, -2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.30));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderLab9"], modelMatrix, mapTextures["white"]);


		modelMatrix = glm::mat4(1);
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloud3*3), glm::vec3(1, 0,0 ));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2, -1, 14));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloud1), glm::vec3(1, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.50));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderLab9"], modelMatrix, mapTextures["white"]);

		modelMatrix = glm::mat4(1);
			modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloud3 *3), glm::vec3(1 , 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2, -1.5, 14));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloud1), glm::vec3(-1, 1, -2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.50));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderLab9"], modelMatrix, mapTextures["white"]);

		modelMatrix = glm::mat4(1);
			modelMatrix = glm::rotate(modelMatrix, RADIANS(-cloud3*3), glm::vec3(1, 0, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2, -2, 14));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(cloud1), glm::vec3(-1, 1, -2));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.30));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderLab9"], modelMatrix, mapTextures["white"]);

	}
	//inimi
	{

		glm::mat4 modelMatrix = glm::mat4(1);
		if (vieti >= 1) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glm::mat4 modelMatrixH =
					glm::translate(glm::mat4(1), glm::vec3(0.3, 0.8, 0)) *
					glm::scale(glm::mat4(1), glm::vec3(0.1)) *
					glm::translate(glm::mat4(1), glm::vec3(-12, 1, 0))*glm::rotate(glm::mat4(1), RADIANS(180), glm::vec3(0, 0, 0.01));

				RenderSimpleMesh(meshes["quad"], shaders["ShaderLab9"], modelMatrixH, mapTextures["inima"], NULL, false);
				glDisable(GL_BLEND);
		}
		
		if (vieti >= 2) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glm::mat4 modelMatrixH =
				glm::translate(glm::mat4(1), glm::vec3(0.3, 0.8, 0)) *
				glm::scale(glm::mat4(1), glm::vec3(0.1)) *
				glm::translate(glm::mat4(1), glm::vec3(-10.3, 1, 0))*glm::rotate(glm::mat4(1), RADIANS(180), glm::vec3(0, 0, 0.01));

			RenderSimpleMesh(meshes["quad"], shaders["ShaderLab9"], modelMatrixH, mapTextures["inima"], NULL, false);
			glDisable(GL_BLEND);
		}


		if (vieti >= 3) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glm::mat4 modelMatrixH =
				glm::translate(glm::mat4(1), glm::vec3(0.3, 0.8, 0)) *
				glm::scale(glm::mat4(1), glm::vec3(0.1)) *
				glm::translate(glm::mat4(1), glm::vec3(-8.6, 1, 0))*glm::rotate(glm::mat4(1), RADIANS(180), glm::vec3(0, 0, 0.01));

			RenderSimpleMesh(meshes["quad"], shaders["ShaderLab9"], modelMatrixH, mapTextures["inima"], NULL, false);
			glDisable(GL_BLEND);
		}
		
	}
	//obstacol1
	{
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 4, cubeX));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
			Render(meshes["cube"], shaders["light"], modelMatrix, glm::vec3(1, 1, 0));


			glm::vec3 planePosition = glm::vec3(planeModelMatrix[3]);
			glm::vec3 obstaclePosition = glm::vec3(modelMatrix[3]);

			float distance = formula(planePosition.x, planePosition.y, planePosition.z,
				obstaclePosition.x, obstaclePosition.y, obstaclePosition.z);


			threshold = 0.5;
			if (distance < threshold)
			{
				if (vieti > 0 && viatza > 0)
				{
					vieti--;
					cubeX = 12;
				}
			}
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 8, cubeX1));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(helixang), glm::vec3(1, 0, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
			Render(meshes["cube"], shaders["light"], modelMatrix, glm::vec3(1, 1, 0));
			planePosition = glm::vec3(planeModelMatrix[3]);
			obstaclePosition = glm::vec3(modelMatrix[3]);

			distance = formula(planePosition.x, planePosition.y, planePosition.z,
				obstaclePosition.x, obstaclePosition.y, obstaclePosition.z);


			threshold = 0.5;
			if (distance < threshold)
			{
				if (vieti > 0 && viatza > 0)
				{
					vieti--;
					cubeX1 = 22;
				}
			}
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 12, cubeX3));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(270), glm::vec3(1, 0, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
			Render(meshes["cube"], shaders["light"], modelMatrix, glm::vec3(1, 1, 0));
			planePosition = glm::vec3(planeModelMatrix[3]);
			obstaclePosition = glm::vec3(modelMatrix[3]);

			distance = formula(planePosition.x, planePosition.y, planePosition.z,
				obstaclePosition.x, obstaclePosition.y, obstaclePosition.z);


			threshold = 0.5;
			if (distance < threshold)
			{
				if (vieti > 0 && viatza > 0)
				{
					vieti--;
					cubeX3 = 17;
				}
			}
			//ia benzina
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, pozbenz, cubeX4));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(helixang), glm::vec3(10, 10, 10));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
			Render(meshes["cube"], shaders["light"], modelMatrix, glm::vec3(1, 0, 0));
			planePosition = glm::vec3(planeModelMatrix[3]);
			obstaclePosition = glm::vec3(modelMatrix[3]);

			distance = formula(planePosition.x, planePosition.y, planePosition.z,
				obstaclePosition.x, obstaclePosition.y, obstaclePosition.z);


			threshold = 0.5;
			if (distance < threshold)
			{
				if (vieti > 0 && viatza > 0)
				{
					umple = true;
					cubeX4 = 15;
					pozbenz = 3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (12 - 3)));
				}
			}
			//bonuslife
			modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0, pozviata, cubeX5));
			modelMatrix = glm::rotate(modelMatrix, RADIANS(helixang), glm::vec3(10, 10, 10));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
			Render(meshes["cube"], shaders["light"], modelMatrix, glm::vec3(0, 0, 1));
			planePosition = glm::vec3(planeModelMatrix[3]);
			obstaclePosition = glm::vec3(modelMatrix[3]);

			distance = formula(planePosition.x, planePosition.y, planePosition.z,
				obstaclePosition.x, obstaclePosition.y, obstaclePosition.z);


			threshold = 0.5;
			if (distance < threshold)
			{
				if (vieti > 0 && viatza > 0 && vieti <3)
				{
					vieti++;
					cubeX5 = 27;
					pozviata = 3 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (11 - 3)));
				}
			}
		}

		//farurile
		if(vieti > 0 && viatza > 0)
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, lightPosition);
			lightPosition =glm::vec3(0, 7+piloty, 0);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
			//Render(meshes["sphere"], shaders["light"], modelMatrix, glm::vec3(1, 1, 0));
		}
		else
			lightPosition = glm::vec3(100, -100, 100);

	}

}

void Laborator9::FrameEnd()
{
	//DrawCoordinatSystem();
}

void Laborator9::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, Texture2D* texture1, Texture2D* texture2, bool useViewAndProjection)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;
	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	int timeLoc = glGetUniformLocation(shader->program, "time");
	glUniform1f(timeLoc, Engine::GetElapsedTime());

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	if (useViewAndProjection)
	{
	
		glm::mat4 viewMatrix = glm::lookAt(glm::vec3(-12, 7, 0), glm::vec3(0, 7, 0), glm::vec3(0, 1, 0));
		if (tard_parsan)
		{
			viewMatrix = glm::lookAt(glm::vec3(0, piloty+7, -5), glm::vec3(0,piloty+7 , 0), glm::vec3(0, 1, 0));
		}
		int loc_view_matrix = glGetUniformLocation(shader->program, "View");
		glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

		// Bind projection matrix
		int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
		glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}
	else 
	{
		int loc_view_matrix = glGetUniformLocation(shader->program, "View");
		glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
		int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
		glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
	}

	if (texture1)
	{
		int i = 0;
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->program, "texture_1"), i);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Laborator9::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_I)) angleOX += deltaTime * 5;
	if (window->KeyHold(GLFW_KEY_J)) angleOY += deltaTime * 5;
	if (window->KeyHold(GLFW_KEY_K)) angleOX -= deltaTime * 5;
	if (window->KeyHold(GLFW_KEY_L)) angleOY -= deltaTime * 5;


	if (window->KeyHold(GLFW_KEY_F)) cutoff_angle += deltaTime * 20;
	if (window->KeyHold(GLFW_KEY_G)) cutoff_angle -= deltaTime * 20;

	glm::mat4 turn = glm::mat4(1);
	turn = glm::rotate(turn, angleOY, glm::vec3(0, 1, 0));
	turn = glm::rotate(turn, angleOX, glm::vec3(1, 0, 0));

	lightDirection = glm::vec3(0, -1, 0);
	lightDirection = glm::vec3(turn * glm::vec4(lightDirection, 0));

	
}

void Laborator9::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_T)
	{
		tard_parsan = !tard_parsan;
	}
	if (key == GLFW_KEY_X)
	{
		if(vieti>0)
		vieti--;
	}
	else
		if (key == GLFW_KEY_Z)
		{
			if(vieti<3)
			vieti++;
		}
	if (key == GLFW_KEY_M) {
		//umplere += 0.01;
		umple = true;
	}
}

void Laborator9::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator9::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (viatza > 0 && vieti != 0)
	{
		piloty += -deltaY * 0.03;
	}
	
	if (deltaY >= 0)
		sign = -1;
	else
		sign = 1;

}

void Laborator9::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator9::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator9::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator9::OnWindowResize(int width, int height)
{
}

void Laborator9::Render(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform
	int light_position = glGetUniformLocation(shader->program, "light_position");
	glUniform3f(light_position, lightPosition.x, lightPosition.y, lightPosition.z);

	int light_direction = glGetUniformLocation(shader->program, "light_direction");
	glUniform3f(light_direction, lightDirection.x, lightDirection.y, lightDirection.z);

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 
	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int object_color = glGetUniformLocation(shader->program, "object_color");
	glUniform3f(object_color, color.r, color.g, color.b);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(-12, 7, 0), glm::vec3(0, 7, 0), glm::vec3(0, 1, 0));
	if (tard_parsan)
	{
		viewMatrix = glm::lookAt(glm::vec3(0, piloty + 7, -5), glm::vec3(0, piloty + 7, 0), glm::vec3(0, 1, 0));
	}
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int type = glGetUniformLocation(shader->program, "t");
	glUniform1i(type, typeOfLight);

	int cut_off_angle = glGetUniformLocation(shader->program, "cut_off_angle");
	glUniform1f(cut_off_angle, cutoff_angle);

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}
