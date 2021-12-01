/** @file TwoDList.cpp
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
#include <vector>
#include <random>
#include "TwoDList.h"

/* Constructor:
   Sets maxLevels_ to 1 by default while allowing
   a specified number of levels to be passed by its one parameter.
   Dynamically allocates heads_ and tails_ array of pointers  */
TwoDList::TwoDList(int maxLevels) : maxLevels_(maxLevels) {
   // Set maxLevels 
   heads_ = new TwoDListNode* [maxLevels_];
   tails_ = new TwoDListNode* [maxLevels_];

   for(int i = 0; i < maxLevels_; i++) {
      heads_[i] = nullptr;
      tails_[i] = nullptr;
   }
}

/* Destructor:
   Deallocates all nodes and nulls all pointers */
TwoDList::~TwoDList() { 
   // Clear each level via loop
   clear(); 
}

/* TwoDListNode Constructor:
   Sets all data members. Pointers are nullptr, data
   is set by argument passed */
TwoDList::TwoDListNode::TwoDListNode(int data)
   : data_(data), next_(nullptr), prev_(nullptr),
   upLevel_(nullptr), downLevel_(nullptr) {}

/* Overloaded ostream operator, prints out details
   from the skip-list; details all nodes present in
   each level. */
std::ostream &operator<<(std::ostream& os, const TwoDList& list) {
   // os << "After adding " << 
   /* Start at the highest level, then work down */
   for (int i = (list.maxLevels_ - 1); i >= 0; i--) {
      TwoDList::TwoDListNode* cur = list.heads_[i];
      os << "Level: " << i << " -- ";
      if (list.heads_[i] == nullptr) {
        os << "empty";
      } else {
         while (cur != nullptr) {
            if (cur->next_ != nullptr)
               os << cur->data_ << ", ";
            else
               // Last number (without comma)
               os << cur->data_;
            cur = cur->next_;
         } // end while
      } // end if
      os << std::endl;
   }
   return os;
}

/** Adds a new entry to this list.
   @pre none.
   @post  If successful, newEntry is stored at the lowest
   level of the list and sorted. There is a 50% chance that
   newEntry will also be added to one level higher.
   @param newEntry  The object to be added as a new entry.
   @return Always returns true */ 
bool TwoDList::insert(int newEntry) 
{
   // Avoid duplicates:
   if (contains(newEntry)) {
      return false;
   } else {
      int level = maxLevels_ - 1;
      TwoDListNode* cur = heads_[level];
      TwoDListNode* nn_ = new TwoDListNode(newEntry);

      // Create a vector of pointers
      std::vector<TwoDListNode*> addBeforePtrs (maxLevels_, nullptr);
      // !!!! reset cur every time first, then do the up/down connections

      /** Collect temporary pointers */
      while (level >= 0) {
         cur = heads_[level];
         if(levelIsEmpty(level)) {
         // If levelIsEmpty, new node may be the head
            addBeforePtrs[level] = heads_[level];
         } else {
            while (newEntry > cur->data_ && cur->next_ != nullptr) {
               cur = cur->next_;
            } // end while
            
            if (newEntry > cur->data_) {
               // If it is the largest, we should add before tails_
               addBeforePtrs[level] = tails_[level];
            } else {
               addBeforePtrs[level] = cur;
            } // end if
         } // end if   
         level--; // down one level
      }

      // // Add to level 0
      addBefore(nn_,addBeforePtrs[0], 0);

      // Add to levels
      int higher = 1;
      while ((alsoHigher() && higher < maxLevels_ )) {
         TwoDListNode* nwn_ = new TwoDListNode(newEntry);
         // add before the next node
         addBefore(nwn_, addBeforePtrs[higher], higher);
         nn_->upLevel_ = nwn_;
         nwn_->downLevel_ = nn_;
         higher++;
      }
      return true;
   } // end if
} // end insert

/** 
   @pre newNode is set up to precede nextNode at the appropriate level
   @post The pointers are arranged to complete the insertion of newNode
   @param newNode The node to be added
   @param nextNode The node to be immediately after newNode, contains a
      value greater than newNode
   @param level The level at which the insertion is to be done */
