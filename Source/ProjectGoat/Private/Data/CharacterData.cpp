#include "Data/CharacterData.h"





FCharacterData::FCharacterData()
	:Name(NAME_None),
	ID(INDEX_NONE),
	Lv(1),
	MaxHealth(100),
	Health(MaxHealth)

{

}
void FCharacterData::UpdateHealth()
{
	Health = MaxHealth;
}



//FEnemyData::FEnemyData()
//	:FCharacterData(),
//	MaxHealth(100),
//	Health(MaxHealth)
//{
//
//}

