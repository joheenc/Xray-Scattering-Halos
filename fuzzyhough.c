#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double dist(int ay, int ax, int by, int bx) {
	return sqrt(pow(ax-bx, 2.0)-pow(ay-by, 2.0));
}


int *hough(int rows, int cols, int img[rows][cols], double radius, int fuzzy, int startx, int starty, int endx, int endy, int centerstep, int cutoff) {
	int (*accumulator)[cols] = malloc(sizeof(int[rows][cols]));
	int acc;
	for (int r = startx; r < endx; r += centerstep) {
		for (int c = starty; c < endy; c += centerstep) {
			acc = 0;
			for (int a = r - radius - fuzzy; a < r + radius + fuzzy; a++) {
				for (int b = c - radius - fuzzy; b < r + radius + fuzzy; b++) {
					double distance = dist(a, b, r, c);
					if (img[a][b] == 1 && distance >= radius-fuzzy && distance <= radius+fuzzy)
						acc++;
				}
			}
			accumulator[r][c] = acc;
		}
	}

	int returnPointCount = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (accumulator[i][j] >= cutoff)
				returnPointCount++;
		}
	}

	int *returnPoints = malloc(returnPointCount * sizeof(int) * 2);
	int count = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (accumulator[i][j] >= cutoff) {
				returnPoints[count++] = i;
				returnPoints[count++] = j;
				printf("%d, %d, rad=%f\n", i, j, radius);
			}
		}
	}
	
	free(accumulator);
	return returnPoints;
}
int main() {
	char* filename = "normimg.txt";
	char buf[4096];
	FILE *fp;

	if ((fp = fopen(filename, "r")) == NULL) {
		printf("Error opening file\n");
		exit(1);
	}

	int rows = 0;
	int cols = 0;
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		if (rows == 1)
			cols = strlen(buf)-1;
		rows++;
	}

	int (*img)[cols] = malloc(sizeof(int[rows][cols]));
	fseek(fp, 0, SEEK_SET);
	char *cp;
	int r = 0;
	int c;
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		c = 0;
		cp = buf;
		while (*cp)
			img[r][c++] = *cp++ - 48;
		r++;
	}

	for (int rad = 10.0; rad < 200.0; rad+=10.0) {
		int *houghCenters = hough(rows, cols, img, rad, 5, 0, 0, rows, cols, 3, 10);
		free(houghCenters);
	}
	fclose(fp);
	free(img);
	return 0;
}

