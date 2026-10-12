// Alex Goldberg rights reserved


#include "Controllers/WarriorHeroController.h"

/// Constructor for the Warrior Hero Controller
AWarriorHeroController::AWarriorHeroController()
{
	HeroTeamId = FGenericTeamId(0);
}

/// Returns the team ID of this controller
FGenericTeamId AWarriorHeroController::GetGenericTeamId() const
{
	return HeroTeamId;
}
