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
	/**
	 * @brief Main renderer for voxel engine using instanced rendering
	 *
	 * Implements efficient voxel rendering using:
	 * - Instanced rendering: https://learnopengl.com/Advanced-OpenGL/Instancing
	 * - Texture arrays: https://www.khronos.org/opengl/wiki/Array_Texture
	 * - Chunk-based frustum culling: https://sites.google.com/site/letsmakeavoxelengine/home/frustum-culling
	 * - Shared base mesh for all voxels
	 */
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

		/**
		* @brief Render all chunks using instancing
		* Performs frustum culling and renders visible chunks
		*/
		void Render(const std::vector<world::Chunk*>& chunks,
			const glm::mat4 &view,
			const glm::mat4 &projection);

		void SetRenderDistance(const int distance) { mRenderDistance = (float)distance * 16.0f; }
		GLuint GetCubeVBO() const { return mCubeVBO; }

	private:
		/**
		 * @brief Simple frustum culling test for chunks
		 * Based on distance and dot product with camera forward
		 */
		bool IsChunkVisible(const glm::vec3 &chunkWorldPos,
			const glm::vec3 &cameraPos,
			const glm::vec3 &cameraFront,
			float renderDistance) const;

		void CreateCubeMesh();
		void LoadShaders();
		void LoadTextures();
	};

}