#include "GameObject.h"
#include "MoveBehaviour.h"
#include "AngerControlBehaviour.h"


class MrAngryCube : public GameObject
{
public:
    MrAngryCube(Model* model, std::vector<Material*> materials);
    ~MrAngryCube();
    void Render() override;
    void Update(float deltaTime) override;
    float GetAnger() const;
    unsigned int GetMoveBehaviourIndex() const;
    void SetAnger(float newAnger);
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
    MoveBehaviourName currentMoveBehaviourName = nextMoveBehaviourName;

private:
    void ApplyMoveBehaviourChange();
    MACMoveBehaviourBase* m_MoveBehaviour;
    NormalAngerControlBehaviour* m_AngerControlBehaviour;
};