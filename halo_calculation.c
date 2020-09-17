#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double dist(int ay, int ax, int by, int bx) {
	return sqrt(pow(ax - bx, 2.0) + pow(ay - by, 2.0));
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

int *hough(int xs[], int ys[], int photonCount) {
	int rows = max(ys, photonCount) - min(ys, photonCount);
	int cols = max(xs, photonCount) - min(xs, photonCount);
	int minRadius = (rows < cols ? rows : cols) / 25.0; //the smallest radius size you want to test
	int maxRadius = (rows < cols ? rows : cols) / 1.5;
	int nCenters = (rows*cols)/10;	//number of different centers you test for each radius; they're uniformly spaced throughout the image
	int nRadii = 20;	//number of radii you want to test
	double alpha = 20;	//number of pixels leeway you want to give the photons; accumulator merit value falls off quadratically, scaled b/w 0 and 1 
	double maxSigma = 0;
	double photonDensity = (double)photonCount / (rows * cols);
	printf("%d photons detected; image dimensions are %d x %d; trying %d centers\n", photonCount, rows, cols, nCenters);
	
	int centerX = 0, centerY = 0, stepAcc = 0, photonX = 0, photonY = 0, rCounter = 1;
	double quadAcc, distance, tdist;

	int maxAcc = 0, argmaxX = 0, argmaxY = 0, argmaxRad = 0;
	double maxQuadAcc = 0;

	for (double radius = minRadius; radius < maxRadius; radius += (maxRadius - minRadius) / (double)nRadii) {
		if (rCounter % 10 == 1 || rCounter == nRadii)
			printf("Generating Accumulator for r=%f (%d/%d)\n", radius, rCounter, nRadii); //allocates accumulator matrix
		rCounter += 1;
		//int *stepAccumulator = malloc(photonCount * sizeof(int));
		for (int centralPixel = 0; centralPixel <= rows*cols; centralPixel += (rows*cols) / nCenters) { //iterates over points in the image to check distances of other photons
			centerX = centralPixel % cols;
			centerY = centralPixel / rows;
			quadAcc = 0;
			stepAcc = 0;
			for (int testPhoton = 0; testPhoton < photonCount; testPhoton++) {
				photonX = xs[testPhoton];
				photonY = ys[testPhoton];
				distance = dist(photonY, photonX, centerY, centerX);
				tdist = abs(radius - distance); //transformed distance
				if (tdist <= alpha) {
					quadAcc += pow((alpha-tdist)/(alpha), 2.0);
					stepAcc += 1;
				}
			}
			if (quadAcc != 0)
//				printf("(%d, %d), r = %f, acc=%f, sigma=%f\n", centerX, centerY, radius, quadAcc, (stepAcc) / (3.1416 * (pow(radius+alpha, 2) - pow(radius-alpha, 2))) / photonDensity);
			if (quadAcc > maxQuadAcc) {
				maxQuadAcc = quadAcc;
				argmaxX = centerX;
				argmaxY = centerY;
				argmaxRad = radius;
				maxSigma = (stepAcc) / (3.1416 * (pow(radius+alpha, 2) - pow(radius-alpha, 2))) / photonDensity;
//				printf("new max: (%d, %d), r = %f, acc=%f, sigma=%f\n", centerX, centerY, radius, quadAcc, maxSigma); //formally, this isn't necessarily the max sigma, because the sigma is calculated using a step function cutoff, whereas the quadAcc is calculated with a quadratic cutoff
			}
		}
	}
	printf("exited accumulator loop\n");

	int *ret = malloc(4 * sizeof(int));
	ret[0] = (int)argmaxX;
	ret[1] = (int)argmaxY;
	ret[2] = (int)argmaxRad;
	ret[3] = (int)maxSigma;
	return ret;
}
