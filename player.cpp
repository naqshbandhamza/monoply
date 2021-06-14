#include "player.h"
#include "button.h"
#include "messagebox.h"
#include "game.h"
#include "Textbox.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;


/*bool check_if_string_is_number(string temporary)
{
	for (int i = 0; i < temporary.length(); i++)
	{
		if (temporary[i] - '0' < 0 || temporary[i] - '0' > 9)
			return false;
	}
	return true;
}*/

player::player(int player_number)
{
	no_of_groups = 0;
	no_of_properties = 0;
	prev_pos = 1;
	player_num = player_number;
	if (player_num == 1)
		color = sf::Color(255, 178, 102);//light oranges
	else if (player_num == 2)
		color = sf::Color(153, 204, 255);//light blue
	else if (player_num == 3)
		color = sf::Color(102, 255, 102);//vibrant green
	else if (player_num == 4)
		color = sf::Color(255, 153, 255);//pink
	else if (player_num == 5)
		color = sf::Color(255, 255, 102);//medium yellow
	else if (player_num == 6)
		color = sf::Color(204, 153, 255);//light purple
	else if (player_num == 7)
		color = sf::Color(153, 0, 76);//mehroon
	else if (player_num == 8)
		color = sf::Color(153, 76, 0);//brown
	pos = 1; 
	
	player_money = 7000;
	player_status = "current";
	position = new char[15];
	jail_status = false;
	free_jail_pass = false;
	list_of_properties = NULL;
	list_of_groups = NULL;
}

std::string player::getnameofpropertyatpos(int pos)
{
	for (int i = 0; i < this->no_of_properties; i++)
	{
		if (this->list_of_properties[i]->pos == pos)
			return list_of_properties[i]->name;
	}
	return "none";
}

int player::getpriceofpropertyatpos(int pos)
{
	int price = 0;
	for (int i = 0; i < this->no_of_properties; i++)
	{
		if (this->list_of_properties[i]->pos == pos)
		{
			price = list_of_properties[i]->price;
			break;
		}
	}
	return price;
}

int player::getindexofpropertyatpos(int pos)
{
	int index = 0;
	for (int i = 0; i < this->no_of_properties; i++)
	{
		if (this->list_of_properties[i]->pos == pos)
		{
			index = i;
			break;
		}
	}
	return index;
}

property* player::getpropertyatpos(int pos)
{
	for (int i = 0; i < this->no_of_properties; i++)
	{
		if (this->list_of_properties[i]->pos == pos)
		{
			return list_of_properties[i];
		}
	}
	return 0;
}

int player::get_n_properties()
{
	return this->no_of_properties;
}

sf::Color& player::getcolor() {
	return color;
}

bool player::buy_property(property* ptr,int next)
{


	


	property** temp;

	int pprice = 0;

	if (next == 0)
		pprice = ptr->price;
	else
		pprice = next;

	if (player_money - pprice >= 0)
	{
		no_of_properties++;
		if (list_of_properties == NULL)
		{
			list_of_properties = new property * [no_of_properties];
			list_of_properties[0] = ptr;
		}
		else
		{
			temp = new property * [no_of_properties];
			for (int i = 0; i < no_of_properties - 1; i++)
			{
				temp[i] = list_of_properties[i];
			}
			temp[no_of_properties - 1] = ptr;
			delete[] list_of_properties;
			list_of_properties = temp;
		}
		ptr->owned = true;
		player_money -= pprice;

		

		return true;
	}
	return false;
}

bool player::pay_bail()
{
	if (this->player_money - 500 >= 0)
	{
		player_money -= 500;
		this->jail_status = false;
		return true;
	}
	return false;
}



void player::disown_any_group_property()
{

}


bool player::check_if_can_pay_off(int money)
{
	if (money == 0)
		return true;

	int m = this->player_money;
	if (no_of_properties > 0)
	{
		for (int i = 0; i < no_of_properties; i++)
		{
			m += this->list_of_properties[i]->price;
		}
	}

	if (no_of_properties > 0)
		return true;
	else if (player_money >= money)
		return true;
	else
		return false;

	/*if (m >= money)
		return true;
	else
		return false;*/
}

