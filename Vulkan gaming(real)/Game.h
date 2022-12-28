#pragma once

#include <memory>

#include "VulkanResources.h"
#include "EventHandler.h"
#include "Text.h"
#include "Map.h"
#include "Button.h"

class EventHandler;

class Game
{
public:
	Game();

private:
	bool gameShouldStop();
	void startLoop();

	void onMouseButtonPressed(int button);
	void onMouseButtonHeld(int button);
	void onMouseButtonReleased(int button);

	void onKeyPressed(int key);
	void onKeyHeld(int key);

	void update();
	void processInput();

	bool showFPSCounter = false;
	uint64_t FPSCount = 0;
	std::unique_ptr<Text> FPSCounter;
	void updateFPSCounter();
	void toggleFPSCounter();

	Font debugFont;
	TextBox debugTextBox;

	std::unique_ptr<VulkanResources> vulkan;

	EventHandler eventHandler;
	friend class EventHandler;

	Map mineMap;
	Button<MemberFunction<void, Map>> resetButton;
};

