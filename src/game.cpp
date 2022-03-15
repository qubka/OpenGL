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
}

// Initialisation:  This method only runs once at startup
void Game::init() {
    // Set the clear colour and depth
    glCall(glClearColor, 1.0f, 1.0f, 1.0f, 1.0f);
    glCall(glClearStencil, 0);
    glCall(glClearDepth, 1.0f);
    glCall(glEnable, GL_CULL_FACE);
    glCall(glEnable, GL_LIGHTING);
    glCall(glEnable, GL_TEXTURE_2D);
    glCall(glEnable, GL_DEPTH_TEST);
    glCall(glEnable, GL_COLOR_MATERIAL);
    glCall(glDepthFunc, GL_LEQUAL);
    glCall(glShadeModel, GL_SMOOTH);
    glCall(glPixelStorei, GL_UNPACK_ALIGNMENT, 4);

    // Initialise audio and play background music
    audioManager.load("resources/audio/Boing.wav");                    // Royalty free sound from freesound.org
    audioManager.load("resources/audio/fsm-team-escp-paradox.wav");    // Royalty free sound from freesound.org
    audioManager.play("resources/audio/fsm-team-escp-paradox.wav", camera.getPosition());

    mainShader = std::make_unique<Shader>();
    mainShader->link("resources/shaders/mainShader.vert", "resources/shaders/mainShader.frag");

    // Initialise lights and fog
    directionalLight.color = glm::vec3{1.0f, 1.0f, 1.0f};
    directionalLight.ambientIntensity = darkMode ? 0.15f : 1.0f;
    directionalLight.diffuseIntensity = darkMode ? 0.1f : 1.0f;
    directionalLight.direction = glm::normalize(glm::vec3{0.0f, -1.0f, 0.0f});

    PointLight pointLight;
    pointLight.position = glm::vec3{20.0f, 10.0f, 150.0f};
    pointLight.color = glm::vec3{1.0f, 0.0f, 1.0f};
    pointLight.ambientIntensity = 0.25f;
    pointLight.diffuseIntensity = 0.6f;
    pointLights.push_back(pointLight);

    pointLight.position = glm::vec3{70.0f, 10.0f, 100.0f};
    pointLight.color = glm::vec3{0.0f, 1.0f, 1.0f};
    pointLight.ambientIntensity = 0.25f;
    pointLight.diffuseIntensity = 0.6f;
    pointLights.push_back(pointLight);

    SpotLight spotLight;
    spotLight.position = glm::vec3{40.0f, 10.0f, 50.0f};
    spotLight.color = glm::vec3{0.0f, 0.0f, 1.0f};
    spotLight.ambientIntensity = 0.9f;
    spotLight.diffuseIntensity = 0.9f;
    spotLight.direction = glm::vec3{0, -1, 0};
    spotLight.cutoff = 0.9f;
    spotLights.push_back(spotLight);

    mainShader->use();
    mainShader->setUniform("fog_on", true);
    mainShader->setUniform("fog_colour", glm::vec3{0.5});
    mainShader->setUniform("fog_factor_type", 0);
    mainShader->setUniform("fog_start", 20.f);
    mainShader->setUniform("fog_end", 500.f);

    mainShader->setUniform("lighting_on", true);
    mainShader->setUniform("transparency", 1.0f);
    mainShader->setUniform("gMatSpecularIntensity", 1.f);
    mainShader->setUniform("gSpecularPower", 10.f);
    mainShader->setUniform("gNumPointLights", static_cast<int>(pointLights.size()));
    mainShader->setUniform("gNumSpotLights", static_cast<int>(spotLights.size()));
    directionalLight.submit(mainShader);

    // generate path for pipe
    std::vector<glm::vec3> path = shape::spiralPath(4, 1, -3, 3, 3.5, 200);
    std::cout << "fitst point: " << glm::to_string(path[0]) << std::endl;
    std::cout << "last point: " << glm::to_string(path[path.size()-1]) << std::endl;

    // sectional contour of pipe
    std::vector<glm::vec3> circle = shape::circle({ 0.5f, 0.5f }, 48); // radius, segments

    // configure pipe
    pipe.set(path, circle);

    pipeMesh = geometry::pipe(pipe, std::make_shared<Texture>(255, 255, 0));

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

    // Create cubemap skybox
    std::array<std::string, 6> faces {
        "resources/skyboxes/desertlf.jpg",
        "resources/skyboxes/desertrt.jpg",
        "resources/skyboxes/desertup.jpg",
        "resources/skyboxes/desertdn.jpg",
        "resources/skyboxes/desertft.jpg",
        "resources/skyboxes/desertbk.jpg",
    };
    skybox = std::make_unique<Skybox>(faces);

    skyboxShader = std::make_unique<Shader>();
    skyboxShader->link("resources/shaders/skyboxShader.vert", "resources/shaders/skyboxShader.frag");

    //////////////////////////////////////////////////////////////

    /*tessShader = std::make_unique<Shader>();
    tessShader->link("resources/shaders/splineShader.vert",
                       "resources/shaders/splineShader.frag",
                       "resources/shaders/splineShader.tesc",
                       "resources/shaders/splineShader.tese");

    pointsShader = std::make_unique<Shader>();
    pointsShader->link("resources/shaders/splineShader.vert", "resources/shaders/splineShader.frag");

    spline = std::make_unique<CatmullRom>();*/

    //////////////////////////////////////////////////////////////

    // Create texture atlasses for several font sizes
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        std::cerr << "ERROR: Failed to init FreeType" << std::endl;
        return;
    }

    FontLibrary library;
    FontFace roboto_face{library, "resources/fonts/Roboto-Black.ttf"};
    FontFace icon_face{ library, "resources/fonts/Font90Icons-2ePo.ttf"};

    textShader = std::make_unique<Shader>();
    textShader->link("resources/shaders/textShader.vert", "resources/shaders/textShader.frag");

    textMesh = std::make_unique<TextMesh>();
    font = std::make_unique<Font>(roboto_face, 32);
    icons = std::make_unique<Font>(icon_face, 32);
}

