 1 //Author: kaevin Barta
  2 //version: 1.0
  3 //file: floatfun.c
  4 //Date: 1/29/2024
  5 //Purpose: this program is helping us learn how floating point numbers are stored and how we can convert a float to binary/hex/mantissa. We are also getting practice with bitwise oprands again.
  6 #include <stdlib.h>   /* atof */
  7 #include <stdio.h>
  8
  9 //printBits helps print out the Hex and binary representation of the float. param num- A float
 10 void printBits(float num);
 11
 12 int main(int argc,char *argv[] ) {
 13     float y;
 14     printf("Author: Kaevin Barta\n");
 15     /* verify a cmd line arg was provided */
 16     if( argc != 2 ) {
 17         printf("Usage: %s float_value\n", argv[0]);
 18         return -1;
 19     }
 20
 21     /* get float value from cmd line */
 22     y = (float) atof(argv[1]);
 23
 24     //print out the decimal number
 25     printf("float val: %.6f\n",y);
 26
 27     //function for hex and binary printing
 28     printBits(y);
 29
 30     //print each component induvadualy by shifting through the bits using bitwise oprands and creating a pointer to store the adress of the float.
 31     unsigned int* ptr = (unsigned int*)&y;
 32     //10000000000000000000000000000000 = 0x80000000
 33     unsigned int sign = (*ptr & 0x80000000) >> 31;
 34     //01111111100000000000000000000000 = 0x7F800000
 35     unsigned int exponent = (*ptr & 0x7F800000) >> 23;
 36     //00000000011111111111111111111111 = 0x007FFFFF
 37     unsigned int fraction = *ptr & 0x007FFFFF;
 38
 39     printf("\nSign:         %u       0x%x   %u\n", sign, sign, sign);
 40     printf("Exp bits:   %u      0x%x   ", exponent, exponent);
 41     for (int i = 7; i >= 0; i--) printf("%d", (exponent >> i) & 1);//bitwise AND oprand with 1 for binary
 42     printf("\n");
 43     printf("Fraction:     %u   0x%05x   ", fraction, fraction);
 44     //print in binary
 45     for (int i = 22; i >= 0; i--) printf("%d", (fraction >> i) & 1);
 46     printf("\n");
 47
 48     /* note: you can’t print a float as hex.  One solution    */
 49     /* is to create an unsigned int* and store the address of */
 50     /* the float variable in it, and use that when printing   */
 51     /* in hex and binary. A second solution would be to use a */
 52     /* union (will require a bit of research on your part)    */
 53     return 0;
 54 }
 55
 56 void printBits(float num) {
 57     //cast to an unsigned int
 58     unsigned int* ptr = (unsigned int*)& num;
 59     //print in hex
 60     printf("In hex:     0x%08x\n", *ptr);
 61     printf("In binary:  ");
 62     //get size of int
 63     int size = sizeof(int) * 8;
 64
 65     // Loop through each bit
 66     for (int i = size - 1; i >= 0; --i) {
 67         // Shift the number to the right by i bits, then bitwise AND with 1
 68         int bit = (*ptr >> i) & 1;
 69         printf("%d", bit);//print int as a signed int
 70         if (i % 4 == 0){
 71             printf(" ");//this is for athsetics,puts a space between bytes
 72         }
 73     }
 74
 75     printf("\n");
 76 }
        
