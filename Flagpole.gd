extends Node

const OVERFLOW_BIT : int = 32

var _db_capacity : int 
var _flag_pool = PoolIntArray([])
var _temporary_bit : int = 0

func init( capacity ):
	self._db_capacity = capacity
# warning-ignore:integer_division
	for _i in range(self._db_capacity/self.OVERFLOW_BIT + 1):
		_flag_pool.push_back(0)

func set_flag( bit : int ):
	if bit <= self._db_capacity:
# warning-ignore:integer_division
		self._temporary_bit = bit/self.OVERFLOW_BIT
		self._flag_pool.set( _temporary_bit, self._flag_pool[_temporary_bit] | (1 << (bit%self.OVERFLOW_BIT)))

func clear_flag( bit : int ):
	if bit <= self._db_capacity:
# warning-ignore:integer_division
		self._temporary_bit = bit/self.OVERFLOW_BIT
		self._flag_pool.set( _temporary_bit, self._flag_pool[_temporary_bit] & ~(1 << (bit%self.OVERFLOW_BIT)))

#func toggle_flag( bit : int ):
#	if bit <= self._db_capacity:
## warning-ignore:integer_division
#		self._temporary_bit = bit/self.OVERFLOW_BIT
#		self._flag_pool.set( _temporary_bit, self._flag_pool[_temporary_bit] ^ (1 << (bit%self.OVERFLOW_BIT)))

func get_flag( bit : int ) -> bool:
# warning-ignore:integer_division
	return (self._flag_pool[ bit/self.OVERFLOW_BIT ] & (1 << (bit%self.OVERFLOW_BIT))) > 0 and bit <= self._db_capacity

"""
func show():
	printraw( "FlagDB Capacity :\t", self._db_capacity,      "\n")
	printraw( "FlagDB Pools:    \t", self._flag_pool.size(), "\n")
	#for i in range(self._db_capacity):
	#	printraw( int(self.get(i)) )
	#	if i % self.OVERFLOW_BIT == 30:
	#		printraw( "\n" )
	for num in _flag_pool:
		for i in range(self.OVERFLOW_BIT):
			printraw( int(num & (1 << i) > 0) )
		printraw("\n")
"""
