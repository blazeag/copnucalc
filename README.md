# CoPNuF - Compulsive Prime Numbers Finder

This senseless but unrestrainable C program allows you to compulsively calculate all prime numbers that your RAM can hold.
The basic algorithm consists in checking all odd numbers from 3 to infinity. Each number is divided by all primes already found, which are smaller than its half. If none of them is a divider, the number is prime and is put into the list.

Calculation can be interrupted and resumed at any time, because at the end of each session (which happens whenever Enter is pressed) the list of found numbers is stored into a specified file as plain text, separated by newline character \n.

### Requirements

* A PC running *nix OS
* As much RAM as possible

### Usage

./copnuf &lt;filename&gt;
