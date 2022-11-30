#include "Game.h"

//Private functions
void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "Swaglords of Space - Game 3", sf::Style::Close | sf::Style::Titlebar);
	this->window->setFramerateLimit(144);
	this->window->setVerticalSyncEnabled(false);
}

void Game::initTextures()
{
	this->textures["plBULLET"] = new sf::Texture();
	this->textures["plBULLET"]->loadFromFile("Textures/bullet.png");

	this->textures2["ckBULLET"] = new sf::Texture();
	this->textures2["ckBULLET"]->loadFromFile("Textures/egg.png");
}

void Game::initGUI()
{
	//Load font
	if (!this->font.loadFromFile("Fonts/PixellettersFull.ttf"))
		std::cout << "ERROR::GAME::Failed to load font" << "\n";

	//Init point text
	this->pointText.setPosition(700.f, 25.f);
	this->pointText.setFont(this->font);
	this->pointText.setCharacterSize(20);
	this->pointText.setFillColor(sf::Color::White);
	this->pointText.setString("test");

	this->gameOverText.setFont(this->font);
	this->gameOverText.setCharacterSize(60);
	this->gameOverText.setFillColor(sf::Color::Red);
	this->gameOverText.setString("Game Over!");
	this->gameOverText.setPosition(
		this->window->getSize().x / 2.f - this->gameOverText.getGlobalBounds().width / 2.f,
		this->window->getSize().y / 2.f - this->gameOverText.getGlobalBounds().height / 2.f);

	//Init player GUI
	this->playerHpBar.setSize(sf::Vector2f(300.f, 25.f));
	this->playerHpBar.setFillColor(sf::Color::Red);
	this->playerHpBar.setPosition(sf::Vector2f(20.f, 20.f));

	this->playerHpBarBack = this->playerHpBar;
	this->playerHpBarBack.setFillColor(sf::Color(25, 25, 25, 200));
}

void Game::initWorld()
{
	if (!this->worldBackgroundTex.loadFromFile("Textures/background1.jpg"))
	{
		std::cout << "ERROR::GAME::COULD NOT LOAD BACKGROUND TEXTURE" << "\n";
	}

	this->worldBackground.setTexture(this->worldBackgroundTex);
}

void Game::initSystems()
{
	this->points = 0;
}

void Game::initPlayer()
{
	this->player = new Player();
	this->player->setPosition((this->window->getSize().x - this->player->getBounds().width) / 2, (this->window->getSize().y - this->player->getBounds().height));
}

void Game::initCheckens()
{
	this->spawnTimerMax = 50.f;
	this->spawnTimer = this->spawnTimerMax;
}

void Game::initckBullets()
{
	this->spawnTimerMax = 50.f;
	this->spawnTimer1 = this->spawnTimerMax;
}

int Game::connectSocket()
{
	// Step 1: Construct a UDP socket
	if ((client_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		// Call socket() to create a socket
		perror("\nError: ");
		return 0;
	}

	// Step 2: Define the address of the server
	//bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERV_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERV_IP); // converse ip adderss from string to network ip (int)
	std::cout << "Server IP: " << SERV_IP << " - Port: d" << SERV_PORT << "\n";

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, SERV_IP, &server_addr.sin_addr) <= 0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(client_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		std::cout << "Connection Failed" <<"\n";
		return -1;
	}

	recv(client_sock, &server_response, sizeof(server_response), 0);

	std::cout << "Tu server: "<< server_response << "\n";
	char password[BUFF_SIZE] = "test send mes";
	while (1)
	{
		send(client_sock, password, sizeof(password), 0);

		recv(client_sock, &server_response, sizeof(server_response), 0);
		std::cout << "Tu server: "<< server_response << "\n";
	}
	return 0;
}

//Con/Des
Game::Game()
{
	this->initWindow();
	this->initTextures();
	this->initGUI();
	this->initWorld();
	this->initSystems();

	this->initPlayer();
	this->initCheckens();
	this->connectSocket();
}

Game::~Game()
{
	delete this->window;
	delete this->player;

	//Delete textures
	for (auto& i : this->textures)
	{
		delete i.second;
	}

	//Delete plBullets
	for (auto* i : this->plBullets)
	{
		delete i;
	}

	//Delete ckBullets
	for (auto* i : this->ckBullet)
	{
		delete i;
	}

	//Delete chickens
	for (auto* i : this->chickens)
	{
		delete i;
	}
}

//Functions
void Game::run()
{
	while (this->window->isOpen())
	{
		this->updatePollEvents();

		if (this->player->getHp() > 0)
			this->update();

		this->render();
	}
}

void Game::updatePollEvents()
{
	sf::Event e;
	while (this->window->pollEvent(e))
	{
		if (e.Event::type == sf::Event::Closed)
			this->window->close();
		if (e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape)
			this->window->close();
	}
}