// Render method runs repeatedly in a loop
void Game::render() {
    // Clear the buffers and enable depth testing (z-buffering)
    glCall(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glCall(glDisable, GL_BLEND);
    glCall(glEnable, GL_DEPTH_TEST);

    // Get camera view and proj matricies
    auto viewMatrix = camera.getViewMatrix();
    auto projMatrix = camera.getPerspectiveProjectionMatrix();

    // Use the main shader program
    mainShader->use();
    mainShader->setUniform("u_view_projection", projMatrix * viewMatrix);
    mainShader->setUniform("gEyeWorldPos", camera.getPosition());
    mainShader->setUniform("fog_on", darkMode);
    directionalLight.ambientIntensity = darkMode ? 0.15f : 1.0f;
    directionalLight.diffuseIntensity = darkMode ? 0.1f : 1.0f;
    directionalLight.submit(mainShader);

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

    mainShader->setUniform("lighting_on", false);
    for (size_t i = 0; i < pointLights.size(); i++) {
        pointLights[i].submit(mainShader, i);
    }
    for (size_t i = 0; i < spotLights.size(); i++) {
        spotLights[i].submit(mainShader, i);
    }
    mainShader->setUniform("lighting_on", true);

    glCall(glDisable, GL_CULL_FACE);

    //mainShader->setUniform("u_transform", glm::scale(glm::mat4{1}, glm::vec3{100.0f}));
    pipeMesh->render(mainShader);

    glCall(glEnable, GL_CULL_FACE);

    //////////////////////////////////////////////////////////////

    skyboxShader->use();
    skyboxShader->setUniform("u_view", glm::mat4{glm::mat3{viewMatrix}}); // remove translation from the view matrix
    skyboxShader->setUniform("u_projection", projMatrix);
    skyboxShader->setUniform("skybox", 0);

    skybox->render();

    //////////////////////////////////////////////////////////////

    /*pointsShader->use();
    pointsShader->setUniform("u_view", viewMatrix); // remove translation from the view matrix
    pointsShader->setUniform("u_projection", projMatrix);

    spline->render();

    tessShader->use();
    tessShader->setUniform("u_view", viewMatrix); // remove translation from the view matrix
    tessShader->setUniform("u_projection", projMatrix);

    spline->tessellate();*/

    //////////////////////////////////////////////////////////////

    // Disable depth and enable blend for text rendering
    glCall(glDisable, GL_DEPTH_TEST);
    glCall(glEnable, GL_BLEND);
    glCall(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    textShader->use();
    textShader->setUniform("u_projection", camera.getOrthographicProjectionMatrix());
    textShader->setUniform("color", glm::vec4{1});
    textShader->setUniform("atlas", 0);

    font->bind();

    textMesh->render(font, "Press TAB to lock mouse and use camera", 20, 20, 1);
    textMesh->render(font, "Press ESC to exit", 20, 50, 1);
    textMesh->render(font, "Press F1 to enable wiremode renderer", 20, 80, 1);
    textMesh->render(font, "Press F2 to toggle lighting", 20, 110, 1);
    textMesh->render(font, glm::to_string(camera.getPosition()), window.getWidth() / 2, window.getHeight() - 30, 1);

	// Draw the 2D graphics after the 3D graphics
	displayFrameRate();

    font->unbind();

    // Draw icons

    icons->bind();

    textShader->setUniform("color", glm::vec4{1, 0, 0, 1});

    textMesh->render(icons, "abcdefghijkl", 20, window.getHeight() / 2, 1);

    textShader->setUniform("color", glm::vec4{0, 1, 0, 1});

    textMesh->render(icons, "mnopqrstuvwxyz", 20, window.getHeight() / 2 - 30, 1);

    textShader->setUniform("color", glm::vec4{0, 0, 1, 1});

    textMesh->render(icons, "ABCDEFGHIJKLMN\nOPQRSTUVWXYZ", 20, window.getHeight() / 2 - 60, 1);

    icons->unbind();
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

    if (Input::GetKeyDown(GLFW_KEY_F2))
        darkMode = !darkMode;

    /*if (Input::GetKeyDown(GLFW_KEY_E))
        spline->addPoint(camera.getPosition());*/

    camera.update(dt);
}

void Game::displayFrameRate() {
    // Increase the elapsed time and frame counter
    frameNumber++;
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
        textMesh->render(font, "FPS: " + std::to_string(framesPerSecond), 20, window.getHeight() - 30, 1.0f);
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