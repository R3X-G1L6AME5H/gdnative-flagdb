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

        unsigned int *flag_pool;
        unsigned int flag_pool_size;
        unsigned int flag_pool_capacity;

        void resize_pool(unsigned int new_size);

    public:
        /* BOILER-PLATE */
        static void _register_methods();

        FlagDB();
        ~FlagDB();
        void _init();

        /* METHODS */
        bool read  ( unsigned int bit );
        void tick  ( unsigned int bit );
        void clear ( unsigned int bit );
        void flip  ( unsigned int bit );
        void save  ( String path );
        void load  ( String path );

        /* DEBUG */
        void bit_table ( unsigned int start, unsigned int range);

        unsigned int size();
        unsigned int capacity();
    };

}


#endif // FLAGPOLE_H_
