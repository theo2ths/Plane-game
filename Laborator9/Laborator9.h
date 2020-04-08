#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include "LabCamera.h"
class Laborator9 : public SimpleScene
{
	public:
		Laborator9();
		~Laborator9();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL, bool useViewAndProjection = true);

		void Render(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void DrawWaterBlob(glm::vec3 pos, glm::vec3 scale, int numBlobs,std::string fig);
		void OnWindowResize(int width, int height) override;
		float pilotx, piloty, pilotz;
		float helixang = 30.f;
		std::unordered_map<std::string, Texture2D*> mapTextures;
		GLuint randomTextureID;
		glm::vec3 lightPosition;
		glm::vec3 lightDirection;
		unsigned int materialShininess;
		float materialKd;
		float materialKs;
		int typeOfLight = 1;
		float angleOX = 45, angleOY = 0;
		float cutoff_angle;
	protected:
		Laborator::Camera *camera;
		glm::mat4 projectionMatrix;
		float translateX, translateY, translateZ;
		float scaleX, scaleY, scaleZ;
		float angularStepOX, angularStepOY, angularStepOZ;
		bool renderCameraTarget;
		bool tard_parsan = false;
		float viatza = 1.f;
};
