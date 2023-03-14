#pragma once

#include <utility>
#include <cstdint>

// This class represents DVD logo scroll simulation
// environment
class DVDSimulation 
{
    using DVDpoint = std::pair<float, float>;
public:
    // Default class constructor.
    DVDSimulation(const DVDpoint& velocity, const DVDpoint& position)
       : m_LogoVelocity(velocity), m_LogoPosition(position)
    {
    }

    // Step in the movement simulation
    DVDpoint step();

    // Get the current position
    DVDpoint getPosition() {return m_LogoPosition; }

    // Get the current velocity of the logo
    DVDpoint inline getVelocity() const { return m_LogoVelocity; }

    // Set the current velocity
    void inline setVelocity(const DVDpoint& newVelocity) { m_LogoVelocity = newVelocity; }
    
private:
    DVDpoint m_LogoVelocity;
    DVDpoint m_LogoPosition;
};