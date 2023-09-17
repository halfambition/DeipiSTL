//
// Created by deipi on 2023/9/16.
//

#ifndef deipi_MATRIX_h
#define deipi_MATRIX_h
#include "Allocator.h"
#include "Uninitalized.h"
#include "ReverseIterator.h"
#include "Construct.h"
namespace DeipiSTL{
    //matrix iterator
    template <class T> //搞这么多主要是为了const迭代器
    struct matrix_iterator {
        typedef matrix_iterator<T>                      iterator;
        typedef matrix_iterator<T>                      const_iterator;

        typedef DeipiSTL::random_access_iterator_tag    iterator_category;
        typedef T                                       value_type;
        typedef T*                                      pointer;
        typedef T&                                      reference;
        typedef size_t                                  size_type;
        typedef ptrdiff_t                               difference_type;
        typedef T**                                     row_pointer;

    protected:
        //保持与容器的联结
        row_pointer p_row;                  //pointing to map
        pointer cur;
        pointer first;                      //buf head
        size_type column_size;              //size of one row
        void set_row(row_pointer _row){
            this->p_row = _row;
            first = *_row;
        }
        void set_size(size_type _n_row){
            this->n_row = _n_row;
        }
    public:
        matrix_iterator() : cur(0), first(0), column_size(0), p_row(0) { }
        matrix_iterator(const iterator& x):cur(x.cur), first(x.first), column_size(x.n_row), p_row(x.p_row) { }
        matrix_iterator(iterator&& x) noexcept :cur(x.cur), first(x.first), column_size(x.n_row), p_row(x.p_row) {
            x.cur = nullptr;
            x.first = nullptr;
            x.n_row = 0;
            x.p_row = nullptr;
        }
        matrix_iterator(row_pointer x, pointer y, size_type _column_size):p_row(x), cur(y), first(*x), column_size(_column_size) { }
        iterator& operator=(const iterator& x){
            if(x == this)
                return *this;
            this->cur = x.cur;
            this->n_row = x.n_row;
            this->first = x.first;
            this->p_row = x.p_row;
            return *this;
        }
        reference operator*()const{
            return *cur;
        }
        pointer operator->()const{
            return cur;
        }
        iterator operator++(){
            ++cur;
            if(cur == first + column_size){
                this->set_row(p_row+1);
                cur = first;
            }
            return *this;
        }
        iterator operator++(int){
            iterator temp(*this);
            ++cur;
            operator++();
            return cur;
        }
        iterator operator--(){
            if(cur == first) {
                set_row(p_row - 1);
                cur = first + column_size - 1;
            }
            else
                --cur;
            return *this;
        }
        iterator operator--(int) {
            iterator temp(*this);
            operator--();
            return temp;
        }
        bool operator==(iterator it){
            return cur == it.cur;
        }
        bool operator!=(iterator it){
            return cur != it.cur;
        }
        iterator operator+(int i){
            return matrix_iterator(p_row, first + i, first, column_size);
        }
    };

    template <typename T, typename first_level_Alloc = deipi_Alloc>
    class Matrix {
    public:
        ///type_traits
        typedef T				value_type;
        typedef T*				pointer;
        typedef const T*		const_pointer;
        typedef T&				reference;
        typedef const T&		const_reference;
        typedef T**             row_pointer;
        typedef const T**       const_row_pointer;
        typedef size_t		    size_type;
        //about iterator
        typedef matrix_iterator<T>           iterator;
        typedef matrix_iterator<T>           const_iterator;

