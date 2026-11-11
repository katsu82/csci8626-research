#ifndef PARTICLE_4620
#define PARTICLE_4620

#include "Vector3.h"
class Particle
{
public:
    Particle() {};
    Particle(const Vector3& pos, const Vector3& sc);
    void update();

    Vector3 getScale()const {return scale; };
    void setScale(const Vector3& scale);

    Vector3 getPosition()const {return position;};
    void setPosition(const Vector3& pos);


private:
    float lifeTime;
    Vector3 position;
    Vector3 velocity;
    Vector3 scale = Vector3{1, 1, 1};
};

#endif