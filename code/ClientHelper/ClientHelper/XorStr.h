#pragma once
template<class T> FORCEINLINE T __ROL__(T value, int count)
{
	const int nbits = sizeof(T) * 8;
	count %= nbits;
	T high = value >> (nbits - count);
	if (T(-1) < 0) // signed value
		high &= ~((T(-1) << count));
	value <<= count;
	value |= high;
	return value;
}
constexpr auto timeS = __TIME__;
constexpr auto seed = static_cast< int >(timeS[7]) + static_cast<int>(timeS[6]) + static_cast<int>(timeS[4]) + static_cast<int>(timeS[3]) + static_cast<int>(timeS[1]) + static_cast<int>(timeS[0]);
template < int N >
struct RandomGenerator
{
private:
	static constexpr unsigned a = 16807; // 7^5
	static constexpr unsigned m = 2147483647; // 2^31 - 1

	static constexpr unsigned s = RandomGenerator< N - 1 >::value;
	static constexpr unsigned lo = a * (s & 0xFFFF); // Multiply lower 16 bits by 16807
	static constexpr unsigned hi = a * (s >> 16); // Multiply higher 16 bits by 16807
	static constexpr unsigned lo2 = lo + ((hi & 0x7FFF) << 16); // Combine lower 15 bits of hi with lo's upper bits
	static constexpr unsigned hi2 = hi >> 15; // Discard lower 15 bits of hi
	static constexpr unsigned lo3 = lo2 + hi;

public:
	static constexpr unsigned max = m;
	static constexpr unsigned value = lo3 > m ? lo3 - m : lo3;
};
template <>// 0 + 6 + 4 + 3 + 
struct RandomGenerator< 0 >
{
	static constexpr unsigned value = seed;
};
template < int N, int M >
struct RandomInt
{
	static constexpr auto value = RandomGenerator< N + 1 >::value % M;
};
template < int N >
struct RandomChar
{
	static const char value = static_cast< char >(1 + RandomInt< N, 0x7F - 1 >::value);
};
template <int... Pack> struct IndexList {};

template <typename IndexList, int Right> struct Append;
template <int... Left, int Right> struct Append<IndexList<Left...>, Right> {
	typedef IndexList<Left..., Right> Result;
};
template <int N> struct ConstructIndexList {
	typedef typename Append<typename ConstructIndexList<N - 1>::Result, N - 1>::Result Result;
};
template <> struct ConstructIndexList<0> {
	typedef IndexList<> Result;
};

const char XORKEY_A = static_cast<char>(RandomChar<__COUNTER__>::value);

FORCEINLINE char EncryptCharacterA(const char Character, int Index, int vl) {
	return (Character ^ XORKEY_A) ^ (__ROL__(XORKEY_A, Index + vl) + Index);
}

template <typename IndexList> class CXorStringA;
template <int... Index> class CXorStringA<IndexList<Index...> > {
private:
	int vl = (int)XORKEY_A ^ 29;
	char Value[sizeof...(Index)+1];
public:
	FORCEINLINE constexpr CXorStringA(const char* const String)
		: Value{ EncryptCharacterA(String[Index], Index, vl)... } {}

	FORCEINLINE char* decrypt() {
		for (int t = 0; t < sizeof...(Index); t++) {
			Value[t] = (Value[t] ^ XORKEY_A) ^ (__ROL__(XORKEY_A, t + vl) + t);
		}
		Value[sizeof...(Index)] = '\0';
		return Value;
	}

	wchar_t* get() {
		return Value;
	}
};

#define XorStr( String ) ( CXorStringA<ConstructIndexList<sizeof( String ) - 1>::Result>( String ).decrypt() )  
