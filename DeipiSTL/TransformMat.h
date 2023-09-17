//
// Created by deipi on 2023/9/17.
//

#ifndef deipi_TRANSFORMMAT_h
#define deipi_TRANSFORMMAT_h
#include "Matrix.h"
#include <cmath>
namespace DeipiSTL{
    template <size_t dimension, typename T>
    class Mat{
    public:
        ///type_traits
        typedef typename Matrix<T>::value_type			value_type;
        typedef typename Matrix<T>::pointer			    pointer;
        typedef typename Matrix<T>::const_pointer		const_pointer;
        typedef typename Matrix<T>::reference			reference;
        typedef typename Matrix<T>::const_reference	    const_reference;
        typedef typename Matrix<T>::row_pointer         row_pointer;
        typedef typename Matrix<T>::const_row_pointer   const_row_pointer;
        typedef typename Matrix<T>::size_type		    size_type;
        //about iterator
        typedef typename Matrix<T>::iterator            iterator;
        typedef typename Matrix<T>::const_iterator      const_iterator;
    protected:
        Matrix<T> matrix;

        void copy_from_Matrix(const Mat& mat){
            for (int i = 0; i < dimension; ++i) {
                for(int j = 0; j < dimension; ++j){
                    if(i>mat.get_row()||j>mat.get_column())
                        matrix(i, j) = 0;
                    else
                        matrix(i, j) = mat(i, j);
                }
            }
        }
    public:
        Mat():matrix(dimension){ }
        explicit Mat(const T& n):matrix(dimension){         //make identity matrix
            for (int i = 0; i < dimension; ++i) {
                matrix(i, i) = n;
            }
        }
        Mat(const Mat& mat):matrix(mat){ }
        explicit Mat(const Matrix<T>& mat):matrix(dimension){
            copy_from_Matrix(mat);
        }
        Mat(Mat&& mat) noexcept :matrix(DeipiSTL::move(mat)){ }
        Mat<dimension, T>& operator=(const Mat& mat){
            if(&mat==this)
                return *this;
            matrix = mat.matrix;
        }
        Mat<dimension, T>& operator=(Mat&& mat) noexcept {
            if(&mat==this)
                return *this;
            matrix = DeipiSTL::move(mat.matrix);
        }
        ~Mat()= default;
    public:
        inline Mat<dimension, T>&& operator+(const Mat& mat){
            return DeipiSTL::forward(matrix + mat.matrix);
        }
        inline Mat<dimension, T>&& operator-(const Mat& mat){
            return DeipiSTL::forward(matrix - mat.matrix);
        }
        inline Mat<dimension, T>&& operator*(const Mat& mat){
            return DeipiSTL::forward(matrix * mat.matrix);
        }
        inline Mat<dimension, T> transpose(){
            return matrix.transpose();
        }
        inline Mat<dimension, T>&& operator+(const T& t){
            return DeipiSTL::forward(matrix + t);
        }
        inline Mat<dimension, T>&& operator-(const T& t){
            return DeipiSTL::forward(matrix - t);
        }
        inline Mat<dimension, T>&& operator*(const T& t){
            return DeipiSTL::forward(matrix * t);
        }
        template<size_t D, typename T1>
        friend Mat<D, T1>&& operator+(const T& t, const Mat<D, T1>& mat);
        template<size_t D, typename T1>
        friend Mat<D, T1>&& operator-(const T& t, const Mat<D, T1>& mat);
        template<size_t D, typename T1>
        friend Mat<D, T1>&& operator*(const T& t, const Mat<D, T1>& mat);
    public:
        inline size_type get_dimension(){
            return dimension;
        }
        inline reference operator()(size_type i, size_type j){
            return matrix(i, j);
        }
    };

    template<size_t D, typename T1>
    Mat<D, T1>&& operator+(const T1 &t, const Mat<D, T1>& mat) {
        return DeipiSTL::forward(mat.matrix + t);
    }
    template<size_t D, typename T1>
    Mat<D, T1>&& operator-(const T1 &t, const Mat<D, T1>& mat) {
        return DeipiSTL::forward(mat.matrix - t);
    }template<size_t D, typename T1>
    Mat<D, T1>&& operator*(const T1 &t, const Mat<D, T1>& mat) {
        return DeipiSTL::forward(mat.matrix * t);
    }
}


