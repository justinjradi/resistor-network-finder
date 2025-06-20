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

int init_stock();
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
    max_size = 4;
    target_value = 111;
    init_stock();
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
int init_stock()
{
    int start_decade = 1;
    int end_decade = 5;
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
    stock_size = E3.size * (end_decade - start_decade + 1);
    stock = malloc(stock_size * sizeof(struct Component));
    if (stock == NULL)
    {
        printf("Error: Failed to allocate memory.\n");
        return 1;
    }
    struct Component temp_component;
    temp_component.tolerance = E3.tolerance;
    for (int d = start_decade; d <= end_decade; d++)
    {
       for (int c = 0; c < E3.size; c++)
       {
            temp_component.value = E3.values[c] * (int)pow(10, d);
            create_symbol(temp_component.value, E3.tolerance, temp_component.symbol);
            stock[(d - start_decade)*E3.size + c] = temp_component;
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