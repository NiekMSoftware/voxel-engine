#pragma once

#include <cstdint>
#include <vector>

#ifdef WINDOWS_BUILD
#include <glad/glad.h>
#endif

#ifdef Raspberry_BUILD
#include <GLES3/gl3.h>
#endif
#include <glm/vec3.hpp>

constexpr uint8_t CHUNK_SIZE = 16;

namespace voxel_engine::world
{
	struct Voxel 
	{
		uint8_t type;  // 0 = air, 1 = grass, etc...
		uint8_t textureID;

		bool IsActive() const { return type != 0; }
	};

	struct VoxelInstance 
	{
		glm::vec3 position;
		uint8_t textureID;

		// Align to 16 bytes for GPU
		uint8_t padding[3];
	};

	class Chunk 
	{
		glm::ivec3 mChunkPos;
		Voxel mVoxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
		
		// OpenGL Resources
		GLuint mChunkVAO;
		GLuint mInstanceVBO;
		size_t mInstanceCount;
		bool bIsDirty;

	public:
		Chunk(const glm::ivec3 &chunkPos, GLuint cubeVBO);
		~Chunk();

		// Voxel access
		Voxel *GetVoxel(int x, int y, int z);
		void SetVoxel(int x, int y, int z, const Voxel &voxel);

		// Rendering
		void BuildInstanceData();
		void Render();

		// State
		bool IsDirty() const { return bIsDirty; }
		void MarkDirty() { bIsDirty = true; }
		const glm::ivec3 &GetPosition() const { return mChunkPos; }
		size_t GetInstanceCount() const { return mInstanceCount; }

	private:
		// Helper
		bool IsVoxelVisible(int x, int y, int z) const;
	};
}

// EoF