#pragma once

namespace Category
{
	enum Type
	{
		// Each category has one bit to set to 1 except None- this allows you to have multiple categories
		// Size_t anyaircraft = Category::PlayerAircraft || Category::AlliedAircraft || Category::EnemyAircraft

		None = 0,
		Scene = 1 << 0,
		PlayerAircraft = 1 << 1,
		AlliedAircraft = 1 << 2,
		EnemyAircraft = 1 << 3,
	};
}	