void player::clear_group(group* g)
{
	bool checkyo = false;
	for (int i = 0; i < this->no_of_groups; i++)
	{
		if (list_of_groups[i] == g)
		{
			checkyo = true; break;
		}
	}
	if (checkyo)
	{
		int newsize = -1;
		if (no_of_groups == 1)
			newsize = 0;
		else
			newsize = no_of_groups - 1;
		if (newsize > 0)
		{
			group** temp = new group * [newsize];
			for (int i = 0, index = 0; i < this->no_of_groups; i++)
			{
				if (list_of_groups[i] != g)
				{
					temp[index] = list_of_groups[i]; index++;
				}
			}
			g->owned = false;
			g->owned_by = -1;
			delete[]list_of_groups;
			list_of_groups = temp;
			this->no_of_groups--;
		}
		else
		{
			delete[]list_of_groups;
			list_of_groups = NULL;
			g->owned = false;
			g->owned_by = -1;
			this->no_of_groups--;
		}
	}
}

void player::own_group(int g_n,group*ptr)
{
	group** temp;

	this->no_of_groups++;
		if (this->list_of_groups == NULL)
		{
			list_of_groups = new group * [no_of_groups];
			list_of_groups[0] = ptr;
		}
		else
		{
			temp = new group * [no_of_groups];
			for (int i = 0; i < no_of_groups - 1; i++)
			{
				temp[i] = list_of_groups[i];
			}
			temp[no_of_groups - 1] = ptr;
			delete[] list_of_groups;
			list_of_groups = temp;
		}
		ptr->owned = true;
		ptr->owned_by = player_num;
}

bool player::check_if_this_player_owns_property(property* ptr)
{
	for (int i = 0; i < no_of_properties; i++)
	{
		if (*list_of_properties[i] == ptr)
		{
			return true;
		}
	}
	return false;
}

bool player::pay_rent(property* ptr)
{
	if (player_money - ptr->rent >= 0)
	{
		player_money -= ptr->rent;
		return true;
	}
	return false;
}

bool player::pay_landtax()
{
	cout << "you own " << no_of_properties;
	int total_tax = 0,i = 0;
	while (i < this->no_of_properties)
	{
		total_tax+=this->list_of_properties[i]->price * 20.0 / 100;
		i++;
	}
	cout << " and you have paid " << total_tax << " land tax" << endl;
	cout << "money before paying tax:" << player_money << endl;
	string mm = "you own " + to_string(no_of_properties) + " and your amount is " + to_string(total_tax);
	messagebox message(500, 500, mm);
	if (player_money - total_tax >= 0)
	{
		this->player_money -= total_tax;
		return true;
	}
	cout << "money after paying tax:" << player_money << endl;
	return false;
}

int player::compute_landtax()
{
	cout << "you own " << no_of_properties;
	int total_tax = 0, i = 0;
	while (i < this->no_of_properties)
	{
		total_tax += this->list_of_properties[i]->price * 20.0 / 100;
		i++;
	}
	return total_tax;
}

int player::compute_propertytax()
{
	int total_tax = 0, i = 0;
	while (i < this->no_of_properties)
	{
		total_tax += this->list_of_properties[i]->price * 10.0 / 100;
		if (this->list_of_properties[i]->build)
		{
			for (int j = 0; j < list_of_properties[i]->no_of_buildings; j++)
			{
				if (list_of_properties[i]->list_of_building[j] == "house")
					total_tax += this->list_of_properties[i]->price * 20.0 / 100;
				else if (list_of_properties[i]->list_of_building[j] == "shop" || list_of_properties[i]->list_of_building[j] == "hotel")
					total_tax += this->list_of_properties[i]->price * 30.0 / 100;
			}
		}
		i++;
	}
	return total_tax;
}

bool player::pay_propertytax()
{
	int total_tax = 0, i = 0;
	while (i < this->no_of_properties)
	{
		total_tax += this->list_of_properties[i]->price * 10.0 / 100;
		if (this->list_of_properties[i]->build)
		{
			for (int j = 0; j < list_of_properties[i]->no_of_buildings; j++)
			{
				if(list_of_properties[i]->list_of_building[j]=="house")
					total_tax+= this->list_of_properties[i]->price * 20.0 / 100;
				else if (list_of_properties[i]->list_of_building[j] == "shop" || list_of_properties[i]->list_of_building[j] == "hotel")
					total_tax += this->list_of_properties[i]->price * 30.0 / 100;
			}
		}
		i++;
	}
	string mm = "you own "+to_string(no_of_properties)+" and your amount is "+to_string(total_tax);
	messagebox message(500, 500, mm);
	cout << " and you have paid " << total_tax << " property tax" << endl;
	cout << "money before paying tax:" << player_money << endl;
	if (player_money - total_tax >= 0)
	{
		this->player_money -= total_tax;
		return true;
	}
	cout << "money after paying tax:" << player_money << endl;
	return false;
}

