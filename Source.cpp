#include <iostream>
#include <crtdbg.h>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include "Character.h"
#include "messagebox.h"
#include "button.h"
#include "Textbox.h"
#include "person.h"
#include "player.h"
#include "group.h"
#include "property.h"
#include "game.h"
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

bool check_if_string_is_number(string temporary)
{
	for (int i = 0; i < temporary.length(); i++)
	{
		if (temporary[i] - '0' < 0 || temporary[i] - '0' > 9)
			return false;
	}
	return true;
}

int main()
{
	bool state_start = true;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(2696);
	int n_players = 0; string temporary = "";
	sf::RenderWindow window(sf::VideoMode(1340, 780), "SFML works!");
	window.setKeyRepeatEnabled(false);
	sf::Texture texture;
	texture.loadFromFile("back.jpg");
	/*sf::Font* font1;
	sf::Mouse mos;
	font1 = new sf::Font;
	font1->loadFromFile("PoiretOne.ttf");
	sf::Font font2;
	font2.loadFromFile("arial.ttf");
	string text = "New Game";
	button b1(450, 200, 400, 70, font1, font2, text, sf::Color(51, 0, 102), sf::Color(224, 224, 224), sf::Color::Transparent);
	bool flag = false;int b_c = 0;
	while (window.isOpen())
	{
		sf::Event event;
		if (flag)
		{
			this_thread::sleep_for(1s);
			flag = false;
			b_c = 0;
		}
		else
		{
			while (window.pollEvent(event))
			{
				if (b1.isPressed())
				{
					if (b_c == 0)
					{
						cout << "b1 is pressed" << endl;
					}
					flag = true;
					b_c++;

				}
			}
		}
		window.clear();
		b1.render(&window);
		sf::Vector2i mpos = mos.getPosition();
		b1.update(sf::Vector2f(mpos.x, mpos.y), window);
		window.display();
	}*/
    sf::Texture texture1;
	texture1.loadFromFile("back1.jpg");
	sf::Sprite sprite;
	sf::Sprite sprite2;
	sf::Font* font1;
	sf::Mouse mos;
	font1 = new sf::Font;
	font1->loadFromFile("PoiretOne.ttf");
	sf::Font font2;
	font2.loadFromFile("arial.ttf");
	string text = "New Game";
	sprite.setTexture(texture);
	sprite2.setTexture(texture1);
	string tttt = "CS", tt1 = "EE",text1 = "Roll\nDice";
	button b1(450, 200, 400, 70, font1, font2, text, sf::Color(51, 0, 102), sf::Color(224, 224, 224), sf::Color::Transparent);//main for new game
	button loadgame(450, 400, 400, 70, font1, font2, "load previous game", sf::Color(51, 0, 102), sf::Color(224, 224, 224), sf::Color::Transparent);//main for load prev game
	button b2(50, 50, 80, 80, font1, font2, text1, sf::Color(51, 0, 102), sf::Color(224, 224, 224), sf::Color::Transparent,1);

	sf::RectangleShape diceinfo;
	diceinfo.setFillColor(sf::Color(51, 153, 255));
	diceinfo.setSize(sf::Vector2f(80, 80));
	diceinfo.setPosition(sf::Vector2f(150, 50));
	sf::Font fontdice;
	fontdice.loadFromFile("INVASION2000.ttf");
	sf::Text dicetext;
	dicetext.setFont(fontdice);
	dicetext.setString("-");
	dicetext.setCharacterSize(40);
	dicetext.setFillColor(sf::Color::White);
	dicetext.setPosition(sf::Vector2f(180, 50));

	sf::RenderTarget* target = &window;
	Textbox textbox1(15, sf::Color(64, 64, 64), true);
	sf::Font fontxx;
	if (!fontxx.loadFromFile("arial.ttf"))
		return EXIT_FAILURE;
	textbox1.setFont(fontxx);
	textbox1.setPosition({ 480, 200 });
	sf::RectangleShape rect(sf::Vector2f(300, 50));
	rect.setFillColor(sf::Color(192, 192, 192));
	rect.setPosition({ 480,200 });
	sf::Text entertext("Enter number of Players", *font1, 30);
	entertext.setStyle(sf::Text::Bold);
	entertext.setPosition({ 480,150 });
	sf::RectangleShape list[40];
	sf::Text listtext[40];
	string liststring[40] =
	{
		"GO","Iqbal\nA","Iqbal\nB","Iqbal\nC","Chance","Johar\nA","Johar\nB","Johar\nC","Community\nChest","Jail","Faisal\nA","Faisal\nB",
		"Faisal\nC","Land\nTax","Model\nA","Model\nB","Model\nC","Gulberg\nA","Gulberg\nB","Gulberg\nC","Free\nParking","DHA\nA","DHA\nB",
		"DHA\nC","Chance","Bahria\nA","Bahria\nB","Bahria\nC","Community\nChest","Metro","Bus\nstation","Railway\nstation","Property\ntax",
	    "Go to\nJail","SUI","WASA","LESCO","PTCL","Hotel","Chance"
	};
	sf::RectangleShape infobox;
    infobox.setFillColor(sf::Color(153, 51, 255));
	infobox.setOutlineThickness(1);
	infobox.setOutlineColor(sf::Color(96, 96, 96));
	infobox.setSize(sf::Vector2f(300, 400));
	infobox.setPosition(sf::Vector2f(50, 250));
	int posx = 550, posy = 20;
	for (int i = 0; i < 40; i++)
	{
		list[i].setFillColor(sf::Color(255,51,51));
		list[i].setOutlineThickness(5);
		list[i].setOutlineColor(sf::Color(96, 96, 96));
		list[i].setSize(sf::Vector2f(60, 60));
		list[i].setPosition(sf::Vector2f(posx, posy));

		listtext[i].setFont(fontxx);
		listtext[i].setString(liststring[i]);
		listtext[i].setCharacterSize(12);
		listtext[i].setFillColor(sf::Color::White);
		listtext[i].setPosition(sf::Vector2f(posx, posy));

		if (i + 2 <= 11)
			posx += 65;
		else if (i + 2 > 11 && i + 1 < 21)
			posy += 65;
		else if (i + 2 >= 21 && i + 1 < 31)
			posx -= 65;
		else
			posy -= 65;
	}
	sf::Font fontx;
	fontx.loadFromFile("brush.otf");
	sf::Text monoplytext;
	monoplytext.setFont(fontx);
	monoplytext.setString("MONOPLY");
	monoplytext.setCharacterSize(100);
	monoplytext.setFillColor(sf::Color(255,255,94));
	monoplytext.setPosition(sf::Vector2f(700, 300));
	monoplytext.setOutlineColor(sf::Color::Black);
	monoplytext.setOutlineThickness(1);

	bool flag = false, flag2 = false, flag3 = false,flag4 = false; int player_turn = 1,next_player=-99;
	bool flag_load_game = false;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (b1.isPressed())
				flag = true;
			if (loadgame.isPressed())
				flag_load_game = true;
				
		}
		if (flag)
		{
			string text1="s";
			while (flag)
			{
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
					{
						flag = false;
						textbox1.setstring();
						window.close();
					}
					if(event.type==sf::Event::TextEntered)
					{
						textbox1.typedOn(event, text1);
						if (text1[text1.length() - 1] == ']')
						{
							for (int i = 0; i < text1.length() - 1; i++)
								temporary += text1[i];
							int x = check_if_string_is_number(temporary);
							if (x)
							{
								n_players = stoi(temporary);
								if (n_players < 1 || n_players>8)
								{
									string mm = "sorry there can be only 2-8 players in the game";
									messagebox message(500, 500, mm);
									text1 = "s";
									temporary = "";
								}
								else
								{
									flag2 = true;
									flag = false;
								}
							}
							else
							{
								string mm = "please enter a number";
								messagebox message(500, 500, mm);
								text1 = "s";
								temporary = "";
							}
						}
					}
				}
				window.clear();
				window.draw(sprite);
				window.draw(rect);
				window.draw(entertext);
				textbox1.drawTo(window);
				window.display();
			}
		}
		if (flag_load_game)
		{
			ifstream readfile;
			readfile.open("totalplayers.txt");
			string tn;
			char temppp;
			readfile >> temppp;
			tn = temppp;
			n_players = stoi(tn);
			readfile >> temppp;
			readfile >> temppp;
			tn = ""; tn = temppp;
			player_turn = stoi(tn);
			readfile.close();
			flag2=true;
			cout << "n_players:" << n_players << endl;
		}
		if (flag2)
		{
			
			game* ptr = new game(n_players);

			sf::SoundBuffer buffer;
			if (!buffer.loadFromFile("fireforce.wav"))
				return -1;

			sf::Sound sound;
			sound.setBuffer(buffer);
			sound.setVolume(5);
			sound.setPitch(1);
			sound.play();
			sound.setLoop(true);

			//cout << "here after ptr initialized " << n_players<< endl;
			if (flag_load_game)
			{
				for (int k = 0; k < n_players; k++)
				{
					if (k == player_turn - 1)
						ptr->turns[k] = 0;
					else
						ptr->turns[k] = 1;
				}
				for (int i = 0; i < n_players; i++)
				{
					ptr->list_of_players[i]->read_data_from_file(ptr,list);
					cout << "here after player read" << endl;
					ptr->list_of_characters[i]->read_data_from_file(ptr);
					cout << "out here after charcater read" << endl;
				}
			}
			cout << "here" << endl;

			sf::Text playertext;
			playertext.setFont(fontdice);
			std::string ppppp_turn = "Player " + to_string(player_turn) + "'s turn";
			playertext.setString(ppppp_turn);
			playertext.setCharacterSize(30);
			playertext.setFillColor(sf::Color::White);
			playertext.setPosition(sf::Vector2f(60, 250));
			sf::Text playermoney;
			playermoney.setFont(fontdice);
			playermoney.setString("cash in hand:"+to_string(ptr->list_of_players[player_turn-1]->player_money));
			playermoney.setCharacterSize(20);
			playermoney.setFillColor(sf::Color(51,255,51));
			playermoney.setPosition(sf::Vector2f(60, 280));
			while (flag2)
			{
				while (window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
					{
						//we can add save game here aswell
						flag2 = false;
						window.close();
					}
					if (b2.isPressed())
					{
						flag4 = true;
						if (state_start)
							state_start = false;
					}
				}
				if (flag4)
				{
					cout << "player_turn before:" << player_turn << endl;
					int dicenum = ptr->rolldice();
					dicetext.setString(to_string(dicenum));
					player_turn = ptr->taketurn(dicenum);
					cout << "player_turn after:" << player_turn << endl;
					if (ptr->list_of_players[player_turn - 1]->pos == 10 && ptr->list_of_players[player_turn - 1]->jail_status == true)
					{
						ptr->landedonsomeproperty(player_turn, window, sprite2, list, listtext, monoplytext, diceinfo, dicetext, infobox, playertext, playermoney);
					}
					else
					{
						int count = 0;
						while (count < dicenum) //dicenum
						{
							ptr->takemove(player_turn, dicenum, window, sprite2, list, listtext, monoplytext, diceinfo, dicetext, infobox, playertext, playermoney);
							count++;
						}

						if (ptr->list_of_players[player_turn - 1]->pos != 34)
							ptr->landedonsomeproperty(player_turn, window, sprite2, list, listtext, monoplytext, diceinfo, dicetext, infobox, playertext, playermoney);
						else
						{

							sf::SoundBuffer buffer;
							buffer.loadFromFile("gotojail.wav");
							sf::Sound sound;
							sound.setBuffer(buffer);
							sound.setVolume(500);
							sound.setPitch(1);
							sound.play();


							//cout << "in jail" << endl;
							while (ptr->list_of_players[player_turn - 1]->pos != 10)
							{
								ptr->takemove(player_turn, dicenum, window, sprite2, list, listtext, monoplytext, diceinfo, dicetext, infobox, playertext, playermoney);
							}
							ptr->list_of_players[player_turn - 1]->jail_status = true;

						}
					}
						if (player_turn != n_players)
						{
							//cout << "player:" << player_turn << endl;
							playertext.setString("Player " + to_string(player_turn + 1) + " 's turn");
							playermoney.setString("cash in hand:" + to_string(ptr->list_of_players[player_turn]->player_money));
							next_player = player_turn + 1;
						}
						else
						{
							playertext.setString("Player 1's turn");
							playermoney.setString("cash in hand:" + to_string(ptr->list_of_players[0]->player_money));
							next_player = 1;
						}
					
				    if( !(ptr->list_of_players[next_player-1]->player_status=="out") )
					    flag4 = false;
					else
					{
						bool loopflag = true;
						while (loopflag)
						{
							player_turn = ptr->taketurn(dicenum);
							if (player_turn != n_players)
							{
								playertext.setString("Player " + to_string(player_turn + 1) + " 's turn");
								playermoney.setString("cash in hand:" + to_string(ptr->list_of_players[player_turn]->player_money));
								next_player = player_turn + 1;
							}
							else
							{
								playertext.setString("Player 1's turn");
								playermoney.setString("cash in hand:" + to_string(ptr->list_of_players[0]->player_money));
								next_player = 1;
							}

							if (ptr->list_of_players[next_player - 1]->player_status != "out")
								loopflag = false;
						}
						flag4 = false;
					}

				}

				
				window.clear();
				window.draw(sprite2);
				for (int i = 0; i < 40; i++)
				{
					window.draw(list[i]);
					window.draw(listtext[i]);
				}
				window.draw(monoplytext);
				if (true)
				{
					b2.render(target);
					sf::Vector2i mpos = mos.getPosition();
					b2.update(sf::Vector2f(mpos.x, mpos.y), window);
				}
				window.draw(diceinfo);
				window.draw(dicetext);
				window.draw(infobox);
				window.draw(playertext);
				window.draw(playermoney);
				for (int i = 0; i < ptr->no_players; i++)
				{
					if(ptr->list_of_players[i]->player_status!="out")
					    ptr->list_of_characters[i]->Draw(window);
				}
				window.display();
			}
			delete ptr;
			ofstream up;
			up.open("totalplayers.txt",ios::app);
			if (player_turn != n_players)
			{
				if(!state_start)
				    up << '\n' << player_turn + 1;
				else
					up << '\n' << player_turn;
			}
			else
				up << '\n' << 1;
			up.close();
		}
		window.clear();
		window.draw(sprite);
		b1.render(target);
		sf::Vector2i mpos = mos.getPosition();
		b1.update(sf::Vector2f(mpos.x, mpos.y),window);
		loadgame.render(target);
		mpos = mos.getPosition();
		loadgame.update(sf::Vector2f(mpos.x, mpos.y), window);
		window.display();
	}
	delete font1;
    /*sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");
	//string walkpattern[4] = {"Up","Right","Down","Left"};
	string walkpattern[4] = { "Down","Left","Right","Up" };
	string filename = "c8.png";
	Character *c1 = new Character({0,0},2,256,256,4,4,walkpattern,filename);
	sf::Vector2f dir = { 0.0f,0.0f };
	bool flag = false,flag1=false,flag2=false,flag3=false;
    while (window.isOpen())
    {
	    sf::Event event;
	    while (window.pollEvent(event))
	    {
		    if (event.type == sf::Event::Closed)
			    window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				flag = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				flag1 = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				flag2 = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				flag3 = true;
	    }
		if (flag)
		{
			dir.y = 0.0f;
			cout << "true" << endl;
			if (dir.x < 0.0f)
				dir.x *= -1;
			dir.x += 1.0f;
			c1->setDirection(dir,0);
			c1->Update(1.0f / 60.f);
			flag = false;
		}
		if (flag1)
		{
			dir.y = 0.0f;
			if (dir.x > 0.0f)
				dir.x *= -1;
			cout << "true" << endl;
			dir.x -= 1.0f;
			c1->setDirection(dir,1);
			c1->Update(1.0f / 60.f);
			flag1 = false;
		}
		if (flag2)
		{
			dir.x = 0.0f;
			cout << "true" << endl;
			if (dir.y < 0.0f)
			{
				dir.y *= -1;
			}
			dir.y += 2.0f;
			c1->setDirection(dir,2);
			c1->Update(1.0f / 60.f);
			flag2 = false;
		}
		if (flag3)
		{
			dir.x = 0.0f;
			cout << "true" << endl;
			if (dir.y > 0.0f)
			{
				dir.y *= -1;
			}
			dir.y -= 2.0f;
			c1->setDirection(dir,3);
			c1->Update(1.0f / 60.f);
			flag3 = false;
		}
	    window.clear();
		c1->Draw(window);
	    window.display();
    }
	delete c1;
    return EXIT_SUCCESS;*/
	_CrtDumpMemoryLeaks();
}