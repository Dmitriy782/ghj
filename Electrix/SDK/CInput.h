#pragma once

class CInput 
{
public:
	std::byte pad[12];
	bool isTrackIRAvailable;
	bool isMouseInitialized;
	bool isMouseActive;
	std::byte pad1[158];
	bool isCameraInThirdPerson;
	std::byte pad2;
	Vector cameraOffset;
};