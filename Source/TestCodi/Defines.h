#pragma once
#define MidaMapa 6
#define RoomBaseWidth 100
#define RoomBaseHeigth 75

#define DiferenciaEnX 1045
#define DiferenciaEnY 1355


struct pos {
	int posX;
	int posY;
};

struct infoCaselles {
	int id;		//Guardem les id's de les caselles = rooms
	bool nord;
	bool sud;
	bool est;
	bool oest;
	bool hemEstat;
};