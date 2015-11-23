#include <math.h>
#include <stdlib.h>
#include "RainPS.h"

#define GET_RANDOM (((float)rand() - (float)rand()) / RAND_MAX)

CRainPS::CRainPS(float vX, float vY, float vZ,
		float dX, float dY, float dZ)
{
	m_width = 0; m_height = 0; m_depth = 0;
	m_particles = 0; m_maxParticles = 0;
	m_particleCount = 0; m_totalTime = 0;

	m_velocity[0] = vX;
	m_velocity[1] = vY;
	m_velocity[2] = vZ;

	m_velDelta[0] = dX;
	m_velDelta[1] = dY;
	m_velDelta[2] = dZ;
}

bool CRainPS::Initialize(float x, float y, float z, int maxP,
		float w, float h, float d)
{
	Shutdown();

	m_width = w; m_height = h; m_depth = d;
	m_particles = NULL;
	m_maxParticles = maxP;
	m_particleCount = 0;
	m_totalTime = 0;
	m_pos[0] = x; m_pos[1] = y; m_pos[2] = z;

	if(m_maxParticles <= 0) m_maxParticles = 1;
	m_particles = new CParticle[m_maxParticles];
	if(!m_particles) return false;

	return true;
}

void CRainPS::Update(float scalar)
{
	int numParticles = 0;

	for(int i=0; i<m_particleCount;)
	{
		m_particles[i].m_pos[0] += m_particles[i].m_vel[0] * scalar;
		m_particles[i].m_pos[1] += m_particles[i].m_vel[1] * scalar;
		m_particles[i].m_pos[2] += m_particles[i].m_vel[2] * scalar;

		if(m_particles[i].m_pos[1] <= m_pos[1])
		{
			m_particleCount--;
			m_particles[i] = m_particles[m_particleCount];
		}
		else
			i++;
	}

	m_totalTime += scalar;
	numParticles = (int)(m_totalTime * UPDATE_AMT);
	if(m_totalTime > 1) m_totalTime = 0;

	CreateParticle(numParticles);
}

void CRainPS::CreateParticle(int amount)
{
	for(int i=0; i<amount; i++)
	{
		if(m_particleCount >= m_maxParticles) break;

		m_particles[m_particleCount].m_pos[0] = m_pos[0] + GET_RANDOM * m_width;
		m_particles[m_particleCount].m_pos[1] = m_height + GET_RANDOM * m_height;
		m_particles[m_particleCount].m_pos[2] = m_pos[2] + GET_RANDOM * m_depth;
		m_particles[m_particleCount].m_vel[0] = m_velocity[0] + GET_RANDOM * m_velDelta[0];
		m_particles[m_particleCount].m_vel[1] = m_velocity[1] + GET_RANDOM * m_velDelta[1];
		m_particles[m_particleCount].m_vel[2] = m_velocity[2] + GET_RANDOM * m_velDelta[2];

		m_particleCount++;
	}
}

void CRainPS::Shutdown()
{
	if(m_particles) delete[] m_particles;

	m_particles = NULL;
	m_particleCount = 0;
	m_totalTime = 0;
}