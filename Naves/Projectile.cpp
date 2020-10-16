#include "Projectile.h"

Projectile::Projectile(float x, float y, Game* game) :
	Actor("res/bloque_metal.png", x, y, 20, 20 ,game) {

	// lo que mide la imagen
	fileWidth = 40;
	fileHeight = 32;
	// lo que va a medir en el juego
	width = 20;
	height = 20;

	vx = 9;
	vy = -1; // La gravedad inicial es 1
}

void Projectile::update() {
	vy = vy - 1; // La gravedad suma 1 en cada frame
}