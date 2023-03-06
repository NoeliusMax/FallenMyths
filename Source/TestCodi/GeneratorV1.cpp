// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratorV1.h"

// Sets default values
AGeneratorV1::AGeneratorV1() {

	_posInicial.posX = 0;
	_posInicial.posY = 0;

	_posSeguent.posX = 0;
	_posSeguent.posY = 0;

	_posActual.posX = 0;
	_posActual.posY = 0;

	_ultimaPos.posX = 0;
	_ultimaPos.posY = 0;

	_posFinal.posX = 0;
	_posFinal.posY = 0;

	_numCasellesFetes = 1;
	_valorAleatori = 0;

	for (int a = 0; a < MidaMapa; a++) {
		for (int b = 0; b < MidaMapa; b++) {

			_infoCasella[a][b].nord = true;
			_infoCasella[a][b].sud = true;
			_infoCasella[a][b].est = true;
			_infoCasella[a][b].oest = true;
			_infoCasella[a][b].hemEstat = false;
			_infoCasella[a][b].id = -1;

			//Fem que per defecte, cadascuna de les caselles no tingui cap direcció
			_mapaLogic[a][b] = 0;
		}
	}
	myWorld = GetWorld();
	referenciesOk = false;
	maxRooms = 22;
	minRooms = 17;
	midaMapa = MidaMapa;
	//spawnRoom(0, FVector2D{ 0,0 });
}

AGeneratorV1::~AGeneratorV1()
{
}

void AGeneratorV1::comprovacioPasEnrere() {

	if ((_infoCasella[_posActual.posX][_posActual.posY].nord == false) && (_infoCasella[_posActual.posX][_posActual.posY].sud == false) && (_infoCasella[_posActual.posX][_posActual.posY].est == false) && (_infoCasella[_posActual.posX][_posActual.posY].oest == false)) {
		_infoCasella[_posActual.posX][_posActual.posY].hemEstat = true;	//Marquem la posició com a feta servir

		if (_casellesFetes.size() > 0) {
			_posActual = _casellesFetes.back();
			_casellesFetes.pop_back();
		}
		else {
			_casellesFetes.push_back(_posInicial);
			_posActual = _casellesFetes.back();
		}
	}
}

void AGeneratorV1::clearData() {

	_posInicial.posX = 0;
	_posInicial.posY = 0;

	_posSeguent.posX = 0;
	_posSeguent.posY = 0;

	_posActual.posX = 0;
	_posActual.posY = 0;

	_ultimaPos.posX = 0;
	_ultimaPos.posY = 0;

	_posFinal.posX = 0;
	_posFinal.posY = 0;

	_numCasellesFetes = 1;
	_valorAleatori = 0;


	for (int a = 0; a < MidaMapa; a++) {
		for (int b = 0; b < MidaMapa; b++) {

			_infoCasella[a][b].nord = true;
			_infoCasella[a][b].sud = true;
			_infoCasella[a][b].est = true;
			_infoCasella[a][b].oest = true;
			_infoCasella[a][b].hemEstat = false;
			_infoCasella[a][b].id = -1;

			//Fem que per defecte, cadascuna de les caselles no tingui cap direcció
			_mapaLogic[a][b] = 0;
		}
	}
	myWorld = GetWorld();
	referenciesOk = false;
	maxRooms = 22;
	minRooms = 17;
	midaMapa = MidaMapa;

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("2.Variables netes")));
}

