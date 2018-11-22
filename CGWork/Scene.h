#pragma once

#include "Geometry.h"
#include "Model.h"
#include "Camera.h"
#include "ALMath.h"

class Scene
{
private:
	std::vector<Model*> models;
	Camera* camera;
	//std::vector<Geometry*> geos;

	bool isCalcNormal;
	Vec4 bgColor;

	Scene();

public:
	static Scene& GetInstance()
	{
		static Scene instance; // Guaranteed to be destroyed.
							   // Instantiated on first use.
		return instance;
	}
	
	~Scene();

	Scene(const Scene& s) = delete;
	Scene& operator=(const Scene& s) = delete;

	void CreateModel();

	const std::vector<Model*>& GetModels() const;
	Camera* GetCamera() const;

	void SetCalcNormalState(bool isCalcNorm);
	bool GetCalcNormalState() const;

	void SetBackgroundColor(int r, int g, int b);
	void SetBackgroundColor(const Vec4& color);
	Vec4 GetBackgroundColor() const;

	void ResetScene(); // TODO: Implement reset scene (resets it when loading a new file)
	void Draw(); // TODO: Implement draw (will call do all the matrix multiplications and call renderer)
};