bool player::pay_hotelrent()
{
	if (player_money - 200 >= 0)
	{
		this->player_money -= 200;
		return true;
	}
	return false;
}

void player::read_data_from_file(game* ptr, sf::RectangleShape list[])
{
	ifstream read;
	string filename = "player" + to_string(this->player_num) + ".txt";
	read.open(filename); int count = 0; char temp; string textra = "";
	while (!read.eof())
	{
		read >> noskipws >> temp;
		if (temp != '\n' && !read.eof())
		{
			textra = textra + temp;
		}
		else
		{
			if (count == 0)
			{
				this->player_num = stoi(textra); textra = "";
				cout << "player number assigned" << endl;
			}
			else if (count == 1)
			{
				this->player_money = stoi(textra); textra = "";
				cout << "player money assigned" << endl;
			}
			else if (count == 2)
			{
				this->player_status = textra; textra = "";
				cout << "player status assigned" << endl;
			}
			else if (count == 3)
			{
				if (textra == "1")
					this->jail_status = true;
				else if (textra == "0")
					this->jail_status = false;
				cout << "player jail status assigned" << endl;
				textra = "";
			}
			else if (count == 4)
			{
				if (textra == "1")
					this->free_jail_pass = true;
				else if (textra == "0")
					this->free_jail_pass = false;
				cout << "4 assigned" << endl;
				textra = "";
			}
			else if (count == 5)
			{
				this->prev_pos = stoi(textra);
				textra = "";
				cout << "5 assigned" << endl;
			}
			else if (count == 6)
			{
				this->pos = stoi(textra);
				textra = "";
				cout << "6 assigned" << endl;
			}
			else if (count == 7)
			{
				int inner_count = 0;
				this->no_of_properties = stoi(textra); textra = "";
				if (this->no_of_properties > 0)
				{
					this->list_of_properties = new property * [this->no_of_properties];
					char forread; string forrr = "";
					while (inner_count < this->no_of_properties)
					{
						read >> noskipws >> forread;
						if (forread != '\n')
							forrr = forrr + forread;
						else
						{
							int g_n=0, p_n=0;
							for (int i = 0; i < ptr->no_groups; i++)
							{
								bool check = false;
								for (int j = 0; j < ptr->list_of_groups[i]->no_properties; j++)
								{
									if (ptr->list_of_groups[i]->list_of_properties[j]->name == forrr)
									{
										g_n = i; p_n = j;
										check = true; break;
									}
								}
								if (check)
									break;
							}
							this->list_of_properties[inner_count] = ptr->list_of_groups[g_n]->list_of_properties[p_n];
							this->list_of_properties[inner_count]->owned_by_player = this->player_num;
							this->list_of_properties[inner_count]->owned = true;
							list[this->list_of_properties[inner_count]->pos - 1].setFillColor(this->getcolor());
							inner_count++;
							forrr = "";
						}
					}
					cout << "7 assigned" << endl;
				}
				else
					list_of_properties = NULL;
			}
			else if (count == 8)
			{
				int inner_count = 0;
				this->no_of_groups = stoi(textra); textra = "";
				if (this->no_of_groups > 0)
				{
					this->list_of_groups = new group * [this->no_of_groups];
					char forread; string forrr = "";
					while (inner_count < this->no_of_groups)
					{
						read >> noskipws >> forread;
						if (forread != '\n' && !read.eof())
							forrr = forrr + forread;
						else
						{
							int g_n=0, p_n=0;
							for (int i = 0; i < ptr->no_groups; i++)
							{
								if (ptr->list_of_groups[i]->group_name == forrr)
								{
									g_n = i;
									break;
								}
							}
							this->list_of_groups[inner_count] = ptr->list_of_groups[g_n];
							this->list_of_groups[inner_count]->owned_by = this->player_num;
							this->list_of_groups[inner_count]->owned = true;
							inner_count++;
							forrr = "";
						}
					}
					cout << "8 assigned" << endl;
				}
				else
					list_of_groups = NULL;
			}
			count++;
		}
	}
}


