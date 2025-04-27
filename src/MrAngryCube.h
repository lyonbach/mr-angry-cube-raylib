#include "GameObject.h"

class MrAngryCube : public GameObject
{
public:
    MrAngryCube(Model* model, Material* material, Texture* texture);
    ~MrAngryCube() = default;
    void Render() override;
};