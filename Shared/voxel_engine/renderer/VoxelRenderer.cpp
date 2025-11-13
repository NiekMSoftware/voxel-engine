#include "VoxelRenderer.h"
#include "voxel_engine/world/Chunk.h"
#include <glm/gtc/matrix_transform.hpp>

namespace voxel_engine::rendering
{
	VoxelRenderer::VoxelRenderer()
		: mCubeVAO(0)
		, mCubeVBO(0)
		, mShaderProgram(0)
		, mTextureArray(0)
	{ }

	VoxelRenderer::~VoxelRenderer()
	{
		if (mCubeVAO) glDeleteVertexArrays(1, &mCubeVAO);
		if (mCubeVBO) glDeleteBuffers(1, &mCubeVBO);
		if (mShaderProgram) glDeleteProgram(mShaderProgram);
		if (mTextureArray) glDeleteTextures(1, &mTextureArray);
	}

	void VoxelRenderer::Initialize()
	{
		CreateCubeMesh();
		LoadShaders();
		LoadTextures();
	}

	void VoxelRenderer::CreateCubeMesh()
	{
        const float vertices[] = {
            // position(3)        UV (2)       normal (3) = 8 floats per vertex
            // Front face (+Z)
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f,  1.0f,

            // Back face (-Z)
             0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

             // Top face (+Y)
             -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
             -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
              0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
              0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
              0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
             -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,

             // Bottom face (-Y)
             -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
             -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
              0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
              0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
              0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
             -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,

             // Right face (+X)
              0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
              0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
              0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
              0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
              0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
              0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,

              // Left face (-X)
              -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
              -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
              -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
              -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
              -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
              -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
        };

        glGenVertexArrays(1, &mCubeVAO);
        glGenBuffers(1, &mCubeVBO);

        glBindVertexArray(mCubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, mCubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position (location 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
        glEnableVertexAttribArray(0);

        // UV (location 3)
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(3);

        // Normal (location 4)
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(4);

        glBindVertexArray(0);
	}

	void VoxelRenderer::LoadShaders()
	{
        const char *vertexShaderSource = R"(
        #version 330 core
        layout(location = 0) in vec3 aPosition;
        layout(location = 1) in vec3 aInstancePos;
        layout(location = 2) in uint aTextureID;
        layout(location = 3) in vec2 aUV;
        layout(location = 4) in vec3 aNormal;
        
        uniform mat4 view;
        uniform mat4 projection;
        
        out vec2 vUV;
        out vec3 vNormal;
        flat out uint vTextureID;
        
        void main() {
            vec3 worldPos = aPosition + aInstancePos;
            gl_Position = projection * view * vec4(worldPos, 1.0);
            vUV = aUV;
            vNormal = aNormal;
            vTextureID = aTextureID;
        }
		)";

        // Simple fragment shader
        const char *fragmentShaderSource = R"(
        #version 330 core
        in vec2 vUV;
        in vec3 vNormal;
        flat in uint vTextureID;
        
        out vec4 fragColor;
        
        uniform sampler2DArray texArray;
        
        void main() {
            // Simple directional lighting
            vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
            float diffuse = max(dot(vNormal, lightDir), 0.3);
            
            vec3 color = texture(texArray, vec3(vUV, float(vTextureID))).rgb;
            fragColor = vec4(color * diffuse, 1.0);
        }
		)";

        // Compile shaders
        const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glCompileShader(vertexShader);

        const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        glCompileShader(fragmentShader);

        // Link program
        mShaderProgram = glCreateProgram();
        glAttachShader(mShaderProgram, vertexShader);
        glAttachShader(mShaderProgram, fragmentShader);
        glLinkProgram(mShaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
	}

	void VoxelRenderer::LoadTextures()
	{
        // TODO: Load a texture atlas and initialize properties

        glGenTextures(1, &mTextureArray);
        glBindTexture(GL_TEXTURE_2D_ARRAY, mTextureArray);

        // TODO: Create texture atlas
        glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB8, 16, 16, 3, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        // Fill with placeholder colors
        unsigned char grassData[16 * 16 * 3];
        for (int i = 0; i < 16 * 16 * 3; i += 3) {
            grassData[i] = 50; grassData[i + 1] = 150; grassData[i + 2] = 50;
        }
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, 16, 16, 1, GL_RGB, GL_UNSIGNED_BYTE, grassData);

        unsigned char dirtData[16 * 16 * 3];
        for (int i = 0; i < 16 * 16 * 3; i += 3) {
            dirtData[i] = 139; dirtData[i + 1] = 90; dirtData[i + 2] = 43;
        }
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 1, 16, 16, 1, GL_RGB, GL_UNSIGNED_BYTE, dirtData);

        unsigned char stoneData[16 * 16 * 3];
        for (int i = 0; i < 16 * 16 * 3; i += 3) {
            stoneData[i] = 128; stoneData[i + 1] = 128; stoneData[i + 2] = 128;
        }
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 2, 16, 16, 1, GL_RGB, GL_UNSIGNED_BYTE, stoneData);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

    bool VoxelRenderer::IsChunkVisible(const glm::vec3& chunkWorldPos, const glm::vec3& cameraPos,
        const glm::vec3& cameraFront, const float renderDistance) const
	{
        const glm::vec3 chunkCenter = chunkWorldPos + glm::vec3(8.0f);
        const float distance = glm::length(chunkCenter - cameraPos);

        // Always render chunks within distance
        if (distance < 24.0f) {
            return true;
        }

        // Distance culling
        if (distance > renderDistance) {
            return false;  // too far
        }

        // Simply frustum check
        const glm::vec3 toChunk = glm::normalize(chunkCenter - cameraPos);
        const float dotProduct = glm::dot(toChunk, cameraFront);
        if (dotProduct < -0.2f) {  // slightly behind
            return false;
        }

        return true;
	}

	void VoxelRenderer::Render(const std::vector<world::Chunk*>& chunks, const glm::mat4 &view,
		const glm::mat4 &projection)
	{
        glUseProgram(mShaderProgram);

        // Set uniforms
        const GLint viewLoc = glGetUniformLocation(mShaderProgram, "view");
        const GLint projLoc = glGetUniformLocation(mShaderProgram, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

        // Bind texture array
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, mTextureArray);
        glUniform1i(glGetUniformLocation(mShaderProgram, "texArray"), 0);

        // Extract camera position and direction from view matrix
        const glm::vec3 cameraPos = glm::vec3(glm::inverse(view)[3]);
        const glm::vec3 cameraFront = -glm::vec3(view[0][2], view[1][2], view[2][2]);

        int chunksRendered = 0;
        int chunksSkipped = 0;

        // Render visible chunks only
        for (world::Chunk *chunk : chunks) {
            glm::vec3 chunkWorldPos = glm::vec3(chunk->GetPosition()) * 16.0f;

            // Frustum culling
            if (!IsChunkVisible(chunkWorldPos, cameraPos, cameraFront, mRenderDistance)) {
                chunksSkipped++;
                continue;
            }

            if (chunk->IsDirty()) {
                chunk->BuildInstanceData();
            }

            // Skip empty chunks
            if (chunk->GetInstanceCount() > 0) {
                chunk->Render();
                chunksRendered++;
            }
        }

        glBindVertexArray(0);

        static float debugTimer = 0.0f;
        debugTimer += 0.016f;  // Approximate
        if (debugTimer > 1.0f) {
            printf("Rendered: %d chunks, Culled: %d chunks\n", chunksRendered, chunksSkipped);
            debugTimer = 0.0f;
        }
	}
}

// EoF