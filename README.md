# Dragan Silvia - HashMap Implementation in C

## Hashing Function
I chose to implement the djb2 hashing algorithm because, despite its simplicity, it is efficient. The constants chosen (5381 and 33) were not selected by me, but by Daniel J. Bernstein back in 1991. These constants have been repeatedly proven to be ideal for producing a minimal number of collisions.

1. Multiply the hash variable by 33.
2. Add the ASCII value of the current character to it.

## Collisions
Handling collisions is essential in a hash table, but it's typically best done when the table is half or three-quarters full. If you don't resize it early enough, collisions will become more common, and both lookups and inserts will slow down. Waiting until it's almost full essentially leads to linear search behavior.

