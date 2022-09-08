#include "flagdb.h"

using namespace godot;

void FlagDB::_register_methods() {
    register_method("read",  &FlagDB::read);
    register_method("tick",  &FlagDB::tick);
    register_method("clear", &FlagDB::clear);
    register_method("flip",  &FlagDB::flip);
    register_method("show",  &FlagDB::show);
}

FlagDB::FlagDB() {}
FlagDB::~FlagDB() {
    ::free( this->flag_pool );
}

void FlagDB::_init(uint64_t size){
    if (size == 0){
        this->flag_pool_size = 10;
        this->flag_pool_capacity = 640;
        this->flag_pool = (uint64_t*)calloc(this->flag_pool_size, sizeof(uint64_t));

    } else {
        this->flag_pool_size = size;
        this->flag_pool_capacity = this->flag_pool_size * WORD_LENGTH;
        this->flag_pool = (uint64_t*)calloc(this->flag_pool_size, sizeof(uint64_t));
    }
}

bool FlagDB::read( uint64_t bit ){
    if (bit <= flag_pool_capacity) {
        uint64_t mask  = 1 << (bit % WORD_LENGTH);
        uint64_t index = bit / WORD_LENGTH;
        return (this->flag_pool[index] & mask) > 0;
    }
    return 0;
}

void FlagDB::tick( uint64_t bit ){
	if (bit > this->flag_pool_capacity){
        // reallocate memory
        this->flag_pool_size = (bit / WORD_LENGTH) + 1;
        this->flag_pool_capacity = this->flag_pool_size * WORD_LENGTH;

        this->flag_pool = (uint64_t*)realloc(this->flag_pool, this->flag_pool_size * sizeof(uint64_t));
    }

    uint64_t index = bit / WORD_LENGTH;

    uint64_t mask  = 1 << (bit % WORD_LENGTH);
    uint64_t value = this->flag_pool[index] | mask;

    this->flag_pool[index] = value;
}


void FlagDB::clear( uint64_t bit ){
    if (bit <= this->flag_pool_capacity) {
        uint64_t index = bit / WORD_LENGTH;
        uint64_t mask  = 1 << (bit % WORD_LENGTH);
        uint64_t value = this->flag_pool[index] & ~mask;

        this->flag_pool[index] = value;
    }
}
void FlagDB::flip( uint64_t bit ){
    if (bit <= this->flag_pool_capacity) {
        uint64_t index = bit / WORD_LENGTH;
        uint64_t mask  = 1 << (bit % WORD_LENGTH);
        uint64_t value = this->flag_pool[index] ^ ~mask;

        this->flag_pool[index] = value;
    }
}



void FlagDB::show( uint64_t start_int, uint64_t range){

    Godot::print("Size: " + String::num_int64( this->flag_pool_size ));
    Godot::print("Capacity: " + String::num_int64( this->flag_pool_capacity ));

    String buff = "";
    int bit_counter;
    uint64_t mask;
    int64_t index = range + start_int;

    for (; start_int < index; index--) {
       if (index > this->flag_pool_size) {continue;}
       buff += String::num_int64( index ) + "[" + String::num_int64(this->flag_pool[index]) + "]: ";

       bit_counter = 64;
       mask = 1;
       while( bit_counter --> 0 ){
           buff += ((mask & this->flag_pool[index]) > 0 ) ? "1" : "0";
           if (bit_counter % 4 == 0) { buff += " "; }
           mask = mask << 1;
       }

        Godot::print(buff);
        buff = "";
    }
}

void FlagDB::save   ( String path ){}
void FlagDB::load   ( String path ){}
