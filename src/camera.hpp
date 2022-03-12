#pragma once

class Camera {
public:
	Camera();										// Constructor - sets default values for camera position, viewvector, upvector, and speed
	~Camera();										// Destructor

    glm::vec3& getPosition();					    // Gets the position of the camera centre of projection
    glm::quat& getRotation();					    // Gets the rotation of the camera
    glm::vec3 getUpVector() const;					// Gets the camera up vector
    glm::vec3 getForwardVector() const;				// Gets the camera forward vector
    glm::vec3 getRightVector() const;				// Gets the camera right vector
    glm::mat4 getViewMatrix() const;					    // Gets the camera view
    glm::mat4& getPerspectiveProjectionMatrix();	// Gets the camera perspective projection matrix
	glm::mat4& getOrthographicProjectionMatrix();	// Gets the camera orthographic projection matrix

	// Respond to mouse movement to rotate the camera
	void setViewByMouse();

	// Respond to keyboard presses on arrow keys to translate the camera
	void translateByKeyboard(float dt);

	// Update the camera
	void update(float dt);

	// Set the projection matrices
	void setPerspectiveProjectionMatrix(float fov, float aspectRatio, float nearClippingPlane, float farClippingPlane);
	void setOrthographicProjectionMatrix(int width, int height);

    enum class ViewMode {
        FirstPerson,
        ThirdPerson,
        SideView,
        TopView
    };

private:
	glm::vec3 position{ 0.0f };
	glm::quat rotation{ 1, 0, 0, 0 };

	float speed{ 0.0f };
    float yaw{ 0.0f };
    float pitch{ 0.0f };

	glm::mat4 perspectiveProjectionMatrix{ 1.0f };
	glm::mat4 orthographicProjectionMatrix{ 1.0f };

    ViewMode mode{ ViewMode::FirstPerson };
};
