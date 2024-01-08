// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameMode.h"
#include "Controllers/SPlayerController.h"
#include "Characters/SPlayerPawn.h"
#include "Characters/SRPGCharacter.h"

ASGameMode::ASGameMode()
{
	PlayerControllerClass = ASPlayerController::StaticClass();
	DefaultPawnClass = ASRPGCharacter::StaticClass();
}


