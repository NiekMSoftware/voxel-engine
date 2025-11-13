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
	/**
	 * @brief Represents a single voxel (block)
	 * Minimal data structure - just type and texture ID
	 */
	struct Voxel 
	{
		uint8_t type;		// 0 = air, 1 = grass, 2 = dirt, etc.
		uint8_t textureID;	// Index into texture array

		bool IsActive() const { return type != 0; }
	};

	/**
	 * @brief Per-instance data sent to GPU for rendering
	 * Used by instanced rendering to position and texture each voxel
	 */
	struct VoxelInstance 
	{
		glm::vec3 position;		// World position of this voxel
		uint8_t textureID;		// Which texture layer to use

		// Align to 16 bytes for GPU
		uint8_t padding[3];
	};

	/**
	 * @brief A 16x16x16 grid of voxels with its own mesh
	 *
	 * Each chunk:
	 * - Stores voxel data in a 3D array
	 * - Maintains its own VAO and instance buffer
	 * - Performs face culling to only render visible voxels
	 * - Uses dirty flag pattern for efficient updates
	 *
	 * References:
	 * - Chunk architecture: https://sites.google.com/site/letsmakeavoxelengine/home/basic-block-rendering
	 * - Minecraft technical discussion: https://www.realtimerendering.com/blog/really-another-minecraft-article/
	 */
	class Chunk 
	{
		size_t mInstanceCount;

		glm::ivec3 mChunkPos;
		Voxel mVoxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
		
		// OpenGL Resources
		GLuint mChunkVAO;
		GLuint mInstanceVBO;
		bool bIsDirty;

	public:
		Chunk(const glm::ivec3 &chunkPos, GLuint cubeVBO);
		~Chunk();

		// Voxel access
		Voxel *GetVoxel(int x, int y, int z);
		void SetVoxel(int x, int y, int z, const Voxel &voxel);

		/**
		 * @brief Rebuild instance buffer with visible voxels
		 * Called when chunk is modified (dirty flag set)
		 */
		void BuildInstanceData();

		/**
		 * @brief Render all voxels in one instanced draw call
		 */
		void Render();

		// State
		bool IsDirty() const { return bIsDirty; }
		void MarkDirty() { bIsDirty = true; }
		const glm::ivec3 &GetPosition() const { return mChunkPos; }
		size_t GetInstanceCount() const { return mInstanceCount; }

	private:
		/**
		 * @brief Naive face culling - checks if voxel has air neighbors
		 * Based on "Meshing in a Minecraft Game" culling approach:
		 * https://0fps.net/2012/06/30/meshing-in-a-minecraft-game/
		 */
		bool IsVoxelVisible(int x, int y, int z) const;
	};
}

// EoF