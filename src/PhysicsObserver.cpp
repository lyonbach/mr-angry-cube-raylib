#include "PhysicsObserver.h"


void PhysicsObserver::Update()
{
    positionArray.push_back(std::pair(observed->GetPosition(), GetTime()));
    if (positionArray.size() >= positionArraySize)
    {
        positionArray.erase(positionArray.begin());
    } 
}

Vector3 PhysicsObserver::GetVelocity() const
{
    if (positionArray.size() < 2) { return Vector3({0, 0, 0}); }
    std::pair<Vector3, float> firstRecord = positionArray.at(0);
    std::pair<Vector3, float> lastRecord = positionArray.at(positionArray.size() - 1);

    return (lastRecord.first - firstRecord.first) / (lastRecord.second - firstRecord.second);
}