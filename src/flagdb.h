#ifndef FLAGPOLE_H_
#define FLAGPOLE_H_


#include <stdint.h>
#include <stdlib.h>
#include <Godot.hpp>
#include <Reference.hpp>


namespace godot {

class FlagDB : public Reference {
    GODOT_CLASS(FlagDB, Reference)

    private:
        const uint8_t WORD_LENGTH = 64;

        uint64_t *flag_pool;
        uint64_t flag_pool_size;
        uint64_t flag_pool_capacity;

        void _expand_pool(uint);

    public:
        static void _register_methods();

        FlagDB();
        ~FlagDB();
        void _init( uint64_t size = 0);

        bool read  ( uint64_t bit );
        void tick  ( uint64_t bit );
        void clear ( uint64_t bit );
        void flip  ( uint64_t bit );
        void show  ( uint64_t start, uint64_t range);
        void save  ( String path );
        void load  ( String path );
    };

}


#endif // FLAGPOLE_H_
