#include "Chunk.h"

namespace voxel_engine::world
{
	Chunk::Chunk(const glm::ivec3 &chunkPos, GLuint cubeVBO) 
		: mChunkPos(chunkPos)
		, mInstanceVBO(0)
		, mChunkVAO(0)
		, mInstanceCount(0)
		, bIsDirty(true)
	{
		// Initialize all voxels as air
		memset(mVoxels, 0, sizeof(mVoxels));

		// Create buffers
		glGenBuffers(1, &mInstanceVBO);
		glGenVertexArrays(1, &mChunkVAO);

		glBindVertexArray(mChunkVAO);

		// Cube mesh attributes (shared across chunks)
		// 
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

		// Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
		glEnableVertexAttribArray(0);

		// UV
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(3);

		// Normal
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(4);
		//
		// End of Cube mesh attributes

		// Instanced attributes
		glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);

		// Instanced position attrib
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VoxelInstance), (void*)nullptr);
		glVertexAttribDivisor(1, 1);

		// Instanced texture attrib
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

	bool Chunk::IsVoxelVisible(int x, int y, int z) const 
	{
		const Voxel &voxel = mVoxels[x][y][z];
		if (!voxel.IsActive()) return false;

		// Check if any neighboring voxel is air (visible face)
		auto checkNeighbor = [&](int nx, int ny, int nz) 
		{
			if (nx < 0 || nx >= CHUNK_SIZE ||
				ny < 0 || ny >= CHUNK_SIZE ||
				nz < 0 || nz >= CHUNK_SIZE) 
				return true;

			return !mVoxels[nx][ny][nz].IsActive();
		};

		return checkNeighbor(x - 1, y, z) || checkNeighbor(x + 1, y, z) ||
			   checkNeighbor(x, y - 1, z) || checkNeighbor(x, y + 1, z) ||
			   checkNeighbor(x, y, z - 1) || checkNeighbor(x, y, z + 1);
	}

	void Chunk::BuildInstanceData() 
	{
		if (!bIsDirty) return;

		std::vector<VoxelInstance> instances;
		instances.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE / 4);

		// World-space chunk offset
		glm::vec3 chunkOffset = glm::vec3(mChunkPos) * glm::vec3(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE);

		// Build instance data for visible voxels
		for (int x = 0; x < CHUNK_SIZE; x++)
		{
			for (int y = 0; y < CHUNK_SIZE; y++)
			{
				for (int z = 0; z < CHUNK_SIZE; z++)
				{
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

		// Upload to GPU
		glBindBuffer(GL_ARRAY_BUFFER, mInstanceVBO);
		glBufferData(GL_ARRAY_BUFFER,
					 instances.size() * sizeof(VoxelInstance),
					 instances.data(),
					 GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		bIsDirty = false;
	}

	void Chunk::Render() 
	{
		if (mInstanceCount == 0) return;

		// Draw all instances
		glBindVertexArray(mChunkVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 36, static_cast<GLsizei>(mInstanceCount));
	}
}