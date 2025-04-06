#include "raylib.h"
#include "raymath.h"

class GameObject
{
    private:
        Model m_Model;
        Texture m_Texture;
        Shader m_Shader;
        Material m_Material;
        
    public:
        Matrix m_Transform;
        Vector3 m_Rotation;
        
        float m_Size;
        float m_HalfSize = m_Size / 2.0f;
        float m_Hypotenuse = sqrt(m_HalfSize * m_HalfSize * 2);

        GameObject();
        ~GameObject();

        void Render();
        void Update(Vector3 rotationAxis, float increment);
};
