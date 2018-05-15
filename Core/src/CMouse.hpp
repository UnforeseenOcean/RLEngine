#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Core/Input/MouseButtons.hpp"
#include "Core/Containers/Pair.hpp"

class CMouse {
public:
	// Starts the mouse facility, and registers a raw input device.
	// This is called automatically when starting the Core module.
	static bool CoreStartup();

	// Shuts down the mouse facility, and unregisters the raw input device.
	// This is called automatically when shutting down the Core module.
	static bool CoreShutdown();

public:
	typedef _internal::MouseButton Button;

	typedef PairInt Delta;

	CMouse();
	~CMouse();

	// Clears certain parts of the internal state at the start of a frame.
	// This function must be called once a frame to ensure that states are updated before the game state advances.
	void OnFrameStart();

	// An event handler for WM_INPUT mouse messages.
	// This method translates the RAWINPUT data contained in WM_INPUT messages
	// and caches the results in the members of this class.
	void Update(RAWINPUT& lpb);

	inline bool WentDown(Button::Enum button) const;
	inline bool WentUp(Button::Enum button) const;
	inline bool IsPressed(Button::Enum button) const;
	inline const Delta& GetDelta() const;
	inline int GetWheelDelta() const;

private:
	struct ButtonState {
		bool m_wentDown : 1;
		bool m_wentUp : 1;
		bool m_isPressed : 1;
		bool m_isDoubleClicked : 1;
	};

	Delta m_Delta;
	int m_wheelDelta;
	ButtonState m_states[Button::TOTAL];
};

#include "Core/Input/Mouse.inl"