namespace DeipiSTL{
    template <size_t dimension, typename T, typename Alloc = allocator<T, deipi_Alloc>>
    class Vec{
    public:
        ///type_traits
        typedef T			    value_type;
        typedef T*			    pointer;
        typedef const T*		const_pointer;
        typedef	T&      		reference;
        typedef const T&        const_reference;
        typedef size_t  	    size_type;
    private:
        typedef Alloc           data_allocator;	//default space allocator
        pointer start;
    private:
        inline void init_vec(const T& t){
            for(int i=0;i<dimension;++i){
                DeipiSTL::Construct(start+i, t);
            }
        }
        inline void deallocate_and_destroy(){
            DeipiSTL::Destroy(start, start + dimension);
            data_allocator::Deallocate(start, dimension);
        }
        inline void allocate_and_copy(const Vec& vec){
            start = data_allocator::Allocate(dimension);
            for(int i=0; i<dimension; ++i){
                DeipiSTL::Construct(start+i, vec[i]);
            }
        }
        inline void allocate_and_move(Vec&& vec){
            deallocate_and_destroy();
            start = vec.start;
            vec.start = nullptr;
        }
    public:
        Vec():start(nullptr){
            start = data_allocator::Allocate(dimension);
            init_vec(0);
        }
        explicit Vec(T t):start(nullptr){
            start = data_allocator::Allocate(dimension);
            init_vec(t);
        }
        Vec(T t1, T t2, T t3=0, T t4=0):start(nullptr){
            start = data_allocator::Allocate(dimension);
            DeipiSTL::Construct(start, t1);
            if (dimension >= 2)
                DeipiSTL::Construct(start+1, t2);
            if (dimension >= 3)
                DeipiSTL::Construct(start+2, t3);
            if (dimension >= 4)
                DeipiSTL::Construct(start+3, t4);
        }
        Vec(const Vec& vec):start(nullptr){
            allocate_and_copy(vec);
        }
        Vec(Vec&& vec) noexcept :start(nullptr){
            allocate_and_move(DeipiSTL::move(vec));
        }
        Vec(Vec<dimension-1, T> vec, T n){
            start = data_allocator::Allocate(dimension);
            for(int i=0; i<dimension-1; ++i){
                DeipiSTL::Construct(start+i, vec[i]);
            }
            DeipiSTL::Construct(start+dimension-1, n);
        }
        Vec& operator=(const Vec& vec){
            if(&vec==this)
                return *this;
            allocate_and_copy(vec);
        }
        Vec& operator=(Vec&& vec) noexcept {
            if(&vec==this)
                return *this;
            allocate_and_move(DeipiSTL::move(vec));
        }
        ~Vec(){
            deallocate_and_destroy();
        }
    public:
        inline size_type get_dimension() const{
            return dimension;
        }
        inline reference operator[](size_type n) const{
            if(n >= dimension)
                throw "out of range";
            return *(start + n);
        }
        inline Vec operator+(const Vec& v) const{
            Vec<dimension, T> new_vec(v);
            for(int i=0;i<dimension;++i){
                new_vec[i] += *this[i];
            }
            return new_vec;
        }
        inline Vec operator-(const Vec& v) const{
            Vec<dimension, T> new_vec(v);
            for(int i=0;i<dimension;++i){
                new_vec[i] -= *this[i];
            }
            return new_vec;
        }
        inline T dot(const Vec& v) const{
            T res = 0;
            for(int i=0;i<dimension;++i){
                res += (*this[i] * v[i]);
            }
            return res;
        }
        inline Vec cross(const Vec& v) const{
            if(dimension!=3)
                throw "error dimension";
            Vec<dimension, T> res;
            res[0] = (*this)[1] * v[2] - (*this)[2] * v[1];
            res[1] = (*this)[2] * v[0] - (*this)[0] * v[2];
            res[2] = (*this)[0] * v[1] - (*this)[1] * v[0];
            return res;
        }
        T modulus() const{
            T res = 1;
            for(int i=0;i<dimension;++i){
                res += ((*this)[i] * (*this)[i]);
            }
            return res;
        }
    };
    using vec3 = Vec<3, float>;
    using vec4 = Vec<4, float>;
}

namespace DeipiSTL{
    //operation of vector with Matrix
    double degree_transform(double degree){
        return degree * M_PI / 180.0;
    }
    template <size_t dimension, typename T>
    Vec<dimension, T> operator*(Vec<dimension, T> vec, Mat<dimension, T> mat){
        Vec<dimension, T> res;
        for(int i=0;i<dimension; ++i){
            for (int j = 0; j < dimension; ++j) {
                res[i] = vec[j] * mat(j, i);
            }
        }
        return res;
    }
    template <size_t dimension, typename T>
    Vec<dimension, T> operator*(const Mat<dimension, T>& mat, const Vec<dimension, T>& vec){
        Vec<dimension, T> res;
        for(int i=0;i<dimension; ++i){
            for (int j = 0; j < dimension; ++j) {
                res[i] = mat(i, j) * vec[j];
            }
        }
        return res;
    }
    template <size_t dimension, typename T>
    Mat<dimension, T> translate(const Mat<dimension, T>& mat, const Vec<dimension-1, T>& vec){
        Mat<dimension, T> res(1);
        for(int i=0;i<dimension-1;++i){
            res(i, dimension-1) = vec[i];
        }
        res = res * mat;
        return res;
    }
    template <typename T>       //2D use normal formula
    Mat<3, T> rotate(const Mat<3, T>& mat, double theta){        //axis is the normal vector
        Mat<3, T> res(1);
        res(0, 0) = cos(theta);
        res(1, 1) = cos(theta);
        res(0, 1) = -sin(theta);
        res(1, 0) = sin(theta);
        res = res * mat;
        return res;
    }
    template <typename T>       //3D use Rodriguez formula
    Mat<4, T> rotate(const Mat<4, T>& mat, double theta, const Vec<3, T>& axis){        //axis is the normal vector
        Mat<4, T> res(1);
        res = res * cos(theta);
        Mat<4, T> N;
        N(0, 1) = -axis[2];
        N(0, 2) = axis[1];
        N(1, 0) = axis[2];
        N(1, 2) = -axis[0];
        N(2, 0) = -axis[1];
        N(2, 1) = axis[0];
        res = res + (1 - (N*N*cos(theta)));
        res = res + (sin(theta) * N);
        res(3, 3) = 1;
        return res;
    }
    template <size_t dimension, typename T>
    Mat<dimension, T> scale(const Mat<dimension, T>& mat, const Vec<dimension-1, T>& vec){
        Mat<dimension, T> res(1);
        for(int i=0;i<dimension-1;++i){
            res(i,i) = vec[i];
        }
        res = res * mat;
        return res;
    }
}

#endif //deipi_TRANSFORMMAT_h
