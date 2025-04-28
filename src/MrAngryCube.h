#include "GameObject.h"
#include "MoveBehaviour.h"


class MrAngryCube : public GameObject
{
public:
    MrAngryCube(Model* model, Material* material, Texture* texture);
    ~MrAngryCube() = default;
    void Render() override;
    void Update(float deltaTime) override;
    bool IsAtQuarterRotation(Vector3& vector) const;
    bool HasEverMoved() const;
    void SetMoveBehaviour(MoveBehaviourName behaviourType);

    bool canMove = true;
    float hypotenuse;
    float size;
    float halfSize;
    float moveSpeed;
    Vector3 rotation;
    MoveBehaviourName nextMoveBehaviourName = MoveBehaviourName::NoMoveBehaviour;

private:
    void ApplyMoveBehaviourChange();
    MACMoveBehaviourBase* m_MoveBehaviour;
};