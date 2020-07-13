#include "Data/CharacterData.h"





FCharacterData::FCharacterData()
	:Name(NAME_None),
	ID(INDEX_NONE),
	Lv(1),
	MaxHealth(100),
	Health(MaxHealth), 
	bIsBrittle(false),
	bCanBeBrittle(true),
	bTeam(false)
{

}
void FCharacterData::UpdateHealth()
{
	Health = MaxHealth;
}



FWaveStructData::FWaveStructData()
	:WaveNumber(0),
	 GruntNumber(0),
	 ReaverNumber(0),
     MiniBossNumber(0),
	 GoldDropRate(0)
{
}
