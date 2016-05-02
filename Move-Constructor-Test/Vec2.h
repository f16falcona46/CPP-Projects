#ifndef JL_CPP_VECTOR_H
#define JL_CPP_VECTOR_H

#include <iostream>
#include <cmath>

namespace JLcpp {
template <typename T>
class Vec2 {
	private:
	T x;
	T y;
	public:
	Vec2() {
		x = 0;
		y = 0;
	}

	Vec2(T newx, T newy) {
		x = newx;
		y = newy;
	}


	T getX() const { return x; }
	T getY() const { return y; }
	void setX(T newx) { this->x = newx; }
	void setY(T newy) { this->y = newy; }
	T distance() const {
		return std::sqrt(x*x + y*y);
	}
	T angle() const {
		return std::atan2(y,x);
	}
	
	template <typename U>
	T dot(Vec2<U> other) const {
		return x*other.x + y*other.y;
	}
	//Vec2<T> rotate(T);
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vec2<T> v) {
	os << '(' << v.getX() << ',' << v.getY() << ')';
	return os;
}

}
#endif
