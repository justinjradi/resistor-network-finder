#include <stdio.h>
#include <string.h>

#define VALUE_DECIMALS 3
#define TOLERANCE_DECIMALS 2
#define SYMBOL_SIZE 16
#define SYMBOL_TEMPLATE "VVV.VVVP@TT.TT%"

struct Component
{
    float value;
    float tolerance;
    char symbol[SYMBOL_SIZE];
};

struct ComponentSeries
{
    float* values;
    float tolerance;
    int size;
};

// Standard component series
// https://en.wikipedia.org/wiki/E_series_of_preferred_numbers

const float E3_values[] = {1.0, 2.2, 4.7};
const struct ComponentSeries E3 = {E3_values, 0.40, sizeof(E3_values) / sizeof(float)};

const float E6_values[] = {1.0, 1.5, 2.2, 3.3, 4.7, 6.8};
const struct ComponentSeries E6 = {E6_values, 0.20, sizeof(E6_values) / sizeof(float)};

const float E12_values[] = {1.0, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2};
const struct ComponentSeries E12 = {E12_values, 0.10, sizeof(E12_values) / sizeof(float)};

struct Network
{
    float equivalent_value;
    float equivalent_tolerance;
    int length;
    int width;
    int size;
    char* expression;
};

enum NetworkType
{
    RESISTIVE,
    CAPACITIVE,
    INDUCTIVE,
};

// Global Variables

enum NetworkType networkType;
int max_size = -1;
float target_value = -1;

struct Component* stock;    // Stock of possible components
int stock_size = -1;

// Function Declarations

int init_stock();
int deinit_stock();

// Main Method

int main()
{
    networkType = RESISTIVE;
    max_size = 3;
    target_value = 123;
    init_stock();



    deinit_stock();
}

// Function Definitions

// Creates stock of components
int init_stock()
{
    int min_decade = 1;
    int max_decade = 2;
    stock_size = E3.size;
    stock = malloc(stock_size * sizeof(struct Component));
    if (stock == NULL)
    {
        printf("Error: Failed to allocate memory.\n");
        return 1;
    }
    struct Component temp_component;
    temp_component.tolerance = E3.tolerance;
    for (int d = min_decade; d <= max_decade; d++)
    {
       for (int c = 0; c < E3.size; c++)
       {
            temp_component.value = E3.values[c];
       }
    }
}

int deinit_stock()
{
    free(stock);
}

int create_symbol(float value, float tolerance, char* buffer)
{
    if (buffer == NULL)
    {
        printf("Error: Null pointer.\n");
        return 1;
    }
    // Convert value to engineering notation
    if (value < 0)
    {
        printf("Error: Value out of range.\n");
        return 1;
    }
    int power = 0;
    if (value != 0)
    {
        while (value < 1)
        {
            value *= 10;
            power--;
        }
        while (value > 999)
        {
            value /= 10;
            power++;
        }
        while ((power < 0) && ((power % 3) != 0))
        {
            value *= 10;
            power--;
        }
        while ((power > 0) && ((power % 3) != 0))
        {
            value /= 10;
            power++;
        }
    }


} 