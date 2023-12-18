#include <iostream>

#include "3DArray.h"

Matrix::Matrix(int x, int y, int z) : x_size_(x), y_size_(y), z_size_(z) {
    array_size_ = CalculateNumberOfInt16_t();
    array_ = new uint16_t[array_size_];
    
    // Заполняем массив нулями
    for (int i = 0; i < array_size_; ++i) { array_[i] = 0; }
}
    
Matrix::~Matrix() {
    delete [] array_;
}

Matrix Matrix::make_array(int x, int y, int z) {
    return Matrix(x, y, z);
}

Matrix Matrix::operator*(int rhs) {
    Matrix new_arr(x_size_, y_size_, z_size_);
    
    for (int i = 0; i < x_size_; ++i) {
        for (int j = 0; j < y_size_; ++j) {
            for (int k = 0; k < z_size_; ++k) {
                new_arr[i][j][k] = (*this)[i][j][k] * rhs;
            }
        }
    }
    
    return new_arr;
}

Matrix Matrix::operator+(const Matrix& rhs) {
    ArrayCompatibilityCheck(rhs);
    
    Matrix new_matrix(x_size_, y_size_, z_size_);
    
    for (int i = 0; i < x_size_; ++i) {
        for (int j = 0; j < y_size_; ++j) {
            for (int k = 0; k < z_size_; ++k) {
                new_matrix[i][j][k] = (*this)[i][j][k] + rhs[i][j][k];
            }
        }
    }
    
    return new_matrix;
}

Matrix Matrix::operator-(const Matrix& rhs) {
    ArrayCompatibilityCheck(rhs);
    
    Matrix new_matrix(x_size_, y_size_, z_size_);
    
    for (int i = 0; i < x_size_; ++i) {
        for (int j = 0; j < y_size_; ++j) {
            for (int k = 0; k < z_size_; ++k) {
                new_matrix[i][j][k] = (*this)[i][j][k] - rhs[i][j][k];
            }
        }
    }
    
    return new_matrix;
}

void Matrix::operator=(const Matrix& lhs) {
    for (int i = 0; i < array_size_; ++i) {
        array_[i] = lhs.array_[i];
    }

    array_size_ = lhs.array_size_;
    x_size_ = lhs.x_size_;
    y_size_ = lhs.y_size_;
    z_size_ = lhs.z_size_;
}


Matrix::Proxy::Proxy(const Matrix& matrix, int index) : matrix_(matrix), index_(index) {}

Matrix::Proxy::NextProxy::NextProxy(const Proxy& proxy, int index) : proxy_(proxy), meta_index_(index) {}

Matrix::Proxy::NextProxy& Matrix::Proxy::NextProxy::operator=(uint32_t value) {
    SetValue(value);

    return *this;
}

uint32_t Matrix::Proxy::NextProxy::operator*(const int& rhs) const {
    return GetValue() * rhs;
}

uint32_t Matrix::Proxy::NextProxy::operator+(const NextProxy& rhs) const {
    return (*this).GetValue() + rhs.GetValue();
}

uint32_t Matrix::Proxy::NextProxy::operator-(const NextProxy& rhs) const {
    return (*this).GetValue() - rhs.GetValue();
}

bool Matrix::Proxy::NextProxy::operator!=(const int& value) const {
    return (GetValue() != value);
}
bool Matrix::Proxy::NextProxy::operator==(const int& value) const {
    return (GetValue() == value);
}
            
std::istream& operator>>(std::istream& in, Matrix::Proxy::NextProxy& matrix) {
    uint32_t temp;
    
    in >> temp;
    matrix = temp;
    
    return in;
}

std::ostream& operator<<(std::ostream& os, const Matrix::Proxy::NextProxy& matrix) {
    os << matrix.GetValue();

    return os;
}

Matrix::Proxy::NextProxy& Matrix::Proxy::NextProxy::operator[](int index) {
    meta_index_ += index;
    return *this;
}

void Matrix::Proxy::NextProxy::SetValue(uint32_t value) {
    // Так как 17-битное число располагается в двух элементах сразу (в двух 16-битных числах),
    // то вычисляем индекс первого элемента
    int first_of_two_indexes = meta_index_ + meta_index_ / kBitsPerBlock;
    // Чтобы запрашиваемый index_ не выходил за границы [0, 16]
    int position = meta_index_ % kBitsPerBlock + 1;
    
    proxy_.matrix_.array_[first_of_two_indexes] &= ((1 << position) - 1) << (kBitsPerBlock - position);
    proxy_.matrix_.array_[first_of_two_indexes] |= value >> position;
    
    proxy_.matrix_.array_[first_of_two_indexes+1] &= ((~((1 << position) - 1) << (kBitsPerBlock - position)) & kMaximumSixteenBitNumber);
    proxy_.matrix_.array_[first_of_two_indexes+1] |= ((value & ((1 << position) - 1)) << (kBitsPerBlock - position));
}

uint32_t Matrix::Proxy::NextProxy::GetValue() const {
    // Так как 17-битное число располагается в двух элементах сразу (в двух 16-битных числах),
    // то вычисляем индекс первого элемента
    int first_of_two_indexes = (*this).meta_index_ + (*this).meta_index_ / kBitsPerBlock;
    // Чтобы запрашиваемый index_ не выходил за границы [0, 16]
    int position = (*this).meta_index_ % kBitsPerBlock + 1;

    
     uint32_t first_number_part = ((*this).proxy_.matrix_.array_[first_of_two_indexes] & ((1 << (kBitsPerBlock - position+1)) - 1)) << position;
     uint32_t second_number_part = (*this).proxy_.matrix_.array_[first_of_two_indexes+1] >> (kBitsPerBlock - position);
    
    return first_number_part + second_number_part;
}
        
Matrix::Proxy::NextProxy Matrix::Proxy::operator[](int sub_index) const {
    return NextProxy(*this, index_ + matrix_.z_size_ * sub_index);
}

Matrix::Proxy Matrix::operator[] (int index) const {
    return Proxy(*this, index * y_size_ * z_size_);
}

size_t Matrix::CalculateNumberOfInt16_t() const {
    return (x_size_ * y_size_ * z_size_) + (x_size_ * y_size_ * z_size_) / kBitsPerBlock + ((x_size_ * y_size_ * z_size_) % kBitsPerBlock != 0);
}

void Matrix::ArrayCompatibilityCheck(const Matrix& rhs) const {
    if (x_size_ != rhs.x_size_ || y_size_ != rhs.y_size_ || z_size_ != rhs.z_size_) {
        throw std::invalid_argument("Different array sizes\n");
    }
}
