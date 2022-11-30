#pragma once

#include<map>
#include<string>
#include<sstream>
#include<bits/stdc++.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include"Player.h"
#include"plBullet.h"
#include"Chickens.h"
#include "ckBullets.h"

using namespace std;
#define SERV_PORT 5550
#define SERV_IP "127.0.0.1"
#define BUFF_SIZE 1024

class Game
{
private:
	//Socket
	
	int bytes_sent, bytes_received, sin_size;
	int client_sock;
	struct sockaddr_in server_addr;
	char buff[BUFF_SIZE];
	char server_response[256];

	//Window
	sf::RenderWindow* window;

	//Resources
	std::map<std::string, sf::Texture*> textures;
	std::map<std::string, sf::Texture*> textures2;
	std::vector<plBullet*> plBullets;
	std::vector<ckBullets*> ckBullet;

	//GUI
	sf::Font font;
	sf::Text pointText;

	sf::Text gameOverText;

	//World
	sf::Texture worldBackgroundTex;
	sf::Sprite worldBackground;

	//Systems
	unsigned points;

	//Player
	Player* player;
	//Checkens* chickens;

	//PlayerGUI
	sf::RectangleShape playerHpBar;
	sf::RectangleShape playerHpBarBack;

	//Enemies
	float spawnTimer;
	float spawnTimerMax;
	std::vector<Checkens *> chickens;

	float spawnTimer1;
	float spawnTimerMax1;
	//Private functions
	void initWindow();
	void initTextures();
	void initGUI();
	void initWorld();
	void initSystems();

	void initPlayer();
	void initCheckens();
	void initckBullets();

	//Socket connect
	int connectSocket();

public:
	Game();
	virtual ~Game();

	//Functions
	void run();

	void updatePollEvents();
	void updateInput();
	void updateGUI();
	void updateWorld();
	void updateCollision();
	void updateplBullets();
	void updateckBullet();
	void updateCheckens();
	void updateCombat();
	void update();

	void renderGUI();
	void renderWorld();
	void render();
};

