#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double dist(int ay, int ax, int by, int bx) {
	return sqrt(pow(ax-bx, 2.0)+pow(ay-by, 2.0));
}


int *hough(int rows, int cols, int img[rows][cols], double radius, int fuzzy, int xs[], int ys[], int photonCount, int cutoff) {
	// int (*accumulator)[cols] = malloc(sizeof(int[rows][cols]));
	int *accumulator = malloc(photonCount * sizeof(int));
	int acc;
	printf("Generating Accumulator\n");

	for (int a = 0; a < photonCount; a++) {
		int centerX = xs[a];
		int centerY = ys[a];
		int acc = 0;
		printf("Center: (%d, %d)\n", centerX, centerY);
		for (int b = 0; b < photonCount; b++) {
			int photonX = xs[b];
			int photonY = ys[b];
			if (a != b) {
				 int distance = dist(photonY, photonX, centerY, centerX);
				 printf("[%d/%d] Checking photon: (%d, %d), dist = %d\n", a, photonCount, photonX, photonY, distance);
				 if (distance >= radius-fuzzy && distance < radius+fuzzy)
					 acc++;
			}
		}
		accumulator[a] = acc;
	}

	printf("Accumulator created: \n");
	for (int i = 0; i < photonCount; i++)
		printf("%d ", accumulator[i]);
	// free(accumulator);
	return accumulator;
}

int main(int argc, char **argv) {
	char* filename = argv[1];
	char buf[4096];
	FILE *fp;

	if ((fp = fopen(filename, "r")) == NULL) {
		printf("Error opening file\n");
		exit(1);
	}

	int rows = 0;
	int cols = 0;
	int numPhotons = 0;
	while (fgets(buf, sizeof(buf), fp) != NULL) {	//get dimensions of image, store in rows & cols
		if (rows == 1) //just so we only do this count once
			cols = strlen(buf)-1;
		rows++;
	}

	int (*img)[cols] = malloc(sizeof(int[rows][cols])); //stores image in 2d int array img[][]
	fseek(fp, 0, SEEK_SET);
	char *cp;
	int r = 0;
	int c;
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		c = 0;
		cp = buf;
		while (*cp) {
			int pix = *cp++ - 48;
			img[r][c++] = pix;
			if (pix > 0)
				numPhotons++;
		}
		r++;
	}

	int photonxs[numPhotons], photonys[numPhotons];
	int a = 0;
	for (r = 0; r < rows; r++) {	//stores locations of photons in the image
		for (c = 0; c < cols; c++) {
			if (img[r][c] > 0) {
				photonxs[a] = r;
				photonys[a] = r;
				a++;
			}
			if (a == numPhotons)
				break;
		}
		if (a == numPhotons)
			break;
	}

	int* houghCenters = hough(rows, cols, img, 50, 5, photonxs, photonys, numPhotons, 10);

	fclose(fp);
	free(img);
	free(houghCenters);
	return 0;
}
