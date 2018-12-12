#include <iostream>

#include <ASSIMP/Importer.hpp>
#include <ASSIMP/postprocess.h>

#include "objects/model.hpp"

#include "render_base/exception.hpp"
#include "render_base/shader.hpp"

using namespace Render3D;
using namespace Math3D;

Model::Model(const char* filePath) : Primitive3D() {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        throw Exception(std::string("Unable to load model, ") + importer.GetErrorString());
	}

	std::string path = std::string(filePath);

	directory = path.substr(0, path.find_last_of('/'));

	meshes.reserve(scene->mNumMeshes);
	processNode(scene->mRootNode, scene);
}

Model::~Model() {
    for (unsigned int i = 0; i < textureCache.size(); ++i) {
        delete textureCache[i].second;
    }
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	// Process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 

		// Process mesh
		meshes.push_back(processMesh(mesh, scene));
	}

	// Then do the same for each of its children
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	//Process vertices, normals, and texture coordinates

	std::vector<Vector4> vertices;
	vertices.reserve(mesh->mNumVertices);
	std::vector<Vector4> normals;
	normals.reserve(mesh->mNumVertices);
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
			norm[2] = -aiVec.z;
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

		loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", textures);

		loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", textures);
	}

	return Mesh(vertices, normals, textureCoords, textures, indices);
}

void Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, std::vector<TextureData>& textures) {
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

        std::string filePath = directory + "/" + std::string(str.C_Str());
        bool found = false;
        for (unsigned int i2 = 0; i2 < textureCache.size(); ++i2) {
            if (textureCache[i2].first == filePath) {
                textures.push_back(TextureData(typeName, textureCache[i2].second));
                found = true;
                break;
            }
        }
        if (!found) {
            textureCache.push_back(std::pair<std::string, Texture*>(filePath, new Texture(filePath.c_str())));
            textures.push_back(TextureData(typeName, textureCache.back().second));
        }

		std::cout << typeName << ", " << str.C_Str() << ", " << i << std::endl;
	}
}

void Model::render(Window& win, TextureManager& textureManager) {
	applyVariables(win);
    
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].render(*shader, win, textureManager);
	}
}

void Model::prepareContent(Window& win, TextureManager& textureManager) {	
    for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].prepareContent(win, textureManager);
	}

    for (unsigned int i = 0; i < textureCache.size(); ++i) {
        textureCache[i].second->prepareContent(win, textureManager);
    }
}

void Model::destroyContent(Window& win, TextureManager& textureManager) {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].destroyContent(win, textureManager);
	}

    for (unsigned int i = 0; i < textureCache.size(); ++i) {
        textureCache[i].second->destroyContent(win, textureManager);
    }
}
