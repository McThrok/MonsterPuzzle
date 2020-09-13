#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include <list>
#include <random>
#include "Graphics/Buffers/DynamicVertexBuffer.h"
#include "Time/Time.h"
#include "Graphics/Device.h"
#include "Component.h"
#include "Graphics/Vertex.h"
#include "Graphics/Texture.h"

namespace sm = DirectX::SimpleMath;

namespace fe
{
	struct ParticleVertex
	{
		sm::Vector3 Pos;
		float Age;
		float Angle;
		float Size;
		float ttl;
		int version;
		static D3D11_INPUT_ELEMENT_DESC layout[6];

		ParticleVertex() : Pos(0.0f, 0.0f, 0.0f), Age(0.0f), Angle(0.0f), Size(0.0f) { }
	};

	struct ParticleVelocities
	{
		sm::Vector3 Velocity;
		float AngularVelocity;

		ParticleVelocities() : Velocity(0.0f, 0.0f, 0.0f), AngularVelocity(0.0f) { }
	};

	struct Particle
	{
		ParticleVertex Vertex;
		ParticleVelocities Velocities;
	};

	class ParticleSystem : public Component
	{
		CLASS_DECLARATION(ParticleSystem)
	public:

		ParticleSystem(int MAX_PARTICLES, Texture* colorTextre, Texture* particleTexture, sm::Vector3 m_emitterPos);

		void Update(Device* device);
		void Render(Device* device);


		sm::Vector3 m_emitterPos;
		sm::Vector3 EMITTER_DIR;	//mean direction of particles' velocity
		float TIME_TO_LIVE;	//time of particle's life in miliseconds
		float EMISSION_RATE;	//number of particles to be born per second
		float MAX_ANGLE;		//maximal angle declination from mean direction
		float MIN_VELOCITY;	//minimal value of particle's velocity
		float MAX_VELOCITY;	//maximal value of particle's velocity
		float PARTICLE_SIZE;	//initial size of a particle
		float PARTICLE_SCALE;	//size += size*scale*dtime
		float MIN_ANGLE_VEL;	//minimal rotation speed

		float MAX_ANGLE_VEL;	//maximal rotation speed

	private:
		void EnsureInitialized(Device* device);
		Device* device;

		int MAX_PARTICLES;		//maximal number of particles in the system

		float m_particlesToCreate;
		unsigned int m_particlesCount;

		std::list<Particle> m_particles;

		Texture* colorTextre;
		Texture* particleTexture;
		DynamicVertexBuffer<ParticleVertex> m_vertices;
		std::default_random_engine m_random;

		sm::Vector3 RandomVelocity();
		void AddNewParticle();
		void UpdateParticle(Particle& p, float dt);
		void UpdateVertexBuffer();
	};
}

