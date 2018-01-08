#ifndef REPROJECTION_H_
#define REPROJECTION_H_

mediump vec3 RGB_to_YCgCo(mediump vec3 c)
{
    return vec3(
        0.25 * c.r + 0.5 * c.g + 0.25 * c.b,
        0.5 * c.g - 0.25 * c.r - 0.25 * c.b,
        0.5 * c.r - 0.5 * c.b);
}

mediump vec3 YCgCo_to_RGB(mediump vec3 c)
{
    mediump float tmp = c.x - c.y;
    return vec3(tmp + c.z, c.x + c.y, tmp - c.z);

    // c.x - c.y + c.z = [0.25, 0.5, 0.25] - [-0.25, 0.5, -0.25] + [0.5, 0.0, -0.5] = [1.0, 0.0, 0.0]
    // c.x + c.y       = [0.25, 0.5, 0.25] + [-0.25, 0.5, -0.25]                    = [0.0, 1.0, 0.0]
    // c.x - c.y - c.z = [0.25, 0.5, 0.25] - [-0.25, 0.5, -0.25] - [0.5, 0.0, -0.5] = [0.0, 0.0, 1.0]
}

#define CLAMP_AABB 1
mediump vec3 clamp_history(mediump vec3 color, mediump vec3 c0, mediump vec3 c1, mediump vec3 c2, mediump vec3 c3)
{
    mediump vec3 lo = c0;
	mediump vec3 hi = c0;
    lo = min(lo, c1);
	lo = min(lo, c2);
	lo = min(lo, c3);
    hi = max(hi, c1);
	hi = max(hi, c2);
	hi = max(hi, c3);

#if CLAMP_AABB
    mediump vec3 center = 0.5 * (lo + hi);
    mediump vec3 radius = max(0.5 * (hi - lo), vec3(0.0001));
    mediump vec3 v = color - center;
    mediump vec3 units = v / radius;
    mediump vec3 a_units = abs(units);
    mediump float max_unit = max(max(a_units.x, a_units.y), a_units.z);
    if (max_unit > 1.0)
        return center + v / max_unit;
    else
        return color;
#else
    return clamp(color, lo, hi);
#endif
}

#endif