void Game::updateInput()
{
	//Move player
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		this->player->move(-1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		this->player->move(1.f, 0.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		this->player->move(0.f, -1.f);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		this->player->move(0.f, 1.f);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->player->canAttack())
	{
		this->plBullets.push_back(
			new plBullet(
				this->textures["plBULLET"],
				this->player->getPos().x + this->player->getBounds().width / 2.f,
				this->player->getPos().y,0.f,-1.f,5.f)
		);
	}
}

void Game::updateGUI()
{
	std::stringstream ss;

	ss << "Points: " << this->points;

	this->pointText.setString(ss.str());

	//Update player GUI
	float hpPercent = static_cast<float>(this->player->getHp()) / this->player->getHpMax();
	this->playerHpBar.setSize(sf::Vector2f(300.f * hpPercent, this->playerHpBar.getSize().y));
}

void Game::updateWorld()
{

}

void Game::updateCollision()
{
	//Left world collision
	if (this->player->getBounds().left < 0.f)
	{
		this->player->setPosition(0.f, this->player->getBounds().top);
	}
	//Right world collison
	else if (this->player->getBounds().left + this->player->getBounds().width >= this->window->getSize().x)
	{
		this->player->setPosition(this->window->getSize().x - this->player->getBounds().width, this->player->getBounds().top);
	}

	//Top world collision
	if (this->player->getBounds().top < 0.f)
	{
		this->player->setPosition(this->player->getBounds().left, 0.f);
	}
	//Bottom world collision
	else if (this->player->getBounds().top + this->player->getBounds().height >= this->window->getSize().y)
	{
		this->player->setPosition(this->player->getBounds().left, this->window->getSize().y - this->player->getBounds().height);
	}
}

void Game::updateplBullets()
{
	unsigned counter = 0;
	for (auto* bullet : this->plBullets)
	{
		bullet->update();

		//Bullet culling (top of screen)
		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{
			//Delete bullet
			delete this->plBullets.at(counter);
			this->plBullets.erase(this->plBullets.begin() + counter);
		}

		++counter;
	}
}

void Game::updateckBullet()
{
	unsigned counter = 0;
	for (auto* bullet : this->ckBullet)
	{
		bullet->update();

		//Enemy player collision
		if (bullet->getBounds().intersects(this->player->getBounds()))
		{
		this->player->loseHp(this->ckBullet.at(counter)->getDamage());
		delete this->ckBullet.at(counter);
		this->ckBullet.erase(this->ckBullet.begin() + counter);
		}

		/*
		//Bullet culling (top of screen)
		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{
			//Delete bullet
			delete this->plBullets.at(counter);
			this->ckBullet.erase(this->ckBullet.begin() + counter);
		}
		*/
		++counter;
	}
}

void Game::updateCheckens()
{
	//Spawning
	this->spawnTimer += 0.5f;
	if (this->spawnTimer >= this->spawnTimerMax)
	{
		this->chickens.push_back(new Checkens(*this->window));
		this->spawnTimer = 0.f;
		
	}

	//Update
	unsigned counter = 0;
	for (auto* enemy : this->chickens)
	{
		enemy->update();

		//Bullet culling (top of screen)
		if (enemy->getBounds().top > this->window->getSize().y)
		{
			//Delete enemy
			delete this->chickens.at(counter);
			this->chickens.erase(this->chickens.begin() + counter);
		}
		

		//Spawning
		this->spawnTimer += 0.1f;
		if (this->spawnTimer >= this->spawnTimerMax)
		{
			this->ckBullet.push_back(
				new ckBullets(
					this->textures2["ckBULLET"],
					enemy->getPos().x + enemy->getBounds().width / 2.f - 8.f,
					enemy->getPos().y + enemy->getBounds().height / 2.f , 0.f, 1.f, 1.f)
			);
			this->spawnTimer = 0.f;

		}
		++counter;
	}
}

void Game::updateCombat()
{
	for (int i = 0; i < this->chickens.size(); ++i)
	{
		
		
		bool enemy_deleted = false;
		for (size_t k = 0; k < this->plBullets.size() && enemy_deleted == false; k++)
		{
			

			if (this->chickens[i]->getBounds().intersects(this->plBullets[k]->getBounds()))
			{
				this->points += this->chickens[i]->getPoints();

				delete this->chickens[i];
				this->chickens.erase(this->chickens.begin() + i);

				delete this->plBullets[k];
				this->plBullets.erase(this->plBullets.begin() + k);

				enemy_deleted = true;
			}
		}
	}
}

void Game::update()
{
	this->updateInput();

	this->player->update();

	this->updateCollision();

	this->updateplBullets();

	this->updateckBullet();

	this->updateCheckens();

	this->updateCombat();

	this->updateGUI();

	this->updateWorld();
}

void Game::renderGUI()
{
	this->window->draw(this->pointText);
	this->window->draw(this->playerHpBarBack);
	this->window->draw(this->playerHpBar);
}

void Game::renderWorld()
{
	this->window->draw(this->worldBackground);
}

void Game::render()
{
	this->window->clear();

	//Draw world
	this->renderWorld();

	//Draw all the stuffs
	this->player->render(*this->window);
	

	for (auto* bullet : this->plBullets)
	{
		bullet->render(this->window);
	}

	for (auto* ckBullet : this->ckBullet)
	{
		ckBullet->render(this->window);
	}

	for (auto* enemy : this->chickens)
	{
		enemy->render(this->window);
	}

	this->renderGUI();

	//Game over screen
	if (this->player->getHp() <= 0)
		this->window->draw(this->gameOverText);

	this->window->display();
}
