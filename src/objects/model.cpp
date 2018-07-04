#include <iostream>

#include <ASSIMP/Importer.hpp>
#include <ASSIMP/postprocess.h>

#include "objects/model.hpp"

using namespace Render3D;
using namespace Math3D;

Model::Model(char const *filePath) {
	shaderName = "defaultPerspective";

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	std::string path = std::string(filePath);

	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 

		// Process mesh
		Mesh meshObject = processMesh(mesh, scene);
		meshes.push_back(meshObject);
	}

	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	//Process vertices, normals, and texture coordinates

	std::vector<Vector4> vertices;
	std::vector<Vector4> normals;
	std::vector<TextureCoord> textureCoords;
	std::vector<TextureData> textures;
	std::vector<GLuint> indices;

	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		Vector4 vert;
        aiVector3t<float> aiVec;
        if (mesh->HasPositions()) {
		    aiVec = mesh->mVertices[i];
            vert[0] = aiVec.x;
            vert[1] = aiVec.y;
            vert[2] = aiVec.z;
        } else {
            // will throw exception eventually
        }
		vertices.push_back(vert);

		Vector4 norm;
        if (mesh->HasNormals()) {
            aiVec = mesh->mNormals[i];
            norm[0] = aiVec.x;
            norm[1] = aiVec.y;
            norm[2] = aiVec.z;
        } else {
            // will throw exception eventually
        }
		normals.push_back(norm);

		TextureCoord tex;
		if (mesh->HasTextureCoords(0)) {
			tex.x = mesh->mTextureCoords[0][i].x; 
			tex.y = mesh->mTextureCoords[0][i].y;
		}
		textureCoords.push_back(tex);
	}

	// Process indices

	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++) {
			GLuint index = face.mIndices[j];
			indices.push_back(index);
		}
	} 

	// Process material

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<TextureData> diffuseTextures = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());

		std::vector<TextureData> specularTextures = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
	}

	return Mesh(vertices, normals, textureCoords, textures, indices);
}

std::vector<TextureData> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
	std::vector<TextureData> textures;

	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		GLboolean skip = false;
		for (GLuint j = 0; j < texturesLoaded.size(); j++) {
			if (texturesLoaded[j].path == str) {
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}

		std::cout << typeName << ", " << str.C_Str() << ", " << i << std::endl;

		if (!skip) {   // If texture hasn't been loaded already, load it
			TextureData texture;
			std::cout << str.C_Str() << "   " << directory << std::endl;
			std::string filePath = directory + "/" + std::string(str.C_Str());
			texture.fullPath = filePath;
			texture.type = typeName;
			texture.path = str;
            Texture tex(filePath.c_str());
            textureManager.addTexture(filePath.c_str(), tex);
			textures.push_back(texture);
			texturesLoaded.push_back(texture);
		}
	}

	return textures;
}

void Model::updateVertices() {
	/*GLfloat vertices[3 * indices.size()];

	for (int i = 0; i < indices.size(); i++) {
		Vector4 vert = originalVertices[indices[i]];

		vert = vert * size;

		vertices[i * 3] = vert[0];
		vertices[i * 3 + 1] = vert[1];
		vertices[i * 3 + 2] = vert[2];
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);*/
}

void Model::updateNormals() {
	/*Matrix4x4 rotation = cframe.rotation();

	GLfloat normals[3 * indices.size()];

	for (int i = 0; i < indices.size(); i++) {
		Vector4 norm = originalNormals[indices[i]];

		norm = rotation * norm;

		normals[i * 3] = norm[0];
		normals[i * 3 + 1] = norm[1];
		normals[i * 3 + 2] = norm[2];
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(normals), normals);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);	*/
}

/*int Model::getVertexCount() {
	return indices.size();
}*/

void Model::bindTextures(Shader& shader, TextureManager& textureManager) {

}

void Model::render(Shader& shader) {
	Matrix4x4 rotation = cframe.rotation();

	shader.setVariable("modelCFrame", cframe);
	shader.setVariable("modelRotation", rotation);
	shader.setVariable("modelSize", size);
	shader.setVariable("modelColor", color);

	for (int i = 0; i < meshes.size(); i++) {
		meshes[i].render(shader);
	}
}
