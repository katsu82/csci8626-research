#ifndef PARTICLE_4620
#define PARTICLE_4620
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "Vector3.h"
class Particle
{
public:
    Particle() {};
    Particle(const Vector3& pos, const Vector3& sc, const Vector3 orient, const glm::vec4 col);
    void update();

    Vector3 getScale()const {return scale; };
    void setScale(const Vector3& sc);

    Vector3 getPosition()const {return position;};
    void setPosition(const Vector3& pos);

    Vector3 getOrientation()const {return orientation;};
    void setOrientation(const Vector3& orient);

    glm::vec4 getColor()const {return color;};
    void setColor(const glm::vec4& col);

    glm::mat4 getModel()const {return model;};


private:
    float lifeTime;
    Vector3 position;
    Vector3 velocity;
    Vector3 orientation = Vector3{0, 0, 0};
    Vector3 scale = Vector3{1, 1, 1};
    glm::vec4 color = glm::vec4(1, 1, 1, 1);
    bool transformChange = false;
    glm::mat4 model = glm::mat4(1.0f);
};

#endif