

int ToInt(float x) { StopScript(x); }

float WayRatioX(int wp1, int wp2)
{
    return (GetWaypointX(wp1) - GetWaypointX(wp2)) * 1.0 / Distance(GetWaypointX(wp1), GetWaypointY(wp1), GetWaypointX(wp2), GetWaypointY(wp2));
}

float WayRatioY(int wp1, int wp2)
{
    return (GetWaypointY(wp1) - GetWaypointY(wp2)) * 1.0 / Distance(GetWaypointX(wp1), GetWaypointY(wp1), GetWaypointX(wp2), GetWaypointY(wp2));
}

float GetSineValue(int wp, float c)
{
    float x_ratio = WayRatioX(wp, wp + 1), y_ratio = WayRatioY(wp, wp + 1), res;

    res = GetWaypointX(wp) - GetWaypointX(wp + 1);
    MoveWaypoint(wp + 1, GetWaypointX(wp) - (c * y_ratio) - x_ratio, GetWaypointY(wp) + (c * x_ratio) - y_ratio);
    return res;
}

//sin 값을 구합니다
//angle 은 각도입니다
//size 은 sin 값에 곱해질 수 입니다
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
}

//유닛이 보는 방향의 x벡터를 구합니다
float UnitAngleCos(int unit, float size)
{
    return MathSine((GetDirection(unit) * 45 / 32) + 90, size);
}

//유닛이 보는 방향의 y벡터를 구합니다
float UnitAngleSin(int unit, float size)
{
    return MathSine(GetDirection(unit) * 45 / 32, size);
}

//두 유닛간 x벡터를 구합니다
float UnitRatioX(int unit, int target, float size)
{
    return (GetObjectX(unit) - GetObjectX(target)) * size / Distance(GetObjectX(unit), GetObjectY(unit), GetObjectX(target), GetObjectY(target));
}

//두 유닛간 y벡터를 구합니다
float UnitRatioY(int unit, int target, float size)
{
    return (GetObjectY(unit) - GetObjectY(target)) * size / Distance(GetObjectX(unit), GetObjectY(unit), GetObjectX(target), GetObjectY(target));
}

//you can override it if you need
int InitMathSineBaseWaypointNumber()
{
	return 1;
}

//자동으로 호출되므로 수동호출 금지
void InitMathSine()
{
	int location = InitMathSineBaseWaypointNumber() + 1;
	float tempX = GetWaypointX(location), tempY = GetWaypointY(location);
	
    MathSine(InitMathSineBaseWaypointNumber(), 1.0 / 57.3);
	MoveWaypoint(location, tempX, tempY);
}

void NOXLibraryEntryPointFunction()
{
    "export ToInt";
	"export WayRatioX";
	"export WayRatioY";
	"export GetSineValue";
	"export MathSine";
	"export UnitAngleCos";
	"export UnitAngleSin";
	"export UnitRatioX";
	"export UnitRatioY";
	"export InitMathSineBaseWaypointNumber";
	"export needinit InitMathSine";
}