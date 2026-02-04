#include "logic_gate.h"

int logic_and(int a, int b)
{
    return (a && b);
}

int logic_or(int a, int b)
{
    return (a || b);
}

int logic_not(int a)
{
    return (!a);
}