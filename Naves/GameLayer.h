#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Projectile.h"
#include "Tile.h"


#include "Audio.h" // importar
#include "Space.h" // importar

#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String
#include <list>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);
	void gamePadToControls(SDL_Event event); // USO DE MOUSE
	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	void calculateScroll();
	// Elementos de interfaz
	SDL_GameController* gamePad1;
	Space* space;
	float scrollX;
	int mapWidth;
	list<Tile*> tiles;
	Player* player1;
	Background* background;
	bool controlContinue = false;
	bool controlShoot1 = false;
	int controlMoveY1 = 0;
	int controlMoveX1 = 0;
	list<Projectile*> projectiles;
};
