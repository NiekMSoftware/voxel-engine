#include "ChunkManager.h"
#include "Chunk.h"

namespace voxel_engine::world
{
	ChunkManager::ChunkManager(const GLuint cubeVBO)
		: mCubeVBO(cubeVBO)
	{ }

	// Get all chunks for rendering
	// Following the chunk management pattern from "Let's Make a Voxel Engine":
	// https://sites.google.com/site/letsmakeavoxelengine/home/chunk-management
	std::vector<Chunk*> ChunkManager::GetAllChunks()
	{
		std::vector<Chunk*> chunks;
		chunks.reserve(mChunks.size());

		for (auto& [pos, chunk] : mChunks) 
		{
			chunks.push_back(chunk.get());
		}

		return chunks;
	}

	Chunk* ChunkManager::GetChunk(const glm::ivec3& chunkPos)
	{
		const auto it = mChunks.find(chunkPos);
		return it != mChunks.end() ? it->second.get() : nullptr;
	}

	Chunk* ChunkManager::CreateChunk(const glm::ivec3& chunkPos)
	{
		auto chunk = std::make_unique<Chunk>(chunkPos, mCubeVBO);
		Chunk *ptr = chunk.get();
		mChunks[chunkPos] = std::move(chunk);
		return ptr;
	}

	Voxel* ChunkManager::GetVoxel(const int worldX, const int worldY, const int worldZ)
	{
		const glm::ivec3 chunkPos = WorldToChunkPos(worldX, worldY, worldZ);
		const glm::ivec3 localPos = WorldToLocalPos(worldX, worldY, worldZ);

		Chunk* chunk = GetChunk(chunkPos);
		if (!chunk) return nullptr;

		return chunk->GetVoxel(localPos.x, localPos.y, localPos.z);
	}

	void ChunkManager::SetVoxel(const int worldX, const int worldY, const int worldZ, const Voxel& voxel)
	{
		const glm::ivec3 chunkPos = WorldToChunkPos(worldX, worldY, worldZ);
		const glm::ivec3 localPos = WorldToLocalPos(worldX, worldY, worldZ);

		if (Chunk *chunk = GetChunk(chunkPos)) 
		{
			chunk->SetVoxel(localPos.x, localPos.y, localPos.z, voxel);
		}
	}

	// Convert world coordinates to chunk coordinates
	// Uses bit shifting for fast division by 16 (CHUNK_SIZE)
	// >> 4 is equivalent to / 16
	glm::ivec3 ChunkManager::WorldToChunkPos(const int worldX, const int worldY, const int worldZ) {
		return glm::ivec3(
			worldX >> 4,  
			0,            
			worldZ >> 4
		);
	}

	// Convert world coordinates to chunk-local coordinates
	// Uses bitwise AND with 15 for fast modulo 16
	// & 15 is equivalent to % 16
	glm::ivec3 ChunkManager::WorldToLocalPos(const int worldX, const int worldY, const int worldZ) {
		// Modulo 16
		return glm::ivec3(
			worldX & 15,  
			0,
			worldZ & 15
		);
	}

}

// EoF