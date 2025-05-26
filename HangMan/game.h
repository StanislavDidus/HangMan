#pragma once

#include "SDL.h"
#include <iostream>
#include <cstdio>
#include <string>

namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Restart();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseUp(int button, int x, int y) { CheckButtons(x, y); }
	void MouseDown( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseMove( int x, int y ) { /* implement if you want to detect mouse movement */ }
	void KeyUp(int key) { if (key >= 'a' && key <= 'z') current_letter = static_cast<char>(key), SubmitLetter(); }
	void KeyDown(int key) {}
	void SubmitLetter();
	void CheckButtons(int mx, int my);
private:
	char current_letter = '*';

	Surface* screen;
};

}; // namespace Tmpl8