#ifndef VEC_2_HH
#define VEC_2_HH

#include "vec.h"

namespace vl {
    /*
        *   class vec2 - derived from vec, implements features specilized for
        *   2D vectors only.
        */
    template<class T = float>
    class vec2 : public vec<T, 2> {
    public:
        vec2();
        //vec2(const vec2<T>& v) { *this = v; };
        vec2(const std::initializer_list<T>& l);

        template<typename _T>
        vec2<T>& operator=(const vec2<_T>&);
        vec2<T>& operator=(const vec2<T>&);
        template<typename _T>
        vec2<T>& operator=(const vec<_T, 2>&);
        vec2<T>& operator=(const vec<T, 2>&);

        template<typename ... Args> 
        vec2(const T& e1, const Args& ... par);

        T& x;
        T& y;
    };

    template<typename T>
    vec2<T>::vec2() :
        x(this->_data[0]),
        y(this->_data[1]) {
        std::cout << "PTR: " << (this->_data[0]);
    }

    template<typename T>
    vec2<T>::vec2(const std::initializer_list<T>& l) :
        vec<T, 2>(l),
        x(this->_data[0]),
        y(this->_data[1]) {}

    template<typename T>
    template<typename _T>
    vec2<T>& vec2<T>::operator=(const vec2<_T>& v) {
        this->_data[0] = v[0];
        this->_data[1] = v[1];
        return *this;
    }

    template<typename T>
    vec2<T>& vec2<T>::operator=(const vec2<T>& v) {
        this->_data[0] = v[0];
        this->_data[1] = v[1];
        return *this;
    }

    template<typename T>
    vec2<T>& vec2<T>::operator=(const vec<T, 2>& v) {
        this->_data[0] = v[0];
        this->_data[1] = v[1];
        return *this;
    }

    template<typename T>
    template<typename _T>
    vec2<T>& vec2<T>::operator=(const vec<_T, 2>& v) {
        this->_data[0] = v[0];
        this->_data[1] = v[1];
        return *this;
    }

    template<typename T>
    template<typename ... Args>
    vec2<T>::vec2(const T& e1, const Args& ... arg) :
        vec<T, 2>(e1, arg...),
        x(this->_data[0]),
        y(this->_data[1]) {}

    //using vec2i = vec2<int>;
    //using vec2f = vec2<float>;
    //using vec2d = vec2<double>;
}

#endif