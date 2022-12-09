"# ChickenInvader" 
On linux sistem:
Step 1: add sfml to your local project where have main.cpp file
sudo apt-get install libsfml-dev

Step 2: build and run server:
g++ -o server server.cpp
./server

Step 3: build and run game:
g++ -o main main.cpp Chickens.cpp ckBullets.cpp Player.cpp plBullet.cpp Soc_connect.cpp Game.cpp -lsfml-graphics -lsfml-window -lsfml-system
./main