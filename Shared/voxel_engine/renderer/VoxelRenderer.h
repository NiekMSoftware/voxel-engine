#pragma once

#include <vector>
#include <glm/glm.hpp>

#ifdef WINDOWS_BUILD
#include <glad/glad.h>
#endif

#ifdef Raspberry_BUILD
#include <GLES3/gl3.h>
#endif

namespace voxel_engine::world
{
	class Chunk;
}

namespace voxel_engine::rendering
{
	/** @brief A simple renderer to render all the Voxels. */
	class VoxelRenderer
	{
		GLuint mCubeVAO;
		GLuint mCubeVBO;
		GLuint mShaderProgram;
		GLuint mTextureArray;

		float mRenderDistance = 1.0f;

	public:
		VoxelRenderer();
		~VoxelRenderer();

		void Initialize();
		void Render(const std::vector<world::Chunk*>& chunks,
			const glm::mat4 &view,
			const glm::mat4 &projection);

		void SetRenderDistance(const int distance) { mRenderDistance = (float)distance * 16.0f; }
		GLuint GetCubeVBO() const { return mCubeVBO; }

	private:
		bool IsChunkVisible(const glm::vec3 &chunkWorldPos,
			const glm::vec3 &cameraPos,
			const glm::vec3 &cameraFront,
			float renderDistance) const;

		void CreateCubeMesh();
		void LoadShaders();
		void LoadTextures();
	};

}