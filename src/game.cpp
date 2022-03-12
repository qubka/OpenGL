#include "game.hpp"
#include "font.hpp"
#include "components.hpp"
#include "shapes.hpp"
#include "texture.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H

// Constructor
Game::Game() : window{ "OpenGL Template", { 1280, 720 }}, camera{}, audioManager{} {
    Input::Setup(window);
}

// Destructor
Game::~Game() {
    audioManager.destroy();
}

// Initialisation:  This method only runs once at startup
void Game::init() {
    // Set the clear colour and depth
    glCall(glClearColor, 1, 1, 1, 1);
    glCall(glEnable, GL_CULL_FACE);
    glCall(glEnable, GL_DEPTH_TEST);

    // Initialise audio and play background music
    audioManager.init();
    audioManager.load("resources/audio/Boing.wav");                    // Royalty free sound from freesound.org
    audioManager.load("resources/audio/fsm-team-escp-paradox.wav");    // Royalty free sound from freesound.org
    //audioManager.play("resources/audio/Boing.wav", camera.getPosition());

    auto mainShader = std::make_unique<Shader>();
    mainShader->createVertexShader("resources/shaders/mainShader.vert");
    mainShader->createFragmentShader("resources/shaders/mainShader.frag");
    mainShader->link();

    directionalLight.color = glm::vec3{1.0f, 1.0f, 1.0f};
    directionalLight.ambientIntensity = 0.25f;
    directionalLight.diffuseIntensity = 0.6f;
    directionalLight.direction = glm::normalize(glm::vec3{0.0f, -1.0f, 0.0f});

    // set color texture unit
    mainShader->use();
    mainShader->setUniform("lighting_on", true);
    directionalLight.submit(mainShader);

    shaders.push_back(std::move(mainShader));

    // Create entities

    auto entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec3{0.0f}, glm::vec3{0.0f, glm::radians(180.0f), 0.0f}, glm::vec3{2.5f});
    registry.emplace<ModelComponent>(entity, Model::Load("resources/models/Horse/horse2.obj"));

    entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec3{100.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{5.0f});
    registry.emplace<ModelComponent>(entity, Model::Load("resources/models/Barrel/barrel02.obj"));

    entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec3{250.0f, 0.0f, 100.0f}, glm::vec3{glm::radians(-90.0f), 0.0f, 0.0f}, glm::vec3{0.01f});
    registry.emplace<ModelComponent>(entity, Model::Load("resources/models/Building/AIT01W2W7.FBX"));

    entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec3{100.0f, 0.0f, 100.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{5.0f});
    registry.emplace<ModelComponent>(entity, Model::Load("resources/models/Car/car.obj"));

    entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec3{100.0f, 0.0f, 50.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.225f});
    registry.emplace<ModelComponent>(entity, Model::Load("resources/models/Tree/tree2.fbx"));

    entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec3{10.0f, 5.0f, 150.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{5.0f});
    registry.emplace<ModelComponent>(entity, Model::Load("resources/models/Cube/cube.fbx"));

    entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec3{20.0f, 10.0f, 150.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{4.0f});
    registry.emplace<MeshComponent>(entity, geometry::cuboid(glm::vec3{1}, false, std::make_shared<Texture>("resources/textures/Tile41a.jpg", true, false)));

    entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec3{50.0f, 0.0f, 50.0f}, glm::vec3{glm::radians(-90.0f), 0.0f, 0.0f}, glm::vec3{1.0f});
    registry.emplace<MeshComponent>(entity, geometry::quad(glm::vec2{500.0f}, std::make_shared<Texture>("resources/textures/terrain.jpg", true, false, glm::vec2{10.0f})));

    entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec3{30.0f, 10.0f, 150.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{4.0f});
    registry.emplace<MeshComponent>(entity, geometry::sphere(10, 20, 0.5f, std::make_shared<Texture>(255, 0, 0)));

    entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec3{40.0f, 10.0f, 150.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{4.0f});
    registry.emplace<MeshComponent>(entity, geometry::octahedron(glm::vec3{1.0f, 2.0f, 1.0f}, std::make_shared<Texture>("resources/textures/Stone.png", true, false)));

    auto mesh = geometry::tetrahedron(glm::vec3{1.5f, 1.0f, 1.5f}, std::make_shared<Texture>(255, 255, 0));

    // Write to file
    Model::Create("output/tetrahedron.obj", mesh, "obj");

    entity = registry.create();
    registry.emplace<TransformComponent>(entity, glm::vec3{50.0f, 10.0f, 150.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{4.0f});
    registry.emplace<MeshComponent>(entity, std::move(mesh));

    //////////////////////////////////////////////////////////////

    std::array<std::string, 6> faces {
        "resources/skyboxes/desertlf.jpg",
        "resources/skyboxes/desertrt.jpg",
        "resources/skyboxes/desertup.jpg",
        "resources/skyboxes/desertdn.jpg",
        "resources/skyboxes/desertft.jpg",
        "resources/skyboxes/desertbk.jpg",
    };
    skybox = std::make_unique<Skybox>(faces);

    auto skyboxShader = std::make_unique<Shader>();
    skyboxShader->createVertexShader("resources/shaders/skyboxShader.vert");
    skyboxShader->createFragmentShader("resources/shaders/skyboxShader.frag");
    skyboxShader->link();

    shaders.push_back(std::move(skyboxShader));

    //////////////////////////////////////////////////////////////

    /* Create texture atlasses for several font sizes */
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "Failed to init FreeType" << std::endl;
        return;
    }

    FT_Face face;
    if (FT_New_Face(ft, "resources/fonts/arial.ttf", 0, &face)) {
        std::cerr << "Failed to load font: " << "resources/fonts/arial.ttf" << std::endl;
        return;
    }

    auto textShader = std::make_unique<Shader>();
    textShader->createVertexShader("resources/shaders/textShader.vert");
    textShader->createFragmentShader("resources/shaders/textShader.frag");
    textShader->link();

    shaders.push_back(std::move(textShader));

    textMesh = std::make_unique<TextMesh>();
    font = std::make_unique<Font>(face, 32);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