void AGeneratorV1::generateLevel()	//Generació de mapa logic i creació de mapa real
{
	srand((unsigned int)time(NULL));

	//1.Triem una casella inicial al atzar (millor si esta en un dels marges)
	_posInicial.posX = rand() % MidaMapa;	//Del 0 al 6
	_posInicial.posY = rand() % MidaMapa;
	//_numCasellesFetes += 1;	//Si fos 0, mai s'arribarien a fer les 16, ja que sempre anem una enderrera

	_posActual = _posInicial;
	_infoCasella[_posActual.posX][_posActual.posY].hemEstat = true;	
	
	int nRooms = minRooms + rand() % (maxRooms + 1);
	while (_numCasellesFetes < 15) {
		_valorAleatori = rand() % 4 + 1;

		switch (_valorAleatori) {

		case 1:		//Nord

			if ((_posActual.posY - 1 >= 0) && (_posActual.posY - 1 < MidaMapa)) {		//En teoria només caldira comprovar si la Y serà més petita que 0, però no costa res assegurar-se de que no sortira del mapa

				if (_infoCasella[_posActual.posX][_posActual.posY].nord == true) {	//Si la direcció és valida

					//Ens guardem la posició de la casella a on aniriem (més facilitat a l'hora de manipular)
					_posSeguent.posX = _posActual.posX;
					_posSeguent.posY = _posActual.posY - 1;


					//Hem de mirar si ha hem estat (fem servir un array, pero també podriem fer servir el vector de les posicions i anar comprovant una per una)

					if (_infoCasella[_posSeguent.posX][_posSeguent.posY].hemEstat == false) {	//Si es compleix amb la condicio, vol dir que podem canviar de casella

						//Si no hi hem estat li hem d'assignar la direccio a la nostre casella i la direccio corresponent a la següent, tot i que després li afegirem una altra

						//Direccio actual
						_mapaLogic[_posActual.posX][_posActual.posY] += 1;	//Nord

						//Direccio de la seguent casella (hem de conectar els camins)
						_mapaLogic[_posSeguent.posX][_posSeguent.posY] += 2;	//Sud

						_numCasellesFetes += 1;
						_casellesFetes.push_back(_posActual);

						_infoCasella[_posActual.posX][_posActual.posY].hemEstat = true;	//Marquem la posició com a feta servir
						_infoCasella[_posActual.posX][_posActual.posY].nord = false; //Marquem la direccio com a feta servir, per si despres tornem a passar per la mateixa casella, no fer el mateix recorregut
						_infoCasella[_posSeguent.posX][_posSeguent.posY].sud = false; //Marquem la direccio com a feta servir, per si despres tornem a passar per la mateixa casella, no fer el mateix recorregut
						_posActual = _posSeguent;	//Canviem la nostra posició actual
						break;
					}
					else {
						//Ja hem estat, per tant la direccio no és valida
						_infoCasella[_posActual.posX][_posActual.posY].nord = false;
					}
				}
			}
			else {
				//Esta fora del mapa, per tant la direccio no és valida
				_infoCasella[_posActual.posX][_posActual.posY].nord = false;
			}

			//Si cap direcció funciona, tirem a la casella que estavem abans
			comprovacioPasEnrere();
			break;

		case 2:		//Sud

			if ((_posActual.posY + 1 >= 0) && (_posActual.posY + 1 < MidaMapa)) {	//Si esta dintre dels limits

				if (_infoCasella[_posActual.posX][_posActual.posY].sud == true) {//Si la direcció és valida
					_posSeguent.posX = _posActual.posX;
					_posSeguent.posY = _posActual.posY + 1;

					if (_infoCasella[_posSeguent.posX][_posSeguent.posY].hemEstat == false) {	//Si es compleix amb la condicio, vol dir que podem canviar de casella

						_mapaLogic[_posActual.posX][_posActual.posY] += 2;	//Sud
						_mapaLogic[_posSeguent.posX][_posSeguent.posY] += 1;	//Nord
						_numCasellesFetes += 1;
						_casellesFetes.push_back(_posActual);
						_infoCasella[_posActual.posX][_posActual.posY].hemEstat = true;
						_infoCasella[_posActual.posX][_posActual.posY].sud = false;
						_infoCasella[_posSeguent.posX][_posSeguent.posY].nord = false;
						_posActual = _posSeguent;
						break;
					}
					else {
						_infoCasella[_posActual.posX][_posActual.posY].sud = false;
					}
				}
			}
			else {
				_infoCasella[_posActual.posX][_posActual.posY].sud = false;
			}
			comprovacioPasEnrere();
			break;

		case 3:		//Est

			if ((_posActual.posX + 1 >= 0) && (_posActual.posX + 1 < MidaMapa)) {		//Si esta dintre dels limits

				if (_infoCasella[_posActual.posX][_posActual.posY].est == true) {	//Si la direcció és valida
					_posSeguent.posX = _posActual.posX + 1;
					_posSeguent.posY = _posActual.posY;

					if (_infoCasella[_posSeguent.posX][_posSeguent.posY].hemEstat == false) {	//Si es compleix amb la condicio, vol dir que podem canviar de casella

						_mapaLogic[_posActual.posX][_posActual.posY] += 4;	//Est
						_mapaLogic[_posSeguent.posX][_posSeguent.posY] += 8;	//Oest
						_numCasellesFetes += 1;
						_casellesFetes.push_back(_posActual);
						_infoCasella[_posActual.posX][_posActual.posY].hemEstat = true;	//Marquem la posició com a feta servir
						_infoCasella[_posActual.posX][_posActual.posY].est = false; //Marquem la direccio com a feta servir
						_infoCasella[_posSeguent.posX][_posSeguent.posY].oest = false; //Marquem la direccio com a feta servir
						_posActual = _posSeguent;	//Canviem la nostra posició actual
						break;
					}
					else {
						_infoCasella[_posActual.posX][_posActual.posY].est = false;
					}
				}
			}
			else {
				_infoCasella[_posActual.posX][_posActual.posY].est = false;
			}
			comprovacioPasEnrere();
			break;

		case 4:		//Oest

			if ((_posActual.posX - 1 >= 0) && (_posActual.posX - 1 < MidaMapa)) {	//Si esta dintre del mapa

				if (_infoCasella[_posActual.posX][_posActual.posY].oest == true) {	//Si la direcció és valida

					_posSeguent.posX = _posActual.posX - 1;
					_posSeguent.posY = _posActual.posY;

					if (_infoCasella[_posSeguent.posX][_posSeguent.posY].hemEstat == false) {	//Si es compleix amb la condicio, vol dir que podem canviar de casella

						_mapaLogic[_posActual.posX][_posActual.posY] += 8;	//Oest
						_mapaLogic[_posSeguent.posX][_posSeguent.posY] += 4;	//Est
						_numCasellesFetes += 1;
						_casellesFetes.push_back(_posActual);
						_infoCasella[_posActual.posX][_posActual.posY].hemEstat = true;	//Marquem la posició com a feta servir
						_infoCasella[_posActual.posX][_posActual.posY].oest = false; //Marquem la direccio com a feta servir
						_infoCasella[_posSeguent.posX][_posSeguent.posY].est = false; //Marquem la direccio com a feta servir
						_posActual = _posSeguent;	//Canviem la nostra posició actual
						break;
					}
					else {
						//Ja hem estat, per tant la direccio no és valida
						_infoCasella[_posActual.posX][_posActual.posY].oest = false;
					}
				}
			}
			else {
				_infoCasella[_posActual.posX][_posActual.posY].oest = false;
			}
			comprovacioPasEnrere();
			break;

		default:
			break;

		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("3.Mapa logic creat")));
}

