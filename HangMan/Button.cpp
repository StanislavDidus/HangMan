#include "Button.h"

Button::Button(int x1, int y1, int x2, int y2, const std::function<void()>& func, const std::string& text, int text_scale, Tmpl8::Pixel color) : x1(x1), y1(y1), x2(x2), y2(y2),
func(func), text(text), text_scale(text_scale), color(color)
{

}

Button::~Button()
{

}

void Button::CallibrateScale()
{
	int lc = (int)strlen(text.data());
	int lp = (5 * text_scale * lc + (lc - 1) * 6);
	x2 = lp + x1;
	y2 = 5 * text_scale + y1;
}

void Button::render(Tmpl8::Surface* screen)
{
	//screen->Bar(x1,y1,x2,y2, Tmpl8::Pixel(0x00FF00));
	screen->PrintScaled(&text[0], x1, y1, text_scale, text_scale, color);
}
