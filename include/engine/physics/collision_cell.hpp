#pragma once
#include <cstdint>
#include "../common/vec.hpp"
#include "../common/grid.hpp"


constexpr uint8_t max_cell_capacity = 16;


struct CollisionCell
{
	// Overlap workaround
	uint8_t objects_count;
	uint32_t objects[max_cell_capacity];

	CollisionCell()
		: objects_count(0u)
	{}

	bool addAtom(uint32_t atom)
	{
		// Overflow safe, but dataloss
		objects[objects_count] = atom;
		objects_count += objects_count < (max_cell_capacity - 1);
		return objects_count == 1;
	}

	void clear()
	{
		objects_count = 0u;
	}
};

struct CollisionGrid : public Grid<CollisionCell>
{
	std::vector<uint32_t> used;
	uint32_t used_count;

	CollisionGrid()
		: Grid<CollisionCell>()
		, used(0)
		, used_count(0)
	{}

	CollisionGrid(int32_t width, int32_t height)
		: Grid<CollisionCell>(width, height)
		, used(width * height, 0)
		, used_count(0)
	{}

	bool addAtom(uint32_t x, uint32_t y, uint32_t atom)
	{
		const uint32_t id = x + y * width;
		CollisionCell& cell = data[id];
		// Add to grid
		const bool first = data[id].addAtom(atom);
		used[used_count] += first * id;
		used_count += first;
		return true;
	}

	void clear()
	{
		for (uint32_t i(used_count); i--;) {
			data[used[i]].clear();
			used[i] = 0;
		}
		used_count = 0;
	}
};