void AGeneratorV1::setRoomsID()
{
	
	for (int x = 0; x < MidaMapa; x++) {
		for (int y = 0; y < MidaMapa; y++) {
			if (_infoCasella[x][y].id == -1 && _mapaLogic[x][y] != 0) {
				_infoCasella[x][y].id = rand() % 18 + 3; //Del 3 al 12
			}
		}
	}	
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("4.Room ID's fetes~~~~")));
}

void AGeneratorV1::setSpecialRooms() {
	//Aquesta funcio genera les sales especials, Inici, Loot, Boss
	//I les col·loca en sales buides (sense portes)
	//També conecta aquestes sales noves amb les que té al costat

	//Bucle per nº de sales especials

	//Busquem una casella buida, que estigui al costat d'una amb portes i no estiguin conectades

	int salesFetes = 0;

	//int nRooms = arrayRooms.size();
	int x;
	int y;
	bool salaOk = false;

	while (salesFetes < 3) {
		x = rand() % MidaMapa;
		y = rand() % MidaMapa;
		salaOk = false;

		if (_infoCasella[x][y].id == -1 && _mapaLogic[x][y] == 0) {	//Si no te ID ni direcció
			//Esquerra 
			if (x > 0 && _mapaLogic[x - 1][y] != 0 && !salaOk && _infoCasella[x-1][y].id > 2) {	//Comprovem els limits de la X i que la id no sigui cap de les 3 especials

				//Mirem que només esigui conectada amb una sala
				_mapaLogic[x - 1][y] += 4;		//Li sumem 4 = porta cap a la dreta
				_mapaLogic[x][y] += 8;			//Porta cap a la esquerra
				_infoCasella[x][y].id = salesFetes;
				salesFetes++;
				salaOk = true;
			}
			//Dreta
			else if (x < MidaMapa - 1 && _mapaLogic[x + 1][y] != 0 && !salaOk && _infoCasella[x + 1][y].id > 2) {
				_mapaLogic[x + 1][y] += 8;
				_mapaLogic[x][y] += 4;
				_infoCasella[x][y].id = salesFetes;
				salesFetes++;
				salaOk = true;
			}
			//Amunt
			else if (y > 0 && _mapaLogic[x][y - 1] != 0 && !salaOk && _infoCasella[x][y-1].id > 2) {
				_mapaLogic[x][y - 1] += 2;
				_mapaLogic[x][y] += 1;
				_infoCasella[x][y].id = salesFetes;
				salesFetes++;
				salaOk = true;
			}
			//Avall
			else if (y < MidaMapa - 1 && _mapaLogic[x][y + 1] != 0 && !salaOk && _infoCasella[x][y + 1].id > 2) {
				_mapaLogic[x][y + 1] += 1;
				_mapaLogic[x][y] += 2;
				_infoCasella[x][y].id = salesFetes;
				salesFetes++;
				salaOk = true;
			}
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("5.ID's especials fetes")));
}

