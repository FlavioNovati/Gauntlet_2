#include "Game/Interfaces/Damageable.h"

void IDamageable::NativeDamage(float amount)
{
	//Call BP OnDamage for blueprint implementation
	BP_OnDamage(amount);
}
