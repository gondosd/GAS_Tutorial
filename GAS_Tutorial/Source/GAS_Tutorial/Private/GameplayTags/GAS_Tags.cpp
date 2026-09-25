#include "GameplayTags/GAS_Tags.h"

namespace GASTags
{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(None, "GASTags.None", "None")
	
	namespace SetByCaller
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Projectile, "GASTags.SetByCaller.Projectile", "Tag for Set by Caller Magnitude for projectiles")
	}

	namespace GASAbilities
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActivateOnGiven, "GASTags.GASAbilities.ActivateOnGiven", "Tag for Abilities that should activate itself when its given")

		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Primary, "GASTags.GASAbilities.Primary", "Tag for the Primary Ability")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Secondary, "GASTags.GASAbilities.Secondary", "Tag for the Secondary Ability")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tertiary, "GASTags.GASAbilities.Tertiary", "Tag for the Tertiary Ability")

		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Attack, "GASTags.GASAbilities.Enemy.Attack", "Enemy Attack Tag")
		}
	}

	namespace Events
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(KillScored, "GASTags.Events.KillScored", "Tag for the KillScored Event")

		namespace Player
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "GASTags.Events.Player.HitReact", "Tag for the Player HitReact Event")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(Death, "GASTags.Events.Player.Death", "Tag for the Player Death Event")
		}
		
		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact, "GASTags.Events.Enemy.HitReact", "Tag for the Enemy HitReact Event")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(EndAttack, "GASTags.Events.Enemy.EndAttack", "Tag for the Enemy Ending an attack")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(MeleeTraceHit, "GASTags.Events.Enemy.MeleeTraceHit", "Tag for the Enemy Melee Trace Hit")
		}
	}
}
