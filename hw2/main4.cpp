#include <concepts>
#include <iostream>
#include <string>
#include <utility>

namespace detail {
template <std::integral Int>
constexpr static int next_pow2(Int x) {
    [[assume(x > 0)]];
    return x == 1 ? 1
                  : 1 << (CHAR_BIT * sizeof(Int) -
                          std::countl_zero(static_cast<std::make_unsigned_t<Int>>(x - 1)));
}
}  // namespace detail

template <typename T>
struct Vector {
public:
    Vector() : m_data{}, m_capacity{}, m_size{} {}
    Vector(int size) : Vector(size, T{}) {}
    Vector(int size, const T& value) {
        m_size = size;
        if (size > 0) {
            m_capacity = detail::next_pow2(size);
            m_data = new T[m_capacity];
            for (int i{0}; i < m_capacity; i++) {
                m_data[i] = value;
            }
        } else {
            m_capacity = 0;
            m_data = nullptr;
        }
    }
    Vector(const Vector<T>& other) {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        if (m_capacity > 0) {
            m_data = new T[m_capacity];
            for (int i{0}; i < m_capacity; i++) {
                m_data[i] = other.m_data[i];
            }
        } else {
            m_data = nullptr;
        }
    }
    Vector(Vector<T>&& other) : Vector() {
        this->swap(other);
    }
    ~Vector() {
        delete[] m_data;
    }

    // copy-and-swap here
    Vector<T>& operator=(Vector<T> other) {
        this->swap(other);
        return *this;
    }

    int size() const {
        return m_size;
    }

    // do not write duplicate code!
    // (std::forward_like not ready yet)
    template <typename Self>
    std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>, const T&, T&> operator[](
        this Self&& self, int index) {
        return self.m_data[index];
    }

    void swap(Vector<T>& other) {
        std::swap(m_size, other.m_size);
        std::swap(m_capacity, other.m_capacity);
        std::swap(m_data, other.m_data);
    }

private:
    T* m_data;
    int m_capacity;
    int m_size;
};

int main() {
    Vector<int> a(10);
    for (int i = 0; i < a.size(); i++) {
        a[i] = i + 1;
    }
    const Vector<int> b = a;
    for (int i = 0; i < b.size(); i++) {
        std::cout << b[i] << std::endl;  //
    }
}