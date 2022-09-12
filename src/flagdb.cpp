#include "flagdb.h"

using namespace godot;

void FlagDB::_register_methods() {
    register_method("read",  &FlagDB::read);
    register_method("tick",  &FlagDB::tick);
    register_method("clear", &FlagDB::clear);
    register_method("flip",  &FlagDB::flip);
    register_method("bit_table",  &FlagDB::bit_table);

    register_method("size",     &FlagDB::size);
    register_method("capacity", &FlagDB::capacity);
}

FlagDB::FlagDB() {}
FlagDB::~FlagDB() {
    ::free( this->flag_pool );
}

void FlagDB::_init(unsigned int size){
    if (size == 0) { this->flag_pool_size = 10;   }
    else           { this->flag_pool_size = size; }

    this->flag_pool_capacity = this->flag_pool_size * WORD_LENGTH;
    this->flag_pool = (uint_least64_t*)calloc(this->flag_pool_size, sizeof(uint_least64_t));
    for (int i = 0; i < this->flag_pool_size; i++){ this->flag_pool[i] = 0; }
}

bool FlagDB::read( unsigned int bit ){
    if (bit <= flag_pool_capacity) {
        unsigned long mask  = 1 << (bit % WORD_LENGTH);
        unsigned long index = bit / WORD_LENGTH;
        return (this->flag_pool[index] & mask) > 0;
    }
    return 0;
}

void FlagDB::tick( unsigned int bit ){
	if (bit > this->flag_pool_capacity){
        // reallocate memory
        int i = this->flag_pool_size;
        this->flag_pool_size = (bit / WORD_LENGTH) + 1;
        this->flag_pool_capacity = this->flag_pool_size * WORD_LENGTH;
        this->flag_pool = (uint_least64_t*)realloc(this->flag_pool, this->flag_pool_size * sizeof(uint_least64_t));
        for (; i < this->flag_pool_size; i++){this->flag_pool[i] = 0;}
    }

    unsigned int index = bit / WORD_LENGTH;

    uint64_t mask  = 1 << (bit % WORD_LENGTH);
    uint64_t value = this->flag_pool[index] | mask;

    this->flag_pool[index] = value;
}


void FlagDB::clear( unsigned int bit ){
    if (bit <= this->flag_pool_capacity) {
        unsigned int index = bit / WORD_LENGTH;
        uint64_t mask  = 1 << (bit % WORD_LENGTH);
        uint64_t value = this->flag_pool[index] & ~mask;

        this->flag_pool[index] = value;
    }
}
void FlagDB::flip( unsigned int bit ){
    if (bit <= this->flag_pool_capacity) {
        unsigned int index = bit / WORD_LENGTH;
        uint64_t mask  = 1 << (bit % WORD_LENGTH);
        uint64_t value = this->flag_pool[index] ^ mask;

        this->flag_pool[index] = value;
    }
}



void FlagDB::bit_table( unsigned int start_int, unsigned int range){
    String buff = "";
    /*
    if (start_int < 0){ start_int = 0; }
    buff += "Integers: ";
    for (int i = 0; i < this->flag_pool_size; i++){
        buff += String::num_int64(this->flag_pool[i]) + ", ";
    }
    Godot::print(buff);
    buff = "";
    */
    unsigned int bit_counter;
    uint64_t mask;
    int end = start_int + range;

    for (int i = start_int;  i < end; i++) {
        if (i > this->flag_pool_size) {continue;}
        buff += String::num_int64( i ) + " - ";

        bit_counter = this->WORD_LENGTH;
        while( bit_counter --> 0 ){
            buff += (this->read(bit_counter + i * this->WORD_LENGTH) > 0 ) ? "1" : "0";
            if (bit_counter % 4  == 0) { buff += " "; }
        }

        buff +=  " [" + String::num_int64(this->flag_pool[i]) + "]";
        Godot::print(buff);
        buff = "";
    }
}

unsigned int FlagDB::size(){
    return this->flag_pool_size;
}
unsigned int FlagDB::capacity(){
    return this->flag_pool_capacity;
}


void FlagDB::save   ( String path ){}
void FlagDB::load   ( String path ){}
