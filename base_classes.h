#ifndef BASE_CLASSES_H

#define BASE_CLASSES_H

#include <math.h>

class Vector3;

class Vertex3 {
public:
	Vertex3(double a = 0, double b = 0, double c = 0);
	Vertex3(const Vertex3 &vertex);
	Vertex3 operator=(const Vertex3 &vertex);
	Vertex3 operator+(const Vector3 &vector) const;
	void operator+=(const Vector3 &vector);
	Vector3 operator-(const Vertex3 &vertex) const;
	double X() const;
	double Y() const;
	double Z() const;

private:
	double x;
	double y;
	double z;
};

class Vector3 {
public:
	Vector3(double a = 0, double b = 0, double c = 0);
	Vector3(const Vector3 &vector);
	Vector3 operator=(const Vector3 &vector);
	Vector3 operator+(const Vector3 &vector) const;
	Vector3 operator-(const Vector3 &vector) const;
	void operator+=(const Vector3 &vector);
	void operator-=(const Vector3 &vector);
	double length() const;
	double dot(const Vector3 &vector) const;
	Vector3 operator*(const Vector3 &vector) const;
	Vector3 operator*(double a) const;
	Vector3 normalize() const;
	double X() const;
	double Y() const;
	double Z() const;

private:
	double x;
	double y;
	double z;
};

inline Vertex3::Vertex3(const Vertex3 &vertex) {
	x = vertex.x;
	y = vertex.y;
	z = vertex.z;
}

inline Vertex3::Vertex3(double a, double b, double c)
{
	x = a;
	y = b;
	z = c;
}

inline Vertex3 Vertex3::operator=(const Vertex3 &vertex)
{
	x = vertex.x;
	y = vertex.y;
	z = vertex.z;
	return *this;
}

inline Vertex3 Vertex3::operator+(const Vector3 &vector) const
{
	return Vertex3(x + vector.X(), y + vector.Y(), z + vector.Z());
}

inline void Vertex3::operator+=(const Vector3 &vector)
{
	x += vector.X();
	y += vector.Y();
	z += vector.Z();
}

inline Vector3 Vertex3::operator-(const Vertex3 &vertex) const
{
	return Vector3(x - vertex.X(), y - vertex.Y(), z - vertex.Z());
}

inline double Vertex3::X() const
{
	return x;
}

inline double Vertex3::Y() const
{
	return y;
}

inline double Vertex3::Z() const
{
	return z;
}



inline Vector3::Vector3(const Vector3 &vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
}

inline Vector3::Vector3(double a, double b, double c)
{
	x = a;
	y = b;
	z = c;
}

inline Vector3 Vector3::operator=(const Vector3 &vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	return *this;
}

inline Vector3 Vector3::operator+(const Vector3 &vector) const
{
	return Vector3(x + vector.x, y + vector.y, z + vector.z);
}

inline Vector3 Vector3::operator-(const Vector3 &vector) const
{
	return Vector3(x - vector.x, y - vector.y, z - vector.z);
}

inline void Vector3::operator+=(const Vector3 &vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
}

inline void Vector3::operator-=(const Vector3 &vector)
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
}

inline double Vector3::length() const
{
	return sqrt(x * x + y * y + z * z);
}

inline double Vector3::dot(const Vector3 &vector) const
{
	return x * vector.x + y * vector.y + z * vector.z;
}

inline Vector3 Vector3::operator*(const Vector3 &vector) const
{
	return Vector3(
			y * vector.z - z * vector.y,
			z * vector.x - x * vector.z,
			x * vector.y - y * vector.x);
}

inline Vector3 Vector3::operator*(double a) const
{
	return Vector3(x * a, y * a, z * a);
}

inline Vector3 Vector3::normalize() const
{
	double len = length();
	return Vector3(x / len, y / len, z / len);
}

inline double Vector3::X() const
{
	return x;
}

inline double Vector3::Y() const
{
	return y;
}

inline double Vector3::Z() const
{
	return z;
}


template<class T>
class list {
public:
	list()
	{
		front = 0;
		back = 0;
	}

	list(const list &lst)
	{
		front = back = 0;
		const item *tmp = lst.front;
		while (tmp) {
			push_back(tmp->elem);
			tmp = tmp->next;
		}
	}

	~list()
	{
		while (front) {
			item *tmp = front->next;
			delete front;
			front = tmp;
		}
	}

	bool empty()
	{
		return !front;
	}

	void push_back(const T &e)
	{
		if (front) {
			back->next = new item;
			back = back->next;
			back->next = 0;
			back->elem = e;
		} else {
			front = back = new item;
			back->next = 0;
			back->elem = e;
		}
	}

	void operator+=(const list<T> &lst)
	{
		const item *tmp = lst.front;
		while (tmp) {
			push_back(tmp->elem);
			tmp = tmp->next;
		}
	}

	T pop_front() {
		T e = front->elem;
		if (front == back) {
			delete front;
			front = back = 0;
		} else {
			item *tmp = front;
			front = front->next;
			delete tmp;
		}
		return e;
	}

private:
	struct item {
		T elem;
		item *next;
	};
	item *front;
	item *back;
	void operator=(const list &lst)
	{ }
};



template<class T>
class Matrix3x3 {
public:
	Matrix3x3(T a11, T a12, T a13, T a21, T a22, T a23, T a31, T a32, T a33)
	{
		cont[0][0] = a11;
		cont[0][1] = a12;
		cont[0][2] = a13;
		cont[1][0] = a21;
		cont[1][1] = a22;
		cont[1][2] = a23;
		cont[2][0] = a31;
		cont[2][1] = a32;
		cont[2][2] = a33;
	}

	T det()
	{
		return cont[0][0] * cont[1][1] * cont[2][2] +
				cont[0][1] * cont[1][2] * cont[2][0] +
				cont[0][2] * cont[1][0] * cont[2][1] -
				cont[0][2] * cont[1][1] * cont[2][0] -
				cont[0][1] * cont[1][0] * cont[2][2] -
				cont[0][0] * cont[1][2] * cont[2][1];
	}

private:
	T cont[3][3];
};

template<class T>
class Stack {
public:
	Stack()
	{
		head = 0;
	}

	~Stack()
	{
		clear();
	}

	bool empty() const
	{
		return !head;
	}

	void clear()
	{
		while (head) {
			elem *tmp = head;
			head = head->next;
			delete tmp;
		}
	}

	void push(const T &e)
	{
		elem *tmp = head;
		head = new elem(e);
		head->next = tmp;
	}

	T pop()
	{
		elem *tmp = head;
		head = head->next;
		T i = tmp->item;
		delete tmp;
		return i;
	}

	T get(int idx = 0) const
	{
		elem *tmp = head;
		while (idx > 0) {
			tmp = tmp->next;
			--idx;
		}
		return tmp->item;
	}

private:
	struct elem {
		T item;
		elem *next;

		elem(const T &e): item(e)
		{ }	
	};
	elem *head;

	Stack(const Stack &s)
	{ }

	void operator=(const Stack &s)
	{ }
};

#endif
