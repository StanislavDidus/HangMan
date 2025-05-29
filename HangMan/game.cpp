#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include "template.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <string>

#include <stdio.h>    
#include <stdlib.h>     
#include <time.h> 

#include "Button.h"

namespace Tmpl8
{
	enum GameStatus
	{
		Play, Pause, WinScreen, LooseScreen
	};

	std::vector<std::string> words;

	std::vector<Button> buttons;

	std::string target_word;
	std::string hidden_word;
	int lc;
	int rw; // random word index
	int scale = 7;

	Sprite hangman(new Surface("assets/HangMan.png"), 7);
	int hangman_size_x = 150;
	int hangman_size_y = 300;
	int current_frame = 0;

	GameStatus current_game_status;

	float finish_time = 1.5f;
	float finish_timer = 0.f;

	void Game::SubmitLetter()
	{
		if (current_game_status == GameStatus::Play)
		{
			bool is_change = false;
			for (int i = 0; i < lc; i++)
			{
				if (target_word[i] == current_letter) hidden_word[i] = target_word[i], is_change = true;
			}

			if (!is_change)
			{
				hangman.SetFrame(++current_frame);
			}

			if (current_frame >= 6)
			{
				printf("Game Over\n");
				current_game_status = GameStatus::LooseScreen;
			}

			bool is_completed = true;
			for (int i = 0; i < lc; i++)
			{
				if (hidden_word[i] == '*') is_completed = false;
			}

			if (is_completed)
			{
				printf("You won!\n");
				current_game_status = GameStatus::WinScreen;
			}
		}
	}

	void Game::CheckButtons(int mx, int my)
	{
		if(current_game_status == GameStatus::Pause)
			for (auto& button : buttons)
			{
				if (mx >= button.x1 &&
					mx <= button.x2 &&
					my >= button.y1 &&
					my <= button.y2)
				{
					button.func();
				}
			}
	}

	void Game::Init()
	{
		srand((unsigned int)time(NULL));

		//Read words from the file
		FILE* f = fopen("Word_List.txt", "r");
		char buffer[100];
		while (fscanf(f, "%s", &buffer) == 1)
		{
			words.push_back(buffer);
		}
		
		fclose(f);

		Restart();

		//Init buttons
		buttons.emplace_back(0, 5, 0, 0, [this]() { printf("Restart\n"); Restart(); }, "Restart", 5, Tmpl8::Pixel(0x00FF00));
		buttons.back().CallibrateScale();
		buttons.emplace_back(0, 35, 0, 0, [this]() { printf("Quit\n"); std::exit(0); }, "Quit", 5, Tmpl8::Pixel(0xFF0000));
		buttons.back().CallibrateScale();
	}

	void Game::Restart()
	{
		//Restart all processes and chose new word from the list
		current_frame = 0;
		hangman.SetFrame(0);

		rw = rand() % words.size();

		current_game_status = GameStatus::Play;

		target_word = words[rw];
		lc = (int)strlen(target_word.data());
		hidden_word.clear();
		for (int i = 0; i < lc; i++)
		{
			hidden_word.append("*");
		}
	}

	void Game::Shutdown()
	{
	}

	void Game::Tick(float deltaTime)
	{
		screen->Clear(0);

		deltaTime /= 1000.f;

		switch (current_game_status)
		{
		case GameStatus::Play:
			//Draw hang man
			hangman.DrawScaled(ScreenWidth / 2 - hangman_size_x / 2, ScreenHeight / 2 - hangman_size_y / 2 - 50, hangman_size_x, hangman_size_y, screen);
			//Print uknown word
			screen->PrintScaled(&hidden_word[0], ScreenWidth / 2 - (5 * scale * lc + (lc - 1) * 6) / 2, 400, scale, scale, Tmpl8::Pixel(0xFFFFFF));
			//Print chosen letter
			screen->PrintScaled(&current_letter, 100, 200, 15, 15, Tmpl8::Pixel(0xFFFFFF));
			break;
		case GameStatus::Pause:
			for (auto& button : buttons)
			{
				button.render(screen);
			}
			break;
		case GameStatus::WinScreen:
			finish_timer += deltaTime;
			if (finish_timer >= finish_time)
			{
				finish_timer = 0.f;
				current_game_status = GameStatus::Pause;
			}
			//Draw hang man
			hangman.DrawScaled(ScreenWidth / 2 - hangman_size_x / 2, ScreenHeight / 2 - hangman_size_y / 2 - 50, hangman_size_x, hangman_size_y, screen);
			//Draw unkown word
			screen->PrintScaled(&hidden_word[0], ScreenWidth / 2 - (5 * scale * lc + (lc - 1) * 6) / 2, 400, scale, scale, Tmpl8::Pixel(0xFF00FF));
			//Print chosen letter
			screen->PrintScaled(&current_letter, 100, 200, 15, 15, Tmpl8::Pixel(0xFFFFFF));

			screen->PrintScaled("You won!", ScreenWidth / 2 - (5 * 8 * 8 + 7 * 6) / 2, 50, 8, 8, Tmpl8::Pixel(0x00FFFF));
			break;
		case GameStatus::LooseScreen:
			finish_timer += deltaTime;
			if (finish_timer >= finish_time)
			{
				finish_timer = 0.f;
				current_game_status = GameStatus::Pause;
			}
			//Draw hang man
			hangman.DrawScaled(ScreenWidth / 2 - hangman_size_x / 2, ScreenHeight / 2 - hangman_size_y / 2 - 50, hangman_size_x, hangman_size_y, screen);
			//Draw unkown word
			screen->PrintScaled(&hidden_word[0], ScreenWidth / 2 - (5 * scale * lc + (lc - 1) * 6) / 2, 400, scale, scale, Tmpl8::Pixel(0xFFFFFF));
			//Print chosen letter
			screen->PrintScaled(&current_letter, 100, 200, 15, 15, Tmpl8::Pixel(0xFFFFFF));

			//Print right word
			{
				int twl = (int)strlen(target_word.data());
				screen->PrintScaled(&target_word[0], ScreenWidth / 2 - (5 * 8 * twl + (twl - 1) * 6) / 2, 350, 8, 8, Tmpl8::Pixel(0xFF00FF));
			}

			screen->PrintScaled("Game Over!", ScreenWidth / 2 - (5 * 8 * 10 + 9 * 6) / 2, 50, 8, 8, Tmpl8::Pixel(0xFF0000));
			break;
		}
	}
};