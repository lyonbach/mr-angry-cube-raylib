#include "AngerControlBehaviour.h"
#include "Utilities.h"


void DefaultAngerControlBehaviour::Update()
{
    Utilities::Log("Subclasses should override this method to implement anger increment logic.");
}


void NormalAngerControlBehaviour::Update()
{
    if(GetTime() - m_LastIncrementTime >= m_AngerIncrementInterval)
    {
        Utilities::Log("Incrementing anger by: " + std::to_string(m_AngerIncrement), "AngerIncrementBehaviour", LOG_DEBUG);
        anger += m_AngerIncrement;
        m_LastIncrementTime = GetTime();
    }
}