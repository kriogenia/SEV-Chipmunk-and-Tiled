
#include "Background.h"


Background::Background(string filename, float x, float y, float vx, Game* game)
	: Actor(filename, x, y, WIDTH, HEIGHT, game) {

	// lo que mide la imagen
	fileWidth = WIDTH;
	fileHeight = HEIGHT;
	// lo que va a medir en el juego
	width = WIDTH;
	height = HEIGHT;

	this->vx = vx;
	if (vx != 0) {
		backgroundAux = new Background(filename, x + WIDTH, y, game);
	}
}


void Background::update() {
	if (vx != 0) {
		x = x + vx;

		// se salio por la izquierda
		if (x + width / 2 < 0) {
			// vuelve a aparecer por la derecha
			x = WIDTH + width / 2;
		}
		// se salio por la derecha
		if (x - width / 2 > WIDTH) {
			// vuelve por la izquierda
			x = 0 - width / 2;
		}

	}
}

void Background::draw(float scrollX) {
	Actor::draw(scrollX); // llamar al metodo del hijo

	if (backgroundAux != NULL) {
		// zona sin cubrir por la izquierda
		if (x - width / 2 > 0) {
			// pintar aux por la izquierda
			backgroundAux->x = x - width;
		}
		// zona sin cubrir por la derecha
		if (x + width / 2 < WIDTH) {
			// pintar aux por la derecha
			backgroundAux->x = x + width;
		}
		backgroundAux->draw(scrollX);
	}

}

Background::Background(string filename, float x, float y, Game* game)
	: Actor(filename, x, y, WIDTH, HEIGHT, game) {

}
