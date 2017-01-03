#include <math.h>
#include <stdlib.h>
#include <algorithm>

#include "base/utils.h"
#include "coord.h"
#include "angle.h"
#include "graph.h"
#include "path.h"

namespace Graph {

//=============================================================
//函数: CoordInRect
//功能: 判断点在矩形内
//参数: _coord--判断的坐标
//参数: _vectStart--矩形左上顶点坐标
//参数: _vectEnd--矩形右下顶点坐标
//返回: bool
//=============================================================
bool CoordInRect(const Coord2D& _coord, const Coord2D& _vectStart, const Coord2D& _vectEnd)
{
	return (_coord.x >=_vectStart.x) && (_coord.y >= _vectStart.y) && (_coord.x < _vectEnd.x) && (_coord.y < _vectEnd.y);
}

//=============================================================
//函数: CoordInRect
//功能: 判断线段是否在矩形内
//参数: _segmentStart--线段起点坐标
//参数: _segmentEnd--线段终点坐标
//参数: _vectStart--矩形左上顶点坐标
//参数: _vectEnd--矩形右下顶点坐标
//返回: bool
//=============================================================
bool SegmentInRect(const Coord2D& _segmentStart, const Coord2D& _segmentEnd, const Coord2D& _vectStart, const Coord2D& _vectEnd)
{
	return CoordInRect(_segmentStart, _vectStart, _vectEnd) && \
			CoordInRect(_segmentEnd, _vectStart, _vectEnd);
}

//=============================================================
//函数: SegmentDist
//功能: 求线段的长度
//参数: _startCoord--线段起点坐标
//参数: _endCoord--线段终点坐标
//返回: 线段的长度
//=============================================================
int SegmentDist(const Coord2D& _startCoord, const Coord2D& _endCoord)
{
	double xdist = ((double)(_endCoord.x -_startCoord.x))*(_endCoord.x -_startCoord.x);
	double ydist = ((double)(_endCoord.y -_startCoord.y))*(_endCoord.y -_startCoord.y);
	double dist = sqrt(xdist + ydist);
	//LOG_DEBUG("------SegmentDist_dist = %d", dist);
	//LOG_DEBUG("_endCoord.x = %d, _endCoord.y = %d", _endCoord.x,  _endCoord.y);
	//LOG_DEBUG("_startCoord.x = %d, _startCoord.y = %d" ,_startCoord.x,  _startCoord.y);
	return int(dist);
}

//=============================================================
//函数: SegmentDistSquare
//功能: 求线段的长度的平方
//参数: _startCoord--线段起点坐标
//参数: _endCoord--线段终点坐标
//返回: 线段的长度的平方
//=============================================================
int SegmentDistSquare(const Coord2D& _startCoord, const Coord2D& _endCoord)
{
	return (_endCoord.x-_startCoord.x)*(_endCoord.x-_startCoord.x)+(_endCoord.y-_startCoord.y)*(_endCoord.y-_startCoord.y);
}

//=============================================================
//函数: CoordNearEqual
//功能: 在一定误差范围内判断连个点是否重合,为了计算速度，只比较平方
//		和。
//参数: _startCoord--一个点坐标
//参数: _endCoord--另一个点坐标
//返回: bool
//=============================================================
bool CoordNearEqual(const Coord2D& _startCoord, const Coord2D& _endCoord)
{
	//return ((_endCoord.x == _startCoord.x) && (_endCoord.y == _startCoord.y));
	return (_endCoord.x - _startCoord.x) * (_endCoord.x - _startCoord.x) + (_endCoord.y - _startCoord.y) * (_endCoord.y - _startCoord.y) \
			<= Graph::MAX_DIST_ERROR * Graph::MAX_DIST_ERROR;
}

//=============================================================
//函数: OnSegment
//功能: 判断点是否在线段上
//参数: _startCoord--线段起点坐标
//参数: _endCoord--线段终点坐标
//参数: _judgeCoord--需要判断的点坐标_
//返回: bool
//=============================================================
bool OnSegment(const Coord2D& _startCoord, const Coord2D& _endCoord, const Coord2D& _judgeCoord)
{
	// 判断点是否在线段区域内
	if (_judgeCoord.x < std::min(_startCoord.x, _endCoord.x) || _judgeCoord.x > std::max(_startCoord.x, _endCoord.x)
			|| _judgeCoord.y < std::min(_startCoord.y, _endCoord.y) || _judgeCoord.y > std::max(_startCoord.y, _endCoord.y))
		return false;

	// 判断点到直线的距离不超过MAX_DIST_ERROR,则说明点在直线ax+by+c=0上
	int a = _startCoord.y - _endCoord.y;
	int b = _endCoord.x - _startCoord.x;
	int c = _startCoord.x*_endCoord.y - _endCoord.x*_startCoord.y;
	int dist = int( abs(a*_judgeCoord.x + b*_judgeCoord.y + c) / (int)sqrt((double)a*a + (double)b*b) );

	if (dist <= Graph::MAX_DIST_ERROR)
		return true;
	else
		return false;
}

//=============================================================
//函数: SegmentMove
//功能: 计算离(有向)线段起点一定距离的点坐标
//参数: _startCoord--线段起点坐标
//参数: _endCoord--线段终点坐标
//参数: _deltaDist--离线段起点的距离
//返回: 点坐标
//=============================================================
Coord2D SegmentMove(const Coord2D& _startCoord, const Coord2D& _endCoord, const int _deltaDist)
{
	int segmDist = SegmentDist(_startCoord, _endCoord);

	if (0 == segmDist)
	{
		return _startCoord;
	}

	int x = (_endCoord.x - _startCoord.x)*_deltaDist/segmDist + _startCoord.x;
	int y = (_endCoord.y - _startCoord.y)*_deltaDist/segmDist + _startCoord.y;

	Coord2D coord(x, y);
	return coord;
}

}
