#pragma once
#include <string>
#include <xaudio2.h>
#include <vector>
#include <map>
#include <Windows.h>

//Little-Endian
#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

namespace fe
{
	class AudioEngine
	{
		struct Sound {
			BYTE* data;
			WAVEFORMATEXTENSIBLE* format;
			XAUDIO2_BUFFER* buffer;
			int chunkSize;
		};
	public:
		void Initialize();

		std::pair<IXAudio2SourceVoice*, XAUDIO2_BUFFER*> GetSourceWithBuffer(std::string soundFilename, float scale = 1);
		std::pair<IXAudio2SourceVoice*, XAUDIO2_BUFFER*> GetSourceWithLoopedBuffer(std::string soundFilename, float scale = 1);

	private:
		IXAudio2* pXAudio2;
		HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
		HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset);
		std::map<std::string, Sound*> sounds;
	};
}