#include "GameObject.h"

class MrAngryCube : public GameObject
{
public:
    MrAngryCube(Model* model, Material* material, Texture* texture);
    ~MrAngryCube() = default;
    void Render() override;
    void Update(float deltaTime) override;

    float hypotenuse;
    float size;
    float halfSize;
    float moveSpeed;
    Vector3 rotation;

};