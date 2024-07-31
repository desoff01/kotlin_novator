#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

struct Point {
    int x;
    int y;
};

// Checking if a point is in a circle
bool isCircle(int x, int y, int r) {
    return (x*x) / ((r-.1)*(r-.1)) + (y*y) / ((r-.1)*(r-.1)) <= 1.0;
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

    char buffer[6];
    bool grid[100][100] = {};
    char *sep = ",";
    char *istr;
    struct Point p;

    // Reading coordinates from a file and designating targets in the grid
    while (fgets(buffer, 6, fp) != NULL) {
        istr = strtok(buffer, sep);
        p.x = atoi(istr);
        istr = strtok(NULL, sep);
        p.y = atoi(istr);

        grid[p.x][p.y] = true;
        fgets(buffer, 6, fp);
    }
    fclose(fp);

    int r = atoi(argv[2]);
    
    int tmp = 0, max = 0;
    struct Point coord;
    // Finding the best point for a missile strike
    for (int x_center = 0; x_center < 100; ++x_center) {
        for (int y_center = 0; y_center < 100; ++y_center) {
            for (int x = -r+x_center; x <= r+x_center; ++x) {
                if (x < 0) continue;
                if (x > 99) break;
                for (int y = r+y_center; y >= -r+y_center; --y) {
                    if (y > 99) continue;
                    if (y < 0) break;
                    
                    if (isCircle(x-x_center, y-y_center, r)) {
                        if (grid[x][y] == true) ++tmp;
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

    printf("%s%d\n%s%d\n%s%d\n", "x: ", coord.x, "y: ", coord.y, "Number of targets hit: ", max);

    return 0;
}