void TwoDList::addBefore(TwoDListNode* newNode, TwoDListNode* nextNode, int level) {
   // If the level is empty, heads_ point to the node
   if (heads_[level] == nullptr) {
      heads_[level] = newNode;
      tails_[level] = newNode;
   } else {
      // Check if nextNode is the head node but less than newNode
      if (nextNode == heads_[level] && nextNode->data_ < newNode->data_) {
         nextNode->next_ = newNode;
         newNode->prev_ = nextNode;
         tails_[level] = newNode;
      } else if (nextNode == heads_[level] && nextNode->data_ > newNode->data_) {
         // if nextNode is the head but greater than newNode
         newNode->next_ = nextNode;
         nextNode->prev_ = newNode;
         heads_[level] = newNode;
      } else if (nextNode->data_ < newNode->data_) {
         // Special case for adding to the end
         newNode->prev_ = nextNode;
         nextNode->next_ = newNode;
         tails_[level] = newNode;
      } else {
      // Connect newNode to the list
         newNode->next_ = nextNode;
         newNode->prev_ = nextNode->prev_;
      // Rewire the connections
         newNode->prev_->next_ = newNode;
         nextNode->prev_ = newNode;
      }
   } // end if
} // end addBefore


/** Removes all instances of a given entry from this list,
   if present.
   @pre none.
   @post  If successful, anEntry has been removed from each
   level of the list.
   @param anEntry  The entry to be removed.
   @return True when all nodes have been removed, false otherwise */
bool TwoDList::erase(int anEntry) {
   if (!contains(anEntry)) {
      // return false if the value is not present
      return false;
   } else {
      /** Search for the node from the top*/
      for(int level = (maxLevels_ - 1); level >=0; level--) {
         TwoDListNode* cur = heads_[level];
         if(heads_[level] != nullptr) {
            // if exists, find the node
            while (anEntry != cur->data_ && cur->next_ != nullptr) {
               cur = cur->next_;
            } // end while
            // if there is a match
            if (anEntry == cur->data_) {
               // if there is only one node
               if (cur->prev_ == nullptr && cur->next_ == nullptr) {
                  heads_[level] = nullptr;
               } else if (cur->prev_ == nullptr) {
               // if the entry is the head node   
                  cur->next_->prev_ = nullptr;
                  heads_[level] = cur->next_;
               // if the entry is the tail node
               } else if (cur->next_ == nullptr) {
                  cur->prev_->next_ = nullptr;
                  tails_[level] = cur->prev_;
               } else {
                  cur->next_->prev_ = cur->prev_;
                  cur->prev_->next_ = cur->next_;
               }// end if
               // deallocate cur:
               delete cur;
            } // end if
         }// end if
      }// end forloop
   } // end if
   return true;
} // end erase

/** Tests whether this list contains a given entry at any level.
   @pre none.
   @param anEntry  The entry to locate.
   @return  True if list contains anEntry, or false otherwise. */
bool TwoDList::contains(int anEntry) {
   TwoDListNode* cur = heads_[maxLevels_ - 1];

   // Descend all levels from the top
   for (int i = (maxLevels_ - 1); i >= 0; i--) {
         TwoDListNode* cur = heads_[i];
         if (!levelIsEmpty(i)) {
         while (cur != nullptr) {
            if (cur->data_ == anEntry) {
               return true;
            } // end if
            cur = cur->next_;
         } // end while
      }
   } // end for loop
   return false;
} // end contains

/** 
   @pre none.
   @post If true, a new entry is entered at a higher level.
   @param none.
   @return  True 50% of the time, false 50% of the time */
bool TwoDList::alsoHigher() const
{
   // Coin Toss based on random generator
   static std::default_random_engine generator;
   static std::bernoulli_distribution coin(0.5);
   return coin(generator);
} // end alsoHigher


/** Checks whether a level is empty.
   @pre none.
   @post none.
   @return True if both heads_ and tails_ at the same index are nullptr, 
   false otherwise. */
bool TwoDList::levelIsEmpty(int level) const
{
   if((heads_[level] == nullptr && tails_[level] == nullptr) || heads_[level] == tails_[level]) {
      return true;
   } else {
      return false;
   } // end if
} // end levelIsEmpty


/** removes all nodes via deallocation
   @pre none.
   @post  List contains no items, and the count of items is 0. */
void TwoDList::clear() 
{
   // Set temporary pointer to first node
   for (int i = 0; i < maxLevels_; i++) {
   TwoDListNode* nodeToDelete = heads_[i];
      while(heads_[i] != nullptr) {
         // move heads_ ahead
         heads_[i] = heads_[i]->next_;
         // delete current
         delete nodeToDelete;
         // follow heads_
         nodeToDelete = heads_[i];
      } //end while
   }
   // Remove heads_ and tails_ array pointers
   delete [] heads_;
   delete [] tails_;
   // Remove newNode and newerNode
} // end clear