/** @file TwoDList.h
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


#ifndef TWODLIST_H
#define TWODLIST_H

#include <iostream>

class TwoDList {
   /* Overloaded ostream operator, prints out details
      from the skip-list; details all nodes present in
      each level. */
   friend std::ostream& operator<<(std::ostream& os, const TwoDList& list);

   public:
      /* Constructor, sets maxLevels_ to 1 by default while allowing
         a specified number of levels to be passed by its one parameter.
         Dynamically allocates heads_ and tails_ array of pointers  */
      explicit TwoDList(int maxLevels = 1); 
      
      /* Destructor:
         Deallocates all nodes and nulls all pointers */
      ~TwoDList();
      
      /** Adds a new entry to this list.
         @pre none.
         @post  If successful, newEntry is stored at the lowest
         level of the list and sorted. There is a 50% chance that
         newEntry will also be added to one level higher.
         @param newEntry  The object to be added as a new entry.
         @return Always returns true */ 
      bool insert(int newEntry);

      /** Removes all instances of a given entry from this list,
         if present.
         @pre none.
         @post  If successful, anEntry has been removed from each
         level of the list.
         @param anEntry  The entry to be removed.
         @return True when all nodes have been removed, false otherwise */
      bool erase(int anEntry);


      /** Tests whether this list contains a given entry at any level.
         @pre none.
         @param anEntry  The entry to locate.
         @return  True if list contains anEntry, or false otherwise. */
      bool contains(int anEntry);

   private:
      struct TwoDListNode {
         /* TwoDListNode Constructor:
            Sets all data members. Pointers are nullptr, data
            is set by argument passed */
         explicit TwoDListNode(int data);

         int data_;
         TwoDListNode* next_;
         TwoDListNode* prev_;
         TwoDListNode* upLevel_;
         TwoDListNode* downLevel_;
      };

      int maxLevels_;
      TwoDListNode** heads_;
      TwoDListNode** tails_;


      /** 
         @pre newNode is set up to precede nextNode at the appropriate level
         @post The pointers are arranged to complete the insertion of newNode
         @param newNode The node to be added
         @param nextNode The node to be immediately after newNode, contains a
            value greater than newNode
         @param level The level at which the insertion is to be done */
      void addBefore(TwoDListNode* newNode, TwoDListNode* nextNode, int level);

      /** 
         @pre none.
         @post If true, a new entry is entered at a higher level.
         @param none.
         @return  True 50% of the time, false 50% of the time. */
      bool alsoHigher() const;
      
      /** Checks whether a level is empty.
         @pre none.
         @post none.
         @return True if both heads_ and tails_ at the same index are nullptr, 
         false otherwise. */
      bool levelIsEmpty(int level) const;

      /** removes all nodes via deallocation
         @pre none.
         @post  List contains no items, and the count of items is 0. */
      void clear();

};


#endif