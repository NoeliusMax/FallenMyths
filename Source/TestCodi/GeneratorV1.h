// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Defines.h"
#include "MYBP.h"
#include <vector>

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneratorV1.generated.h"



UCLASS()
class TESTCODI_API AGeneratorV1 : public AActor
{
	GENERATED_BODY()

	int _mapaLogic[MidaMapa][MidaMapa];		//Es guarda la direcció que té cada element
	int _numCasellesFetes;
	pos _ultimaPos;
	pos _posActual;
	pos _posSeguent;
	pos _posInicial;
	pos _posFinal;
	std::vector<pos>_casellesFetes;
	int _valorAleatori;				//Valor per el switch de les direccions dins de cada posibilitat manipulem les direccions manualment


	std::vector<int>idSales;

	bool StartRoom, LootRoom, BossRoom;

	//UWorld* myWorld; // UWorld per col·locar les sales


	infoCaselles _infoCasella[MidaMapa][MidaMapa];	//Guardem les direccions valides de cada casella i s'hi hem estat o no

	virtual void comprovacioPasEnrere();

	//virtual void spawnRoom(int roomNum, int doorNum, FVector2D vectorPos); //Li passem X i Y, l'altura sempre serà la mateixa

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Sets default values for this actor's properties
	AGeneratorV1();
	~AGeneratorV1();

	// Spawn Rooms

	//Nombre maxim de sales - dessincronitzat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator Values")
	int maxRooms;

	//Nombre minim de sales - dessincronitzat
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generator Values")
	int minRooms;
	
	
	//Mida del mapa - dessincronitzat
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generator Values")
	int midaMapa;
/*
	 Base Room -> serà un array amb totes els diferents
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<class AMyBP> BaseRoom;

	// Room Inicial
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<class AMyBP> StartRoom;

	// Room Final
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<class AMyBP> EndRoom;

	// Room Loot
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	TSubclassOf<class AMyBP> LootRoom;*/

	/* Totes les sales normals*/
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rooms")
	//TSubclassOf<class AMyBP> Rooms[];
	//TArray<AMyBP*> RoomList;	//URooms ha de ser una nova classe de C++
	// Mirar com guardar diversos BluePrints en un array i accedir-hi des de C++


	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Reset variables per si volem tornar a jugar, com constructor
	UFUNCTION(BlueprintCallable, Category = "GenerateRooms")
	virtual void clearData();

	//Generacio mapa logic
	UFUNCTION(BlueprintCallable, Category = "GenerateRooms")
	virtual void generateLevel();

	//Generacio mapa logic manualment, per debug
	UFUNCTION(BlueprintCallable, Category = "GenerateRooms")
	virtual void debugGenerateLevel();	

	//Generacio ID's sala
	UFUNCTION(BlueprintCallable, Category = "GenerateRooms")
	virtual void setRoomsID();

	/*
	//Generacio de sales especials
	UFUNCTION(BlueprintCallable, Category = "GenerateRooms")
	virtual void setSpecialRooms();

	//Bucle que s'encarrega de cridar a spawnRoom			//No fer-ho aixi
	UFUNCTION(BlueprintCallable, Category = "GenerateRooms")
	virtual void placeLevel();	
	*/

	//Ens dona el numero de portes segons la X i la Y del mapa logic
	UFUNCTION(BlueprintCallable, Category = "GenerateRooms")
	virtual int getNumDoors(int x, int y);	

	//Ens dona tipus de portes segons la X i la Y de _infoCasella
	UFUNCTION(BlueprintCallable, Category = "GenerateRooms")
	virtual int getRoomID(int x, int y);


};