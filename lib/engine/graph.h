/*
 *  图形几何运算函数集
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <math.h>
#include "coord.h"
#include "angle.h"
#include "path.h"

namespace Graph {

// 如两点间距离小于DistError，则视为两点重合
const int MAX_DIST_ERROR = 5;
// 圆周率
const double PI = 3.1415926;

//=============================================================
//函数: CoordInRect
//功能: 判断点在矩形内
//参数: _coord--判断的坐标
//参数: _vectStart--矩形左上顶点坐标
//参数: _vectEnd--矩形右下顶点坐标
//返回: bool
//=============================================================
bool CoordInRect(const Coord2D& _coord, const Coord2D& _vectStart, const Coord2D& _vectEnd);

//=============================================================
//函数: CoordInRect
//功能: 判断线段是否在矩形内
//参数: _segmentStart--线段起点坐标
//参数: _segmentEnd--线段终点坐标
//参数: _vectStart--矩形左上顶点坐标
//参数: _vectEnd--矩形右下顶点坐标
//返回: bool
//=============================================================
bool SegmentInRect(const Coord2D& _segmentStart, const Coord2D& _segmentEnd, const Coord2D& _vectStart, const Coord2D& _vectEnd);

//=============================================================
//函数: SegmentDist
//功能: 求线段的长度
//参数: _startCoord--线段起点坐标
//参数: _endCoord--线段终点坐标
//返回: 线段的长度
//=============================================================
int SegmentDist(const Coord2D& _startCoord, const Coord2D& _endCoord);

//=============================================================
//函数: SegmentDistSquare
//功能: 求线段的长度的平方
//参数: _startCoord--线段起点坐标
//参数: _endCoord--线段终点坐标
//返回: 线段的长度的平方
//=============================================================
int SegmentDistSquare(const Coord2D& _startCoord, const Coord2D& _endCoord);

//=============================================================
//函数: CoordNearEqual
//功能: 在一定误差范围内判断连个点是否重合
//参数: _startCoord--一个点坐标
//参数: _endCoord--另一个点坐标
//返回: bool
//=============================================================
bool CoordNearEqual(const Coord2D& _startCoord, const Coord2D& _endCoord);

//=============================================================
//函数: OnSegment
//功能: 判断点是否在线段上
//参数: _startCoord--线段起点坐标
//参数: _endCoord--线段终点坐标
//参数: _judgeCoord--需要判断的点坐标_
//返回: bool
//=============================================================
bool OnSegment(const Coord2D& _startCoord, const Coord2D& _endCoord, const Coord2D& _judgeCoord);

//=============================================================
//函数: SegmentMove
//功能: 计算离(有向)线段起点一定距离的点坐标
//参数: _startCoord--线段起点坐标
//参数: _endCoord--线段终点坐标
//参数: _deltaDist--离线段起点的距离
//返回: 点坐标
//=============================================================
Coord2D SegmentMove(const Coord2D& _startCoord, const Coord2D& _endCoord, const int _deltaDist);

}




#endif /* GRAPH_H_ */
