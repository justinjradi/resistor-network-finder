#include <stdio.h>

enum ComponentType
{
    RESISTANCE,
    CAPACITANCE,
    INDUCTANCE,
};

// User Parameters

float stock[] = {10, 12, 15, 18, 22, 27, 33, 39, 47, 56, 68, 82};
int stock_size = sizeof(stock) / sizeof(float);

enum ComponentType componentType = RESISTANCE;
int max_length = 2;
int max_width = 4;
int max_components = 10; 

float target_value = 25.4;

int main()
{
    
}