/*
	FlagDB Header
		by Nemo Czanderlitch/Nino Čandrlić
*/


#ifndef FLAGPOLE_H_
#define FLAGPOLE_H_


#include <stdlib.h>
#include <stdio.h>
#include <Godot.hpp>
#include <Reference.hpp>


namespace godot {

class FlagDB : public Reference {
    GODOT_CLASS(FlagDB, Reference)

    private:
        const unsigned int WORD_LENGTH = sizeof(unsigned int) * 8;

        unsigned int *flag_pool;          // The array that holds all the integers whose bits we are concerned about
        unsigned int flag_pool_size;      // Keeps track of array size
        unsigned int flag_pool_capacity;  // Keeps track of how many flags are available (flag_pool_size * WORD_LENGTH)

    public:
        /* GODOT MANDATED BOILER-PLATE */
        static void _register_methods();

        FlagDB();
        ~FlagDB();
        void _init();

        /* METHODS */
        bool read  ( unsigned int bit );  // returns the boolean value representing the bit-th bit
        // sets the bit-th bit to True
	    //      - when bit > flag_pool_capacity new integers are automatically allocated
        void tick  ( unsigned int bit );
        void clear ( unsigned int bit );  // sets the bit-th bit to False
        void flip  ( unsigned int bit );  // inverts the state of the bit-th bit
        
        // Saves/Loads the database to/from a file
        //      - has no file type checks; the program will crash if incorrect file is selected
        void save  ( String path );
        void load  ( String path );

        /* DEBUG */
        // prints out the integers at index start to index (start + range) in their binary representation 
        void bit_table ( unsigned int start, unsigned int range);
        unsigned int size();     // returns array size
        unsigned int capacity(); // returns the amount of flags the database can currently hold
        
    };

}


#endif // FLAGPOLE_H_