void AGeneratorV1::placeLevel() {
	if (BaseRoom != NULL && StartRoom != NULL && EndRoom != NULL && LootRoom != NULL) {
		if (myWorld) {			//Comprovem que la funcio spawnRoom no pot per referencies nul·les

			for (int x = 0; x < MidaMapa; x++) {
				for (int y = 0; y < MidaMapa; y++) {
					if (_infoCasella[x][y].id != -1 && _mapaLogic[x][y] != 0) {	//Si te ID i no esta buida

						FVector2D vPos;
						//vPos.X = x * DiferenciaEnX;
						//vPos.Y = y * DiferenciaEnY;

						//----- Noves Sales
						vPos.X = x * 1500;
						vPos.Y = y * 1850;

						spawnRoom(_infoCasella[x][y].id, _mapaLogic[x][y], vPos);

					}
				}
			}
			
			if (GEngine) {
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("6.Sales Col·locades")));
			}
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Referencia de món errònea")));
		}
	}
	else {
		
		if (BaseRoom == NULL) {
			///GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Referencia Room NULL - BaseRoom")));
		}
		else if (StartRoom == NULL) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Referencia Room NULL - StartRoom")));
		}
		else if (EndRoom == NULL) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Referencia Room NULL - EndRoom")));
		}
		else if (LootRoom == NULL) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Referencia Room NULL - LootRoom")));
		}
	}	
}

void AGeneratorV1::spawnRoom(int roomNum, int doorNum, FVector2D vectorPos) {
	AMyBP* newRoom;

	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	FRotator spawnRotation;

	FVector spawnLocation = FVector{ vectorPos.X, vectorPos.Y, 0 };
	//FVector spawnLocation = FVector{ 1000, 0, 0 };	// per provar que funcioni correctament
	/*
		0 = Inici
		1 = Final
		2 = Loot
		3 ~ 12 = Sala Normal

	*/
	switch (roomNum) {
	case 0:
		newRoom = myWorld->SpawnActor<AMyBP>(StartRoom, spawnLocation, spawnRotation, spawnParams);
		break;

	case 1:
		newRoom = myWorld->SpawnActor<AMyBP>(EndRoom, spawnLocation, spawnRotation, spawnParams);
		break;

	case 2:
		newRoom = myWorld->SpawnActor<AMyBP>(LootRoom, spawnLocation, spawnRotation, spawnParams);
		break;

	case 3:
		newRoom = myWorld->SpawnActor<AMyBP>(BaseRoom, spawnLocation, spawnRotation, spawnParams);
		break;

		// De 3 a 12

	default:
		newRoom = myWorld->SpawnActor<AMyBP>(BaseRoom, spawnLocation, spawnRotation, spawnParams);
		break;
	}	

	//Segons el numero de portes, n'activem i desactivem.
	//O potser millor si les generem, que per defecte en tingui 0

	switch (doorNum) {
	case 1:
		break;

	case 2:
		break;

	case 3:
		break;

	case 4:
		break;

	case 5:
		break;

	case 6:
		break;

	case 7:
		break;

	case 8:
		break;

	case 9:
		break;

	case 10:
		break;

	case 11:
		break;

	case 12:
		break;

	case 13:
		break;

	case 14:
		break;

	case 15:
		break;

	default:
		break;


	}


}

