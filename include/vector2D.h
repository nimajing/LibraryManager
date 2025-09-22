#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <math.h>

// Class representing a 2D vector
class Vector2D {
    public:
        // Constructor with optional x and y values
        Vector2D(float x = 0, float y = 0) : m_x(x), m_y(y) {}

        // Getters for x and y components
        float getX() { return m_x; }
        float getY() { return m_y; }

        // Calculate the length of the vector
        float length() { return sqrt(m_x * m_x + m_y * m_y); }

        // Setters for x and y components
        void setX(float x) { m_x = x; }
        void setY(float y) { m_y = y; }

        // Normalize the vector to unit length
        void normalize() {
            float l = length();
            if (l > 0) // never divide by zero
                (*this) *= 1 / l;
        }

        // Operator overloads for vector arithmetic
        Vector2D operator+(const Vector2D& v2) const {
            return Vector2D(m_x + v2.m_x, m_y + v2.m_y);
        }

        Vector2D& operator+=(const Vector2D& v2) {
            m_x += v2.m_x;
            m_y += v2.m_y;
            return *this;
        }

        Vector2D operator*(const float& scalar) const {
            return Vector2D(m_x * scalar, m_y * scalar);
        }

        Vector2D& operator*=(const float& scalar) {
            m_x *= scalar;
            m_y *= scalar;
            return *this;
        }

        Vector2D operator-(const Vector2D& v2) const {
            return Vector2D(m_x - v2.m_x, m_y - v2.m_y);
        }

        Vector2D& operator-=(const Vector2D& v2) {
            m_x -= v2.m_x;
            m_y -= v2.m_y;
            return *this;
        }

        Vector2D operator/(const float& scalar) const {
            return Vector2D(m_x / scalar, m_y / scalar);     
        }

        Vector2D& operator/=(const float& scalar) {
            m_x /= scalar;
            m_y /= scalar;
            return *this;
        }

    private:
        float m_x; // x component of the vector
        float m_y; // y component of the vector
};

#endif