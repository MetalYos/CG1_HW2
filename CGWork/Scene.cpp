#include "Scene.h"
#include "StdAfx.h"


Scene::Scene() : camera(new Camera()), isCalcNormal(true), bgColor((AL_BLACK))
{
}


Scene::~Scene()
{
	while (models.size() > 0)
	{
		Model* model = models.back();
		models.pop_back();
		delete model;
	}
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

Camera * Scene::GetCamera() const
{
	return camera;
}

void Scene::SetCalcNormalState(bool isCalcNorm)
{
	this->isCalcNormal = isCalcNorm;
}

bool Scene::GetCalcNormalState() const
{
	return isCalcNormal;
}

void Scene::SetBackgroundColor(int r, int g, int b)
{
	bgColor = Vec4(r, g, b);
}

void Scene::SetBackgroundColor(const Vec4 & color)
{
	bgColor = color;
}

Vec4 Scene::GetBackgroundColor() const
{
	return bgColor;
}
