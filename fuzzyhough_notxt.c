#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double dist(int ay, int ax, int by, int bx) {
	return sqrt(pow(ax-bx, 2.0)+pow(ay-by, 2.0));
}

void threelargest(int accumulator[], int acc_size, double radius, int xs[], int ys[]) { //TEMPORARY -- prints 3 most promising annuli for a given accumulator matrix
    int a, b, c, ai, bi, ci;																		//see int *hough(...) for more on this
		a = b = c = 0;
    for (int i = 0; i < acc_size ; i++) {
        if (accumulator[i] > accumulator[a]) {
            c = b; b = a; a = i;
        }
        else if (accumulator[i] > accumulator[b]) {
            c = b; b = i;
        }
        else if (accumulator[i] > accumulator[c])
            c = i;
    }
		printf("(%d, %d), r=%f: %d\n", xs[a], ys[a], radius, accumulator[a]);
		printf("(%d, %d), r=%f: %d\n", xs[b], ys[b], radius, accumulator[b]);
		printf("(%d, %d), r=%f: %d\n", xs[c], ys[c], radius, accumulator[c]);
}

int *hough(int rows, int cols, int img[rows][cols], double radius, int fuzzy, int xs[], int ys[], int photonCount) {
	printf("Generating Accumulator for r=%f\n", radius); //allocates accumulator matrix
	int *accumulator = malloc(photonCount * sizeof(int));
	int acc;

	for (int a = 0; a < photonCount; a++) { //iterates over photons in the image to check distances of other photons
		int centerX = xs[a];
		int centerY = ys[a];
		int acc = 0;
		// printf("Center: (%d, %d)\n", centerX, centerY);
		for (int b = 0; b < photonCount; b++) {
			int photonX = xs[b];
			int photonY = ys[b];
			if (a != b) { //checks if distance of 2 compared photons falls within fuzzy distance range
				 int distance = dist(photonY, photonX, centerY, centerX);
				 // printf("[%d/%d] Checking photon: (%d, %d), dist = %d\n", a, photonCount, photonX, photonY, distance);
				 if (distance >= radius-fuzzy && distance < radius+fuzzy)
					 acc++;
			}
		}
		accumulator[a] = acc;
	}

	printf("Accumulator completed: \n"); //outputs the 3 "most promising" annuli locations--for now, that's just the accumulator cells with the highest count
																			//the plan is to return to this and use Poissonian statistics to determine local outliers as significant
  threelargest(accumulator, photonCount, radius, xs, ys);
	return accumulator;
}

int main(int rows, int cols, int data[][cols]) {
	int numPhotons = 0, r, c;
	for (r = 0; r < rows; r++)
		for (c = 0; c < cols; c++)
			if (data[r][c] > 0)
				numPhotons++;

	int photonxs[numPhotons], photonys[numPhotons]; //finds and saves locations of photons in the image in photonxs[] and photonys[]
	int a = 0;
	for (r = 0; r < rows; r++) {
		for (c = 0; c < cols; c++) {
			if (img[r][c] > 0) {
				photonxs[a] = c;
				photonys[a] = r;
				a++;
			}
			if (a == numPhotons)
				break;
		}
		if (a == numPhotons)
			break;
	}

	int* houghCenters; //runs the fuzzy hough transform on the collected data with test radii spanning the size of the image
	for (double radius = 25; radius < (rows < cols ? rows : cols) / 2.0; radius += (rows < cols ? rows : cols) / 20.0) {
		houghCenters = hough(rows, cols, img, radius, 20, photonxs, photonys, numPhotons);
		free(houghCenters);
	}

	fclose(fp); //cleanup
	free(img);
	return 0;
}