// Render method runs repeatedly in a loop
void Game::render() {
    // Clear the buffers and enable depth testing (z-buffering)
    glCall(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCall(glDisable, GL_BLEND);
    glCall(glEnable, GL_DEPTH_TEST);

    // Get camera view and proj matricies
    auto viewMatrix = camera.getViewMatrix();
    auto projMatrix = camera.getPerspectiveProjectionMatrix();

    // Use the main shader program
    auto& mainShader = shaders[0];
    mainShader->use();
    mainShader->setUniform("u_view_projection", projMatrix * viewMatrix);

    // Render scene

    auto models = registry.view<const TransformComponent, const ModelComponent>();
    for(auto [entity, transform, model]: models.each()) {
        glm::mat4 transformMatrix{ transform };
        glm::mat3 normalMatrix{ glm::transpose(glm::inverse(glm::mat3{ transformMatrix })) };

        mainShader->setUniform("u_transform", transformMatrix);
        mainShader->setUniform("u_normal", normalMatrix);
        model()->render(mainShader);
    }

    auto meshes = registry.view<const TransformComponent, const MeshComponent>();
    for(auto [entity, transform, mesh]: meshes.each()) {
        glm::mat4 transformMatrix{ transform };
        glm::mat3 normalMatrix{ glm::transpose(glm::inverse(glm::mat3{transformMatrix})) };

        mainShader->setUniform("u_transform", transformMatrix);
        mainShader->setUniform("u_normal", normalMatrix);
        mesh()->render(mainShader);
    }

    //////////////////////////////////////////////////////////////

    // change depth function so depth test passes when values are equal to depth buffer's content
    glCall(glDepthFunc, GL_LEQUAL);

    auto& skyboxShader = shaders[1];

    skyboxShader->use();
    skyboxShader->setUniform("u_view", glm::mat4{glm::mat3{viewMatrix}}); // remove translation from the view matrix
    skyboxShader->setUniform("u_projection", projMatrix);
    skyboxShader->setUniform("skybox", 0);

    skybox->render();

    // set depth function back to default
    glCall(glDepthFunc, GL_LESS);

    //////////////////////////////////////////////////////////////

    // Disable depth and enable blend for text rendering
    glCall(glDisable, GL_DEPTH_TEST);
    glCall(glEnable, GL_BLEND);
    glCall(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto& textShader = shaders[2];

    textShader->use();
    textShader->setUniform("u_projection", camera.getOrthographicProjectionMatrix());
    textShader->setUniform("color", glm::vec4{ 1});
    textShader->setUniform("atlas", 0);

    font->bind();

    textMesh->render(font, "Press TAB to lock mouse and use camera", 20, 20, 1);
    textMesh->render(font, "Press ESC to exit", 20, 50, 1);
    textMesh->render(font, "Press F1 to enable wiremode renderer", 20, 80, 1);

	// Draw the 2D graphics after the 3D graphics
	displayFrameRate();

    font->unbind();
}

// Update method runs repeatedly with the Render method
void Game::update() {
    // Set the orthographic and perspective projection matrices based on the image size
    camera.setOrthographicProjectionMatrix(window.getWidth(), window.getHeight());
    camera.setPerspectiveProjectionMatrix(45.0f, window.getAspect(), 0.01f, 1000.0f);

    if (Input::GetKeyDown(GLFW_KEY_ESCAPE))
        window.shouldClose(true);

    if (Input::GetKeyDown(GLFW_KEY_TAB))
        window.toggleCursor();

    if (Input::GetKeyDown(GLFW_KEY_F1))
        window.toggleWireframe();

    camera.update(dt);

    audioManager.play("resources/audio/fsm-team-escp-paradox.wav", camera.getPosition());
}

void Game::displayFrameRate() {
    // Increase the elapsed time and frame counter
    elapsedTime += dt;
    frameCount++;

    // Now we want to subtract the current time by the last time that was stored
    // to see if the time elapsed has been over a second, which means we found our FPS.
    if (elapsedTime > 1.0f) {
        elapsedTime = 0;
        framesPerSecond = frameCount;

        // Reset the frames per second
        frameCount = 0;
    }

    if (framesPerSecond > 0) {
        textMesh->render(font, "FPS: " + std::to_string(framesPerSecond), 20, window.getHeight() - 30, 1);
    }
}

// The game loop runs repeatedly until game over
void Game::run() {
    double currentTime = glfwGetTime();
    double previousTime = currentTime;

    while (!window.shouldClose()) {
        currentTime = glfwGetTime();
        dt = currentTime - previousTime;
        previousTime = currentTime;

        update();
        render();

        Input::Update();

        window.swapBuffers();
        window.pollEvents();
    }
}

Game& Game::getInstance() {
    static Game instance;
    return instance;
}

int main(int args, char** argv) {
    Game& game = Game::getInstance();
    try {
        game.init();
        game.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}