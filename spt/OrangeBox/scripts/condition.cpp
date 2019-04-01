#include "stdafx.h"
#include "condition.hpp"
#include "..\..\utils\ent_utils.hpp"
#include "..\..\utils\property_getter.hpp"
#include "..\modules.hpp"
#include "..\modules\ClientDLL.hpp"

namespace scripts
{
	TickRangeCondition::TickRangeCondition(int low, int high, bool reverse)
	    : lowTick(low), highTick(high), reverse(reverse)
	{
		if (low >= high)
			throw std::exception("Low tick should be lower than high");
	}

	bool TickRangeCondition::IsTrue(int tick, int totalTicks) const
	{
		int value;

		if (reverse)
			value = totalTicks - tick;
		else
			value = tick;

		return value >= lowTick && value <= highTick;
	}

	bool TickRangeCondition::ShouldTerminate(int tick, int totalTicks) const
	{
		if (reverse)
			return (totalTicks - tick) < lowTick;
		else
			return tick > highTick;
	}

	PosSpeedCondition::PosSpeedCondition(float low, float high, Axis axis, bool isPos)
	    : low(low), high(high), axis(axis), isPos(isPos)
	{
		if (low >= high)
			throw std::exception("Low value should be lower than high");
	}

	bool PosSpeedCondition::IsTrue(int tick, int totalTicks) const
	{
		Vector v;

		if (isPos)
			v = clientDLL.GetPlayerEyePos();
		else
			v = clientDLL.GetPlayerVelocity();

		float val;

		switch (axis)
		{
		case Axis::AxisX:
			val = v.x;
			break;
		case Axis::AxisY:
			val = v.y;
			break;
		case Axis::AxisZ:
			val = v.z;
			break;
		case Axis::TwoD:
			val = v.Length2D();
			break;
		case Axis::Abs:
			val = v.Length();
			break;
		default:
			throw std::exception("Unknown type for speed/position condition");
		}

		return val >= low && val <= high;
	}

	bool PosSpeedCondition::ShouldTerminate(int tick, int totalTicks) const
	{
		return false;
	}

	JBCondition::JBCondition(float z) : height(z) {}

	bool JBCondition::IsTrue(int tick, int totalTicks) const
	{
		return utils::CanJB(height).canJB;
	}

	bool JBCondition::ShouldTerminate(int tick, int totalTicks) const
	{
		return false;
	}

	AliveCondition::AliveCondition() {}

	bool AliveCondition::IsTrue(int tick, int totalTicks) const
	{
		return utils::GetProperty<int>(0, "m_iHealth") > 0;
	}

	bool AliveCondition::ShouldTerminate(int tick, int totalTicks) const
	{
		return utils::GetProperty<int>(0, "m_iHealth") <= 0;
	}
} // namespace scripts
