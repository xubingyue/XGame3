#ifndef POSITION_H__
#define POSITION_H__

#include "engine/coord.h"
#include "engine/angle.h"
#include "base/sys_types.h"
#include "base/utils.h"

struct Position
{
	Coord3D		coord; // 所在位置
	Angle		angle; // 朝向
	int			mapid; // 所在的地图配置id

	Position (const Position& _pos)
		: coord(_pos.coord), angle(_pos.angle), mapid(_pos.mapid)
	{
		//XXZ_ASSERT(_pos.coord.x >= 0);
		//XXZ_ASSERT(_pos.coord.y >= 0); 
	}

	Position(const Coord3D& _coord, const Angle& _angle, int _mapID)
		: coord(_coord), angle(_angle), mapid(_mapID)
	{
		//XXZ_ASSERT(_coord.x >= 0);
		//XXZ_ASSERT(_coord.y >= 0);
	}

	Position() 
	{
		coord.x = 0;
		coord.y = 0;
		coord.z = 0;
		mapid	= 0;

		//XXZ_ASSERT(coord.x >= 0);
		//XXZ_ASSERT(coord.y >= 0);
	}

	inline void Set(int _mapID, const Coord3D& _coord, const Angle& _angle)
	{
		mapid = _mapID;
		//XXZ_ASSERT(_coord.x >= 0);
		//XXZ_ASSERT(_coord.y >= 0);
		coord.Set(_coord.x, _coord.y, _coord.z);
		angle.Set(_angle.x, _angle.y);
	}

	inline void SetCoord(int _x, int _y, int _z)
	{
		//XXZ_ASSERT(_x >= 0);
		//XXZ_ASSERT(_y >= 0);
		coord.Set(_x, _y, _z);
	}
	inline void SetCoord(const Coord3D& _coord)
	{
		//XXZ_ASSERT(_coord.x >= 0);
		//XXZ_ASSERT(_coord.y >= 0);
		coord.Set(_coord.x, _coord.y, _coord.z);
	}
	inline void SetAngle(const Angle& _angle)
	{
		angle.Set(_angle.x, _angle.y);
	}
	inline void SetAngle(int x, int y)
	{
		angle.Set(x, y);
	}
	Position& operator = (const Position& _pos){
		angle.x	= _pos.angle.x;
		angle.y	= _pos.angle.y;
		mapid		= _pos.mapid;
		//XXZ_ASSERT(_pos.coord.x >= 0);
		//XXZ_ASSERT(_pos.coord.y >= 0);
		coord = _pos.coord;
		return *this;
	}
};

#endif // POSITION_H__
