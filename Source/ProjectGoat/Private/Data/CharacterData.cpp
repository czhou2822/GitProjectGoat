#include "Data/CharacterData.h"





FCharacterData::FCharacterData()
	:Name(NAME_None),
	ID(INDEX_NONE),
	Lv(-1),
	MaxHealth(0),
	Health(MaxHealth), 
	bIsBrittle(false),
	bCanBeBrittle(true),
	bTeam(false)
{

}
void FCharacterData::UpdateStats()
{
	Health = MaxHealth;
}


