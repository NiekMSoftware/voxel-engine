#include "Chunk.h"
#include <cstring>

namespace voxel_engine::world
{
	Chunk::Chunk(const glm::ivec3 &chunkPos, const GLuint cubeVBO) 
		: mInstanceCount(0)
		, mChunkPos(chunkPos)
		, mChunkVAO(0)
		, mInstanceVBO(0)
		, bIsDirty(true)
	{
		// Initialize all voxels as air
		memset(mVoxels, 0, sizeof(mVoxels));

		// Create OpenGL buffers for this chunk
		// Each chunk has its own VAO and instance buffer
		glGenBuffers(1, &mInstanceVBO);
		glGenVertexArrays(1, &mChunkVAO);

		glBindVertexArray(mChunkVAO);

		// ===================================================================
		// SECTION 1: Base cube mesh attributes (shared, from VoxelRenderer)
		// These use divisor=0 meaning they advance per-vertex
		// ===================================================================
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

		// Position (location 0)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
		glEnableVertexAttribArray(0);

		// UV (location 3)
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(3);

		// Normal (location 4)
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(4);

		// ===================================================================
		// SECTION 2: Per-instance attributes
		// These use divisor=1 meaning they advance per-instance
		// This is the key to instanced rendering:
		// - LearnOpenGL: https://learnopengl.com/Advanced-OpenGL/Instancing
		// - OpenGL Tutorial: http://www.opengl-tutorial.org/intermediate-tutorials/billboards-particles/particles-instancing/
		// ===================================================================
		glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);

		// Instance position (location 1) - where to draw this voxel
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelInstance), (void*)nullptr);
		glVertexAttribDivisor(1, 1);

		// Instance texture ID (location 2) - which texture layer to use
		glEnableVertexAttribArray(2);
		glVertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, sizeof(VoxelInstance),
			(void*)offsetof(VoxelInstance, textureID));
		glVertexAttribDivisor(2, 1);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	Chunk::~Chunk() 
	{
		if (mInstanceVBO) glDeleteBuffers(1, &mInstanceVBO);
		if (mChunkVAO) glDeleteVertexArrays(1, &mChunkVAO);
	}

	Voxel *Chunk::GetVoxel(int x, int y, int z)
	{
		if (x < 0 || x >= CHUNK_SIZE ||
			y < 0 || y >= CHUNK_SIZE ||
			z < 0 || z >= CHUNK_SIZE)
			return nullptr;

		return &mVoxels[x][y][z];
	}

	void Chunk::SetVoxel(int x, int y, int z, const Voxel &voxel) 
	{
		if (Voxel *v = GetVoxel(x, y, z)) 
		{
			*v = voxel;
			MarkDirty();
		}
	}

	// Naive face culling - checks if voxel has any air neighbors
	// Based on techniques from "Meshing in a Minecraft Game":
	// https://0fps.net/2012/06/30/meshing-in-a-minecraft-game/
	bool Chunk::IsVoxelVisible(int x, int y, int z) const 
	{
		const Voxel &voxel = mVoxels[x][y][z];
		if (!voxel.IsActive()) return false;

		// Lambda to check if a neighbor position is air or out of bounds
		auto checkNeighbor = [&](int nx, int ny, int nz) 
		{
			if (nx < 0 || nx >= CHUNK_SIZE ||
				ny < 0 || ny >= CHUNK_SIZE ||
				nz < 0 || nz >= CHUNK_SIZE) 
				return true;

			return !mVoxels[nx][ny][nz].IsActive();
		};

		// Check all 6 neighbors - if any are air, this voxel needs to be rendered
		return checkNeighbor(x - 1, y, z) || checkNeighbor(x + 1, y, z) ||
			   checkNeighbor(x, y - 1, z) || checkNeighbor(x, y + 1, z) ||
			   checkNeighbor(x, y, z - 1) || checkNeighbor(x, y, z + 1);
	}

	// Build instance data for GPU rendering
	// Creates a compact array of only visible voxels (face culling optimization)
	// This follows the chunk meshing approach from voxel engine tutorials:
	// - Let's Make a Voxel Engine: https://sites.google.com/site/letsmakeavoxelengine/
	// - Voxel rendering discussion: https://www.realtimerendering.com/blog/really-another-minecraft-article/
	void Chunk::BuildInstanceData() 
	{
		if (!bIsDirty) return;

		std::vector<VoxelInstance> instances;
		instances.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE / 4);  // Estimate

		// World-space chunk offset
		glm::vec3 chunkOffset = glm::vec3(mChunkPos) * glm::vec3(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE);

		// Build instance data for visible voxels
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			for (int y = 0; y < CHUNK_SIZE; y++)
			{
				for (int z = 0; z < CHUNK_SIZE; z++)
				{
					// Only create instances for visible voxels
					// This is the naive culling approach - faces are culled at voxel level
					if (IsVoxelVisible(x, y, z))
					{
						VoxelInstance instance{ };
						instance.position = chunkOffset + glm::vec3(x, y, z);
						instance.textureID = mVoxels[x][y][z].textureID;
						instances.push_back(instance);
					}
				}
			}
		}

		mInstanceCount = instances.size();

		// Upload instance data to GPU
		// DYNAMIC_DRAW because chunks can be modified frequently
		glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);
		glBufferData(GL_ARRAY_BUFFER,
					 instances.size() * sizeof(VoxelInstance),
					 instances.data(),
					 GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		bIsDirty = false;
	}

	// Render all voxels in this chunk with a single draw call
	// This is the power of instanced rendering - one call draws thousands of cubes
	void Chunk::Render() 
	{
		if (mInstanceCount == 0) return;

		// Draw all instances
		glBindVertexArray(mChunkVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 36, static_cast<GLsizei>(mInstanceCount));
	}
}