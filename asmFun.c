 1 //Author: kaevin Barta
  2 //file: asmfun.c
  3 //version: 1.0
  4 //Date 2/12/24
  5 //this program is getting us more confrable with in depth assembly using the floating oint unit to oprate on floating point numbers, This project also helps with learning how to read dense intel manuels.
  6 #include <stdio.h>
  7 //this function will print out the VendorID from the cpu using
  8 void printCpuVendorID();
  9 //this function take in a rdius and will calculate the volume of a sphere witht that radius using embedded assembly. param radius - radius of sphere  returns- volume of sphere
 10 double sphereVolume(double radius);
 11
 12 int main() {
 13     printf("Author: Kaevin Barta\n");
 14
 15     double radius = 4.5;
 16     double result = sphereVolume(radius);
 17     printf("Sphere volume with a radius %.2f: %.2f\n", radius, result);
 18     printCpuVendorID();
 19     return 0;
 20 }
 21
 22
 23 double sphereVolume(double radius) {
 24     double volume;
 25     double pi = 3.14159;
 26     double fourOverThree = 4.0/3.0;
 27     asm(
 28             "fldl %[fourOverThree] \n"//load 3/4 onto the floating point stack
 29             "fldl %[pi] \n"//load pi onto the floating point stack
 30             "fmulp \n"//multiply the two values presnt on the FP stack and stores it there too
 31             "fldl %[radius] \n"//load the radius onto the FPU
 32             "fldl %[radius] \n"//load the value again so we can raise to the power of 2
 33             "fmulp \n"//multiply the radius by itself
 34             "fldl %[radius] \n"//one more time for cube
 35             "fmulp \n"//multiply r^2 by r
 36             "fmulp \n"//multiply the last two items on stack being r^3 and pi*4/3
 37             "fstpl %[volume] \n"//store the value in the FPU into the memory ardress denoted by volume
 38             : [volume] "=m" (volume)//outputs
 39             : [radius] "m" (radius), [pi] "m" (pi), [fourOverThree] "m" (fourOverThree)//inputs
 40             ://no clobbers
 41        );
 42
 43     return volume;
 44 }
 45
 46 void printCpuVendorID() {
 47     char vendorID1[5], vendorID2[5], vendorID3[5];
 48     //If the EAX register contains an input value of 0, the CPUID instruction also returns the vendor identification string in the EBX, EDX, and ECX registers (see Figure 2). These registers contain the ASCII strings
 49     //insert assembly to get vendor/manufacturer ID string
 50     //insert C code to print out the ID string
 51     asm(
 52             "mov $0, %%eax \n"//set th EAX register to contain input of 0
 53             "cpuid \n"
 54             "mov %%ebx, %[vendorID1] \n"//double percent sign because its a register not an oprand
 55             "mov %%edx, %[vendorID2] \n"//move the secound part to ID2
 56             "mov %%ecx, %[vendorID3] \n"//move the last of vendor ID3
 57             :[vendorID1] "=m" (vendorID1),[vendorID2] "=m" (vendorID2),[vendorID3] "=m" (vendorID3) //vars
 58             ://no inputs
 59             : "%eax", "%ebx", "%ecx", "%edx"//clobbered register list
 60        );
 61     vendorID1[4] = '\0';//printf function needs null terminated
 62     vendorID2[4] = '\0';
 63     vendorID3[4] = '\0';
 64     printf("The CPU vendor ID for this computer is: %s%s%s\n", vendorID1, vendorID2, vendorID3);//this was actually a very intresting project!
 65 }
 66
