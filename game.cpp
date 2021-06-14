#include "game.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
//#include "button.h"
#include <cstdlib>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

void read_until_newline(string &temp,ifstream& read)
{
	char Temp='0';
	while (Temp != '\n')
	{
		read >> noskipws>>Temp;
		if(Temp!='\n')
		    temp += Temp;
	}

}

int find_first_comma_after_index_I(string temp,int I)
{
	int i = I;
	while (temp[i] != ',' && i!=temp.length())
	{
		i++;
	}
	return i - 1;
}

game::game(int no_of_players)
{
	no_players = no_of_players;
	list_of_characters = new Character * [no_players];
	ifstream read;
	string nnn="";
	read.open("characters.txt");
	for (int i = 0; i < no_players; i++)
	{
		string filename = ""; int dimenX = 0, dimenY = 0, Frames = 0, cycles = 0; string pattern[4] = {"-","-","-","-"};
		char temp[20];
		for (int j = 0; j < 3; j++)
		{
			read.getline(temp,20,'\n');
			if (j == 0)
			{
				for (int k = 0; k < strlen(temp); k++)
					filename += temp[k];
				cout << filename << endl;
			}
			else if (j == 1)
			{
				cout << temp << endl;
				for (int k = 0; k < strlen(temp); k++)
				{
					if (dimenX == 0)
					{
						if (temp[k] != ',')
							nnn += temp[k];
						else
						{
							dimenX = stoi(nnn);
							cout << dimenX << endl;
							nnn = "";
						}
					}
					else if (dimenY == 0)
					{
						if (temp[k] != ',')
							nnn += temp[k];
						else
						{
							dimenY = stoi(nnn);
							cout << dimenY << endl;
							nnn = "";
						}
					}
					else if (Frames == 0)
					{
						if (temp[k] != ',')
							nnn += temp[k];
						else
						{
							Frames = stoi(nnn);
							cout << Frames << endl;
							nnn = "";
						}
					}
					else if (cycles == 0)
					{
						if (k != strlen(temp))
							nnn += temp[k];
						if(k+1==strlen(temp))
						{
							cycles = stoi(nnn);
							cout << cycles << endl;
							nnn = "";
						}
					}
				}
			}
			else if (j == 2)
			{
				for (int k = 0; k < strlen(temp); k++)
				{
					if (pattern[0] == "-")
					{
						if (temp[k] != ',')
							nnn += temp[k];
						else
						{
							pattern[0]=nnn;
							cout << pattern[0] << endl;
							nnn = "";
						}
					}
					else if (pattern[1] == "-")
					{
						if (temp[k] != ',')
							nnn += temp[k];
						else
						{
							pattern[1] = nnn;
							cout << pattern[1] << endl;
							nnn = "";
						}
					}
					else if (pattern[2] == "-")
					{
						if (temp[k] != ',')
							nnn += temp[k];
						else
						{
							pattern[2] = nnn;
							cout << pattern[2] << endl;
							nnn = "";
						}
					}
					else if (pattern[3] == "-")
					{
						if (k != strlen(temp))
							nnn += temp[k];
						if (k + 1 == strlen(temp))
						{
							pattern[3] = nnn;
							cout << pattern[3] << endl;
							nnn = "";
						}
					}
				}
			}
		}
		list_of_characters[i] = new Character(i+1,{550,20},i+1,dimenX,dimenY,Frames,cycles,pattern,filename);
	}
	read.close();
	list_of_players = new player * [no_of_players];
	for (int i = 0; i < no_of_players; i++)
		list_of_players[i] = new player(i+1);
	read.open("properties.txt");
	int loopcount = 0; string temp = "";
	read_until_newline(temp,read);
	loopcount = stoi(temp); no_groups = loopcount;
	list_of_groups = new group * [no_groups];
	int count = 0;
	while (count < loopcount)
	{
		temp = "";
		read_until_newline(temp, read);
		list_of_groups[count] = new group(temp); temp = "";
		read_until_newline(temp, read);
		int innerloopcount = stoi(temp);
		list_of_groups[count]->no_properties = innerloopcount;
		list_of_groups[count]->list_of_properties = new property * [innerloopcount];
		for (int i = 0; i < innerloopcount; i++)
		{
			temp = "";
			read_until_newline(temp, read);
			int I = 0,end = 0;
			end = find_first_comma_after_index_I(temp,I);
			list_of_groups[count]->list_of_properties[i] = new property(list_of_groups[count]->return_string_index_to_index(I,end,temp),count);
			I = end + 2;
			end = find_first_comma_after_index_I(temp, I);
			list_of_groups[count]->list_of_properties[i]->price = stoi(list_of_groups[count]->return_string_index_to_index(I, end, temp));
			I = end + 2;
			end = find_first_comma_after_index_I(temp, I);
			list_of_groups[count]->list_of_properties[i]->rent = stoi(list_of_groups[count]->return_string_index_to_index(I, end, temp));
			string thing = "private",thing2 = "public";
			if (list_of_groups[count]->endswith(thing))
			{
				list_of_groups[count]->list_of_properties[i]->type = "private";
			}
			else
			{
				list_of_groups[count]->list_of_properties[i]->type = "public";
			}
			//have to work from here
		}
		count++;
	}
	turns = new int[no_players];
	turns[0] = 0;
	for (int i = 1; i < no_players; i++)
		turns[i] = 1;
}

bool game::check_if_string_is_number(string temporary)
{
	for (int i = 0; i < temporary.length(); i++)
	{
		if (temporary[i] - '0' < 0 || temporary[i] - '0' > 9)
			return false;
	}
	return true;
}

bool game::check_if_array_filled(int* arr, int size)
{
	for (int i = 0; i < size; i++)
	{
		if (arr[i] == -1)
			return false;
	}
	if (size == 0)
		return false;
	return true;
}

bool game::check_if_array_has_number(int* arr,int num,int* aarr, int size)
{
	int max = -1;
	for (int i = 0; i < size; i++)
	{
		if (max < aarr[arr[i]])
		{
			max = aarr[arr[i]];
		}
	}

	if (num <= max)
		return true;

	return false;
}

int game::find_largest(int arr[], int size)
{

	int max = 0;
	for (int i = 0; i < size; i++)
	{
		if (arr[i] > arr[max])
			max = i;
	}
	return max;

}

int game::rolldice()
{
	srand(time(NULL));
	int v2 = rand() % 6;
	return v2 + 1;
}

