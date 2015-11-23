#ifndef _RAIN_PS_H_
#define _RAIN_PS_H_

#define UPDATE_AMT 1000

class CParticle
{
public:
	CParticle()
	{
		m_pos[0] = m_pos[1] = m_pos[2] = 0;
		m_vel[0] = m_vel[1] = m_vel[2] = 0;
	}

	float m_pos[3];
	float m_vel[3];
};

class CRainPS
{
public:
	CRainPS(float vX, float vY, float vZ,
		float dX, float dY, float dZ);
	~CRainPS() { Shutdown();}

	bool Initialize(float x, float y, float z, int maxP,
		float w, float h, float d);

	void Update(float scalar);

	void CreateParticle(int amount);

	void Shutdown();


	float m_width;
	float m_height;
	float m_depth;

	float m_pos[3];
	CParticle *m_particles;
	int m_maxParticles;
	int m_particleCount;
	float m_totalTime;

	float m_velocity[3];
	float m_velDelta[3];
};

#endif