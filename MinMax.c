#include<stdio.h>

int * minMax(int *storedValues, int numValues);
int main(void){
	int numValues;

	printf("Number of values you want to input: ");
	scanf("%d", &numValues);

	int storedValues[numValues];

	printf("Input %d values:\n", numValues);
	for(int i = 0; i < numValues; i++){
		scanf("%d", storedValues + i);
	}
	
	int *returnedValues = minMax(storedValues, numValues);

	printf("Min: %d\nMax: %d\n", *returnedValues, *(returnedValues + 1));
	return 0;
}

int * minMax(int *storedValues, int numValues){
	int max, min;
	static int minMax[2];

	max = *(storedValues);
	min = *(storedValues);

	for(int i = 0; i < numValues; i++){
		if(*(storedValues + i) > max){
			max = *(storedValues + i);
		} else if(*(storedValues + i) < min){
			min = *(storedValues + i);
		}
	}

	*(minMax) = min;
	*(minMax + 1) = max;

	return minMax;
}