    protected:
        typedef allocator<pointer , first_level_Alloc>      row_allocator;	    //default row allocator
        typedef allocator<T, first_level_Alloc>             column_allocator;	//default column allocator
        row_pointer             start;
        size_type               row{};
        size_type               colum{};
    protected:
        inline void allocate_aux(){
            start = row_allocator::Allocate(row);
            for (auto p = start; p<start+row; ++p){
                *p = column_allocator::Allocate(colum);
            }
        }
        inline void allocate_zero_matrix(){
            allocate_aux();
            for(auto it = this->begin(); it!= this ->end(); ++it){
                *it = 0;
            }
        }
        inline void allocate_and_copy(const Matrix& mat){
            this->start = mat.start;
            this->colum = mat.colum;
            this->row = mat.row;
            allocate_aux();
            for(auto it1 = this->begin(), it2 = mat->begin(); it1!= this ->end(); ++it1, ++it2){
                *it1 = *it2;
            }
        }
        inline void allocate_and_move(Matrix&& mat){
            deallocate();
            this->start = mat.start;
            this->colum = mat.colum;
            this->row = mat.row;
            mat.start = nullptr;
            mat.colum = 0;
            mat.row = 0;
        }
        inline void allocate_and_copy_2D_array(row_pointer p){
            allocate_aux();
            auto it = this->begin();
            for(int i = 0; i < row; ++i) {
                for(int j=0; j<colum; ++j){
                    *it = p[i][j];
                }
            }
        }
        inline void deallocate(){
            iterator end(start + row, *start, colum);
            for (iterator p(start, *start, colum); p<end; ++p){
                DeipiSTL::Destroy(p, p+colum);
                column_allocator::Deallocate(p, colum);
            }
            column_allocator::Deallocate(start, row);
        }
    public:
        Matrix():start(nullptr), row(0), colum(0) { }               //default create unitary matrix
        explicit Matrix(size_type n):row(n), colum(n){                    //nxn zero matrix
            allocate_zero_matrix();
        }
        Matrix(size_type r, size_type c):start(nullptr), row(r), colum(c) {
            allocate_zero_matrix();
        }
        Matrix(size_type r, size_type c, row_pointer p):start(nullptr), row(r), colum(c) {
            allocate_and_copy_2D_array(p);
        }
        Matrix(const Matrix& mat){
            allocate_and_copy(mat);
        }
        Matrix(Matrix&& mat) noexcept {
            allocate_and_move(DeipiSTL::move(mat));
        }
        ~Matrix(){
            deallocate();
        }
        Matrix<T, first_level_Alloc>& operator=(const Matrix& mat){
            if(&mat==this){
                return *this;
            }
            allocate_and_copy(mat);
            return *this;
        }
        Matrix<T, first_level_Alloc>& operator=(Matrix&& mat) noexcept {
            if(&mat==this){
                return *this;
            }
            allocate_and_move(DeipiSTL::move(mat));
            return *this;
        }
    public:
        inline reference operator()(size_type row_index, size_type column_index) const{
            if(row_index >= row || column_index >= colum)
                throw "out of range";
            return *(*(start + row_index) + column_index);
        }
        inline iterator begin(){
            return iterator(start, *start, colum);
        }
        inline iterator end(){
            return iterator(start+row-1, (*start) + colum, colum);
        }
        inline iterator rbegin() const{
            return reverse_iterator<iterator>(end());
        }
        inline iterator rend() const{
            return reverse_iterator<iterator>(begin());
        }
        inline const_iterator cbegin() const {
            return iterator(start, *start, colum);
        }
        inline const_iterator cend() const {
            return iterator(start+row-1, (*start) + colum, colum);
        }
        size_type get_row() const{
            return row;
        }
        size_type get_column() const{
            return colum;
        }
    public:
        inline bool equal(Matrix& mat) const{
            return this->row==mat.row&&this->colum==mat.colum;
        }
        inline Matrix<T, first_level_Alloc> operator+(Matrix& mat) const{
            if(!this->equal(mat))
                throw "bad add";
            Matrix<T, first_level_Alloc> new_mat(mat);
            for(auto it1 = this->begin(), it2 = new_mat->begin(); it1!= this ->end(); ++it1, ++it2){
                *it2+=*it1;
            }
            return new_mat;
        }
        inline Matrix<T, first_level_Alloc> operator-(Matrix& mat) const{
            if(!this->equal(mat))
                throw "bad add";
            Matrix<T, first_level_Alloc> new_mat(mat);
            for(auto it1 = this->begin(), it2 = new_mat->begin(); it1!= this ->end(); ++it1, ++it2){
                *it2-=*it1;
            }
            return new_mat;
        }
        inline Matrix<T, first_level_Alloc> operator*(Matrix& mat) const{
            bool cond = this->row != mat.colum || this->colum != mat.row;
            if(cond)
                throw "bad add";
            Matrix<T, first_level_Alloc> new_mat(mat);
            for(int i=0; i<row; ++i){
                for(int j=0; j<colum; ++j){
                    new_mat(i, j) *= this->operator()(j, i);
                }
            }
            return new_mat;
        }
        inline Matrix<T, first_level_Alloc> transpose() const{
            Matrix<T, first_level_Alloc> new_mat(colum, row);
            for(int i=0; i<row; ++i){
                for(int j=0; j<colum; ++j){
                    new_mat(i, j) = this->operator()(j, i);
                }
            }
            return new_mat;
        }
    };
    template <typename T, typename first_level_Alloc = deipi_Alloc>
    inline Matrix<T, first_level_Alloc> operator+(const Matrix<T, first_level_Alloc>& mat, T n){
        Matrix<T, first_level_Alloc> new_mat(mat);
        for(auto& a : new_mat){
            a+=n;
        }
        return new_mat;
    }
    template <typename T, typename first_level_Alloc = deipi_Alloc>
    inline Matrix<T, first_level_Alloc> operator+(T n, const Matrix<T, first_level_Alloc>& mat){
        Matrix<T, first_level_Alloc> new_mat(mat);
        for(auto& a : new_mat){
            a+=n;
        }
        return new_mat;
    }
    template <typename T, typename first_level_Alloc = deipi_Alloc>
    inline Matrix<T, first_level_Alloc> operator*(const Matrix<T, first_level_Alloc>& mat, T n){
        Matrix<T, first_level_Alloc> new_mat(mat);
        for(auto& a : new_mat){
            a*=n;
        }
        return new_mat;
    }
    template <typename T, typename first_level_Alloc = deipi_Alloc>
    inline Matrix<T, first_level_Alloc> operator*(T n, const Matrix<T, first_level_Alloc>& mat){
        Matrix<T, first_level_Alloc> new_mat(mat);
        for(auto& a : new_mat){
            a*=n;
        }
        return new_mat;
    }
}
#endif //deipi_MATRIX_h
