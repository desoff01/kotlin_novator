#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

// Если пробелов будет больше 12 - то программа будет падать.
// Лечится увеличением BUFFER_SIZE или контролем данных, которые даются программе.
#define BUFFER_SIZE 20
#define EDGE 100

struct Point {
    int x;
    int y;
};

// Checking if a point is in a circle
bool isCircle(int x, int y, int r) {
    // Убрано лишнее деление
    return ((x*x) + (y*y)) / ((r-.1)*(r-.1)) <= 1.0;
}

int main(int argc, char** argv) {
    if (argc != 3) { 
        printf("%s%s%s", "Usage: ", argv[0], " <file_path> <radius>\n");
        return 1;
    } else if (argv[2][0] < '1' || argv[2][0] > '9') {
        printf("radius must be positive number\n");
        return 2;
    }
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        printf("File does not exist\n");
        return 3;
    }

    char buffer[BUFFER_SIZE];
    int grid[EDGE][EDGE] = {};
    char *sep = ",";
    char *istr;
    struct Point p;

    // Reading coordinates from a file and designating targets in the grid
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        // На случай, если в файле есть пустые строки
        if (buffer[0] == '\n') continue;
        istr = strtok(buffer, sep);
        p.x = atoi(istr);
        istr = strtok(NULL, sep);
        p.y = atoi(istr);

        ++grid[p.x][p.y];
    }
    fclose(fp);

    int r = atoi(argv[2]);
    int max = 0;
    struct Point coord;

    // Finding the best point for a missile strike
    // Отпимизация с использованием формулы об окружности, описанной вокруг прямоугольника
    if (r > sqrt((EDGE*EDGE + EDGE*EDGE)) / 2) {
        for (int x = 0; x < EDGE; ++x) {
            for (int y = 0; y < EDGE; ++y) {
                if (grid[x][y] > 0) max += grid[x][y];
            }
        }
        coord.x = EDGE / 2;
        coord.y = EDGE / 2;
    } else {
        int tmp = 0;
        for (int x_center = r/2; x_center < EDGE-r/2; ++x_center) {
            for (int y_center = r/2; y_center < EDGE-r/2; ++y_center) {
                for (int x = -r+x_center; x <= r+x_center; ++x) {
                    if (x < 0) continue;
                    if (x > EDGE-1) break;
                    for (int y = r+y_center; y >= -r+y_center; --y) {
                        if (y > EDGE-1) continue;
                        if (y < 0) break;
                    
                        if (grid[x][y] > 0 && isCircle(x-x_center, y-y_center, r)) {
                            tmp += grid[x][y];
                        }
                    }
                }
                if (tmp > max) {
                    coord.x = x_center;
                    coord.y = y_center;
                    max = tmp;
                }
                tmp = 0;
            }
        }
    }

    printf("%s%d\n%s%d\n%s%d\n", "x: ", coord.x, "y: ", coord.y, "Number of targets hit: ", max);

    return 0;
}
