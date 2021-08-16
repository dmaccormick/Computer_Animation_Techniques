#include "AssetManager.h"

AssetManager* AssetManager::inst = 0; //The singleton instance of this class

//Constructor, pre-allocates space in the vectors for the number of meshes and textures in the game, saves time
AssetManager::AssetManager()
{
	loadedMeshes.reserve(NUM_MESHES);
	loadedTextures.reserve(NUM_TEXTURES);
}

//Destructor, currently does nothing
AssetManager::~AssetManager()
{
}

/*
	Actually loads in the assets
	Notice the naming conventions!
	Add assets in EXACT order as specified in the enums
*/
void AssetManager::loadAssets()
{
	/* ========== MESHES (Naming: mesh_XXXXXX.obj) ========== */
	loadedMeshes.push_back(Mesh("./res/mesh/mesh_Monkey.obj"));

	/* ========== TEXTURES (Naming: tex_XXXXXX.png) ========== */
}

//Returns a reference to the desired mesh, allows for multiple objects to use the same mesh with only one loading time
Mesh& AssetManager::getMesh(MESH_NAME meshName)
{
	try 
	{
		return loadedMeshes.at(meshName); //Returns the address of the mesh stored within the vector
	}
	catch (std::out_of_range) //Error catching
	{
		std::cout << "Error! The mesh requested is out of bounds of the vector! Aborting!" << std::endl;
		abort();
	}
	catch (...)
	{
		std::cout << "Unknown error when trying to get the mesh! Aborting!" << std::endl;
		abort();
	}
	
}

//Returns a reference to the desired texture, allows for multiples objects to use the same texture with only one loading time
Texture2D& AssetManager::getTexture2D(TEXTURE_NAME textureName)
{
	try 
	{
		return loadedTextures.at(textureName); //Returns the address of the texture stored within the vector
	}
	catch (std::out_of_range) //Error catching
	{
		std::cout << "Error! The texture requested is out of bounds of the vector! Aborting!" << endl;
		abort();
	}
	catch (...)
	{
		std::cout << "Unknown error when trying to get the texture! Aborting!" << std::endl;
		abort();
	}
}

/* Singleton Pattern */
AssetManager* AssetManager::assets()
{
	if (!inst)
		inst = new AssetManager();

	return inst;
}