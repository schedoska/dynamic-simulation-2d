#ifndef MAT_22_HH
#define MAT_22_HH

#include "mat.h"
#include "vec2.h"

namespace vl{
/*
    *   class mat2 - derived from mat, implements features specilized for
    *   2x2 matices only.
    */
    template<class T = float>
    class mat22 : public mat<T, 2, 2> {
    public:
        mat22();
        mat22(const T& v_00,
              const T& v_01, 
              const T& v_10, 
              const T& v_11);

        template<typename _T>
        vec2<T> operator*(const vec2<_T>& v) const; 
        mat22<T>& inv();
        T det(); 

        template<typename _T>
        static mat22<T> rot_mat(const _T& r);
    };

    template<typename T>
    mat22<T>::mat22() : 
        mat<T, 2, 2>() {}

    template<typename T>
    mat22<T>::mat22(const T& v_00,
              const T& v_01, 
              const T& v_10, 
              const T& v_11)
    {
        this->_data[0][0] = v_00;
        this->_data[0][1] = v_01;
        this->_data[1][0] = v_10;
        this->_data[1][1] = v_11; 
    }

    template<typename T>
    inline T mat22<T>::det() {
        return this->_data[0][0] * this->_data[1][1] - 
               this->_data[0][1] * this->_data[1][0];
    }

    template<typename T>
    mat22<T>& mat22<T>::inv() {
        T d = det();
        assert((d != static_cast<T>(0.0)) && "Matrix is singular, can not be inverted");
        std::swap(this->_data[0][0], this->_data[1][1]);
        this->_data[1][0] *= static_cast<T>(-1.0);
        this->_data[0][1] *= static_cast<T>(-1.0);
        *this /= d;
        return *this;
    }

    template<typename T>
    template<typename _T>
    vec2<T> mat22<T>::operator*(const vec2<_T>& v) const {
        T vr1 = this->_data[0][0] * v[0] + this->_data[0][1] * v[1];
        T vr2 = this->_data[1][0] * v[0] + this->_data[1][1] * v[1];
        return vec2<T>(vr1, vr2);
    }

    template<typename T>
    template<typename _T>
    mat22<T> mat22<T>::rot_mat(const _T& r) {
        static_assert(std::is_arithmetic<_T>::value, "Not an arithmetic type");
        return mat22<T>(std::cos(r), -std::sin(r),
                        std::sin(r), std::cos(r));
    }

    using mat22f = mat22<float>;
    using mat22i = mat22<int>;
    using mat22d = mat22<double>;
}

#endif