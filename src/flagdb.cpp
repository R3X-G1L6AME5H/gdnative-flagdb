/*
	Flag Database
		by Nemo Czanderlitch/Nino Čandrlić
*/


#include "flagdb.h"

using namespace godot;


/* GODOT MANDATED BOILERPLATE */

void FlagDB::_register_methods() {
    register_method("read",  &FlagDB::read);
    register_method("tick",  &FlagDB::tick);
    register_method("clear", &FlagDB::clear);
    register_method("flip",  &FlagDB::flip);
    register_method("bit_table",  &FlagDB::bit_table);

    register_method("save",  &FlagDB::save);
    register_method("load",  &FlagDB::load);

    register_method("size",     &FlagDB::size);
    register_method("capacity", &FlagDB::capacity);
}

FlagDB::FlagDB() {}
FlagDB::~FlagDB() {
    ::free( this->flag_pool );
}

void FlagDB::_init(){
    this->flag_pool_size = 4;  // Makes 256 flags available by default
    this->flag_pool_capacity = this->flag_pool_size * WORD_LENGTH;
    this->flag_pool = (unsigned int*)calloc(this->flag_pool_size, sizeof(unsigned int));

    // zero all elements
    for (int i = 0; i < this->flag_pool_size; i++){ this->flag_pool[i] = 0; }
}


// returns the boolean value representing the bit-th bit
bool FlagDB::read( unsigned int bit ){
    if (bit <= flag_pool_capacity) {
        unsigned long mask  = 1 << (bit % WORD_LENGTH);
        unsigned long index = bit / WORD_LENGTH;
        return (this->flag_pool[index] & mask) > 0;
    }
    return 0;
}


// sets the bit-th bit to True
//      - when bit > flag_pool_capacity new integers are automatically allocated 
void FlagDB::tick( unsigned int bit ){
	if (bit > this->flag_pool_capacity){
        // reallocate memory
        int i = this->flag_pool_size;
        this->flag_pool_size = (bit / WORD_LENGTH) + 1;
        this->flag_pool_capacity = this->flag_pool_size * WORD_LENGTH;
        this->flag_pool = (unsigned int*)realloc(this->flag_pool, this->flag_pool_size * sizeof(unsigned int));

        // zero out all new elements
        for (; i < this->flag_pool_size; i++){this->flag_pool[i] = 0;}
    }

    unsigned int index = bit / WORD_LENGTH;

    uint64_t mask  = 1 << (bit % WORD_LENGTH);
    uint64_t value = this->flag_pool[index] | mask;

    this->flag_pool[index] = value;
}

// sets the bit-th bit to False
void FlagDB::clear( unsigned int bit ){
    if (bit <= this->flag_pool_capacity) {
        unsigned int index = bit / WORD_LENGTH;
        uint64_t mask  = 1 << (bit % WORD_LENGTH);
        uint64_t value = this->flag_pool[index] & ~mask;

        this->flag_pool[index] = value;
    }
}

// inverts the bit-th bit
void FlagDB::flip( unsigned int bit ){
    if (bit <= this->flag_pool_capacity) {
        unsigned int index = bit / WORD_LENGTH;
        uint64_t mask  = 1 << (bit % WORD_LENGTH);
        uint64_t value = this->flag_pool[index] ^ mask;

        this->flag_pool[index] = value;
    }
}


// prints out the integers at index start to index (start + range) in their binary representation 
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


// Saves the database to a file
void FlagDB::save   ( String path ){
    char* c_path = path.alloc_c_string();

    FILE* pfile;
    pfile = fopen(c_path, "wb");

    // Make the first 4 bytes the size of the pool
    fwrite( &this->flag_pool_size, sizeof(unsigned int), 1, pfile);
    // Write the pool
    fwrite( this->flag_pool,
            sizeof(unsigned int),
            this->flag_pool_size * sizeof(unsigned int),
            pfile);

    fclose(pfile);

}

// Loads the database from a file
//      - has no file type checks; the program will crash if incorrect file is selected  
void FlagDB::load   ( String path ){
    char* c_path = path.alloc_c_string();

    FILE* pfile;
    pfile = fopen(c_path, "rb");

    if (pfile == NULL) {
        Godot::print_error("File \"" + path + "\" does not exist", "FlagDB::save()", "", 0);
        return;
    }

    unsigned int size_buff;
    fread(&size_buff, sizeof(unsigned int), 1, pfile);
    this->flag_pool_size = size_buff;
    this->flag_pool_capacity = this->flag_pool_size * WORD_LENGTH;
    this->flag_pool = (unsigned int*)calloc(this->flag_pool_size, sizeof(unsigned int));

    fread(this->flag_pool, sizeof(unsigned int), size_buff, pfile);
    fclose(pfile);
}
