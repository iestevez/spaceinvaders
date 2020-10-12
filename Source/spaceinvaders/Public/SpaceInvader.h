#pragma once
#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class InvaderMovementType : uint8 {


	STOP = 0 UMETA(DisplayName = "Stopped"),
	RIGHT = 1 UMETA(DisplayName = "Right"),
	LEFT = 2 UMETA(DisplayName = "Left"),
	DOWN = 3 UMETA(DisplayName = "Down"),
	FREEJUMP = 4 UMETA(DisplayName = "Free Jump")


};

UENUM(BlueprintType)
enum class BulletType : uint8 {
	PLAYER=0 UMETA(DisplayName = "Player Type"),
	INVADER=1 UMETA(DisplayName = "Invader Type")
};