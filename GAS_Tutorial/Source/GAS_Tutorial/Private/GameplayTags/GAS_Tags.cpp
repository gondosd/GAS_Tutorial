#include "GameplayTags/GAS_Tags.h"

namespace GASTags
{
	namespace GASAbilities
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActivateOnGiven, "GASTags.GASAbilities.ActivateOnGiven", "Tag for Abilities that should activate itself when its given")

		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Primary, "GASTags.GASAbilities.Primary", "Tag for the Primary Ability")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Secondary, "GASTags.GASAbilities.Secondary", "Tag for the Secondary Ability")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tertiary, "GASTags.GASAbilities.Tertiary", "Tag for the Tertiary Ability")
	}
	
	namespace Events
	{
		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "GASTags.Events.Enemy.HitReact", "Tag for the Enemy HitReact Event")
		}
	}
}
