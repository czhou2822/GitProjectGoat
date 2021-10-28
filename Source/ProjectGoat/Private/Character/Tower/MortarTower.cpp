// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Tower/MortarTower.h"



AMortarTower::AMortarTower()
{

}

float AMortarTower::GetAngle(FVector InPosition)
{
	//return -1 => non valid

	FVector MyLocation = GetActorLocation();

	//float VerticalDif = InPosition.Z - MyLocation.Z;
	//if (VerticalDif <= VerticalTravelDistanceMin || VerticalDif >= VerticalTravelDistanceMax) return -1;

	float HorizonalDif = FVector::Dist(FVector(InPosition.X, InPosition.Y, 0), FVector(MyLocation.X, MyLocation.Y, 0));
	if (HorizonalDif <= TravelDistanceMin || HorizonalDif >= TravelDistanceMax) return -1;

	float HorizontalVelocity = HorizonalDif / AirTime;
	float Angle = FMath::RadiansToDegrees(FMath::Asin(HorizontalVelocity / FiringVelocity));


	return Angle;
}


void AMortarTower::BeginPlay()
{
	Super::BeginPlay();
}

void AMortarTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

/*
void AMortarTower::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangeEvent)
{
	if (PropertyChangeEvent.GetPropertyName() == "AirTime" || PropertyChangeEvent.GetPropertyName() == "FiringVelocity")
	{
		//TravelDistance = PropertyChangeEvent.
		TravelDistanceMax = FiringVelocity * AirTime * FMath::Cos(FiringAngleMin * PI / (float)180);
		TravelDistanceMin = FiringVelocity * AirTime * FMath::Cos(FiringAngleMax * PI / (float)180);

		VerticalTravelDistanceMin = FiringVelocity * AirTime * FMath::Sin(FiringAngleMin * PI / (float)180) - 490 * AirTime * AirTime;
		VerticalTravelDistanceMax = FiringVelocity * AirTime * FMath::Sin(FiringAngleMax * PI / (float)180) - 490 * AirTime * AirTime;

	}
}
*/


