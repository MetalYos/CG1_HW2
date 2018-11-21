#include "Scene.h"



Scene::Scene() : camera(new Camera())
{
}


Scene::~Scene()
{
	for (Model* m : models)
		delete m;
	delete camera;
}

void Scene::CreateModel()
{
	Model* model = new Model();
	models.push_back(model);
}

const std::vector<Model*>& Scene::GetModels() const
{
	return models;
}

const Mat4 & Scene::GetCameraTransform() const
{
	return camera->GetTranform();
}

const Mat4 & Scene::GetProjections() const
{
	return camera->GetProjection();
}

Camera * Scene::GetCamera() const
{
	return camera;
}
