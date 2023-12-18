#pragma once

#include <iostream>

static const int kBitsPerBlock = 16;
static const int kMaximumSixteenBitNumber = 65535;

class Matrix {
public:
    Matrix(int x, int y, int z);
    ~Matrix();
    
    static Matrix make_array(int x, int y, int z);
    
    Matrix operator*(int rhs);
    Matrix operator+(const Matrix& rhs);
    Matrix operator-(const Matrix& rhs);
    void operator=(const Matrix& lhs);
    
    class Proxy {
    public:
        Proxy(const Matrix& matrix, int index);
        
        class NextProxy {
        public:
            NextProxy(const Proxy& proxy, int index);
            
            NextProxy& operator=(uint32_t value);
            
            uint32_t operator*(const int& rhs) const;
            uint32_t operator+(const NextProxy& rhs) const;
            uint32_t operator-(const NextProxy& rhs) const;
            
            bool operator!=(const int& value) const;
            bool operator==(const int& value) const;
            
            friend std::istream& operator>>(std::istream& in, NextProxy& matrix);
            friend std::ostream& operator<<(std::ostream& os, const NextProxy& matrix);
            
            NextProxy& operator[](int index);
            
        private:
            const Proxy& proxy_;
            int meta_index_;
            
            void SetValue(uint32_t value);
            uint32_t GetValue() const;
        };
        
        NextProxy operator[](int sub_index) const;
        
    private:
        const Matrix& matrix_;
        int index_;
    };
    
    Proxy operator[] (int index) const;
    
private:
    uint16_t* array_;
    size_t array_size_;
    int x_size_;
    int y_size_;
    int z_size_;
    
    size_t CalculateNumberOfInt16_t() const;
    void ArrayCompatibilityCheck(const Matrix& rhs) const;
};
