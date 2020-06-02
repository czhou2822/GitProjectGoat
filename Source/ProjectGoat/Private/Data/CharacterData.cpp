#include "Data/CharacterData.h"





FCharacterData::FCharacterData()
	:Name(NAME_None),
	ID(INDEX_NONE),
	Lv(1)
{

}



FEnemyData::FEnemyData()
	:FCharacterData(),
	MaxHealth(100),
	Health(MaxHealth)
{

}