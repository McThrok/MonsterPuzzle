#pragma once

namespace fe
{
	class Time
	{
	public:
		/// <summary>
		/// The time in milliseconds it took to complete the last frame
		/// </summary>
		static float deltaTime;
		
		/// <summary>
		/// The time in milliseconds since the last level has been loaded
		/// </summary>
		static float timeSinceLevelLoad;
	};
}
