#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

#define SYMBOL_SIZE 16
#define EXPRESSION_SIZE 256
#define NUM_OPTIONS 10

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
const struct ComponentSeries E3 = {(float*)E3_values, 0.40,
    sizeof(E3_values) / sizeof(float)};

const float E6_values[] = {1.0, 1.5, 2.2, 3.3, 4.7, 6.8};
const struct ComponentSeries E6 = {(float*)E6_values, 0.20,
    sizeof(E6_values) / sizeof(float)};

const float E12_values[] = {1.0, 1.2, 1.5, 1.8, 2.2, 2.7, 3.3, 3.9, 4.7, 5.6, 6.8, 8.2};
const struct ComponentSeries E12 = {(float*)E12_values, 0.10,
    sizeof(E12_values) / sizeof(float)};

const float E24_values[] = {1.0, 1.1, 1.2, 1.3, 1.5, 1.6, 1.8, 2.0, 2.2, 2.4, 2.7, 3.0, 3.3,
    3.6, 3.9, 4.3, 4.7, 5.1, 5.6, 6.2, 6.8, 7.5, 8.2, 9.1};
const struct ComponentSeries E24 = {(float*)E24_values, 0.05,
    sizeof(E24_values) / sizeof(float)};

const float E48_values[] = {1.00, 1.05, 1.10, 1.15, 1.21, 1.27, 1.33, 1.40, 1.47,
    1.54, 1.62, 1.69, 1.78, 1.87, 1.96, 2.05, 2.15, 2.26, 2.37, 2.49, 2.61, 2.74, 2.87,
    3.01, 3.16, 3.32, 3.48, 3.65, 3.83, 4.02, 4.22, 4.42, 4.64, 4.87, 5.11, 5.36, 5.62, 5.90,
    6.19, 6.49, 6.81, 7.15, 7.50, 7.87, 8.25, 8.66, 9.09, 9.53};
const struct ComponentSeries E48 = {(float*)E48_values, 0.02,
    sizeof(E48_values) / sizeof(float)};

const float E96_values[] = {1.00, 1.02, 1.05, 1.07, 1.10, 1.13, 1.15, 1.18,
    1.21, 1.24, 1.27, 1.30, 1.33, 1.37, 1.40, 1.43, 1.47, 1.50, 1.54, 1.58, 1.62, 1.65, 1.69,
    1.74, 1.78, 1.82, 1.87, 1.91, 1.96, 2.00, 2.05, 2.10, 2.15, 2.21, 2.26, 2.32, 2.37, 2.43,
    2.49, 2.55, 2.61, 2.67, 2.74, 2.80, 2.87, 2.94, 3.01, 3.09, 3.16, 3.24, 3.32, 3.40, 3.48,
    3.57, 3.65, 3.74, 3.83, 3.92, 4.02, 4.12, 4.22, 4.32, 4.42, 4.53, 4.64, 4.75, 4.87, 4.99,
    5.11, 5.23, 5.36, 5.49, 5.62, 5.76, 5.90, 6.04, 6.19, 6.34, 6.49, 6.65, 6.81, 6.98, 7.15,
    7.32, 7.50, 7.68, 7.87, 8.06, 8.25, 8.45, 8.66, 8.87, 9.09, 9.31, 9.53, 9.76};
const struct ComponentSeries E96 = {(float*)E96_values, 0.01,
    sizeof(E96_values) / sizeof(float)};

const float E192_values[] = {1.00, 1.01, 1.02, 1.04, 1.05, 1.06, 1.07, 1.09, 1.10, 1.11, 1.13,
    1.14, 1.15, 1.17, 1.18, 1.20, 1.21, 1.23, 1.24, 1.26, 1.27, 1.29, 1.30, 1.32, 1.33, 1.35,
    1.37, 1.38, 1.40, 1.42, 1.43, 1.45, 1.47, 1.49, 1.50, 1.52, 1.54, 1.56, 1.58, 1.60, 1.62,
    1.64, 1.65, 1.67, 1.69, 1.72, 1.74, 1.76, 1.78, 1.80, 1.82, 1.84, 1.87, 1.89, 1.91, 1.93,
    1.96, 1.98, 2.00, 2.03, 2.05, 2.08, 2.10, 2.13, 2.15, 2.18, 2.21, 2.23, 2.26, 2.29, 2.32,
    2.34, 2.37, 2.40, 2.43, 2.46, 2.49, 2.52, 2.55, 2.58, 2.61, 2.64, 2.67, 2.71, 2.74, 2.77,
    2.80, 2.84, 2.87, 2.91, 2.94, 2.98, 3.01, 3.05, 3.09, 3.12, 3.16, 3.20, 3.24, 3.28, 3.32,
    3.36, 3.40, 3.44, 3.48, 3.52, 3.57, 3.61, 3.65, 3.70, 3.74, 3.79, 3.83, 3.88, 3.92, 3.97,
    4.02, 4.07, 4.12, 4.17, 4.22, 4.27, 4.32, 4.37, 4.42, 4.48, 4.53, 4.59, 4.64, 4.70, 4.75,
    4.81, 4.87, 4.93, 4.99, 5.05, 5.11, 5.17, 5.23, 5.30, 5.36, 5.42, 5.49, 5.56, 5.62, 5.69,
    5.76, 5.83, 5.90, 5.97, 6.04, 6.12, 6.19, 6.26, 6.34, 6.42, 6.49, 6.57, 6.65, 6.73, 6.81,
    6.90, 6.98, 7.06, 7.15, 7.23, 7.32, 7.41, 7.50, 7.59, 7.68, 7.77, 7.87, 7.96, 8.06, 8.16,
    8.25, 8.35, 8.45, 8.56, 8.66, 8.76, 8.87, 8.98, 9.09, 9.20, 9.31, 9.42, 9.53, 9.65, 9.76, 9.88};
