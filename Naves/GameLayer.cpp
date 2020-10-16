#include "GameLayer.h"

GameLayer::GameLayer(Game* game) : Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	init();
	gamePad1 = SDL_GameControllerOpen(0);
}

void GameLayer::init() {
	player1 = NULL;

	space = new Space(1);
	scrollX = 0;
	tiles.clear();

	background = new Background("res/fondo_2.png", WIDTH*0.5, HEIGHT*0.5, game);

	projectiles.clear();

	loadMap("res/0.txt");
}

void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) {
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en pixels
			// Por carácter (en cada línea)
			for (int j = 0; !streamLine.eof(); j++) {
				streamLine >> character; // Leer character 
				cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 32 + i * 32; // y central
				loadMapObject(character, x, y);
			}
			
			cout << character << endl;
		}
	}
	streamFile.close();
}


void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
	case '1': {
		if (player1 == NULL) {
			player1 = new Player(x, y, game);
			// modificación para empezar a contar desde el suelo.
			player1->y = player1->y - player1->height / 2;
			space->addDynamicActor(player1);
		}
		break;
	}
	case '#': {
		Tile* tile = new Tile("res/bloque_tierra.png", x, y, game);
		// modificación para empezar a contar desde el suelo.
		tile->y = tile->y - tile->height / 2;
		tiles.push_back(tile);
		space->addStaticActor(tile);
		break;
	}
	}
}


void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_CONTROLLERDEVICEADDED) {
			gamePad1 = SDL_GameControllerOpen(0);
			if (gamePad1 == NULL) {
				cout << "error en GamePad 1" << endl;
			}
			else {
				cout << "GamePad 1 conectado" << endl;
			}

		}
		// Cambio automático de input
		// PONER el GamePad
		if (event.type == SDL_CONTROLLERBUTTONDOWN || event.type == SDL_CONTROLLERAXISMOTION) { 
			game->input = game->inputGamePad;
		}
		if (event.type == SDL_KEYDOWN) {
			game->input = game->inputKeyboard;
		}
		// Procesar teclas
		if (game->input == game->inputGamePad) {  // gamePAD
			gamePadToControls(event);
		}
		if (game->input == game->inputKeyboard) {
			keysToControls(event);
		}
	}

	// Disparar
	if (controlShoot1) {
		Projectile* newProjectile = player1->shoot();
		if (newProjectile != NULL) {
			space->addDynamicActor(newProjectile);
			projectiles.push_back(newProjectile);
			controlShoot1 = false;
		}
	}

	// Eje X
	if (controlMoveX1 > 0) {
		player1->moveX(1);
	}
	else if (controlMoveX1 < 0) {
		player1->moveX(-1);
	}
	else {
		player1->moveX(0);
	}

	// Eje Y
	if (controlMoveY1 > 0) {

	}
	else if (controlMoveY1 < 0) {
		player1->jump();
	}
	else {

	}

}


void GameLayer::gamePadToControls(SDL_Event event) {
	// Leer los botones
	bool buttonA = SDL_GameControllerGetButton(gamePad1, SDL_CONTROLLER_BUTTON_A);
	bool buttonB = SDL_GameControllerGetButton(gamePad1, SDL_CONTROLLER_BUTTON_B);
	// SDL_CONTROLLER_BUTTON_A, SDL_CONTROLLER_BUTTON_B
	// SDL_CONTROLLER_BUTTON_X, SDL_CONTROLLER_BUTTON_Y
	cout << "botones:" << buttonA << "," << buttonB << endl;
	int stickX = SDL_GameControllerGetAxis(gamePad1, SDL_CONTROLLER_AXIS_LEFTX);
	cout << "stickX" << stickX << endl;

	// Retorna aproximadamente entre [-32800, 32800], el centro debería estar en 0
	// Si el mando tiene "holgura" el centro varia [-4000 , 4000]
	if (stickX > 4000) {
		controlMoveX1 = 1;
	}else if (stickX < -4000) {
		controlMoveX1 = -1;
	} else {
		controlMoveX1 = 0;
	}

	if (buttonA) {
		controlShoot1 = true;
	} else {
		controlShoot1 = false;
	}

	if (buttonB) {
		controlMoveY1 = -1; // Saltar
	} else {
		controlMoveY1 = 0;
	}
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE: // derecha
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX1 = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX1 = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY1 = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY1 = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot1 = true;
			break;
		}
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX1 == 1) {
				controlMoveX1 = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX1 == -1) {
				controlMoveX1 = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY1 == -1) {
				controlMoveY1 = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY1 == 1) {
				controlMoveY1 = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot1 = false;
			break;
		}
	}
}


void GameLayer::update() {
	// Jugador se cae
	if (player1->y > HEIGHT + 80 ) {
		init();
	}

	space->update();
	
	player1->update();

	for (auto const& projectile : projectiles) {
		projectile->update();
	}

	// Colisiones , Enemy - Projectile
	list<Projectile*> deleteProjectiles;

	for (auto const& projectile : projectiles) {
		if (projectile->isInRender(scrollX) == false || projectile->vx == 0) {
			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}


	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);  // ELIMINAR ELIMINAR
	}
	deleteProjectiles.clear();

}


void GameLayer::draw() {

	background->draw();
	for (auto const& tile : tiles) {
		tile->draw(scrollX);
	}
	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX);
	}
	player1->draw(scrollX);

	SDL_RenderPresent(game->renderer); // Renderiza
}
