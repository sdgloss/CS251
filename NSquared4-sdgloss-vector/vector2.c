#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include <assert.h>

/* Function prototypes. */

/* Take an already existing Vector as a parameter. (Do not create a Vector
 inside this function. Set its internal memory size to match the value given in
 the parameter, allocate an appropriately-sized C array to hold the actual data,
 and initialize the size variable to 0. */

void init(Vector *vector, int memorySize){
  int* vec = (int*) malloc(sizeof(int) * memorySize); 

  vector->array = vec; 
  vector->memorySize = memorySize;
  vector->size = 0; 
}


/* Removes the array associated with the Vector. */

void cleanup(Vector *vector){
  free(vector->array); 
}


/* Print out the Vector for debugging purposes. */

void print(Vector *vector){
  printf("Array: ");
  for(int i = 0; i < vector->size; i++){
    printf("%i ", vector->array[i]);
  }
  printf("\n");
  printf("Memory Size: %i\n", vector->memorySize);
  printf("Array Size: %i\n", vector->size);
}


/* Insert value at location inside the Vector. If the list has items in
 positions 0 through n-1, you are only permitted to insert new values at
 locations 0 through n. (This is identical to Java's ArrayList behavior.) If
 there is already data at the location you are inserting into, insert slides the
 rest of the data down one position to make room for the new value. Returns 1 if
 success, and 0 if the location is invalid (less than 0, or greater than the
 size). Inserting at the very end of the current data (at position equal to
 size) is fine. All inserts should increase size by 1.

 If the internal array is too small to fit a new value, a new array of twice the
 size is created with malloc. The values are copied, the Vector is appropriately
 updated, and the old array is freed.) */

int insert(Vector *vector, int location, int value){ 
  if(location < 0 || location > vector->size){
    return 0; 
  }
  else{
    if(0){
      return 0; 
    }
    else{
      //Only case is making it double automatically
      if(location < vector->size){
        int hold_value = vector->array[location];
        vector->array[location] = value;
        insert(vector, location + 1, hold_value);
        return 1; 
      }
      else{
        vector->array[location] = value;
        vector->size += 1;
        return 1; 
      }
    } 
  }
}


/* Obtains value at location inside the Vector. Returns 1 if success, and 0 if
 the location is invalid (less than 0, or greater than or equal to the
 size). The value itself is returned through the parameter value. */

int get(Vector *vector, int location, int *value){
  if(location < 0 || location >= vector->size){
    return 0; 
  }
  else{
    *value = vector->array[location];
    return 1; 
  }
}


/* Deletes value at location inside the Vector.  Returns 1 if success, and 0 if
 the location is invalid (less than 0, or greater than or equal to the
 size). When an item is deleted, everything else past it in the array should be
 slid down. The internal array itself does not need to be compressed, i.e., you
 don't need to halve it in size if it becomes half full, or anything like
 that. */

int delete(Vector *vector, int location){
  if(location < 0 || location >= vector->size){
    return 0; 
  }
  else{
    for(int i = location; i < vector->size; i++){
      vector->array[i] = vector->array[i+1];
    }
    vector->size = vector->size - 1;
    return 1; 
  }
}

int main(){
  Vector tVec;
  init(&tVec, 100);

  for(int i = 0; i < 10; i++){
    insert(&tVec, i, i); 
  }
  print(&tVec);
  printf("\n");

  insert(&tVec, 3, 100);
  print(&tVec); 
  printf("\n"); 

  insert(&tVec, 6, 200); 
  print(&tVec); 
  printf("\n"); 
  
  int value; 
  get(&tVec, 3, &value);

  int value2;
  get(&tVec, 6, &value2); 

  printf("Array[3]: %i\n", value); 
  printf("Array[6]: %i\n", value2);
  printf("\n");

  delete(&tVec, 3);
  delete(&tVec, 5); 

  print(&tVec); 
}
