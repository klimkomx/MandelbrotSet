#ifndef _COMPLEX_NUM_
#define _COMPLEX_NUM_

#include <math.h>

template<typename T>
class complex
{
private:
public:
	T realPart, imaginePart;

	complex<T>(T real, T imagine) {
        realPart = real, imaginePart = imagine;
	}
    ~complex<T>() {
    }
    complex<T> operator+(complex<T> add) const {
        return complex<T>(this->realPart + add.realPart, this -> imaginePart + add.imaginePart);
    }
    complex<T> operator-(complex<T> add) const {
        return complex<T>(this->realPart - add.realPart, this -> imaginePart - add.imaginePart);
    }
    complex<T> operator*(complex<T> add) const {
        return complex<T>(this -> realPart * add.realPart - this -> imaginePart * add.imaginePart
                          , this -> imaginePart * add.realPart + this -> realPart * add.imaginePart);
    }
    complex<T> operator/(complex<T> add) const {
        return complex<T>((this -> realPart * add.realPart + this -> imaginePart * add.imaginePart) / (add.realPart * add.realPart + add.imaginePart * add.imaginePart),
                          (this -> imaginePart * add.realPart - this -> realPart * add.imaginePart) / (add.realPart * add.realPart + add.imaginePart * add.imaginePart));
    }

    double pointRadius() {
        return sqrt(this -> realPart * this -> realPart + this -> imaginePart * this -> imaginePart);
    }
};

#endif //_COMPLEX_NUM_
