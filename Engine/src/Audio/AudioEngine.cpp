#include "pch.h"
#include "AudioEngine.h"

using namespace std;
namespace fe
{
	std::vector<std::string> get_all_files_names_within_folder(std::string folder)
	{
		std::vector<std::string> names;
		std::string search_path = folder + "/*.wav";
		WIN32_FIND_DATA fd;
		std::wstring stemp = std::wstring(search_path.begin(), search_path.end());
		LPCWSTR sw = stemp.c_str();
		HANDLE hFind = FindFirstFile(sw, &fd);
		if (hFind != INVALID_HANDLE_VALUE) {
			do {
				// read all (real) files in current folder
				// , delete '!' read other 2 default folder . and ..
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
					std::wstring fn = fd.cFileName;

					names.push_back(std::string(fn.begin(), fn.end()));
				}
			} while (FindNextFile(hFind, &fd));
			FindClose(hFind);
		}
		return names;
	}
	void AudioEngine::Initialize()
	{
		this->pXAudio2 = NULL;
		HRESULT hr;
		if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR)))
			throw std::exception("Failed to initialize AudioEngine");
		IXAudio2MasteringVoice* pMasterVoice = NULL;
		if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasterVoice)))
			throw std::exception("Failed to initialize AudioEngine");
		XAUDIO2_BUFFER buffer = { 0 };

		std::string dirName = "./Resources/Audio/";

		for (auto filename : get_all_files_names_within_folder(dirName)) {
			WAVEFORMATEXTENSIBLE* wfx = new	WAVEFORMATEXTENSIBLE{ 0 };
			std::string sound = filename.substr(0, filename.length() - 4);
			filename = dirName + filename;
			std::wstring wfilename = std::wstring(filename.begin(), filename.end());
			LPCWSTR fn = wfilename.c_str();
			HANDLE hFile = CreateFile(
				fn,
				GENERIC_READ,
				FILE_SHARE_READ,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);

			if (INVALID_HANDLE_VALUE == hFile)
				throw std::exception("Failed to initialize AudioEngine");

			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
				throw std::exception("Failed to initialize AudioEngine");

			DWORD dwChunkSize;
			DWORD dwChunkPosition;
			//check the file type, should be fourccWAVE or 'XWMA'
			FindChunk(hFile, fourccRIFF, dwChunkSize, dwChunkPosition);
			DWORD filetype;
			ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
			if (filetype != fourccWAVE)
				throw std::exception("Failed to initialize AudioEngine");

			FindChunk(hFile, fourccFMT, dwChunkSize, dwChunkPosition);
			ReadChunkData(hFile, wfx, dwChunkSize, dwChunkPosition);

			FindChunk(hFile, fourccDATA, dwChunkSize, dwChunkPosition);
			BYTE* pDataBuffer = new BYTE[dwChunkSize];
			ReadChunkData(hFile, pDataBuffer, dwChunkSize, dwChunkPosition);

			Sound* s = new Sound;
			s->format = wfx;
			s->chunkSize = dwChunkSize;
			s->data = pDataBuffer;
			sounds.insert({ sound, s });
		}
	}

	std::pair<IXAudio2SourceVoice*, XAUDIO2_BUFFER*> AudioEngine::GetSourceWithBuffer(std::string soundFilename, float scale)
	{
		HRESULT hr;
		XAUDIO2_BUFFER* buffer = new XAUDIO2_BUFFER{ 0 };
		Sound* s = sounds[soundFilename];
		buffer->AudioBytes = s->chunkSize;
		buffer->pAudioData = s->data;
		buffer->Flags = XAUDIO2_END_OF_STREAM;
		s->buffer = buffer;
		IXAudio2SourceVoice* pSourceVoice;
		if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)s->format)))
			throw std::exception("Failed get audio source");
		pSourceVoice->SetVolume(scale);

		return std::make_pair(pSourceVoice, buffer);
	}

	std::pair<IXAudio2SourceVoice*, XAUDIO2_BUFFER*> AudioEngine::GetSourceWithLoopedBuffer(std::string soundFilename, float scale)
	{
		HRESULT hr;
		XAUDIO2_BUFFER* buffer = new XAUDIO2_BUFFER{ 0 };
		Sound* s = sounds[soundFilename];
		buffer->AudioBytes = s->chunkSize;
		buffer->pAudioData = s->data;
		buffer->Flags = XAUDIO2_END_OF_STREAM;
		buffer->LoopCount = XAUDIO2_LOOP_INFINITE;
		buffer->LoopBegin = 0;
		buffer->LoopLength = s->chunkSize / 4;

		s->buffer = buffer;
		IXAudio2SourceVoice* pSourceVoice;
		if (FAILED(hr = pXAudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)s->format)))
			throw std::exception("Failed get audio source");
		pSourceVoice->SetVolume(scale);

		return std::make_pair(pSourceVoice, buffer);
	}


	HRESULT AudioEngine::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
	{
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
			return HRESULT_FROM_WIN32(GetLastError());

		DWORD dwChunkType;
		DWORD dwChunkDataSize;
		DWORD dwRIFFDataSize = 0;
		DWORD dwFileType;
		DWORD bytesRead = 0;
		DWORD dwOffset = 0;

		while (hr == S_OK)
		{
			DWORD dwRead;
			if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());

			if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());

			switch (dwChunkType)
			{
			case fourccRIFF:
				dwRIFFDataSize = dwChunkDataSize;
				dwChunkDataSize = 4;
				if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
					hr = HRESULT_FROM_WIN32(GetLastError());
				break;

			default:
				if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
					return HRESULT_FROM_WIN32(GetLastError());
			}

			dwOffset += sizeof(DWORD) * 2;

			if (dwChunkType == fourcc)
			{
				dwChunkSize = dwChunkDataSize;
				dwChunkDataPosition = dwOffset;
				return S_OK;
			}

			dwOffset += dwChunkDataSize;

			if (bytesRead >= dwRIFFDataSize) return S_FALSE;

		}

		return S_OK;

	}

	HRESULT AudioEngine::ReadChunkData(HANDLE hFile, void* buffer, DWORD buffersize, DWORD bufferoffset)
	{
		HRESULT hr = S_OK;
		if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufferoffset, NULL, FILE_BEGIN))
			return HRESULT_FROM_WIN32(GetLastError());
		DWORD dwRead;
		if (0 == ReadFile(hFile, buffer, buffersize, &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());
		return hr;
	}

}