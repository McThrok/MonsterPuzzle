#include "pch.h"
#include "ParticleSystem.h"

namespace fe
{
	CLASS_DEFINITION(Component, ParticleSystem)

		using namespace DirectX;
	using namespace std;

	D3D11_INPUT_ELEMENT_DESC ParticleVertex::layout[6] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32_SINT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};



	ParticleSystem::ParticleSystem(int MAX_PARTICLES, Texture* colorTextre, Texture* particleTexture, sm::Vector3 m_emitterPos)
	{
		this->MAX_PARTICLES = MAX_PARTICLES;
		this->colorTextre = colorTextre;
		this->particleTexture = particleTexture;
		this->m_emitterPos = m_emitterPos;

		EMITTER_DIR = XMFLOAT3(0.0f, 1.0f, 0.0f);
		TIME_TO_LIVE = 0.6f;
		EMISSION_RATE = 0.75 * MAX_PARTICLES / TIME_TO_LIVE;
		MIN_VELOCITY = 80.f;
		MAX_VELOCITY = 120.f;
		PARTICLE_SIZE = 10;
		PARTICLE_SCALE = 1.0f;
		MIN_ANGLE_VEL = -XM_PI;
		MAX_ANGLE_VEL = XM_PI;


		m_particlesToCreate = 0;
		m_particlesCount = 0;
		m_random = default_random_engine(random_device{}());
	}

	void ParticleSystem::EnsureInitialized(Device* device)
	{
		if (!this->device)
		{
			this->device = device;
			m_vertices.Initialize(device->get(), device->context(), nullptr, MAX_PARTICLES);
			m_particlesToCreate = -EMISSION_RATE;
		}
	}

	void ParticleSystem::Update(Device* device)
	{
		float dt = Time::deltaTime / 1000.0;
		EnsureInitialized(device);

		for (auto it = m_particles.begin(); it != m_particles.end(); )
		{
			UpdateParticle(*it, dt);
			auto prev = it++;
			if (prev->Vertex.Age >= TIME_TO_LIVE)
			{
				m_particles.erase(prev);
				--m_particlesCount;
			}
		}
		m_particlesToCreate += dt * EMISSION_RATE;

		if (m_particlesCount < MAX_PARTICLES)
			while (m_particlesToCreate >= 1.0f)
			{
				--m_particlesToCreate;
				if (m_particlesCount < MAX_PARTICLES)
				{
					AddNewParticle();
					++m_particlesCount;
				}
			}

		UpdateVertexBuffer();
	}

	void ParticleSystem::Render(Device* device)
	{
		if (device)
		{
			UINT offset = 0;
			device->context()->PSSetShaderResources(0, 1, colorTextre->GetTextureResourceViewAddress());
			device->context()->PSSetShaderResources(1, 1, particleTexture->GetTextureResourceViewAddress());
			device->context()->IASetVertexBuffers(0, 1, m_vertices.GetAddressOf(), m_vertices.StridePtr(), &offset);
			device->context()->Draw(m_particlesCount, 0);
		}
	}

	sm::Vector3 ParticleSystem::RandomVelocity()
	{
		static const uniform_real_distribution<float> angleDist(-XM_PI / 36, XM_PI / 36);
		static const uniform_real_distribution<float> velDist(MIN_VELOCITY, MAX_VELOCITY);
		float angle = angleDist(m_random) + XM_PI / 8;
		sm::Vector3 v{ sin(angle),cos(angle),0 };

		v.Normalize();
		v *= velDist(m_random);

		return v;
	}

	void ParticleSystem::AddNewParticle()
	{
		static const uniform_real_distribution<float> anglularVelDist(MIN_ANGLE_VEL, MAX_ANGLE_VEL);
		static const uniform_real_distribution<float> prop(0, 1);
		Particle p;
		p.Vertex.Age = 0;
		p.Vertex.Size = PARTICLE_SIZE;
		p.Velocities.Velocity = RandomVelocity();
		p.Vertex.Pos = m_emitterPos;
		p.Vertex.Angle = 0;
		p.Vertex.ttl = TIME_TO_LIVE;
		p.Vertex.version = (int)ceil(prop(m_random) * 4);
		p.Velocities.AngularVelocity = 0;

		// TODO : 2.26 Setup initial particle properties

		m_particles.push_back(p);
	}

	void ParticleSystem::UpdateParticle(Particle& p, float dt)
	{
		// TODO : 2.26 Update particle properties

		p.Vertex.Age += dt;
		p.Vertex.Pos = XMFLOAT3(p.Vertex.Pos.x + dt * p.Velocities.Velocity.x,
			p.Vertex.Pos.y + dt * p.Velocities.Velocity.y,
			p.Vertex.Pos.z + dt * p.Velocities.Velocity.z);
		p.Vertex.Angle += dt * p.Velocities.AngularVelocity;
		p.Vertex.Size = PARTICLE_SIZE * (0.7 +p.Vertex.Age*5 / p.Vertex.ttl);
	}

	void ParticleSystem::UpdateVertexBuffer()
	{
		vector<ParticleVertex> vertices(MAX_PARTICLES);
		// TODO : 2.27 Copy particles to a vector and sort them
		int count = 0;
		for (auto& p : m_particles)
		{
			vertices[count++] = p.Vertex;
		}

		std::sort(vertices.begin(), vertices.end(), [](ParticleVertex& a, ParticleVertex& b) {return a.Pos.z < b.Pos.z; });


		D3D11_MAPPED_SUBRESOURCE resource;
		auto hr = device->context()->Map(m_vertices.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		ERROR_IF_FAILED(hr, "Failed to map dynamic vertex buffer.");
		memcpy(resource.pData, vertices.data(), MAX_PARTICLES * sizeof(ParticleVertex));
		device->context()->Unmap(m_vertices.Get(), 0);
	}


}