const struct ComponentSeries E192 = {(float*)E192_values, 0.005,
    sizeof(E192_values) / sizeof(float)};

struct Network
{
    float value;
    float tolerance;
    int size;
    char expression[EXPRESSION_SIZE];
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

struct Network options_list[NUM_OPTIONS];

// Function Declarations

int init_stock(const struct ComponentSeries series);
int deinit_stock();
int create_symbol(float value, float tolerance, char* symbol);
void print_stock();
float series(float A, float B);
float parallel(float A, float B);
float series_tolerance(float A, float B, float t_A, float t_B);
float parallel_tolerance(float A, float B, float t_A, float t_B);
int expand_network(struct Network network, int last_i);
int update_options_list(struct Network network);
int init_options_list();
void print_options();

// Main Method

int main()
{
    networkType = RESISTIVE;
    max_size = 2;
    target_value = 3141.59265359;
    init_stock(E12);
    init_options_list();

    struct Network n;
    n.value = 0;
    n.tolerance = 0;
    n.size = 0;
    n.expression[0] = '\0';

    expand_network(n, 0);

    print_options();

    float a = series(10.000, 10.000);
    float b = series(a, 100.000);
    printf("%.3f  %.3f\n", a, b);

    deinit_stock();
}

// Function Definitions

// Creates stock of components
int init_stock(const struct ComponentSeries series)
{
    int start_decade = 1;
    int end_decade = 6;
    if ((start_decade < -12) || (start_decade > 12))
    {
        printf("Error: Start decade is out of bounds.\n");
        return 1;
    }
    if ((end_decade < -12) || (end_decade > 12))
    {
        printf("Error: End decade is out of bounds.\n");
        return 1;
    }
    if (end_decade < start_decade)
    {
        printf("Error: End decade cannot exceed start decade.\n");
        return 1;
    }
    stock_size = series.size * (end_decade - start_decade + 1);
    stock = malloc(stock_size * sizeof(struct Component));
    if (stock == NULL)
    {
        printf("Error: Failed to allocate memory.\n");
        return 1;
    }
    struct Component temp_component;
    temp_component.tolerance = series.tolerance;
    for (int d = start_decade; d <= end_decade; d++)
    {
       for (int c = 0; c < series.size; c++)
       {
            temp_component.value = series.values[c] * (int)pow(10, d);
            create_symbol(temp_component.value, series.tolerance, temp_component.symbol);
            stock[(d - start_decade)*series.size + c] = temp_component;
       }
    }
}

int deinit_stock()
{
    free(stock);
}

int create_symbol(float value, float tolerance, char* symbol)
{
    if (symbol == NULL)
    {
        printf("Error: Null Pointer.\n");
        return 1;
    }
    if (value < 0)
    {
        printf("Error: Value out of bounds.\n");
        return 1;
    }
    tolerance *= 100;   // ! Convert tolerance to percent for formatting !
    if ((tolerance < 0) || (tolerance > 99.99))
    {
        printf("Error: Tolerance out of bounds.\n");
        return 1;
    }
    // Engineering notation
    int power = 0;
    if (value != 0)
    {
        power = (int)floor(log10(value));
        power = (power < 0) ? ((power - 2) / 3) * 3 : (power / 3) * 3;
        value *= powf(10, -power);
        if (value >= 1000)
        {
            value /= 1000;
            power += 3;
        }
        else if (value < 1)
        {
            value *= 1000;
            power -= 3; 
        }
        if (power < -12)
        {
            value = 0;
            power = 0;
        }
        if (power > 12)
        {
            value = 999.999;
            power = 12;
        }
    }

    // Unit suffix
    const char *unit = "";
    switch (power)
    {
        case 3:   unit = "k"; break;
        case 6:   unit = "M"; break; 
        case 9:   unit = "T"; break;
        case 12:  unit = "G"; break;
        case -3:  unit = "m"; break;
        case -6:  unit = "u"; break;
        case -9:  unit = "n"; break;
        case -12: unit = "p"; break;
    }

    // Format directly into symbol
    snprintf(symbol, SYMBOL_SIZE, "%.3f%s@%.2f%%",
        value, unit, tolerance);
    return 0;
}

void print_stock()
{
    if (stock == NULL)
    {
        return;
    }
    if (stock_size == -1)
    {
        return;
    }
    for (int i = 0; i < stock_size; i++)
    {
        struct Component c = stock[i];
        printf("Component %d: val = %.3f, tol = %.3f, sym = %s\n", i,
            c.value, c.tolerance, c.symbol);
    }
}

float series(float A, float B)
{
    if (A == 0)
    {
        return B;
    }
    if (B == 0)
    {
        return A;
    }
    if (networkType != CAPACITIVE)
    {
        return A + B;
    }
    else
    {
        return 1/((1/A) + (1/B));
    }
}

float parallel(float A, float B)
{
    if (A == 0)
    {
        return B;
    }
    if (B == 0)
    {
        return A;
    }
    if (networkType != CAPACITIVE)
    {
        return 1/((1/A) + (1/B));
    }
    else
    {
        return A + B;
    }
}

float series_tolerance(float A, float B, float t_A, float t_B)
{
    float max;
    float nominal;
    max = series((1 + t_A) * A, (1 + t_B) * B);
    nominal = series(A, B);
    return (max / nominal) - 1;
}

float parallel_tolerance(float A, float B, float t_A, float t_B)
{
    float max;
    float nominal;
    max = parallel((1 + t_A) * A, (1 + t_B) * B);
    nominal = parallel(A, B);
    return (max / nominal) - 1;
}

int expand_network(struct Network network, int last_i)
{
    update_options_list(network);
    if (network.size >= max_size)
    {
        return 0;
    }
    for (int i = last_i; i < stock_size; i++)
    {
        struct Component component = stock[i];
        struct Network new_network;
        new_network.size = network.size + 1;
        // Series
        new_network.value = series(network.value, component.value);
        new_network.tolerance = series_tolerance(network.value, component.value,
            network.tolerance, component.tolerance);
        if (network.size == 0)
        {
            snprintf(new_network.expression, EXPRESSION_SIZE, "%s", component.symbol);
        }
        else
        {
            snprintf(new_network.expression, EXPRESSION_SIZE, "(%s) + %s",
                network.expression, component.symbol);
        }

        // if ((new_network.size == 1) && (new_network.value < 1))
        // {
        //     printf("EXPRESSION %s\n", new_network.expression);
        //     printf("l;koijhugyfutdrtse\n");
        // }

        expand_network(new_network, i);
        // Parallel
        new_network.value = parallel(network.value, component.value);
        new_network.tolerance = parallel_tolerance(network.value, component.value,
            network.tolerance, component.tolerance);
        if (network.size == 0)
        {
            snprintf(new_network.expression, EXPRESSION_SIZE, "%s", component.symbol);
        }
        else
        {
            snprintf(new_network.expression, EXPRESSION_SIZE, "(%s) || %s",
                network.expression, component.symbol);
        }

        // if ((new_network.size == 1) && (new_network.value < 1))
        // {
        //     printf("EXPRESSION %s\n", new_network.expression);
        //     printf("l;koijhugyfutdrtse\n");
        // }

        expand_network(new_network, i);
    }
}

int init_options_list()
{
    struct Network empty_network;
    empty_network.value = FLT_MAX;
    empty_network.tolerance = FLT_MAX;
    empty_network.size = -1;
    empty_network.expression[0] = '\0';
    for (int i = 0; i < NUM_OPTIONS; i++)
    {
        options_list[i] = empty_network;
    }
}

int insert_in_options_list(struct Network network, int x)
{
    for (int i = NUM_OPTIONS - 1; i > x; i--)
    {
        options_list[i] = options_list[i - 1];
    }
    options_list[x] = network;
    return 0;
}

int update_options_list(struct Network network)
{
    if (network.size == 0)
    {
        return 0;
    }
    for (int i = 0; i < NUM_OPTIONS; i++)
    {
        float diff = fabs(network.value - target_value);
        float list_diff = fabs(options_list[i].value - target_value);
        if (diff < list_diff)
        {
            insert_in_options_list(network, i);
            break;
        }
    }
    return 0;
}

void print_options()
{
    for (int i = 0; i < NUM_OPTIONS; i++)
    {
        if (options_list[i].size == -1)
        {   
            continue;
        }
        printf("%d:  %s  %.3f\n", i, options_list[i].expression,
            options_list[i].value);
    } 
}