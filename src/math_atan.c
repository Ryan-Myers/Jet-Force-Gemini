float acosf(float);
float sqrtf(float x);

#define MATH_PI 3.141592741f

float atan2f(float y, float x) {
    float z;

    z = 0.0f;
    if (y == 0.0f) {
        if (x >= 0.0f) {
            z = 0.0f;
        } else {
            z = MATH_PI;
        }
    } else if (x == 0.0f) {
        if (y > 0.0f) {
            z = (MATH_PI / 2);
        } else {
            z = ((3 * MATH_PI) / 2);
        }
    } else {
        float sqrt = sqrtf((y * y) + (x * x));
        if (y > x) {
            z = acosf(x / sqrt);
            if (y < 0.0f) {
                z = (2 * MATH_PI) - z;
            }
        } else {
            z = (MATH_PI / 2) - acosf(y / sqrt);
            if (x < 0.0f) {
                z = MATH_PI - z;
            }
            if (z < 0.0f) {
                z += (2 * MATH_PI);
            }
        }
    }
    return z;
}