int AGeneratorV1::getNumDoors(int x, int y) {
	int num = -1;

	if (x >= 0 && x < MidaMapa && y >= 0 && y < MidaMapa) {
		num = _mapaLogic[x][y];
	}
	return num;
}

int AGeneratorV1::getRoomID(int x, int y) {
	int num = -1;

	if (x >= 0 && x < MidaMapa && y >= 0 && y < MidaMapa) {
		num = _infoCasella[x][y].id;
	}
	return num;
}



// Called when the game starts or when spawned
void AGeneratorV1::BeginPlay()
{
	Super::BeginPlay();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("1.Dins BeginPlay")));	
}

// Called every frame
void AGeneratorV1::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

}

void AGeneratorV1::debugGenerateLevel()
{

	//Portes
	{
	_mapaLogic[0][0] = 4;
	_mapaLogic[0][1] = 6;
	_mapaLogic[0][2] = 5;
	_mapaLogic[0][3] = 0;
	_mapaLogic[0][4] = 0;
	_mapaLogic[0][5] = 0;

	_mapaLogic[1][0] = 14;
	_mapaLogic[1][1] = 9;
	_mapaLogic[1][2] = 14;
	_mapaLogic[1][3] = 7;
	_mapaLogic[1][4] = 1;
	_mapaLogic[1][5] = 0;

	_mapaLogic[2][0] = 10;
	_mapaLogic[2][1] = 5;
	_mapaLogic[2][2] = 8;
	_mapaLogic[2][3] = 12;
	_mapaLogic[2][4] = 0;
	_mapaLogic[2][5] = 0;

	_mapaLogic[3][0] = 4;
	_mapaLogic[3][1] = 12;
	_mapaLogic[3][2] = 0;
	_mapaLogic[3][3] = 12;
	_mapaLogic[3][4] = 0;
	_mapaLogic[3][5] = 0;

	_mapaLogic[4][0] = 12;
	_mapaLogic[4][1] = 12;
	_mapaLogic[4][2] = 6;
	_mapaLogic[4][3] = 9;
	_mapaLogic[4][4] = 0;
	_mapaLogic[4][5] = 0;

	_mapaLogic[5][0] = 10;
	_mapaLogic[5][1] = 9;
	_mapaLogic[5][2] = 10;
	_mapaLogic[5][3] = 3;
	_mapaLogic[5][4] = 3;
	_mapaLogic[5][5] = 1;
	}

	
	//ID's
	{
		// 30: -1?

	_infoCasella[0][0].id = 0;	//---
	_infoCasella[0][1].id = 3;
	_infoCasella[0][2].id = 4;
	_infoCasella[0][3].id = -1;
	_infoCasella[0][4].id = -1;
	_infoCasella[0][5].id = -1;

	_infoCasella[1][0].id = 5;
	_infoCasella[1][1].id = 6;
	_infoCasella[1][2].id = 7;
	_infoCasella[1][3].id = 8;
	_infoCasella[1][4].id = 1;	//---
	_infoCasella[1][5].id = -1;

	_infoCasella[2][0].id = 9;
	_infoCasella[2][1].id = 10;
	_infoCasella[2][2].id = 2;	//---
	_infoCasella[2][3].id = 12;
	_infoCasella[2][4].id = -1;
	_infoCasella[2][5].id = -1;

	_infoCasella[3][0].id = 13;
	_infoCasella[3][1].id = 14;
	_infoCasella[3][2].id = -1;
	_infoCasella[3][3].id = 15;
	_infoCasella[3][4].id = -1;
	_infoCasella[3][5].id = -1;

	_infoCasella[4][0].id = 16;
	_infoCasella[4][1].id = 17;
	_infoCasella[4][2].id = 18;
	_infoCasella[4][3].id = 19;
	_infoCasella[4][4].id = -1;
	_infoCasella[4][5].id = -1;

	_infoCasella[5][0].id = 20;
	_infoCasella[5][1].id = 21;
	_infoCasella[5][2].id = 22;
	_infoCasella[5][3].id = 23;
	_infoCasella[5][4].id = 24;
	_infoCasella[5][5].id = 25;
	}


	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("3.Mapa logic creat")));
}
