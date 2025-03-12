// globals.h
// ----------
// global stuff for mtutils
// written by Matto58 and licensed under the MIT license
//
// github.com/BrilliantOS/mtutils

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// comment out the line below if you'd like to hide debug messages in stdout
// (i.e. prepare for prod)
#define DEBUG

// maxsize
// desc:
//	compares a and b and returns the bigger value
// args:
//	size_t a - value a
//	size_t b - value b
// returns:
//	a if a is bigger than b, otherwise b
size_t maxsize(size_t a, size_t b) {
	return a > b ? a : b;
}
// minsize
// desc:
//	compares a and b and returns the smaller value
// args:
//	size_t a - value a
//	size_t b - value b
// returns:
//	a if a is smaller than b, otherwise b
size_t minsize(size_t a, size_t b) {
	return a < b ? a : b;
}

// orCmp
// desc:
// 	compares a string with 2 other strings using OR
// args:
// 	const char *source - source string
// 	size_t srcLen      - length of the source string
// 	const char *matchA - first string to match against
// 	size_t lenA        - length of the first string
// 	const char *matchB - second string to match against
// 	size_t lenB        - length of the second string
// returns:
// 	1 if source and matchA match or if source and matchB match, otherwise 0
int orCmp(
	const char *source, size_t srcLen,
	const char *matchA, size_t lenA,
	const char *matchB, size_t lenB
) {
	return (
		strncmp(source, matchA, maxsize(srcLen, lenA)) == 0
		|| strncmp(source, matchB, maxsize(srcLen, lenB)) == 0
	);
}
// orCmpS
// desc:
//	simpler version of orCmp
// args:
//	const char *source - source string
//	const char *matchA - first string to match against
//	const char *matchB - second string to match against
// returns:
// 	1 if source and matchA match or if source and matchB match, otherwise 0
int orCmpS(const char *source, const char *matchA, const char *matchB) {
	return orCmp(
		source, strnlen(source, sizeof source),
		matchA, strnlen(matchA, sizeof matchA),
		matchB, strnlen(matchB, sizeof matchB)
	);
}

/*
// sortSwap
// desc:
// 	swaps 2 items in a string array
// args:
// 	char **arr - the array (duh)
// 	int i1     - first index
// 	int i2     - second index
void sortSwap(char **arr, int i1, int i2) {
	char *temp = arr[i1];
	arr[i1] = arr[i2];
	arr[i2] = temp;
}

// bubbleSortStrsKS
// desc:
// 	sorts an array of strings with a known size with bubble sort
// args:
// 	char **arr     - the array, duh
//	size_t len     - size of the array
//	int descending - whether to sort in descending order or not
void bubbleSortStrsKS(char **arr, size_t len, int descending) {
	// TODO: make this actually work
	for (size_t i = 0; i < len-1; i++) {
		for (size_t j = 0; j < len-i-1; j++) {
			int n = strcmp(arr[j], arr[j+1]);
			if (n < 0) {
				sortSwap(arr, j, j+1);
				break;
			}
		}
	}
	if (!descending) {
		// reversing if asc. order
		for (int i = 0; i < len/2; i++) {
			char *temp = arr[i];
			arr[i] = arr[len-1-i];
			arr[len-1-i] = temp;
		}
	}
}

// bubbleSortStrs
// desc:
// 	sorts an array of strings with an unknown size with bubble sort
// args:
// 	char **arr - the array
//	int descending - whether to sort in descending order or not
void bubbleSortStrs(char **arr, int descending) {
	size_t len = (sizeof arr)/(sizeof arr[0]);
	bubbleSortStrsKS(arr, len, descending);
}
*/
