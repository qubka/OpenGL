#pragma once

// Setup includes
#include "window.hpp"
#include "input.hpp"
#include "audiomanager.hpp"

// Game includes
#include "camera.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "mesh.hpp"
#include "lights.hpp"
#include "textmesh.hpp"
#include "skybox.hpp"

#include <entt/entity/registry.hpp>

int main(int argc, char** argv);

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to nullptr and in Game::Initialise, create a new object.  Don't forget to
// delete the object in the destructor.   

class Game {
private:
	Game();
	~Game();

	// Three main methods used in the game.  Initialise runs once, while Update and Render run repeatedly in the game loop.
	void init();
	void update();
	void render();

	// Pointers to game objects.  They will get allocated in Game::Initialise()
	Camera camera;
	AudioManager audioManager;
	std::vector<std::unique_ptr<Shader>> shaders;

    entt::registry registry;

    DirectionalLight directionalLight;
	std::vector<SpotLight> spotLights;
	std::vector<PointLight> pointLights;

    std::unique_ptr<Skybox> skybox;
    std::unique_ptr<TextMesh> textMesh;
    std::unique_ptr<Font> font;

	void displayFrameRate();
	void run();

	Window window;
	int frameCount{ 0 };
	double elapsedTime{ 0.0 };
    double dt{ 0.0 };
    int framesPerSecond{ 0 };

    friend int ::main(int argc, char** argv);

public:
	static Game& getInstance();
};
