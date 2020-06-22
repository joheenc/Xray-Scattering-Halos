#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double dist(int ay, int ax, int by, int bx) {
	return sqrt(pow(ax-bx, 2.0)+pow(ay-by, 2.0));
}

	int max(int arr[], int len) {
		int max = arr[0];
		for (int i = 1; i < len; i++)
			if (arr[i] > max)
				max = arr[i];
		return max;
	}

	int min(int arr[], int len) {
		int min = arr[0];
		for (int i = 1; i < len; i++)
			if (arr[i] < min)
				min = arr[i];
		return min;
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

	int *hough(int rows, int cols, double radius, int fuzzy, int xs[], int ys[], int photonCount) {
		printf("Generating Accumulator for r=%f\n", radius); //allocates accumulator matrix
		int *accumulator = malloc(photonCount * sizeof(int));
		int acc;

		for (int centralPhoton = 0; centralPhoton < photonCount; centralPhoton++) { //iterates over photons in the image to check distances of other photons
			int centerX = xs[centralPhoton];
			int centerY = ys[centralPhoton];
			int acc = 0;
			// printf("Center: (%d, %d)\n", centerX, centerY);
			for (int testPhoton = 0; testPhoton < photonCount; testPhoton++) {
				int photonX = xs[testPhoton];
				int photonY = ys[testPhoton];
				if (centralPhoton != testPhoton) { //checks if distance of 2 compared photons falls within fuzzy distance range
					 int distance = dist(photonY, photonX, centerY, centerX);
					 // printf("[%d/%d] Checking photon: (%d, %d), dist = %d\n", a, photonCount, photonX, photonY, distance);
					 if (distance >= radius-fuzzy && distance < radius+fuzzy)
						 acc++;
				}
			}
			accumulator[centralPhoton] = acc;
		}

		printf("Accumulator completed: \n"); //outputs the 3 "most promising" annuli locations--for now, that's just the accumulator cells with the highest count
																				//the plan is to return to this and use Poissonian statistics to determine local outliers as significant
	  threelargest(accumulator, photonCount, radius, xs, ys);
		return accumulator;
	}

	int main(int photonxs[], int photonys[], int numPhotons) {
		int minRadius = 25; //the smallest radius size you want to test
		int nRadii = 20; //number of radii you want to test
		int fuzzy = 20; //number of pixels of "leeway" you want to give th photons	
		int* houghCenters; //runs the fuzzy hough transform on the collected data with test radii spanning the size of the image
		int rows = max(photonys, numPhotons) - min(photonys, numPhotons);
		int cols = max(photonxs, numPhotons) - min(photonxs, numPhotons);
	for (double radius = minRadius; radius < (rows < cols ? rows : cols) / 2.0; radius += (rows < cols ? rows : cols) / (double) nRadii) {
		houghCenters = hough(rows, cols, radius, fuzzy, photonxs, photonys, numPhotons);
		free(houghCenters);
	}
	return 0;
}
