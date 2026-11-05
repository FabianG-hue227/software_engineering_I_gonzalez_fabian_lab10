#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

class Matrix {
private:
    int size_;
    int* data_;

public:
    Matrix() : size_(0), data_(nullptr) {}

    explicit Matrix(int n) : size_(n), data_(new int[n * n]()) {}

    Matrix(const Matrix& other) : size_(other.size_), data_(new int[other.size_ * other.size_]) {
        std::copy(other.data_, other.data_ + size_ * size_, data_);
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            Matrix temp(other);
            std::swap(size_, temp.size_);
            std::swap(data_, temp.data_);
        }
        return *this;
    }

    ~Matrix() { delete[] data_; }

    int getSize() const { return size_; }
    bool isValidIndex(int i) const { return i >= 0 && i < size_; }
    int get(int row, int col) const { return data_[row * size_ + col]; }
    void set(int row, int col, int value) { data_[row * size_ + col] = value; }

    bool readFrom(std::istream& in) {
        for (int i = 0; i < size_ * size_; ++i)
            if (!(in >> data_[i])) return false;
        return true;
    }

    Matrix operator+(const Matrix& rhs) const {
        Matrix result(size_);
        for (int i = 0; i < size_ * size_; ++i)
            result.data_[i] = data_[i] + rhs.data_[i];
        return result;
    }

    Matrix operator*(const Matrix& rhs) const {
        Matrix result(size_);
        for (int i = 0; i < size_; ++i)
            for (int j = 0; j < size_; ++j)
                for (int k = 0; k < size_; ++k)
                    result.data_[i * size_ + j] += get(i, k) * rhs.get(k, j);
        return result;
    }

    int mainDiagonalSum() const {
        int sum = 0;
        for (int i = 0; i < size_; ++i) sum += get(i, i);
        return sum;
    }

    int secondaryDiagonalSum() const {
        int sum = 0;
        for (int i = 0; i < size_; ++i) sum += get(i, size_ - 1 - i);
        return sum;
    }

    void swapRows(int r1, int r2) {
        for (int j = 0; j < size_; ++j)
            std::swap(data_[r1 * size_ + j], data_[r2 * size_ + j]);
    }

    void swapColumns(int c1, int c2) {
        for (int i = 0; i < size_; ++i)
            std::swap(data_[i * size_ + c1], data_[i * size_ + c2]);
    }
};

std::ostream& operator<<(std::ostream& out, const Matrix& m) {
    for (int i = 0; i < m.getSize(); ++i) {
        for (int j = 0; j < m.getSize(); ++j)
            out << std::setw(4) << m.get(i, j);
        out << '\n';
    }
    return out;
}

Matrix swapRows(Matrix m, int r1 = 0, int r2 = 1) {
    if (m.isValidIndex(r1) && m.isValidIndex(r2))
        m.swapRows(r1, r2);
    else
        std::cerr << "Error: row index out of range\n";
    return m;
}

Matrix swapColumns(Matrix m, int c1 = 0, int c2 = 1) {
    if (m.isValidIndex(c1) && m.isValidIndex(c2))
        m.swapColumns(c1, c2);
    else
        std::cerr << "Error: column index out of range\n";
    return m;
}

Matrix updateElement(Matrix m, int row = 0, int col = 0, int value = 100) {
    if (m.isValidIndex(row) && m.isValidIndex(col))
        m.set(row, col, value);
    else
        std::cerr << "Error: index out of range\n";
    return m;
}

bool readMatricesFromFile(const std::string& filename, Matrix& a, Matrix& b) {
    std::ifstream file(filename);
    int n;
    if (!(file >> n) || n <= 0) {
        std::cerr << "Error: could not read matrix size from '" << filename << "'\n";
        return false;
    }
    a = Matrix(n);
    b = Matrix(n);
    if (!a.readFrom(file) || !b.readFrom(file)) {
        std::cerr << "Error: not enough matrix data in file\n";
        return false;
    }
    return true;
}

int main() {
    std::string filename;
    std::cout << "Enter input filename: ";
    std::getline(std::cin, filename);

    Matrix A, B;
    if (!readMatricesFromFile(filename, A, B))
        return 1;

    std::cout << "Matrix A:\n" << A;
    std::cout << "\nMatrix B:\n" << B;
    std::cout << "\nA + B:\n" << A + B;
    std::cout << "\nA * B:\n" << A * B;

    std::cout << "\nDiagonal sums for Matrix A:\n";
    std::cout << "Main diagonal sum: " << A.mainDiagonalSum() << '\n';
    std::cout << "Secondary diagonal sum: " << A.secondaryDiagonalSum() << '\n';

    std::cout << "\nProblem 5 - Rows 0 and 2 swapped:\n" << swapRows(A, 0, 2);
    std::cout << "\nProblem 6 - Columns 0 and 2 swapped:\n" << swapColumns(A, 0, 2);
    std::cout << "\nProblem 7 - Updated matrix:\n" << updateElement(A, 1, 2, 99);

    return 0;
}