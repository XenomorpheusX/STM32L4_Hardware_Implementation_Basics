/**
  *****************************************************************************
  *  FOR STM32L4
  *
  *      This program checks an input array of grades and implements functions
  *      containing for loops and if statements to sort the array in an ascending
  *      order (bubbleSort) and find the Minimum (findMin), Maximum (findMax),
  *      Average (findAVG), Median (findMedian) and letter grade values
  *      (findLetterGrades) in the array. data can be observed in the 'variables'
  *      tab. My FUNCTIONS are at the bottom part of the code.
  *
  *
  *      @file      STMGradeCalculatorC.c
  *      @author    Dora Avun
  *      @date      2021-03-14
  *****************************************************************************
  */

int grades[] = {0,86,37,47,14,94,69,25,54,10,1,24,91,82,5,41,5,50,48,60,39,51,58,58,72}; //Input Grades

int main(void) // main function - serves like a driver class for my functions.

{

	// declaring/initializing my variables in a compact way.
	int min = 0, max = 0, gradeA = 0, gradeB = 0, gradeC = 0, gradeD = 0, gradeF = 0;
	double avg = 0.0, median = 0.0;

	int arraySize = sizeof(grades) / sizeof(grades[0]); //number of elements in grades[].

	//sort our array of grades using a bubbleSort.
	bubbleSort(grades, arraySize);

	//find min and max values
	max = findMax(grades, arraySize);
	min = findMin(grades, arraySize);

	//find average value
	findAVG(grades, arraySize, &avg); //'&avg' to send the address of avg in the main function to findAVG function.

	// find median value
	findMedian(grades, arraySize, &median); //'&median' to send the address of median in the main function to findMedian function.

	// find the number of each letter grade present.
	findLetterGrades(grades, arraySize, &gradeA, &gradeB, &gradeC, &gradeD, &gradeF); // we send the addresses of int grade(A-F) variables to findLetterGrades function.

}

//FUNCTIONS:


// Bubble Sorting algorithm for our grade data.
void bubbleSort(int arr[], int n)
{
   int i, j; //declaring variables

   for (i = 0; i < n-1; i++)// we use two loops for bubble sort.
   {
	   for (j = 0; j < n-i-1; j++)
	   {
		   if (arr[j] > arr[j+1]) //if value at j is greater than value at j + 1, values are swapped.
		   {
			   swapValues(&arr[j], &arr[j+1]); //swap function is called with addresses of array locations.
		   }
	   }
   }
}


// Swapping algorithm for our Bubble Sort algorithm to function properly.
void swapValues(int *a, int *b) // take parameters with pointers to actually swap locations of inputted values.
{
    int temp = *a; //record 'a' in a temporary variable so we can assign 'b' to 'a' and 'temp' to 'b'.
    *a = *b;
    *b = temp;
}


// Finds the maximum value in the array using a for loop and RETURNS the MAX value.
int findMax(int arr[], int n)
{
	int i = 0, max = 0; //declaring variables
	for (i; i < n; i++)
	{
		if (arr[i] >= max) //if i'th index of array is greater than previous max, new max is set.
		{
			max = arr[i];
		}
	}
	return max; // this is not a void function, so a value is returned
}


// Finds the minimum value in the array using a for loop and RETURNS the MIN value.
int findMin(int arr[], int n)
{
	int i = 0, min = 999; //declaring variables (min is set high to ensure first value we check is set as minimum).
	for (i; i < n; i++)
	{
		if (arr[i] <= min) //if i'th index of array is less than previous min, new min is set.
		{
			min = arr[i]; // this is not a void function, so a value is returned
		}
	}
	return min; //returns minimum value
}


// Finds the average grade value using a for loop.
void findAVG(int arr[], int n, double *avg) // We give a pointer to 'avg' in parameters to actually be able to change its value.
{
	//declaring variables
	int i;
	double sum = 0.0;

	for(i = 0; i < n; i++) //with every pass of the loop, we update the sum.
	{
		sum = sum + arr[i];
	}

	*avg = sum / n; //average is set to sum / number of elements.
}


// Finds the median value of our sorted array.
void findMedian(int arr[], int n, double *median) // We give a pointer to 'median' in parameters to actually be able to change its value.
{
	if (n % 2 == 0) // if number of elements is even
		{
			*median = (arr[(n - 1) / 2] + arr[n/2]) / 2.0; // median is the average of two values in the middle.
		}
			else //if number of elements is odd
				*median = arr[(n+1) / 2 - 1]; // median is the value in the middle of the array.
}


// Finds the numbers of each Letter Grades (A through F) present in our grade array.
void findLetterGrades(int arr[], int n, int *gradeA, int *gradeB, int *gradeC, int *gradeD, int *gradeF) // We give pointers to grade variables to be able to alter them.
{
	int i;
	for(i = 0; i < n; i++)
		{
			if (arr[i] >= 90) // A simple conditional logic to ensure we update corresponding grade counts according to belonging range of i'th index of grades.
				*gradeA = *gradeA + 1;

			else if (arr[i] >= 80)
				*gradeB = *gradeB + 1;

			else if (arr[i] >= 70)
				*gradeC = *gradeC + 1;

			else if (arr[i] >= 60)
				*gradeD = *gradeD + 1;

			else
				*gradeF = *gradeF + 1;
		}
}



