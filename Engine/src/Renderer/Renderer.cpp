#include "pch.h"
#include "Renderer.h"
#include "Graphics/Components/MeshRenderer.h"
#include "Graphics/Components/ColorComponent.h"
#include "Constants/CBConstants.h"
#include "Utils/Vector3Ext.h"

namespace fe
{
	void Renderer::Initialize(Scene* scene, Device* device, ResourceCollection* resources)
	{
		this->scene = scene;
		this->device = device;

		this->cb_vs_mvpMatrices.Initialize(device->get(), device->context());
		this->cb_ps_color.Initialize(device->get(), device->context());

		std::wstring root = StringHelper::StringToWide(PathConstants::SHADERS_PATH);
		particleSystemPS = resources->Shaders.AddFromFilePS(root + L"particleSystem_ps.cso");
		particleSystemVS = resources->Shaders.AddFromFileVS(root + L"particleSystem_vs.cso", ParticleVertex::layout, ARRAYSIZE(ParticleVertex::layout));
		particleSystemGS = resources->Shaders.AddFromFileGS(root + L"particleSystem_gs.cso");
	}

	void Renderer::Render()
	{
		device->context()->VSSetConstantBuffers(CbVsIdxs::MvpMatricesIdx, 1, this->cb_vs_mvpMatrices.GetAddressOf());
		device->context()->GSSetConstantBuffers(CbVsIdxs::MvpMatricesIdx, 1, this->cb_vs_mvpMatrices.GetAddressOf());
		device->context()->PSSetConstantBuffers(CbPsIdxs::Color, 1, this->cb_ps_color.GetAddressOf());

		cb_vs_mvpMatricesData.viewMatrix = scene->camera.GetViewMatrix();
		cb_vs_mvpMatricesData.invViewMatrix = scene->camera.GetViewMatrix().Invert();
		cb_vs_mvpMatricesData.projMatrix = scene->camera.GetProjectionMatrix();
		cb_vs_mvpMatrices.Update(&cb_vs_mvpMatricesData);

		cb_ps_colorData.color = sm::Vector4(1, 1, 1, 1);
		cb_ps_color.Update(&cb_ps_colorData);

		auto gos = this->scene->GetActiveGameObjects();
		sort(gos.begin(), gos.end(), [](GameObject*& a, GameObject*& b) {
			Transform& tra = a->GetComponent<Transform>();
			Transform& trb = b->GetComponent<Transform>();
			if (!&tra && !&trb) return false;
			if (!&tra) return true;
			if (!&trb) return false;
			return (tra.GetPosition().z < trb.GetPosition().z);
			});

		for (auto& go : gos)
			RenderGameObject(go);

		//cleaning
		device->context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	void Renderer::RenderGameObject(GameObject* gameObject)
	{
		ColorComponent& cc = gameObject->GetComponent<ColorComponent>();
		cb_ps_colorData.color = &cc ? cc.Color : sm::Vector4(1, 1, 1, 1);
		cb_ps_color.Update(&cb_ps_colorData);

		MeshRenderer& mr = gameObject->GetComponent<MeshRenderer>();
		if (&mr)
		{
			Transform& tr = gameObject->GetComponent<Transform>();
			cb_vs_mvpMatricesData.modelMatrix = &tr ? tr.GetModelMatrix() : sm::Matrix::Identity;
			cb_vs_mvpMatrices.Update(&cb_vs_mvpMatricesData);


			for (size_t mesh_idx = 0; mesh_idx < mr.GetMeshesSize(); mesh_idx++)
			{
				RenderableMesh& rm = mr.GetMesh(mesh_idx);
				device->context()->IASetInputLayout(rm.vertexShader->GetInputLayout());

				device->context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				device->context()->VSSetShader(rm.vertexShader->GetShader(), NULL, 0);
				device->context()->PSSetShader(rm.pixelShader->GetShader(), NULL, 0);

				if (rm.texture)
					device->context()->PSSetShaderResources(0, 1, rm.texture->GetTextureResourceViewAddress());

				rm.mesh->Draw();
			}

		}

		auto pss = gameObject->GetComponents<ParticleSystem>();
		for (auto ps : pss)
			RenderParticleSystem(ps);
	}
	void Renderer::RenderParticleSystem(ParticleSystem* partricleSystem)
	{
		device->context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
		device->context()->IASetInputLayout(particleSystemVS->GetInputLayout());
		device->context()->VSSetShader(particleSystemVS->GetShader(), NULL, 0);
		device->context()->PSSetShader(particleSystemPS->GetShader(), NULL, 0);
		device->context()->GSSetShader(particleSystemGS->GetShader(), NULL, 0);

		partricleSystem->Render(device);

		device->context()->GSSetShader(nullptr, nullptr, 0);
		device->context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}
