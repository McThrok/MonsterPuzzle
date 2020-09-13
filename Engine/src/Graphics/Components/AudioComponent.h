#pragma once
#include "Component.h"
#include <string>
#include "../../Audio/AudioEngine.h"

namespace fe
{
	class AudioComponent : public Component
	{
		CLASS_DECLARATION(AudioComponent)
	public:
		AudioComponent(std::string soundFilename, AudioEngine* audioEngine, float scale = 1.0f, bool looped = false);
		void Play();
		std::string soundName;
		IXAudio2SourceVoice* audioSource;
	private:
		AudioEngine* audioEngine;
		XAUDIO2_BUFFER* audioBuffer;
	};
}
