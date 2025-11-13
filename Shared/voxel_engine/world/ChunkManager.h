#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

#ifdef WINDOWS_BUILD
#include <glad/glad.h>
#endif

#ifdef Raspberry_BUILD
#include <GLES3/gl3.h>
#endif

#include "Chunk.h"

namespace voxel_engine::world
{
	struct Voxel;
	class Chunk;

	struct IVec3Hash
	{
		std::size_t operator()(const glm::ivec3& v) const
		{
			return std::hash<int>()(v.x) ^
				(std::hash<int>()(v.y) << 1) ^
				(std::hash<int>()(v.z) << 2);
		}
	};

	class ChunkManager
	{
		std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IVec3Hash> mChunks;
		GLuint mCubeVBO;

	public:
		ChunkManager(GLuint cubeVBO);
		~ChunkManager() = default;

		// Get chunks for rendering
		std::vector<Chunk*> GetAllChunks();

		// Chunk access
		Chunk *GetChunk(const glm::ivec3 &chunkPos);
		Chunk *CreateChunk(const glm::ivec3 &chunkPos);

		Voxel *GetVoxel(int worldX, int worldY, int worldZ);
		void SetVoxel(int worldX, int worldY, int worldZ, const Voxel &voxel);

	private:
		glm::ivec3 WorldToChunkPos(int worldX, int worldY, int worldZ);
		glm::ivec3 WorldToLocalPos(int worldX, int worldY, int worldZ);
	};
}