int game::taketurn(int dicenum)
{
	int index = 0;
	while (turns[index] != 0)
		index++;

	int turn = index + 1;
	turns[index] = 1;
	if (index != no_players - 1)
		turns[index + 1] = 0;
	else
		turns[0] = 0;

	list_of_players[turn - 1]->prev_pos = list_of_players[turn - 1]->pos;
	//list_of_players[turn - 1]->pos += dicenum;

	return turn;
}

void game::takemove(int p_n, int dicenum, sf::RenderWindow& target, sf::Sprite sprite2, sf::RectangleShape list[], sf::Text listtext[], sf::Text monoplytext, sf::RectangleShape diceinfo, sf::Text dicetext, sf::RectangleShape infobox, sf::Text playertext,sf::Text playermoney)
{

	sf::SoundBuffer buffer;
	buffer.loadFromFile("walk.wav");
		

	sf::Sound sound;
	sound.setBuffer(buffer);
	sound.setVolume(300);
	sound.setPitch(1);
	sound.play();
	



	sf::Vector2f currentpos = list_of_characters[p_n - 1]->getcharacterposition();
	int targetpos = list_of_players[p_n - 1]->pos + 1;
	sf::Vector2f dir = { 0.0f,0.0f }, vel = {0.0f,0.0f};
	int count = 1;
	bool flag=true;
	while (flag)
	{
		if (list_of_players[p_n-1]->pos+1>=1 && list_of_players[p_n - 1]->pos + 1 < 12)
		{
			dir.y = 0.0f;
			if (dir.x < 0.0f)
				dir.x *= -1;
			dir.x += 1.0f;
			list_of_characters[p_n - 1]->setDirection(dir, 0);
			list_of_characters[p_n - 1]->Update(1.0f / 20.0f);
			/*if (list_of_characters[p_n - 1]->getcharacterposition().x == currentpos.x + 3.0f)
			{           
				list_of_players[p_n - 1]->pos += 1;
				currentpos = list_of_characters[p_n - 1]->getcharacterposition();
			}*/
			if (count < 50)
				count++;
			else
			{
				count = 0;
				list_of_players[p_n - 1]->pos += 1;
			}
			if (list_of_players[p_n - 1]->pos == targetpos)
			{
				flag = false;
				count = 0;
			}
		}
		else if (list_of_players[p_n - 1]->pos + 1 >= 22 && list_of_players[p_n - 1]->pos+1 < 32)
		{
			dir.y = 0.0f;
			if (dir.x > 0.0f)
				dir.x *= -1;
			dir.x -= 1.0f;
			list_of_characters[p_n - 1]->setDirection(dir, 1);
			list_of_characters[p_n - 1]->Update(1.0f / 20.f);
			/*if (dir.x == currentpos.x - 65.0f)
			{
				list_of_players[p_n - 1]->pos += 1;
				currentpos.x -= 65.0f;
			}*/
			if (count < 50)
				count++;
			else
			{
				count = 0;
				list_of_players[p_n - 1]->pos += 1;
			}
			if (list_of_players[p_n - 1]->pos == targetpos)
			{
				flag = false;
				count = 0;
			}
		}
		else if (list_of_players[p_n - 1]->pos + 1 >= 12 && list_of_players[p_n - 1]->pos+1 < 22)
		{
			dir.x = 0.0f;
			if (dir.y < 0.0f)
				dir.y *= -1;
			dir.y += 1.0f;
			list_of_characters[p_n - 1]->setDirection(dir, 2);
			list_of_characters[p_n - 1]->Update(1.0f / 20.f);
			/*if (dir.y == currentpos.y + 65.0f)
			{
				list_of_players[p_n - 1]->pos += 1;
				currentpos.y += 65.0f;
			}*/
			if (count < 50)
				count++;
			else
			{
				count = 0;
				list_of_players[p_n - 1]->pos += 1;
			}
			if (list_of_players[p_n - 1]->pos == targetpos)
			{
				flag = false;
				count = 0;
			}
		}
		else if (list_of_players[p_n - 1]->pos + 1 >= 32 && list_of_players[p_n - 1]->pos+1 < 42)
		{
			dir.x = 0.0f;
			if (dir.y > 0.0f)
				dir.y *= -1;
			dir.y -= 1.0f;
			list_of_characters[p_n - 1]->setDirection(dir, 3);
			list_of_characters[p_n - 1]->Update(1.0f / 20.f);
			/*if (dir.y == currentpos.y - 65.0f)
			{
				if (list_of_players[p_n - 1]->pos + 1 == 41)
					list_of_players[p_n - 1]->pos = 1;
				else
					list_of_players[p_n - 1]->pos += 1;
				currentpos.y -= 65.0f;
			}*/
			/*if (list_of_players[p_n - 1]->pos == targetpos)
				flag = false;
			if (targetpos == 41 && list_of_players[p_n - 1]->pos == 1)
				flag = false;*/

			if (count < 50)
				count++;
			else
			{
				count = 0;
				list_of_players[p_n - 1]->pos += 1;
			}
			if (list_of_players[p_n - 1]->pos == targetpos)
			{
				if (targetpos == 41)
					list_of_players[p_n - 1]->pos = 1;
				flag = false;
				count = 0;
			}
		}
		target.clear();
		target.draw(sprite2);
		for (int i = 0; i < 40; i++)
		{
			target.draw(list[i]);
			target.draw(listtext[i]);
		}
		target.draw(monoplytext);
		target.draw(diceinfo);
		target.draw(dicetext);
		target.draw(infobox);
		target.draw(playertext);
		target.draw(playermoney);
		for (int i = 0; i <no_players; i++)
		{
			if (list_of_players[i]->player_status != "out")
			    list_of_characters[i]->Draw(target);
		}
		target.display();
	}
	
}

