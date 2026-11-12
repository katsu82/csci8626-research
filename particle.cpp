#include "particle.h"

Particle::Particle(const Vector3& pos, const Vector3& sc, const Vector3 orient, const glm::vec4 col)
{
    position = pos;
    scale = sc;
    orientation = orient;
    color = col;
}

void Particle::update()
{
    glm::vec3 pos = glm::vec3(position.x, position.y, position.z);
    //glm::vec3 ori = glm::vec3(orientation.x, orientation.y, orientation.z);
    glm::vec3 sc = glm::vec3(scale.x, scale.y, scale.z);
    model = glm::translate(glm::mat4(1.0f), pos) * glm::eulerAngleXYZ(orientation.x, orientation.y, orientation.z) * glm::scale(glm::mat4(1.0f), sc);
}


void Particle::setScale(const Vector3& sc)
{
    if(sc != scale)
    {
        scale = sc;
        transformChange = true;
    }
}


void Particle::setPosition(const Vector3& pos)
{
    if(pos != position)
    {
        position = pos;
        transformChange = true;
    }
}

void Particle::setOrientation(const Vector3& orient)
{
    if(orient != orientation)
    {
        orientation = orient;
        transformChange = true;
    }
}

void Particle::setColor(const glm::vec4& col)
{
    color = col;
}