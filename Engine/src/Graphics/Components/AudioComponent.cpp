#include "pch.h"
#include "AudioComponent.h"

namespace fe
{
	CLASS_DEFINITION(Component, AudioComponent)

		AudioComponent::AudioComponent(std::string soundFilename, AudioEngine* audioEngine, float scale, bool looped) : audioEngine(audioEngine), soundName(soundFilename)
	{
		if (looped) {
			auto pair = audioEngine->GetSourceWithLoopedBuffer(soundFilename, scale);
			this->audioSource = pair.first;
			this->audioBuffer = pair.second;
		}
		else {
			auto pair = audioEngine->GetSourceWithBuffer(soundFilename, scale);
			this->audioSource = pair.first;
			this->audioBuffer = pair.second;
		}
	}

	void AudioComponent::Play()
	{
		this->audioSource->Stop();
		this->audioSource->FlushSourceBuffers();
		this->audioSource->SubmitSourceBuffer(this->audioBuffer);
		this->audioSource->Start();
	}
}
