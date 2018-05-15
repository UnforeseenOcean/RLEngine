template <typename T>
struct Pair {
	/// Constructs a pair from given values.
	inline Pair(T x, T y);

	/// Adds another pair of values component-wise.
	inline Pair<T> operator+(const Pair<T>& other);

	/// Subtracts another pair of values component-wise.
	inline Pair<T> operator-(const Pair<T>& other);

	/// Sets the values of the pair.
	inline void Set(T x, T y);

	T x;
	T y;
};

#include "Pair.inl"

typedef Pair<int> PairInt;

typedef Pair<float> PairFloat;