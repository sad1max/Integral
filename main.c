extern double f1(double);
extern double f2(double);
extern double f3(double);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "function.h"

#define e1 1e-4
#define e2 1e-4
#define start_a -2
#define start_b 2

typedef double (*func) (double x);

static int num;

static double nothing(double x) {
    return 0.0;
}

static double root(func f, func g, double a, double b, double eps) {
    num = 0;
    while (num < 100) {
        double c =  ((f(a) - g(a)) * b - (f(b) - g(b)) * a) / (f(a) - g(a) - f(b) + g(b));
        if ((f(c) - g(c)) * (f(a) - g(a)) > 0) {
            a = c;
        } else {
            b = c;
        }
        if (fabs(f(c) - g(c)) < eps) {
            return c;
        }
        num++;
    }
    if (num == 100) {
        printf("ROOT NOT FOUND\n");
        exit(0);
    }
    return a;
}

static double integrate_n_steps(func f, double a, double b, int n, double last_n) {
    double res = last_n / 2.0;
    for (int i = 1; i < n; i += 2) {
        double x1 = a + (b - a) / (double)(n) * (double)(i);
        res += f(x1) * (b - a) / (double)(n);   // (b-a) / double(n)
    }                                          // f(x1) 
    return res;
}

static double integral(func f, double a, double b, double eps) {
    int n = 1;
    double n_steps = (f1(a) + f1(b)) / 2.0 * (b - a);
    while (1) {
        double n_steps_2 = integrate_n_steps(f, a, b, 2 * n, n_steps);
        if (fabs(n_steps - n_steps_2) < eps)
            break;
        n_steps = n_steps_2;
        n *= 2;
    }
    return n_steps;
}

static double test(func f, double x) {
    return f(x);
}

int main(int argc, char** argv) {
    double point_f1_f2 = root(f1, f2, start_a, start_b, e1);
    int num_f1_f2 = num;
    double point_f1_f3 = root(f1, f3, start_a, start_b, e1);
    int num_f1_f3 = num;
    double point_f2_f3 = root(f2, f3, start_a, start_b, e1);
    int num_f2_f3 = num; 

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-help")) {
            printf("Command list:\n");
            printf("-valuesofx:     printf X coordinates of intersections points\n");
            printf("-valuesofy:   printf Y coordinates of intersections points\n");
            printf("-numbers:    printf numbers of actions needed to find coordinates\n");
            printf("-test-i {1,2,3} l r: get the integral of function (either 1, 2 or 3) on the bounds [start_x; end_x]\n");
            printf("-test-f\n");
        } else if (!strcmp(argv[i], "-valuesofx")) {
            printf("y = f1(x) and y = f2(x) intersection X coordinate - %lf\n", point_f1_f2);
            printf("y = f1(x) and y = f3(x) intersection X coordinate - %lf\n", point_f1_f3);
            printf("y = f2(x) and y = f3(x) intersection X coordinate - %lf\n", point_f2_f3);
        } else if (!strcmp(argv[i], "-valuesofy")) {
            printf("y = f1(x) and y = f2(x) intersection Y coordinate - %lf\n", f1(point_f1_f2));
            printf("y = f1(x) and y = f3(x) intersection Y coordinate - %lf\n", f1(point_f1_f3));
            printf("y = f2(x) and y = f3(x) intersection Y coordinate - %lf\n", f2(point_f2_f3));
        }else if (!strcmp(argv[i], "-numbers")) {
            printf("y = f1(x) and y = f2(x) numbers to find the intersection points - %d\n", num_f1_f2);
            printf("y = f1(x) and y = f3(x) numbers to find the intersection points - %d\n", num_f1_f3);
            printf("y = f2(x) and y = f3(x) numbers to find the intersection points - %d\n", num_f2_f3);
        }else if(!strcmp(argv[i], "-test-f")) {
            int number = atoi(argv[++i]);
            if (number != 1 && number != 2 && number != 3){
                printf("Invalid function numbers, pls print only 1 or 2 or 3\n");
                exit(0);
            }
            double x = atof(argv[++i]);
            double res;
            if (number == 1)
                res = test(f1, x);
            else if (number == 2)
                res = test(f2, x);
            else if (number == 3)
                res = test(f3, x);
            printf("f%d(x) = %lf\n", number, res);
        }else if (!strcmp(argv[i], "-test-i")) {
            // test integral function
            // get the number of function from the next argument
            int number = atoi(argv[++i]);
            if (number != 1 && number != 2 && number != 3){
                printf("Invalid function numbers, pls print only 1 or 2 or 3\n");
                exit(0);
            }
            double a = atof(argv[++i]);     //left boundary of a line
            double b = atof(argv[++i]);     //right boundary of a line
            double res; //integral function - f from a to b of line
            if (number == 1)
                res = integral(f1, a, b, e2);
            else if (number == 2)
                res = integral(f2, a, b, e2);
            else if (number == 3)
                res = integral(f3, a, b, e2);

            printf("integral function - f from %lf to %lf - %lf\n", a, b, res);
        }else if (!strcmp(argv[i], "-test-r")) {
            // test root function
            // get the number of function from the next argument
            int number = atoi(argv[++i]);
            if (number != 1 && number != 2 && number != 3){
                printf("Invalid function numbers, pls print only 1 or 2 or 3\n");
                exit(0);
            }
            printf("You choose the function number %d\n", number);
            func f;
            if (number == 1)
                f = f1;
            else if (number == 2)
                f = f2;
            else if (number == 3)
                f = f3;

            // get next arguments
            double a = atof(argv[++i]);
            double b = atof(argv[++i]);

            double result = root(f, nothing, a, b, e2);
            printf("Root of function from %lf to %lf is %lf\n", a, b, result);
        }
    }

    double s1 = integral(f1, point_f1_f3, point_f2_f3, e2) - integral(f3, point_f1_f3, point_f2_f3, e2);
    double s2 = integral(f1, point_f2_f3, point_f1_f2, e2) - integral(f2, point_f2_f3, point_f1_f2, e2);
    printf("S1 - left part of the figure = %lf\n", s1);
    printf("S2 - right part of the figure = %lf\n", s2);
    printf("S - total are = %lf\n", s1 + s2);

    return 0;
}