int game::auction(sf::RenderWindow& win, sf::Sprite sprite2, sf::RectangleShape list[], sf::Text listtext[], sf::Text monoplytext, sf::RectangleShape diceinfo, sf::Text dicetext, sf::RectangleShape infobox, sf::Text playertext, sf::Text playermoney,button b3,int &next)
{
	//have to free arr,same,textbox1
	int* arr,*same,count = 0;
	arr = new int[no_players];
	same = new int[no_players];
	int no_same = 0;
	for (int i = 0; i < no_players; i++)
	{
		arr[i] = -1;
		same[i] = -1;
	}
	string extra = "players ",extra1="";
	Textbox *textbox1 = new Textbox(15, sf::Color(64, 64, 64), true);
	sf::Font fontxx;
	sf::Font fontt;
	fontt.loadFromFile("INVASION2000.ttf");
	sf::Text text;
	text.setFont(fontt);
	text.setCharacterSize(15);
	fontxx.loadFromFile("arial.ttf");
	textbox1->setFont(fontxx);
	textbox1->setPosition({ 50, 450 });
	sf::RectangleShape rect(sf::Vector2f(300, 50));
	rect.setFillColor(sf::Color(192, 192, 192));
	rect.setPosition({ 50,450 });
	sf::Mouse mos;
	int player_num,index_of_max=-1;
	player_num = rand() % no_players + 0;
	bool flag = true;
	bool flagcontrol = true; int b_c = 1;
	while (flag)
	{
		text.setString("player "+to_string(player_num+1)+"'s turn\nto bid");
		text.setPosition({50,350});
		string text1 = "s",temporary="";
		sf::Event event;
		bool otherflag = false;
		if (index_of_max != -1)
		{
			if (!check_if_array_filled(same, no_same))
			{
				for (int i = 0; i < no_players; i++)
				{
					if (index_of_max != i)
					{
						if (arr[index_of_max] == arr[i])
						{
							otherflag = true;
							extra += " " + to_string(i + 1);
							same[no_same] = i;
							no_same++;
						}
					}
					if (i + 1 == no_players)
					{
						if (no_same > 0)
						{
							extra += " " + to_string(index_of_max + 1);
							same[no_same] = index_of_max;
							no_same++;
							extra += "\n have bidded\nsame amount\n please these members\nbid higher\nto compete";
							text.setString(extra);
						}
						else
							flag = false;
					}
				}
			}
			if (check_if_array_filled(same, no_same))
			{
				extra1 = "player " + to_string(same[count] + 1);
				text.setString(extra + extra1);
			}
		}

		if (flag)
		{
			if (flagcontrol)
			{
				this_thread::sleep_for(1s);
				flagcontrol = false;
				b_c = 0;
			}
			else
			{
				while (win.pollEvent(event))
				{
					if (event.type == sf::Event::TextEntered)
					{
						textbox1->typedOn(event, text1);
						if (text1[text1.length() - 1] == ']')
						{
							for (int i = 0; i < text1.length() - 1; i++)
								temporary += text1[i];
							int x = check_if_string_is_number(temporary);// here x
							if (x)
							{
								if (index_of_max != -1)
								{
									
										if (!check_if_array_has_number(same, stoi(temporary), arr, no_same))
										{
											if (list_of_players[same[count]]->player_money - stoi(temporary) >= 0)
											{
												arr[same[count]] = stoi(temporary);
												count++;
												if (count == no_same)
												{
													index_of_max = same[0];
													for (int m = 0; m < no_same; m++)
													{
														if (arr[index_of_max] < arr[same[m]])
															index_of_max = same[m];
													}
													//list_of_players[index_of_max]->buy_property();
													/*string mm = "player "+to_string(index_of_max+1)+"bid the highest amount:"+to_string(arr[index_of_max])
														+ "\nwins the bidding and the area";
													messagebox message(500, 500, mm);*/
													flag = false;
													next = arr[index_of_max];
													delete[] arr;
													delete[] same;
													delete textbox1;
													return index_of_max + 1;
												}
											}
											else
											{
												string mm = "you dont have this much money to begin with";
												messagebox message(500, 500, mm);
											}

										}
										else
										{
											string mm = "please enter a number greater than the previous bidder\nor decline";
											messagebox message(500, 500, mm);
										}
									
								}
								else
								{
									if (arr[player_num] == -1)
									{
										if (list_of_players[player_num]->player_money - stoi(temporary) >= 0)
										{
											cout << "yeah" << endl;
											arr[player_num] = stoi(temporary);
											player_num = rand() % no_players + 0;
											if (!check_if_array_filled(arr, no_players))
											{
												while (arr[player_num] != -1)
												{
													player_num = rand() % no_players + 0;
												}
											}
										}
										else
										{
											string mm = "you dont have this much money to begin with";
											messagebox message(500, 500, mm);
										}
									}

									if (check_if_array_filled(arr, no_players))
									{
										cout << "hella" << endl;
										index_of_max = find_largest(arr, no_players);
										cout << "index of max" << endl;
									}
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
					if (b3.isPressed())
					{
						cout << "outhere" << endl;
						if (b_c == 0)
						{
							cout << "b3 is pressed" << endl;
							if (no_same == 0)
							{
								if (arr[player_num] == -1)
								{
									cout << "yeah" << endl;
									//arr[player_num] = stoi(temporary);
									arr[player_num] = -2;
									player_num = rand() % no_players + 0;
									if (!check_if_array_filled(arr, no_players))
									{
										while (arr[player_num] != -1)
										{
											player_num = rand() % no_players + 0;
										}
									}
								}

								if (check_if_array_filled(arr, no_players))
								{
									cout << "hella" << endl;
									index_of_max = find_largest(arr, no_players);
									cout << "index of max" << endl;
								}
							}
							else if (no_same > 0)
							{
								if (index_of_max != -1)
								{
									cout << "here here" << endl;
									//arr[same[count]] = stoi(temporary);
									arr[same[count]] = -2;
									count++;
									if (count == no_same)
									{
										index_of_max = same[0];
										for (int m = 0; m < no_same; m++)
										{
											if (arr[index_of_max] < arr[same[m]])
												index_of_max = same[m];
										}
										//list_of_players[index_of_max]->buy_property();
										//string mm = "player "+to_string(index_of_max+1)+"bid the highest amount:"+to_string(arr[index_of_max])
										//	+ "\nwins the bidding and the area";
										//messagebox message(500, 500, mm);
										flag = false;
										next = arr[index_of_max];
										delete[] arr;
										delete[] same;
										delete textbox1;
										return index_of_max + 1;
									}
								}
							}
						}
						flagcontrol = true;
						b_c++;
					}
				}
			}
		}

		win.clear();
		win.draw(sprite2);
		for (int i = 0; i < 40; i++)
		{
			win.draw(list[i]);
			win.draw(listtext[i]);
		}
		win.draw(monoplytext);
		win.draw(diceinfo);
		win.draw(dicetext);
		win.draw(infobox);
		win.draw(playertext);
		win.draw(playermoney);
		win.draw(text);
		win.draw(rect);
		textbox1->drawTo(win);
		b3.render(&win);
		sf::Vector2i mpos = mos.getPosition();
		b3.update(sf::Vector2f(mpos.x, mpos.y),win);
		for (int i = 0; i < no_players; i++)
		{
			if (list_of_players[i]->player_status != "out")
			    list_of_characters[i]->Draw(win);
		}
		win.display();
	}
	next = arr[index_of_max];
	delete[] arr;
	delete[] same;
	delete textbox1;
	return index_of_max+1;
}


void game::exchange(int p_n,sf::RenderWindow& win, sf::Sprite sprite2, sf::RectangleShape list[], sf::Text listtext[], sf::Text monoplytext, sf::RectangleShape diceinfo, sf::Text dicetext, sf::RectangleShape infobox, sf::Text playertext, sf::Text playermoney, button b3,button b1,int &next,int& ggx)
{
	sf::Event event;
	int level = 2,p_nn = 0,p_nn_property_n=0;
	string extra = "players ", extra1 = "players ";
	for (int i = 0; i < this->no_players ; i++)
	{
		if (this->list_of_players[i]->get_n_properties() > 0)
		{
			if (i != p_n - 1)
			{
				if (i == 0)
					extra1 += to_string(i + 1) + "(orange),";
				else if (i == 1)
					extra1 += to_string(i + 1) + "(blue),";
				else if (i == 2)
					extra1 += to_string(i + 1) + "(green)\n";
				else if (i == 3)
					extra1 += to_string(i + 1) + "(pink),";
				else if (i == 4)
					extra1 += to_string(i + 1) + "(yellow),";
				else if (i == 5)
					extra1 += to_string(i + 1) + "(purple)\n";
				else if (i == 6)
					extra1 += to_string(i + 1) + "(mehroon),";
				else if (i == 7)
					extra1 += to_string(i + 1) + "(brown)";
			}
		}
	}
	extra1 += "\nown a property\nenter the number of the player\nwho u want to\ndeal with";
	Textbox* textbox1 = new Textbox(15, sf::Color(64, 64, 64), true);
	sf::Font fontxx;
	sf::Font fontt;
	fontt.loadFromFile("INVASION2000.ttf");
	sf::Text text;
	text.setFont(fontt);
	text.setCharacterSize(15);
	fontxx.loadFromFile("arial.ttf");
	textbox1->setFont(fontxx);
	textbox1->setPosition({ 50, 500 });
	sf::RectangleShape rect(sf::Vector2f(300, 50));
	rect.setFillColor(sf::Color(192, 192, 192));
	rect.setPosition({ 50,500 });
	sf::Mouse mos;
	int player_num, index_of_max = -1;
	player_num = rand() % no_players + 0;
	bool flag = true;
	bool flagcontrol = true; int b_c = 1;

	while (flag)
	{
		if (level == 2)
		{
			text.setString(extra1);
			text.setPosition({ 50,350 });
		}
		else if (level == 3)
		{
			extra = "PLAYER " + to_string(p_nn) + " turn\n";
			extra += "player " + to_string(p_n) + " wants to deal\nhis property " + this->list_of_players[p_n - 1]->getnameofpropertyatpos(list_of_players[p_n - 1]->pos);
			extra += "\nprice:" + to_string(list_of_players[p_n - 1]->getpriceofpropertyatpos(list_of_players[p_n - 1]->pos)); extra += "\nselect property to sale";
			for (int i = 0; i < this->list_of_players[p_nn - 1]->get_n_properties(); i++)
			{
				extra += "\n" + to_string(i + 1) + ")" + list_of_players[p_nn - 1]->list_of_properties[i]->name + ",price:" + to_string(list_of_players[p_nn - 1]->list_of_properties[i]->price);
			}
			text.setString(extra);
			text.setPosition({ 50,250 });
		}
		else if (level == 4)
		{
			text.setPosition({ 50,350 });
			text.setString("player "+to_string(p_nn)+" is ready to sell\n"+
			this->list_of_players[p_nn-1]->list_of_properties[p_nn_property_n-1]->name+
			",price:"+to_string(this->list_of_players[p_nn - 1]->list_of_properties[p_nn_property_n - 1]->price));
		}
		string text1 = "s", temporary = "";
		
		bool otherflag = false;

		if (flag)
		{
			if (flagcontrol)
			{
				this_thread::sleep_for(1s);
				flagcontrol = false;
				b_c = 0;
			}
			else
			{
				while (win.pollEvent(event))
				{
					if (event.type == sf::Event::TextEntered)
					{
						textbox1->typedOn(event, text1);
						if (text1[text1.length() - 1] == ']')
						{
							for (int i = 0; i < text1.length() - 1; i++)
								temporary += text1[i];
							int x = check_if_string_is_number(temporary);
							if (x)
							{
								if (level == 2)
								{
									level = 3;
									p_nn = stoi(temporary);
								}
								else if (level == 3)
								{
									p_nn_property_n = stoi(temporary);
									level = 4;
									b_c = 1;
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
					if (level == 4)
					{
						if (b1.isPressed())
						{
							if (b_c == 0)
							{

								/*cout << "properties owned by player:" << p_n << ":";
								for (int i = 0; i < list_of_players[p_n - 1]->get_n_properties(); i++)
								{
									cout << list_of_players[p_n - 1]->list_of_properties[i]->name << endl;
								}
								cout << "properties owned by player:" << p_nn << ":";
								for (int i = 0; i < list_of_players[p_nn - 1]->get_n_properties(); i++)
								{
									cout << list_of_players[p_nn - 1]->list_of_properties[i]->name << endl;
								}*/
								cout << "final offer done" << endl;
								int iii = list_of_players[p_n - 1]->getindexofpropertyatpos(list_of_players[p_n - 1]->pos);

								int overflow = 0;
								if (this->list_of_players[p_nn - 1]->list_of_properties[p_nn_property_n - 1]->price > list_of_players[p_n - 1]->list_of_properties[iii]->price)
								{
									overflow = this->list_of_players[p_nn - 1]->list_of_properties[p_nn_property_n - 1]->price - list_of_players[p_n - 1]->list_of_properties[iii]->price;
									this->list_of_players[p_nn - 1]->player_money += overflow;
									this->list_of_players[p_n - 1]->player_money -= overflow;
								}
								else if (this->list_of_players[p_nn - 1]->list_of_properties[p_nn_property_n - 1]->price < list_of_players[p_n - 1]->list_of_properties[iii]->price)
								{
									overflow = list_of_players[p_n - 1]->list_of_properties[iii]->price - this->list_of_players[p_nn - 1]->list_of_properties[p_nn_property_n - 1]->price;
									this->list_of_players[p_nn - 1]->player_money -= overflow;
									this->list_of_players[p_n - 1]->player_money += overflow;
								}

								property* temp = this->list_of_players[p_nn - 1]->list_of_properties[p_nn_property_n - 1];
								this->list_of_players[p_nn - 1]->list_of_properties[p_nn_property_n - 1] = list_of_players[p_n - 1]->getpropertyatpos(list_of_players[p_n - 1]->pos);
								list_of_players[p_n - 1]->list_of_properties[iii] = temp;
								/*cout << "properties owned by player:" << p_n << ":";
								for (int i = 0; i < list_of_players[p_n - 1]->get_n_properties(); i++)
								{
									cout << list_of_players[p_n-1]->list_of_properties[i]->name << endl;
								}
								cout << "properties owned by player:" << p_nn << ":";
								for (int i = 0; i < list_of_players[p_nn - 1]->get_n_properties(); i++)
								{
									cout << list_of_players[p_nn-1]->list_of_properties[i]->name << endl;
								}*/
								list[list_of_players[p_n - 1]->list_of_properties[iii]->pos-1].setFillColor(list_of_players[p_n - 1]->getcolor());
								list[list_of_players[p_nn - 1]->list_of_properties[p_nn_property_n - 1]->pos-1].setFillColor(list_of_players[p_nn - 1]->getcolor());
								list_of_players[p_n - 1]->list_of_properties[iii]->owned_by_player = p_n;
								list_of_players[p_nn - 1]->list_of_properties[p_nn_property_n - 1]->owned_by_player = p_nn;
								next = p_nn;
								ggx = list_of_players[p_n - 1]->list_of_properties[iii]->owned_by_g_no;
								flag = false;
							}
							flagcontrol = true;
							b_c++;
						}
					}
					if (b3.isPressed())
					{
						if (b_c == 0)
						{
							cout << "b3 is pressed" << endl;
						}
						flagcontrol = true;
						if (level == 2)
							flag = false;
						else if (level == 3 || level==4)
							level = 2;
						b_c++;
					}
				}
			}
		}

		win.clear();
		win.draw(sprite2);
		for (int i = 0; i < 40; i++)
		{
			win.draw(list[i]);
			win.draw(listtext[i]);
		}
		win.draw(monoplytext);
		win.draw(diceinfo);
		win.draw(dicetext);
		win.draw(infobox);
		if (level != 3)
		{
			win.draw(playertext);
			win.draw(playermoney);
		}
		win.draw(text);
		if (level != 4)
		{
			win.draw(rect);
			textbox1->drawTo(win);
		}
		if (level == 4)
		{
			b1.settext("accept offer");
			b1.render(&win);
			sf::Vector2i mpos = mos.getPosition();
			b1.update(sf::Vector2f(mpos.x, mpos.y), win);
		}
		b3.render(&win);
		sf::Vector2i mpos = mos.getPosition();
		b3.update(sf::Vector2f(mpos.x, mpos.y), win);
		for (int i = 0; i < no_players; i++)
		{
			if (list_of_players[i]->player_status != "out")
			    list_of_characters[i]->Draw(win);
		}
		win.display();
	}
	delete textbox1;
}


void game::eliminate_player(int no)
{
	this->list_of_players[no]->player_status = "out";
	this->list_of_players[no]->disown_any_group_property();
	this->list_of_players[no]->player_money = 0;
}


string game::mychance()
{
	srand(time(NULL));
	string list[3] = {"get_money","free_jail","advance"};
	int v2 = rand() % 3;
	return list[v2];
}


void game::landedonsomeproperty(int p_n, sf::RenderWindow& win, sf::Sprite sprite2, sf::RectangleShape list[], sf::Text listtext[], sf::Text monoplytext, sf::RectangleShape diceinfo, sf::Text dicetext, sf::RectangleShape infobox, sf::Text playertext, sf::Text playermoney)
{

	string text2 = "X";

	sf::Font* font1 = new sf::Font();
	sf::Font font2;
	font1->loadFromFile("neuropol.ttf");
	font2.loadFromFile("neuropol.ttf");
	int position = list_of_players[p_n - 1]->pos;
	int property_n=0,g_n = 0;
	sf::Mouse mos;

	sf::RectangleShape choicebox;
	choicebox.setFillColor(sf::Color(0, 153, 153));
	choicebox.setOutlineThickness(1);
    choicebox.setOutlineColor(sf::Color(96, 96, 96));
	choicebox.setSize(sf::Vector2f(300,100));
	choicebox.setPosition(sf::Vector2f(50,550));


	sf::RectangleShape box;
	box.setFillColor(sf::Color(255, 102, 102));
	box.setOutlineThickness(1);
	box.setOutlineColor(sf::Color(96, 96, 96));
	box.setSize(sf::Vector2f(300, 100));
	box.setPosition(sf::Vector2f(50, 450));

	sf::Text whereyouat;
	sf::Font wherefont;
	wherefont.loadFromFile("INVASION2000.ttf");
	whereyouat.setCharacterSize(20);
	whereyouat.setFont(wherefont);

	if (true)
	{
		string what = "no";
		if (position >= 2 && position <= 4)
			g_n = 0;
		else if (position >= 6 && position <= 8)
			g_n = 1;
		else if (position >= 11 && position <= 13)
			g_n = 2;
		else if (position >= 15 && position <= 17)
			g_n = 3;
		else if (position >= 18 && position <= 20)
			g_n = 4;
		else if (position >= 22 && position <= 24)
			g_n = 5;
		else if (position >= 26 && position <= 28)
			g_n = 6;
		else if (position >= 30 && position <= 32)
			g_n = 7;
		else if (position >= 35 && position <= 36)
			g_n = 8;
		else if (position >= 37 && position <= 38)
			g_n = 9;
		else if (position == 5 || position == 25 || position == 40)
			what = "chance";
		else if (position == 9 || position == 29)
			what = "community chest";
		else if (position == 10)
			what = "in jail";
		else if (position == 14 || position == 33 || position == 39)
			what = "pay";
		else if (position == 34)
			what = "go to jail";
		else if (position == 21)
			what = "free parking";
		else if (position == 1)
			what = "collect";

		string text = "Buy", text1 = "pass";

		if (what == "no")
		{
			for (int i = 0; i < list_of_groups[g_n]->no_properties; i++)
			{
				if (list_of_groups[g_n]->list_of_properties[i]->pos == position)
				{
					property_n = i;
					break;
				}
			}

			if (list_of_groups[g_n]->list_of_properties[property_n]->owned)
			{
				text = "pay\nrent"; text1 = "sell&\npay";
			}

			whereyouat.setString("you have landed\non "+ list_of_groups[g_n]->list_of_properties[property_n]->name);

		}
		else if (what == "chance")
		{
			text = "chance";
			whereyouat.setString("you have landed\non chance");
		}
		else if (what == "community chest")
		{
			text = "open\nbox";
			whereyouat.setString("you have landed\non community chest");
		}
		else if (what == "pay")
		{
			text = "pay";
			whereyouat.setString("you have landed\non tax zone");
		}
		else if (what == "go to jail")
		{
			text = "go\nto\njail";
			whereyouat.setString("bad luck hahahaha");
		}
		else if (what == "collect")
		{
			text = "collect";
			whereyouat.setString("you have completed\none round\nyou can collect money\nfrom bank now");
		}
		string what11 = "";
		whereyouat.setPosition(sf::Vector2f(50, 450));

		button b1(80, 570, 30, 30, font1, font2, text, sf::Color(128, 255, 0), sf::Color(224, 224, 224), sf::Color::Transparent, 19910);
		button b2(170, 570, 30, 30, font1, font2, text1, sf::Color(255, 255, 51), sf::Color(224, 224, 224), sf::Color::Transparent, 19910);
		button b3(270, 570, 30, 30, font1, font2, text2, sf::Color(255, 51, 51), sf::Color(224, 224, 224), sf::Color::Transparent, 19910);

		bool flag = true;

		if ((position >= 2 && position <= 4) || (position >= 6 && position <= 8) || (position >= 11 && position <= 13)
			|| (position >= 15 && position <= 17) || (position >= 18 && position <= 20)
			|| (position >= 22 && position <= 24) || (position >= 26 && position <= 28) || (position >= 30 && position <= 32)
			|| (position >= 35 && position <= 36) || (position >= 37 && position <= 38))
		{
			if (list_of_players[p_n - 1]->check_if_this_player_owns_property(list_of_groups[g_n]->list_of_properties[property_n]))
			{
				what = "owned";
				text = "exchange";
				b1.settext(text);
				if (list_of_groups[g_n]->owned)
				{
					if (list_of_groups[g_n]->owned_by == p_n)
					{
						what11 = "gs"; text1 = "\nupdate";
						b2.settext(text1);
					}
				}
			}
		}
		if (position == 21)
			flag = false;

		if (position == 10)
		{
			if (list_of_players[p_n - 1]->jail_status == true)
			{
				text = "pay\nbail";
				b1.settext(text);
				text1 = "sell\nand\npay";
				b2.settext(text1);
				if (list_of_players[p_n - 1]->free_jail_pass == true)
					b3.settext("free\njail\npass");
			}
		}
		int inner_count = 0,rent_inner_count=0,landtax_inner_count=0,propertytax_inner_count=0,hotelrent_inner_count=0;
		bool flaglight = true;
		while (flag)
		{
			sf::Event event; 
			while (win.pollEvent(event))
			{
				if (flag)
				{
					if (b1.isPressed())
					{

						int check_bail = -1;
						cout << "b1 is pressed" << endl;
						if ((position >= 2 && position <= 4) || (position >= 6 && position <= 8) || (position >= 11 && position <= 13)
							|| (position >= 15 && position <= 17) || (position >= 18 && position <= 20)
							|| (position >= 22 && position <= 24) || (position >= 26 && position <= 28) || (position >= 30 && position <= 32)
							|| (position >= 35 && position <= 36) || (position >= 37 && position <= 38))
						{
							if (what == "no")
							{
								cout << " up here " << endl;
								if (list_of_groups[g_n]->list_of_properties[property_n]->owned)
								{
									if (list_of_players[p_n - 1]->check_if_can_pay_off(list_of_groups[g_n]->list_of_properties[property_n]->rent))
									{
										if (flag && rent_inner_count == 0)
										{
											rent_inner_count++;
											cout << "here yo " << endl;
											bool rentflag = list_of_players[p_n - 1]->pay_rent(list_of_groups[g_n]->list_of_properties[property_n]);
											cout << "owned by " << list_of_groups[g_n]->list_of_properties[property_n]->owned_by_player << endl;
											if (rentflag)
											{
												flag = false;
												list_of_players[list_of_groups[g_n]->list_of_properties[property_n]->owned_by_player - 1]->player_money += list_of_groups[g_n]->list_of_properties[property_n]->rent;
											}
											else
											{
												text1 = "sell\nand\npay"; b2.settext(text1);
												string mm = "player " + to_string(p_n) + " sell some of your property to pay off rent";
												messagebox message(500, 500, mm);
											}
										}
									}
									else
									{
										if (flag)
										{
											flag = false;
											this->eliminate_player(p_n - 1);

											sf::SoundBuffer buffer;
											buffer.loadFromFile("nani.wav");
											sf::Sound sound;
											sound.setBuffer(buffer);
											sound.setVolume(500);
											sound.setPitch(1);
											sound.play();

											/*//sf::SoundBuffer buffer;
											buffer.loadFromFile("animefuckyou.wav");
											//sf::Sound sound;
											sound.setBuffer(buffer);
											sound.setVolume(500);
											sound.setPitch(1);
											sound.play();*/



											string mm = "player " + to_string(p_n) + " has been eliminated\nfor not been able to pay off";
											messagebox message(500, 500, mm);
										}
									}
								}
								else
								{
									if (flag)
									{
										flag = false;
										cout << "property: " << list_of_groups[g_n]->list_of_properties[property_n]->name << endl;
										cout << "property price : " << list_of_groups[g_n]->list_of_properties[property_n]->price << endl;
										int ccx = list_of_players[p_n - 1]->buy_property(list_of_groups[g_n]->list_of_properties[property_n]);
										if (ccx)
										{
											sf::SoundBuffer buffer;
											buffer.loadFromFile("goodjob.wav");
											sf::Sound sound;
											sound.setBuffer(buffer);
											sound.setVolume(500);
											sound.setPitch(1);
											sound.play();


											list_of_groups[g_n]->list_of_properties[property_n]->setowned(p_n);
											list[list_of_groups[g_n]->list_of_properties[property_n]->pos - 1].setFillColor(list_of_players[p_n - 1]->getcolor());
											if (list_of_groups[g_n]->check_if_group_owned(p_n))
											{
												list_of_players[p_n - 1]->own_group(g_n, list_of_groups[g_n]);
											}
										}
									}
								}
							}
							else if (what == "owned")
							{
								if (flag)
								{
									flag = false;
									cout << "exchanged pressed" << endl; int cxxd = -1; int ggx = -1;
									this->exchange(p_n, win, sprite2, list, listtext, monoplytext, diceinfo, dicetext, infobox, playertext, playermoney, b3, b1, cxxd, ggx);
									if (cxxd != -1 && ggx != -1)
									{
										if (list_of_groups[g_n]->check_if_group_owned(cxxd))
										{
											list_of_players[cxxd - 1]->own_group(g_n, list_of_groups[g_n]);
										}
										if (list_of_groups[ggx]->check_if_group_owned(p_n))
										{
											list_of_players[p_n - 1]->own_group(ggx, list_of_groups[ggx]);
										}
										cout << "p_n: " << p_n << ", cxxd: " << cxxd << endl;
										cout << "g_n: " << g_n << ", ggx: " << ggx << endl;
										list_of_players[p_n - 1]->clear_group(list_of_groups[g_n]);
										list_of_players[cxxd - 1]->clear_group(list_of_groups[ggx]);
									}
								}
							}
						}
						else if (position == 14)
						{
							if (list_of_players[p_n - 1]->check_if_can_pay_off(list_of_players[p_n - 1]->compute_landtax()))
							{
								if (flag && landtax_inner_count == 0)
								{
									landtax_inner_count++;
									bool landtaxflag = list_of_players[p_n - 1]->pay_landtax();
									if (landtaxflag)
									{
										flag = false;
									}
									else
									{
										text1 = "sell\nand\npay"; b2.settext(text1);
										string mm = "player " + to_string(p_n) + " sell some of your property to pay off landtax";
										messagebox message(500, 500, mm);
									}
								}
							}
							else
							{

								if (flag)
								{
									flag = false;
									this->eliminate_player(p_n - 1);
									string mm = "player " + to_string(p_n) + " has been eliminated\nfor not been able to pay off";
									messagebox message(500, 500, mm);
								}
							}
						}
						else if (position == 33)
						{
							if (list_of_players[p_n - 1]->check_if_can_pay_off(list_of_players[p_n - 1]->compute_propertytax()))
							{
								if (flag && propertytax_inner_count == 0)
								{
									propertytax_inner_count++;
									bool propertytaxflag = list_of_players[p_n - 1]->pay_propertytax();
									if (propertytaxflag)
									{
										flag = false;
									}
									else
									{
										text1 = "sell\nand\npay"; b2.settext(text1);
										string mm = "player " + to_string(p_n) + " sell some of your property to pay off propertytax";
										messagebox message(500, 500, mm);
									}
								}
							}
							else
							{
								if (flag)
								{
									flag = false;
									this->eliminate_player(p_n - 1);
									string mm = "player " + to_string(p_n) + " has been eliminated\nfor not been able to pay off";
									messagebox message(500, 500, mm);
								}
							}
						}
						else if (position == 39)
						{
							if (list_of_players[p_n - 1]->check_if_can_pay_off(200))
							{
								if (flag && hotelrent_inner_count == 0)
								{
									hotelrent_inner_count++;
									bool hotelrentflag = list_of_players[p_n - 1]->pay_hotelrent();
									if (hotelrentflag)
									{
										flag = false;
									}
									else
									{
										text1 = "sell\nand\npay"; b2.settext(text1);
										string mm = "player " + to_string(p_n) + " sell some of your property to pay off hotel rent";
										messagebox message(500, 500, mm);
									}
								}
							}
							else
							{
								if (flag)
								{
									flag = false;
									this->eliminate_player(p_n - 1);
									string mm = "player " + to_string(p_n) + " has been eliminated\nfor not been able to pay off";
									messagebox message(500, 500, mm);
								}
							}
						}
						else if (position == 10)
						{
							if (flag)
								check_bail = list_of_players[p_n - 1]->pay_bail();
						}
						else if (position == 1)
						{
						    list_of_players[p_n - 1]->player_money += 500;
                        }
						else if (position == 5 || position == 25 || position == 40)
						{
						     if(flag)
						     {
								 flag = false;
								 string chance_choice = this->mychance();
								 if (chance_choice == "advance")
								 {
									 int ad_chance = rand() % 40;
									 ad_chance++;
									 string mm = "player " + to_string(p_n) + " advance to block "+to_string(ad_chance);
									 messagebox message(500, 500, mm);
									 while (list_of_players[p_n - 1]->pos != ad_chance)
									 {
										 this->takemove(p_n, 1, win, sprite2, list, listtext, monoplytext, diceinfo, dicetext, infobox, playertext, playermoney);
									 }
								 }
								 else if (chance_choice == "free_jail")
								 {
									 string mm = "player " + to_string(p_n) + " recieved a free fail pass";
									 messagebox message(500, 500, mm);
									 list_of_players[p_n - 1]->free_jail_pass = true;
								 }
								 else if (chance_choice == "get_money")
								 {
									 int mon[5] = { 0,500,1000,2000,5000 };
									 int chance_money = mon[rand() % 5];
									 list_of_players[p_n - 1]->player_money += chance_money;
									 string mm = "player " + to_string(p_n) + " has received "+to_string(chance_money)+"\n from chance";
									 messagebox message(500, 500, mm);
								 }
							 }
                        }
						else if (position == 9 || position == 29)
						{
							if (flag)
							{
								flag = false;
								int mon[5] = { 0,500,1000,2000,5000 };
								int chance_money = mon[rand() % 5];
								list_of_players[p_n - 1]->player_money += chance_money;
								string mm = "player " + to_string(p_n) + " has received " + to_string(chance_money) + "\n from community chest";
								messagebox message(500, 500, mm);
							}
                        }
						
						/*if(position!=10)
							flag = false;*/



						if (position == 5 || position == 25 || position == 40 || position == 1 || position == 9 || position == 29)
							flag = false;
						if (position == 10)
						{
							if (check_bail == 0)
							{
								if (!list_of_players[p_n - 1]->check_if_can_pay_off(500))
								{
									if (flag)
									{

										flag = false;
										this->eliminate_player(p_n - 1);
										string mm = "player " + to_string(p_n) + " has been eliminated\nfor not been able to pay off";
										messagebox message(500, 500, mm);
									}
								}
								else
								{
									if (flag && inner_count == 0)
									{
										inner_count++;
										text1 = "sell\nand\npay"; b2.settext(text1);
										string mm = "player " + to_string(p_n) + " sell some of your property to pay bail";
										messagebox message(500, 500, mm);
									}
								}
							}
							else
							{
								if (flag)
								{
									flag = false;
									string mm = "player " + to_string(p_n) + " has been released from bail";
									messagebox message(500, 500, mm);
								}
							}
						}

					}
				}
				if (flag)
				{
					if (b2.isPressed())
					{
						//cout << "debt pressed" << endl;
						if (what11 == "gs")
							cout << "update pressed" << endl;
						if (inner_count > 0 || rent_inner_count > 0 || propertytax_inner_count > 0 || landtax_inner_count > 0 || hotelrent_inner_count > 0)
						{
							flaglight = false;
							inner_count = propertytax_inner_count = landtax_inner_count = hotelrent_inner_count = rent_inner_count = 0;
							list_of_players[p_n - 1]->sell_to_payoff(this, win, sprite2, list, listtext, monoplytext, diceinfo, dicetext, infobox, playertext, playermoney, b3, b1);
						}

						if (flaglight)
							flag = false;
					}
				}
				if (flag)
				{
					if (b3.isPressed())
					{
						if (what != "owned")
						{
							if ((position >= 2 && position <= 4) || (position >= 6 && position <= 8) || (position >= 11 && position <= 13)
								|| (position >= 15 && position <= 17) || (position >= 18 && position <= 20)
								|| (position >= 22 && position <= 24) || (position >= 26 && position <= 28) || (position >= 30 && position <= 32)
								|| (position >= 35 && position <= 36) || (position >= 37 && position <= 38))
							{

								cout << "b3 is pressed" << endl;
								int wala = 0;
								int nnnnnnn = game::auction(win, sprite2, list, listtext, monoplytext, diceinfo, dicetext, infobox, playertext, playermoney, b3, wala);
								int cxx = list_of_players[nnnnnnn - 1]->buy_property(list_of_groups[g_n]->list_of_properties[property_n], wala);
								cout << "wala:" << wala << endl;
								if (cxx)
								{
									list_of_groups[g_n]->list_of_properties[property_n]->owned_by_player = nnnnnnn;
									list[list_of_groups[g_n]->list_of_properties[property_n]->pos - 1].setFillColor(list_of_players[nnnnnnn - 1]->getcolor());
									if (list_of_groups[g_n]->check_if_group_owned(nnnnnnn))
									{
										list_of_players[nnnnnnn - 1]->own_group(g_n, list_of_groups[g_n]);
									}
									cout << nnnnnnn << "wins the bidding" << endl;
								}
							}

							if (what == "in jail")
							{
								if (flag && list_of_players[p_n-1]->jail_status==true)
								{
									flag = false;
									list_of_players[p_n - 1]->jail_status = false;
									list_of_players[p_n - 1]->free_jail_pass = false;
									string mm = "player " + to_string(p_n) + " has been released with free jail card";
									messagebox message(500, 500, mm);
								}
							}
						}
						
						
						flag = false;
					}
				}
			}
			win.clear();
			win.draw(sprite2);
			for (int i = 0; i < 40; i++)
			{
				win.draw(list[i]);
				win.draw(listtext[i]);
			}
			win.draw(monoplytext);
			win.draw(diceinfo);
			win.draw(dicetext);
			win.draw(infobox);
			win.draw(playertext);
			win.draw(playermoney);
			win.draw(box);
			win.draw(whereyouat);
			for (int i = 0; i < no_players; i++)
			{
				if (list_of_players[i]->player_status != "out")
				    list_of_characters[i]->Draw(win);
			}
			win.draw(choicebox);
			sf::Vector2i mpos;
			if (!(list_of_groups[g_n]->list_of_properties[property_n]->mortgaged) && what=="no")
			{
				b1.render(&win);
				mpos = mos.getPosition();
				b1.update(sf::Vector2f(mpos.x, mpos.y),win);
				if (text1=="pass" || text1=="sell\nand\npay" || text1=="\nupdate")
				{
					b2.render(&win);
					mpos = mos.getPosition();
					b2.update(sf::Vector2f(mpos.x, mpos.y), win);
				}
			}
			if (what == "chance" || what=="community chest" || what=="pay" || what=="go to jail" || what=="collect" || what=="owned" || what11=="gs"
				||(position == 10 && list_of_players[p_n - 1]->jail_status == true))
			{
				b1.render(&win);
				mpos = mos.getPosition();
				b1.update(sf::Vector2f(mpos.x, mpos.y),win);
				if (what11 == "gs" || (position == 10 && list_of_players[p_n - 1]->jail_status == true) || 
					(inner_count > 0 || propertytax_inner_count > 0 || landtax_inner_count > 0 || hotelrent_inner_count > 0 || rent_inner_count>0) )
				{
					b2.render(&win);
					mpos = mos.getPosition();
					b2.update(sf::Vector2f(mpos.x, mpos.y), win);
				}
			}
			if (what == "in jail" || what=="free parking" || what=="owned")
			{
				if (!(position == 10 && list_of_players[p_n - 1]->jail_status == true))
				{
					b3.render(&win);
					mpos = mos.getPosition();
					b3.update(sf::Vector2f(mpos.x, mpos.y), win);
				}
				else
				{
					if (list_of_players[p_n - 1]->free_jail_pass == true)
					{
						b3.render(&win);
						mpos = mos.getPosition();
						b3.update(sf::Vector2f(mpos.x, mpos.y), win);
					}
				}
			}
			if (!(list_of_groups[g_n]->list_of_properties[property_n]->owned) && what=="no")
			{
				b3.render(&win);
				mpos = mos.getPosition();
				b3.update(sf::Vector2f(mpos.x, mpos.y),win);
			}
			win.display();
		}

	}
	/*else if (position == 14)
	{
		cout << "Land tax" << endl;
	}
	else if (position == 33)
	{
		cout << "property tax" << endl;
	}
	else if (position == 34)
	{
		cout << "go to jail" << endl;
	}
	else if (position == 39)
	{
		cout << "hotel" << endl;
	}*/
	delete font1;
}

game::~game()
{
	cout << "****************************************************" << endl;
	cout << "game destructor called" << endl;

	ofstream write;
	write.open("totalplayers.txt");
	write << this->no_players;
	write.close();
	for (int i = 0; i < no_players; i++)
	{
		delete list_of_players[i];
		delete list_of_characters[i];
	}
	delete[] list_of_players;
	delete[] list_of_characters;
	for (int i = 0; i < no_groups; i++)
	{
		delete list_of_groups[i];
	}
	delete[] list_of_groups;
	delete[] turns;

}
