#pragma once

struct BRRawData
{
	BString name;
	BString path;
	enum SoundTypes {
				SOUND_BGM,
				SOUND_EFFECT
	};
	union Other
	{
		int		fontsize;
		SoundTypes soundType;
	};
	Other other;
};
