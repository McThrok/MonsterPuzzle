#pragma once

namespace fe
{
	class Layer
	{
	public:
		Layer(std::string name);

		std::string name;

		virtual void ProcssMouseInput() { };
		virtual void ProcssKeyboardInput() { };
		virtual void Update() {};
	};
}
