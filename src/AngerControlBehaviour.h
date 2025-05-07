#pragma once


class DefaultAngerControlBehaviour
{
public:
    unsigned int anger = 0;
    virtual void Update();

protected:
    // Units are seconds.
    float m_LastIncrementTime = 0.0f;
    float m_AngerIncrementInterval = 1.0f;
    unsigned int m_AngerIncrement = 1;
};


class NormalAngerControlBehaviour : public DefaultAngerControlBehaviour
{
public:
    void Update() override;
};