#pragma once

// Setup includes
#include "window.hpp"
#include "input.hpp"

// Game includes
#include "camera.hpp"
#include "shader.hpp"
#include "model.hpp"
#include "mesh.hpp"
#include "lights.hpp"
#include "textmesh.hpp"
#include "skybox.hpp"
#include "catmullrom.hpp"
#include "frustum.hpp"

#include <entt/entity/registry.hpp>

int main(int argc, char** argv);

// Classes used in game.  For a new class, declare it here and provide a pointer to an object of this class below.  Then, in Game.cpp, 
// include the header.  In the Game constructor, set the pointer to nullptr and in Game::Initialise, create a new object.  Don't forget to
// delete the object in the destructor.   

class Game {
private:
	Game();
	~Game();

	void init();
    void run();
    void update();
    void render();

    Window window;
    uint64_t frameNumber{ 0 };
    uint32_t frameCount{ 0 };
    uint32_t framesPerSecond{ 0 };
    float elapsedTime{ 0.0 };
    float dt{ 0.0 };

    entt::registry registry;
    entt::entity spaceship;

    Camera camera;
	Frustum frustum;
	CatmullRom catmullRom;

	DirectionalLight directionalLight;
    std::unique_ptr<Skybox> skybox;
    std::unique_ptr<TextMesh> textMesh;
	std::unique_ptr<Font> font;
	std::unique_ptr<Font> icons;

    std::unique_ptr<Shader> mainShader;
    std::unique_ptr<Shader> skyboxShader;
    std::unique_ptr<Shader> textShader;
    std::unique_ptr<Shader> splineShader;

    bool darkMode{ true };
    int viewMode{ 0 };

	void displayFrameRate();
	void moveShip();
    void blinkEffect();

    friend int ::main(int argc, char** argv);

public:
	static Game& getInstance();
};
