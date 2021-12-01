/** @file main.cpp
   Angelo Williams | 2021.11.03
   
   Improving Search Performance: Skip lists

   This file contains the TwoDList class that is accomodated
   by a driver file which prints out the contents of the list
   including each level and the values inputted at each level
   (displays "empty" if not present at a certain level).
   
   I/O: Input takes integers that are added to the list.
      A loop entering random numbers may be used to test
      the functionality and observe the probabilistic nature
      of which integers ascend to specific levels.
      
      Output can be printed using the ostream insertion operator.

   Key Variables: all pointer and pointer array variables
      The pointer array variables are responsible for keeping track
      of both head and tail pointers.
      
      Each node will have four pointers in each of the 4 "cardinal"
      directions. When insertions and removals are made, no dangling
      pointers should remain.
   
   Exceptions: If there are no nodes in the list, traverseCount
      will be zero. */

#include <iostream>
#include <random>

#include "TwoDList.h"

std::default_random_engine generator;
std::uniform_int_distribution<int> uniform(1,100);

// Testing INSERT
void test01()
{
   std::cout << std::endl;
   std::cout << "Testing insert." << std::endl;
   TwoDList ns(3);
   std::cout << std::endl;

   for (int i = 0; i < 20; ++i) {
      int number = uniform(generator);
      ns.insert(number);
      std::cout << "After adding " << number << std::endl;
      std::cout << ns << std::endl << std::endl;
   }
}
// Testing CONTAINS
void test02()
{
   std::cout << std::endl;
   std::cout << "Testing contains" << std::endl;
   TwoDList ns(19);
   std::cout << std::endl;



   for (int i = 0; i < 100; ++i) {
      int number = uniform(generator);
      ns.insert(number);
   }

   std::cout << std::endl;
   std::cout << ns << std::endl;
   std::cout << std::endl;

   for (int i = 0; i < 10; ++i) {
      int number = uniform(generator);
      std::cout << "Searching for " << number << std::endl;
      if(ns.contains(number))
         std::cout << "This list contains " << number << "." << std::endl;
      else 
         std::cout << number << " is not in the list." << std::endl;
      std::cout << std::endl;
   }

}
// Testing INSERT + ERASE
void test03()
{
   std::cout << std::endl;
   std::cout << "Testing insert & erase" << std::endl;
   TwoDList ns(3);
   std::cout << std::endl;

   std::cout << "Adding 4, 7, 6, and 82" << std::endl;
   std::cout << std::endl;

   ns.insert(4);
   ns.insert(7);
   ns.insert(6);
   ns.insert(82);

   std::cout << ns << std::endl;
   std::cout << std::endl;
   std::cout << "Removing 82..." << std::endl;
   std::cout << std::endl;

   ns.erase(82);
   ns.erase(35);

   std::cout << ns << std::endl;
   std::cout << std::endl;
   std::cout << "Removing 7..." << std::endl;
   std::cout << std::endl;

   ns.erase(123);
   ns.erase(7);

   std::cout << ns << std::endl;
   std::cout << std::endl;
   std::cout << "Removing 6..." << std::endl;
   std::cout << std::endl;
   
   ns.erase(7);
   ns.erase(6);

   std::cout << ns << std::endl;
}

// Testing ONLY ERASE
void test04() {

   std::cout << std::endl;
   std::cout << "Testing erase without any nodes" << std::endl;
   TwoDList ns(3);
   std::cout << std::endl;

   ns.erase(4);
   ns.erase(35);

   std::cout << ns << std::endl;
}

int main() {
   
   test01();
   test02();
   test03();
   test04();
 
   return 0;
}