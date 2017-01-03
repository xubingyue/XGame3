#ifndef PATH_H_
#define PATH_H_

#include "engine/coord.h"

struct Path
{

	static const int MAX_NODE_COUNT = 1024;
	int nodeCount;	// 折点数
	Coord3D nodes[MAX_NODE_COUNT];	// path上折点的坐标序列

	Path(): nodeCount(0) {}

	inline void AddNode(const Coord3D& _coord)		// 添加路径折点
	{
		if (nodeCount < MAX_NODE_COUNT)
			nodes[nodeCount++] = _coord;
	}
};

#endif /* PATH_H_ */
