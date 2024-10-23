#pragma once

#include <cmath>

template<typename T>
T Clamp(const T &x, const T &min, const T &max) {
    if (x < min) {
        return min;
    }

    if (x > max) {
        return max;
    }
    
    return x;
}

// Interpolations
template<typename T>
T Step(const T &a, const T &x) {  // --> [0,1]
    if (a < x) {
        return static_cast<T>(1.0);
    }
    return static_cast<T>(0.0);
}

template<typename T>
T Smoothstep_One(const T &edge0, const T &edge1, const T &x) {  // --> [0,1]
    double t = Clamp(static_cast<T>((x - edge0) / (edge1 - edge0)), 0.0, 1.0);
    return static_cast<T>(t * t * (3.0 - 2.0 * t));
}

template<typename T>
T Smoothstep_Two(const T &edge0, const T &edge1, const T &x) {
	T t = Clamp(static_cast<double>((x - edge0) / (edge1 - edge0)), 0.0, 1.0);
	return static_cast<T>(t * t * t * (t * (6.0 * t - 15.0) + 10.0));
}

template<typename T>
T IntegralSmoothstep(const T &x, const T &t) {
    if( x>t ) return x - t/2.0;
    return static_cast<T>(x*x*x*(1.0-x*0.5/t)/t/t);
}

template<typename T>
T ExpStep(const T &x, const T &n) {
    return static_cast<T>(exp2( -exp2(n)*pow(x,n)));
}

template<typename T>
T RationalBump(const T &x, const T &k) {
    return static_cast<T>(1.0 / (1.0 + k * x * x));
}

template<typename T>
T CubicPulse(const T &x, const T &c, const T &w) {
    T z = fabs(x - c);
    if( z>w ) return 0.0;
    z /= w;
    return static_cast<T>(1.0 - z * z * (3.0 - 2.0 * z));
}

template<typename T>
T Pcurve(const T &x, const T &a, const T &b) {
    float k = pow(a+b,a+b) / (pow(a,a) * pow(b,b));
    return static_cast<T>(k * pow(x,a) * pow(1.0-x,b));
}

template<typename T>
T Parabola(const T &x, const T &k) {
    return static_cast<T>(pow(4.0 * x * (1.0-x), k));
}

template<typename T>
T Gain(const T &x, const T &k) {
    float a = 0.5 * pow(2.0 * ((x<0.5)?x:1.0-x), k);
    return static_cast<T>((x<0.5)?a:1.0-a);
}

template<typename T>
T AlmostUnitIdentity(const T &x) {
    return static_cast<T>(x * x * (2.0-x));
}

template<typename T>
T AlmostIdentityV1(const T &x, const T &m, const T &n) {
    if( x>m ) return x;
    float a = 2.0*n - m;
    float b = 2.0*m - 3.0*n;
    float t = x/m;
    return static_cast<T>((a*t + b)*t*t + n);
}

template<typename T>
T AlmostIdentityV2(const T &x, const T &n) {
    return static_cast<T>(sqrt(x*x+n*n));
}

template<typename T>
T Trunc_fallof(const T &x, const T &m) {
    float z = x / m;
    return static_cast<T>((z-2.0)*z+1.0);
}

template<typename T>
T Sinc(const T &x, const T &k) {
    float a = M_PI * (k * x-1.0);
    return static_cast<T>(sin(a)/a);
}

template<typename T>
T ExpSustainedImpulse(const T &x, const T &f, const T &k) {
    float s = Max(x-f,0.0);
    return static_cast<T>(Min( x*x/(f*f), 1.0+(2.0/f)*s*exp(-k*s)));
}

template<typename T>
T PolyImpulse(const T &x, const T &k, const T &n) {
    return static_cast<T>((n/(n-1.0)) * pow((n-1.0)*k,1.0/n) * x/(1.0+k*pow(x,n)));
}

template<typename T>
T QuaImpulse(const T &x, const T &k) {
    return static_cast<T>(2.0*sqrt(k)*x/(1.0+k*x*x));
}

template<typename T>
T ExpImpulse(const T &x, const T &k) {
    float h = k*x;
    return static_cast<T>(h*exp(1.0-h));
}

template<typename T>
T Lerp(const T &v0, const T &v1, const T &t) {
  return static_cast<T>((1.0 - t) * v0 + t * v1);
}

template<typename T>
T Sin(const T &A, const T &w, const T &x) {
    return static_cast<T>(A * sin(w * x * 2.0 * M_PI));
}

template<typename T>
T Cos(const T &A, const T &w, const T &x) {
    return static_cast<T>(A * cos(w * x * 2.0 * M_PI));
}


// Fonctions simples
template<typename T>
T Min(const T &x, const T &y) {
    if(y < x) {
        return static_cast<T>(y);
    } else {
        return static_cast<T>(x);
    }
}

template<typename T>
T Max(const T &x, const T &y) {
    if(y > x) {
        return static_cast<T>(y);
    } else {
        return static_cast<T>(x);
    }
}

template<typename T>
T Abs(const T &x) {
    return static_cast<T>(fabs(x));
}

template<typename T>
T Sign(const T &x) {
    if(x < 0.) {
        return static_cast<T>(-1.);
    } else if(x == 0.) {
        return static_cast<T>(0.);
    } else if(x > 0.) {
        return static_cast<T>(1.);
    }
}

template<typename T>
T Floor(const T &x) {
    return static_cast<T>(floor(x));
}

template<typename T>
T Ceil(const T &x) {
    return static_cast<T>(ceil(x));
}