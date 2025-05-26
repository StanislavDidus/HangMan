#pragma once

#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include "template.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <functional>

struct Button
{
	Button(int x1, int y1, int x2, int y2, const std::function<void()>& func, const std::string& text, int text_scale, Tmpl8::Pixel color);
	virtual ~Button();

	int x1, y1, x2, y2;
	std::function<void()> func;
	std::string text;
	int text_scale;

	Tmpl8::Pixel color;

	void CallibrateScale();

	void render(Tmpl8::Surface* screen);
};

