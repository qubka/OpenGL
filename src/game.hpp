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
#include "catmullrom.hpp"
#include "pipe.hpp"

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
    double elapsedTime{ 0.0 };
    double dt{ 0.0 };

    entt::registry registry;

    Pipe pipe;
    Camera camera;
	AudioManager audioManager;
    DirectionalLight directionalLight;
	std::vector<SpotLight> spotLights;
	std::vector<PointLight> pointLights;
    std::unique_ptr<Skybox> skybox;
    std::unique_ptr<TextMesh> textMesh;
	std::unique_ptr<Font> font;
	std::unique_ptr<Font> icons;
    //std::unique_ptr<CatmullRom> spline;
    std::unique_ptr<Mesh> pipeMesh;

    std::unique_ptr<Shader> mainShader;
    std::unique_ptr<Shader> skyboxShader;
    std::unique_ptr<Shader> textShader;
    //std::unique_ptr<Shader> pointsShader;
    //std::unique_ptr<Shader> tessShader;

    bool darkMode{ true };

	void displayFrameRate();

    friend int ::main(int argc, char** argv);

public:
	static Game& getInstance();
};
