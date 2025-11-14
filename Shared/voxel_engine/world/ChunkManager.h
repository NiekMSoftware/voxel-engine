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
	/**
	 * @brief Custom hash function for using glm::ivec3 as a map key
	 */
	struct IVec3Hash
	{
		std::size_t operator()(const glm::ivec3& v) const
		{
			return std::hash<int>()(v.x) ^
				(std::hash<int>()(v.y) << 1) ^
				(std::hash<int>()(v.z) << 2);
		}
	};

	/**
	 * @brief Manages all chunks in the world
	 *
	 * Follows the chunk management pattern from "Let's Make a Voxel Engine":
	 * https://sites.google.com/site/letsmakeavoxelengine/home/chunk-management
	 *
	 * - Stores chunks in a hash map for fast lookup
	 * - Provides world-to-chunk coordinate conversion
	 * - Manages chunk creation and retrieval
	 * - Converts between world and chunk-local coordinates
	 */
	class ChunkManager
	{
		GLuint mCubeVBO;
		std::unordered_map<glm::ivec3, std::unique_ptr<Chunk>, IVec3Hash> mChunks;

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
		/**
		 * @brief Convert world coordinates to chunk coordinates
		 * Uses bit shift for efficient division by 16
		 */
		glm::ivec3 WorldToChunkPos(int worldX, int worldY, int worldZ);

		/**
		 * @brief Convert world coordinates to chunk-local coordinates
		 * Uses bitwise AND for efficient modulo 16
		 */
		glm::ivec3 WorldToLocalPos(int worldX, int worldY, int worldZ);
	};
}