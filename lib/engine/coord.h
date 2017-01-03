#ifndef COORD_H_
#define COORD_H_

//=============================================================
//类名: Coord2D
//描述: 2D平面上点的坐标
//=============================================================
#include <cmath>
#include <vector>

template <typename Ty>
struct Vector3D;

template <typename Ty>
struct Vector2D
{
	Ty x;
	Ty y;

	Vector2D() { Set(0, 0); }
	Vector2D(Ty _x, Ty _y) { Set(_x, _y); }

	template <typename Ty2>
	Vector2D(const Vector2D<Ty2> &p){ Set(p.x, p.y); }

	template <typename Ty2>
	Vector2D(const Vector3D<Ty2> &p){ Set(p.x, p.y); }

	void Set(Ty _x, Ty _y)
	{
		x = _x;
		y = _y;
	}

	template <typename Ty2>
	inline Vector2D<Ty> operator+(const Vector2D<Ty2> &p) const
	{
		Vector2D<Ty> tmp(*this);
		tmp.x += p.x;
		tmp.y += p.y;
		return tmp;
	}

	inline Vector2D operator+(float len) const
	{
		Vector2D<Ty> tmp(*this);
		tmp.x += len;
		tmp.y += len;
		return tmp;
	}

	template <typename Ty2>
	inline Vector2D<Ty> operator-(const Vector2D<Ty2> &p) const
	{
		Vector2D<Ty> tmp(*this);
		tmp.x -= p.x;
		tmp.y -= p.y;
		return tmp;
	}

	inline Vector2D<Ty> operator-(float len) const
	{
		Vector2D<Ty> tmp(*this);
		tmp.x -= len;
		tmp.y -= len;
		return tmp;
	}

	inline Vector2D<Ty> operator*(float len) const
	{
		Vector2D<Ty> tmp(*this);
		tmp.x *= len;
		tmp.y *= len;
		return tmp;
	}

	inline float distance() const
	{
		return sqrtf(x * x + y * y);
	}
	
	template <typename Ty2>
	inline float distance(const Vector2D<Ty2> &p) const
	{
		float tx = p.x - x;
		float ty = p.y - y;
		return sqrtf(tx * tx + ty * ty);
	}
	template <typename Ty2>
	inline Ty2 squaredis(const Vector2D<Ty2> &p) const
	{
		return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);
	}
	inline void normalize()
	{
		float len = distance();
		if (len == 0.0f)
		{
			x = y = 0;
			return;
		}
		x /= len;
		y /= len;
	}

	inline Vector2D<Ty> normalized() const
	{
		Vector2D<Ty> tmp(*this);
		tmp.normalize();
		return tmp;
	}

	inline void toZero()
	{
		x = 0;
		y = 0;
	}

	inline bool isZero()
	{
		return x == 0 && y == 0;
	}

	template <typename Ty2>
	inline bool operator==(const Vector2D<Ty2>& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}

	template <typename Ty2>
	inline bool operator!=(const Vector2D<Ty2>& rhs) const
	{
		return !(*this == rhs);
	}
	template <typename Ty2>
	inline void operator =(const Vector2D<Ty2>& _coord)
	{
		x = _coord.x;
		y = _coord.y;
	}

	inline bool isValide()
	{
		return x >= 0 && y >= 0;
	}
};

template <typename Ty>
struct Vector3D
{
	Ty x;
	Ty y;
	Ty z;

	Vector3D() { Set(0, 0, 0); }
	Vector3D(Ty _x, Ty _y, Ty _z) { Set(_x, _y, _z); }

	template <typename Ty2>
	Vector3D(const Vector3D<Ty2> &p){ Set(p.x, p.y, p.z); }

	template <typename Ty2>
	Vector3D(const Vector2D<Ty2> &p){ Set(p.x, p.y, 0); }

	void Set(Ty _x, Ty _y, Ty _z = 0)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	template <typename Ty2>
	inline Vector3D<Ty> operator+(const Vector3D<Ty2> &p) const
	{
		Vector3D<Ty> tmp(*this);
		tmp.x += p.x;
		tmp.y += p.y;
		tmp.z += p.z;
		return tmp;
	}

	inline Vector3D operator+(float len) const
	{
		Vector3D<Ty> tmp(*this);
		tmp.x += len;
		tmp.y += len;
		tmp.z += len;
		return tmp;
	}

	template <typename Ty2>
	inline Vector3D<Ty> operator-(const Vector3D<Ty2> &p) const
	{
		Vector3D<Ty> tmp(*this);
		tmp.x -= p.x;
		tmp.y -= p.y;
		tmp.z -= p.z;
		return tmp;
	}

	inline Vector3D<Ty> operator-(float len) const
	{
		Vector3D<Ty> tmp(*this);
		tmp.x -= len;
		tmp.y -= len;
		tmp.z -= len;
		return tmp;
	}

	inline Vector3D<Ty> operator*(float len) const
	{
		Vector3D<Ty> tmp(*this);
		tmp.x *= len;
		tmp.y *= len;
		tmp.z *= len;
		return tmp;
	}

	inline float distance() const
	{
		return sqrtf(x * x + y * y + z * z);
	}
	inline float plandis() const
	{
		return sqrtf(x * x + y * y);
	}

	template <typename Ty2>
	inline float distance(const Vector3D<Ty2> &p) const
	{
		float tx = p.x - x;
		float ty = p.y - y;
		float tz = p.z - z;
		return sqrtf(tx * tx + ty * ty + tz * tz);
	}

	template <typename Ty2>
	inline float distance2D(const Vector3D<Ty2> &p) const
	{
		float tx = p.x - x;
		float ty = p.y - y;
		return sqrtf(tx * tx + ty * ty);
	}

	template <typename Ty2>
	inline float plandis(const Vector3D<Ty2> &p) const
	{
		float tx = p.x - x;
		float ty = p.y - y;
		return sqrtf(tx * tx + ty * ty);
	}

	template <typename Ty2>
	inline Ty2 squaredis(const Vector3D<Ty2> &p) const
	{
		return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y) + (p.z - z) * (p.z - z);
	}

	template <typename Ty2>
	inline Ty2 squaredis2D(const Vector3D<Ty2> &p) const
	{
		return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);
	}

	template <typename Ty2>
	inline Ty2 plansquaredis(const Vector3D<Ty2> &p) const
	{
		return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);
	}

	inline void normalize()
	{
		float len = distance();
		if (len == 0.0f)
		{
			x = y = z = 0;
			return;
		}
		x /= len;
		y /= len;
		z /= len;
	}

	inline Vector3D<Ty> normalized() const
	{
		Vector3D<Ty> tmp(*this);
		tmp.normalize();
		return tmp;
	}

	inline void toZero()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	inline bool isZero()
	{
		return x == 0 && y == 0 && z == 0;
	}

	template <typename Ty2>
	inline bool operator==(const Vector3D<Ty2>& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}

	template <typename Ty2>
	inline bool operator!=(const Vector3D<Ty2>& rhs) const
	{
		return !(*this == rhs);
	}
	template <typename Ty2>
	inline void operator =(const Vector3D<Ty2>& _coord)
	{
		x = _coord.x;
		y = _coord.y;
		z = _coord.z;
	}

	inline bool isValide()
	{
		return x >= 0 && y >= 0;
	}
};

typedef Vector2D<int>	Coord2D;
typedef Vector2D<float>	Coord2DF;

typedef Vector3D<int>	Coord3D;
typedef Vector3D<float>	Coord3DF;

typedef std::vector<Coord3D> VecCoord3D;

#endif /* COORD_H_ */
