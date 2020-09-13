#pragma once
#include "Scene/Scene.h"
#include "Graphics/Device.h"
#include "Graphics/Components/ParticleSystem.h"
#include "Graphics/Resources/ResourceCollection.h"

namespace fe
{
	class Renderer
	{
	public:
		void Initialize(Scene* scene, Device* device, ResourceCollection* resources);
		void Render();

	private:
		Scene* scene;
		Device* device;

		ConstantBuffer<CB_VS_mvpMatrices> cb_vs_mvpMatrices;
		ConstantBuffer<CB_PS_color> cb_ps_color;
		CB_VS_mvpMatrices cb_vs_mvpMatricesData;
		CB_PS_color cb_ps_colorData;

		VertexShader* particleSystemVS;
		PixelShader* particleSystemPS;
		GeometryShader* particleSystemGS;

		void RenderGameObject(GameObject* gameObject);
		void RenderParticleSystem(ParticleSystem* partricleSystem);
	};
}
