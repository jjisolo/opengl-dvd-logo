#include "dvd_simulation.h"

DVDSimulation::DVDpoint DVDSimulation::step() {
    // Assert that the next logo position are at the screen boundaries, if so reverse its move vector
    if(m_LogoPosition.first + m_LogoVelocity.first >= 0.9f || m_LogoPosition.first + m_LogoVelocity.first <= -0.9f)
        m_LogoVelocity.first = -m_LogoVelocity.first;
    if(m_LogoPosition.second + m_LogoVelocity.second >= 0.9f || m_LogoPosition.second + m_LogoVelocity.second <= -0.9f)
        m_LogoVelocity.second = -m_LogoVelocity.second;
    
    // Move the logo
    m_LogoPosition.first  += m_LogoVelocity.first;
    m_LogoPosition.second += m_LogoVelocity.second;
}

