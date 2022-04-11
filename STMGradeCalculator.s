/**
  *****************************************************************************
  *		 FOR STM32L4
  *
  *      This program checks inputted arrays and implements a for loop to find
  *      Minimum, Maximum, Average and Median Values while holding the array info
  *      In register r0, and accessing values from it. Code also uses nested con-
  *		 ditional statements.
  *
  *
  *      @file      STMGradeCalculator.s
  *      @author    Dora Avun
  *      @date      2021-02-21
  *****************************************************************************
OUTPUT:

V1: 5,4,3,2,1,-1

V2: 0,0,0,-1

V3: 0,-1

V4: 100,100,100,100,-1

V5: 50,60,70,80,90,100,-1

V6: 0,86,37,47,14,94,69,25,54,10,1,24,91,82,5,41,5,50,48,60,39,51,58,58,72,-1

        V1 V2 V3 V4    V5    V6
min:   1   0   0  100   50     0
max:  5   0   0  100   100   94
avg:   3   0   0  100   75     44
med:  3   0   0  100   75     48

*/
.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.section .data
.balign



array: .word 0,86,37,47,14,94,69,25,54,10,1,24,91,82,5,41,5,50,48,60,39,51,58,58,72,-1//DR. Li, please paste the array here.

.section .text
.balign
.global main

main:

	//add_highest: .word highest
	LDR r0, =array
	LDR r1, [r0] //next value to check in array

	MOV r2, 0    // highest value
	MOV r3, 999  // lowest value
	MOV r4, 0    // average value
	MOV r5, 2    // median value

	MOV r6, 4 // array shift index (#4 bits)
	MOV r7, 0 // number of elements in the array
	MOV r8, 0 // summation of all elements in the array

    //NAMING OUR VARIABLES
    gradeData .req r0
	next .req r1
	highest .req r2
	lowest .req r3
	avg .req r4
	median .req r5
	arrayLoc .req r6
	elementCount .req r7
	sum .req r8
	arrayShifter .req r9

	startLoop:                             // Start our loop
		  CMP next, -1					   // Compare next value with -1
		  BEQ endLoop                      // If our next value to check is -1, end the loop
		  	CMP highest, next              // Compare highest value with next value
		  		BGE endif                  // If Highest value is greater than or equal to next value, go to endif
					MOV highest, next      // If next value is greater than highest value, set highest value to the new value.
			  	endif:

 	            CMP lowest, next           // Compare Lowest value with next value
			 	BLE endif1                 // If Lowest value is lower than or equal to next value, go to endif1
			 		MOV lowest, next       // If next value is lower than the lowest value, the lowest value is now the new value

			 	endif1:
			ADD sum, next                  // We update the 'sum' by adding the next value to the sum
			LDR next, [gradeData, arrayLoc]// We load the next value in the array to 'next'
			ADD arrayLoc, arrayLoc, #4     // We increment arrayLoc by 4-bits to ensure we load the next value in the following run
			ADD elementCount, #1           // We increment the number of elements by 1


		  B  startLoop                     // While our element is not equal to -1, we continue our loop
	endLoop:

	SDIV avg, sum, elementCount        //set average to sum / elementCount

	MOV arrayLoc, #4                   //initialize arrayLoc back to 4

	//Median only works with odd number of elements
	SDIV median, elementCount, median  // divide number of elements to #2 which is initialized in r5
	BCS odd                            //if carry is set from division, go to odd
	SUB median, median, #1
	odd:
	MUL median, median, arrayLoc       //Multiply median index by arrayLoc (#4) to load wanted value from array
	LDR median, [gradeData, median]    //Load median value from our data array.

forever:
	b     forever
