#include "lights.hpp"
#include "shader.hpp"

void DirectionalLight::submit(const std::unique_ptr<Shader>& shader) const {
    shader->setUniform("gDirectionalLight.Base.Color", color);
    shader->setUniform("gDirectionalLight.Base.AmbientIntensity", ambientIntensity);
    shader->setUniform("gDirectionalLight.Direction", glm::normalize(direction));
    shader->setUniform("gDirectionalLight.Base.DiffuseIntensity", diffuseIntensity);
}

void PointLight::submit(const std::unique_ptr<Shader>& shader, uint32_t point_light_index) const {
    shader->setUniform("gPointLights[" + std::to_string(point_light_index) + "].Base.Color", color);
    shader->setUniform("gPointLights[" + std::to_string(point_light_index) + "].Base.AmbientIntensity", ambientIntensity);
    shader->setUniform("gPointLights[" + std::to_string(point_light_index) + "].Position", position);
    shader->setUniform("gPointLights[" + std::to_string(point_light_index) + "].Base.DiffuseIntensity", diffuseIntensity);
    shader->setUniform("gPointLights[" + std::to_string(point_light_index) + "].Atten.Constant", attenuation.constant);
    shader->setUniform("gPointLights[" + std::to_string(point_light_index) + "].Atten.Linear", attenuation.linear);
    shader->setUniform("gPointLights[" + std::to_string(point_light_index) + "].Atten.Exp", attenuation.exp);
}

void SpotLight::submit(const std::unique_ptr<Shader>& shader, uint32_t spot_light_index) const {
    shader->setUniform("gSpotLights[" + std::to_string(spot_light_index) + "].Base.Base.Color", color);
    shader->setUniform("gSpotLights[" + std::to_string(spot_light_index) + "].Base.Base.AmbientIntensity", ambientIntensity);
    shader->setUniform("gSpotLights[" + std::to_string(spot_light_index) + "].Base.Position", position);
    shader->setUniform("gSpotLights[" + std::to_string(spot_light_index) + "].Base.Base.DiffuseIntensity", diffuseIntensity);
    shader->setUniform("gSpotLights[" + std::to_string(spot_light_index) + "].Base.Atten.Constant", attenuation.constant);
    shader->setUniform("gSpotLights[" + std::to_string(spot_light_index) + "].Base.Atten.Linear", attenuation.linear);
    shader->setUniform("gSpotLights[" + std::to_string(spot_light_index) + "].Base.Atten.Exp", attenuation.exp);
    shader->setUniform("gSpotLights[" + std::to_string(spot_light_index) + "].Direction", glm::normalize(direction));
    shader->setUniform("gSpotLights[" + std::to_string(spot_light_index) + "].Cutoff", cutoff);
}