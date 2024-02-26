// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/SGameMode.h"
#include "Controllers/SPlayerController.h"
#include "Characters/SPlayerPawn.h"
#include "Characters/SRPGCharacter.h"
#include "Game/SPlayerState.h"
#include "Game/SGameState.h"
#include "Kismet/GameplayStatics.h"

ASGameMode::ASGameMode()
{
	PlayerControllerClass = ASPlayerController::StaticClass();
	DefaultPawnClass = ASRPGCharacter::StaticClass();
    RemainWaitingTimeForEnding = EndingTime;
}


void ASGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    ASGameState* SGameState = GetGameState<ASGameState>();
    if (false == SGameState)
    {
        return;
    }

    //if (MatchState != EMatchState::Waiting)
    if (SGameState->MatchState != EMatchState::Waiting)
    {
        NewPlayer->SetLifeSpan(0.1f);
        return;
    }

    ASPlayerState* PlayerState = NewPlayer->GetPlayerState<ASPlayerState>();
    if (true == ::IsValid(PlayerState))
    {
        PlayerState->InitPlayerState();
    }

    ASPlayerController* NewPlayerController = Cast<ASPlayerController>(NewPlayer);
    if (true == ::IsValid(NewPlayerController))
    {
        AlivePlayerControllers.Add(NewPlayerController);

        NewPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
    }
}

void ASGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    ASPlayerController* ExitingPlayerController = Cast<ASPlayerController>(Exiting);
    if (true == ::IsValid(ExitingPlayerController) && INDEX_NONE != AlivePlayerControllers.Find(ExitingPlayerController))
    {
        AlivePlayerControllers.Remove(ExitingPlayerController);
        DeadPlayerControllers.Add(ExitingPlayerController);
    }
}

void ASGameMode::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &ThisClass::OnMainTimerElapsed, 1.f, true);

    RemainWaitingTimeForPlaying = WaitingTime;
}

void ASGameMode::OnControllerDead(ASPlayerController* InDeadController)
{
    if (false == ::IsValid(InDeadController) || INDEX_NONE == AlivePlayerControllers.Find(InDeadController))
    {
        return;
    }

    InDeadController->ShowLooserUI(AlivePlayerControllers.Num());

    AlivePlayerControllers.Remove(InDeadController);
    DeadPlayerControllers.Add(InDeadController);
}


void ASGameMode::OnMainTimerElapsed()
{
    ASGameState* SGameState = GetGameState<ASGameState>();
    if (false == ::IsValid(SGameState))
    {
        return;
    }

    //switch (MatchState)
    switch (SGameState->MatchState)
    {
    case EMatchState::None:
        break;
    case EMatchState::Waiting:
    {
        FString NotificationString = FString::Printf(TEXT(""));

        if (AlivePlayerControllers.Num() < MinimumPlayerCountForPlaying)
        {
            NotificationString = FString::Printf(TEXT("Wait another players for playing."));

            RemainWaitingTimeForPlaying = WaitingTime; // �ּ��ο��� �ȵȴٸ� ��� �ð� �ʱ�ȭ.
        }
        else
        {
            NotificationString = FString::Printf(TEXT("Wait %d seconds for playing."), RemainWaitingTimeForPlaying);

            --RemainWaitingTimeForPlaying;
        }

        if (0 == RemainWaitingTimeForPlaying)
        {
            NotificationString = FString::Printf(TEXT(""));

            SGameState->MatchState = EMatchState::Playing;
        }

        NotifyToAllPlayer(NotificationString);

        break;
    }
    case EMatchState::Playing:
    {
        /*
        FString NotificationString = FString::Printf(TEXT("%d / %d"), AlivePlayerControllers.Num(), AlivePlayerControllers.Num() + DeadPlayerControllers.Num());

        NotifyToAllPlayer(NotificationString);

        if (AlivePlayerControllers.Num() <= 1)
        {
            MatchState = EMatchState::Ending;
        }
        */

        //ASGameState* SGameState = GetGameState<ASGameState>();
        if (true == ::IsValid(SGameState))
        {
            SGameState->AlivePlayerControllerCount = AlivePlayerControllers.Num();

            FString NotificationString = FString::Printf(TEXT("%d / %d"), SGameState->AlivePlayerControllerCount, SGameState->AlivePlayerControllerCount + DeadPlayerControllers.Num());

            NotifyToAllPlayer(NotificationString);

            if (SGameState->AlivePlayerControllerCount <= 1)
            {
                AlivePlayerControllers[0]->ShowWinnerUI();
                SGameState->MatchState = EMatchState::Ending;
            }

        }

        break;
    }
    case EMatchState::Ending:
    {
        FString NotificationString = FString::Printf(TEXT("Waiting %d for returning to lobby."), RemainWaitingTimeForEnding);

        NotifyToAllPlayer(NotificationString);

        --RemainWaitingTimeForEnding;

        if (0 == RemainWaitingTimeForEnding)
        {
            for (auto AliveController : AlivePlayerControllers)
            {
                AliveController->ReturnToLobby();
            }
            for (auto DeadController : DeadPlayerControllers)
            {
                DeadController->ReturnToLobby();
            }

            MainTimerHandle.Invalidate();

            FName CurrentLevelName = FName(UGameplayStatics::GetCurrentLevelName(this));
            UGameplayStatics::OpenLevel(this, CurrentLevelName, true, FString(TEXT("listen")));
            // ���� ���� ������ ���� ���� ���񽺵�� �����Ǿ� �ִٸ�,
            // �̷��� ������ �ٽ� �غ�� �� ���� �������� �˷���. "���ο� �÷��̾� ���� �� ����."
            // �׷� ���� ���񽺴� ���ο� �÷��̾�鿡�� ���� ������ IP �ּҸ� �������༭ ���� ����.
        }

        break;
    }
    case EMatchState::End:
        break;
    default:
        break;
    }
}

void ASGameMode::NotifyToAllPlayer(const FString& NotificationString)
{
    for (auto AlivePlayerController : AlivePlayerControllers)
    {
        if (true == ::IsValid(AlivePlayerController))
        {
          AlivePlayerController->NotificationText = FText::FromString(NotificationString);
        }
    }

    for (auto DeadPlayerController : DeadPlayerControllers)
    {
        if (true == ::IsValid(DeadPlayerController))
        {
            DeadPlayerController->NotificationText = FText::FromString(NotificationString);
        }
    }
}