

int ToInt(float x) { StopScript(x); }

float WayRatioX(int wp1, int wp2)
{
    return (GetWaypointX(wp1) - GetWaypointX(wp2)) * 1.0 / Distance(GetWaypointX(wp1), GetWaypointY(wp1), GetWaypointX(wp2), GetWaypointY(wp2));
    "export WayRatioX";
}

float WayRatioY(int wp1, int wp2)
{
    return (GetWaypointY(wp1) - GetWaypointY(wp2)) * 1.0 / Distance(GetWaypointX(wp1), GetWaypointY(wp1), GetWaypointX(wp2), GetWaypointY(wp2));
    "export WayRatioY";
}

float GetSineValue(int wp, float c)
{
    float x_ratio = WayRatioX(wp, wp + 1), y_ratio = WayRatioY(wp, wp + 1), res;

    res = GetWaypointX(wp) - GetWaypointX(wp + 1);
    MoveWaypoint(wp + 1, GetWaypointX(wp) - (c * y_ratio) - x_ratio, GetWaypointY(wp) + (c * x_ratio) - y_ratio);
    return res;
    "export GetSineValue";
}

float MathSine(int angle, float size)
{
    float var_0[91];
    int i, k;
 
    if (!ToInt(var_0[90]))
    {
        MoveWaypoint(angle + 1, GetWaypointX(angle), GetWaypointY(angle) - 1.0);
        for (i = 0 ; i <= 90 ; i += 1)
        {
            var_0[i] = GetSineValue(angle, size);
            Delete(k + i + 1);
        }
        return var_0[0];
    }
    k = angle / 90;
    i = angle - (k * 90);

    if (k % 2) i = 90 - i;
    if ((angle / 180) % 2) return -var_0[i] * size;
	else return var_0[i] * size;
    "export MathSine";
}

float UnitAngleCos(int unit, float size)
{
    return MathSine((GetDirection(unit) * 45 / 32) + 90, size);
    "export UnitAngleCos";
}

float UnitAngleSin(int unit, float size)
{
    return MathSine(GetDirection(unit) * 45 / 32, size);
    "export UnitAngleSin";
}

float UnitRatioX(int unit, int target, float size)
{
    return (GetObjectX(unit) - GetObjectX(target)) * size / Distance(GetObjectX(unit), GetObjectY(unit), GetObjectX(target), GetObjectY(target));
    "export UnitRatioX";
}

float UnitRatioY(int unit, int target, float size)
{
    return (GetObjectY(unit) - GetObjectY(target)) * size / Distance(GetObjectX(unit), GetObjectY(unit), GetObjectX(target), GetObjectY(target));
    "export UnitRatioY";
}

void InitMathSine(int wp)
{
    "export InitMathSine";
    MathSine(wp, 1.0 / 57.3);
}

void NOXLibraryEntryPointFunction()
{
    "export ToInt";
}