player::~player()
{
	cout << "player destructor called" << endl;
	
	ofstream write;
	std::string filename = "player" + to_string(this->player_num) + ".txt";
	write.open(filename);
	write << this->player_num;
	write << '\n' << player_money;
	write << '\n' << player_status;
	write << '\n' << jail_status;
	write << '\n' << this->free_jail_pass;
	write << '\n' << this->prev_pos;
	write << '\n' << this->pos;
	write << '\n' << this->no_of_properties;
	for (int i = 0; i < this->no_of_properties; i++)
		write << '\n' << this->list_of_properties[i]->name;
	write << '\n' << this->no_of_groups;
	for (int i = 0; i < this->no_of_groups; i++)
		write << '\n' << this->list_of_groups[i]->group_name;
	write.close();

	delete[]position;
	
	if (no_of_properties > 0)
		delete[]list_of_properties;
}


void player::sell_property(property* ptr,game*gptr,sf::RectangleShape list[])
{
	cout << "here sell property" << endl;
	if (this->no_of_properties == 1)
	{
		if (list_of_properties[0] == ptr)
		{
			ptr->owned = false;
			ptr->owned_by_player = -1;
			this->no_of_properties--;
			delete[]list_of_properties;
			list_of_properties = NULL;
			list[ptr->pos - 1].setFillColor(sf::Color::Red);
			this->player_money += ptr->price;
		}
	}
	else if(this->no_of_properties > 1)
	{
		int s = this->no_of_properties - 1;
		property** temp = new property * [s];
		for (int i = 0,index = 0; i < this->no_of_properties; i++)
		{
			if (list_of_properties[i] != ptr)
			{
				
				temp[index] = this->list_of_properties[i];
				index++;
			}
			else
			{
				this->player_money += ptr->price;
				ptr->owned = false;
				ptr->owned_by_player = -1;
			}
		}
		this->no_of_properties--;
		this->clear_group(gptr->list_of_groups[ptr->owned_by_g_no]);
		delete []list_of_properties;
		list_of_properties = temp;
		list[ptr->pos - 1].setFillColor(sf::Color::Red);
	}
}


void player::sell_to_payoff(game*ptr,sf::RenderWindow& win, sf::Sprite sprite2, sf::RectangleShape list[], sf::Text listtext[], sf::Text monoplytext, sf::RectangleShape diceinfo, sf::Text dicetext, sf::RectangleShape infobox, sf::Text playertext, sf::Text playermoney, button b3, button b1)
{
	cout << "in sell to pay off" << endl;
	sf::Event event;
	string extra = "player " + to_string(this->player_num);
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
	bool flag = true;
	bool flagcontrol = true; int b_c = 1;
	int p_nn; int level = 1,inner_count = 0;
	while (flag)
	{
		extra = "PLAYER " + to_string(this->player_num) + " turn\n";

		extra += "\nselect property to sell";
		for (int i = 0; i < this->get_n_properties(); i++)
		{
			extra += "\n" + to_string(i + 1) + ")" + this->list_of_properties[i]->name + ",price:" + to_string(this->list_of_properties[i]->price);
		}
		text.setString(extra);
		text.setPosition({ 50,350 });

		string text1 = "s", temporary = "";

		//bool otherflag = false;

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

								p_nn = stoi(temporary);
								level = 2;
								inner_count = 0;
							}
						
					}
					if (level == 2)
					{
						if (b1.isPressed())
						{
							if (b_c == 0)
							{
								cout << "property saled" << endl;
								level = 1;
								if (inner_count == 0)
									this->sell_property(this->list_of_properties[p_nn - 1], ptr, list);
								inner_count++;
							}
							b_c++;
							flagcontrol = true;
						}
					}
					
					if (b3.isPressed())
					{
						if (b_c == 0)
						{
							cout << "b3 is pressed" << endl;
							if (level == 1)
								flag = false;
							else if (level == 2)
								level = 1;
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
		if (level == 1)
		{
			win.draw(rect);
			textbox1->drawTo(win);
		}
		if (level == 2)
		{
			b1.settext("sell\nproperty");
			b1.render(&win);
			sf::Vector2i mpos = mos.getPosition();
			b1.update(sf::Vector2f(mpos.x, mpos.y), win);
		}
		b3.render(&win);
		sf::Vector2i mpos = mos.getPosition();
		b3.update(sf::Vector2f(mpos.x, mpos.y), win);
		for (int i = 0; i < ptr->no_players; i++)
		{
			if (ptr->list_of_players[i]->player_status != "out")
				ptr->list_of_characters[i]->Draw(win);
		}
		win.display();
	}
}

