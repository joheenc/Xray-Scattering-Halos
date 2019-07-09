#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	char* filename = "normimg.txt";
	char buf[4096];
	FILE *fp;

	if ((fp = fopen(filename, "r")) == NULL) {
		printf("Error opening file\n");
		exit(1);
	}

	int r = 0;
	int c = 0;
	while (fgets(buf, sizeof(buf), fp) != NULL) {
		if (r == 1)
			c = strlen(buf)-1;
		r++;
	}
	
	printf("%d\n%d\n", r, c);
	int img[r][c];

	fclose(fp);
	return 0;
}

//int hough[](int img[][], int startRadius, int endRadius) {

//}
