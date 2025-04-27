#include "GameObject.h"
#include "Behaviours.h"


class MrAngryCube : public GameObject
{
public:
    MrAngryCube(Model* model, Material* material, Texture* texture);
    ~MrAngryCube() = default;
    void Render() override;
    void Update(float deltaTime) override;
    bool IsAtQuarterRotation(Vector3& vector) const;
    bool HasEverMoved() const;

    bool canMove = true;
    float hypotenuse;
    float size;
    float halfSize;
    float moveSpeed;
    Vector3 rotation;

};