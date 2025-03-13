#ifndef VEC2_H
#define VEC2_H

#include <iostream>
#include <cmath>

class hpvec2 {
public:

    double x;
    double y;

    hpvec2() : x(0.0), y(0.0) {}

    hpvec2(double x_val, double y_val) : x(x_val), y(y_val) {}

    // Copy constructor
    hpvec2(const hpvec2& other) : x(other.x), y(other.y) {}

    double getX() const { return x; }
    double getY() const { return y; }

    void setX(double x_val) { x = x_val; }
    void setY(double y_val) { y = y_val; }

    double length() const {
        return std::sqrt(x * x + y * y);
    }

    double lengthSquared() const {
        return x * x + y * y;
    }

    hpvec2 normalized() const {
        double len = length();
        if (len < 1e-10) {
            return hpvec2(0.0, 0.0);
        }
        return hpvec2(x / len, y / len);
    }

    double dot(const hpvec2& other) const {
        return x * other.x + y * other.y;
    }

    hpvec2 operator+(const hpvec2& rhs) const {
        return hpvec2(x + rhs.x, y + rhs.y);
    }

    hpvec2 operator-(const hpvec2& rhs) const {
        return hpvec2(x - rhs.x, y - rhs.y);
    }

    hpvec2 operator*(double scalar) const {
        return hpvec2(x * scalar, y * scalar);
    }

    hpvec2 operator/(double scalar) const {
        if (std::abs(scalar) < 1e-10) {
            throw std::runtime_error("wtf bro");
        }
        return hpvec2(x / scalar, y / scalar);
    }

    hpvec2& operator+=(const hpvec2& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    hpvec2& operator-=(const hpvec2& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    hpvec2& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    hpvec2& operator/=(double scalar) {
        if (std::abs(scalar) < 1e-10) {
            throw std::runtime_error("wtf nig");
        }
        x /= scalar;
        y /= scalar;
        return *this;
    }

    bool operator==(const hpvec2& rhs) const {
        const double epsilon = 1e-10;
        return std::abs(x - rhs.x) < epsilon && std::abs(y - rhs.y) < epsilon;
    }

    bool operator!=(const hpvec2& rhs) const {
        return !(*this == rhs);
    }

    hpvec2 operator-() const {
        return hpvec2(-x, -y);
    }

    friend hpvec2 operator*(double scalar, const hpvec2& vec);
    friend std::ostream& operator<<(std::ostream& os, const hpvec2& vec);
};

inline hpvec2 operator*(double scalar, const hpvec2& vec) {
    return vec * scalar;
}

inline std::ostream& operator<<(std::ostream& os, const hpvec2& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

class lpvec2 {
public:

    float x;
    float y;

    lpvec2() : x(0.0), y(0.0) {}

    lpvec2(double x_val, double y_val) : x(x_val), y(y_val) {}

    // Copy constructor
    lpvec2(const lpvec2& other) : x(other.x), y(other.y) {}

    double getX() const { return x; }
    double getY() const { return y; }

    void setX(double x_val) { x = x_val; }
    void setY(double y_val) { y = y_val; }

    double length() const {
        return std::sqrt(x * x + y * y);
    }

    double lengthSquared() const {
        return x * x + y * y;
    }

    lpvec2 normalized() const {
        double len = length();
        if (len < 1e-10) {
            return lpvec2(0.0, 0.0);
        }
        return lpvec2(x / len, y / len);
    }

    double dot(const lpvec2& other) const {
        return x * other.x + y * other.y;
    }

    lpvec2 operator+(const lpvec2& rhs) const {
        return lpvec2(x + rhs.x, y + rhs.y);
    }

    lpvec2 operator-(const lpvec2& rhs) const {
        return lpvec2(x - rhs.x, y - rhs.y);
    }

    lpvec2 operator*(double scalar) const {
        return lpvec2(x * scalar, y * scalar);
    }

    lpvec2 operator/(double scalar) const {
        if (std::abs(scalar) < 1e-10) {
            throw std::runtime_error("wtf nig");
        }
        return lpvec2(x / scalar, y / scalar);
    }

    lpvec2& operator+=(const lpvec2& rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    lpvec2& operator-=(const lpvec2& rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    lpvec2& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    lpvec2& operator/=(double scalar) {
        if (std::abs(scalar) < 1e-10) {
            throw std::runtime_error("wtf bro");
        }
        x /= scalar;
        y /= scalar;
        return *this;
    }

    bool operator==(const lpvec2& rhs) const {
        const double epsilon = 1e-10;
        return std::abs(x - rhs.x) < epsilon && std::abs(y - rhs.y) < epsilon;
    }

    bool operator!=(const lpvec2& rhs) const {
        return !(*this == rhs);
    }

    lpvec2 operator-() const {
        return lpvec2(-x, -y);
    }

    friend lpvec2 operator*(double scalar, const lpvec2& vec);
    friend std::ostream& operator<<(std::ostream& os, const lpvec2& vec);
};

inline lpvec2 operator*(double scalar, const lpvec2& vec) {
    return vec * scalar;
}

inline std::ostream& operator<<(std::ostream& os, const lpvec2& vec) {
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

#endif 