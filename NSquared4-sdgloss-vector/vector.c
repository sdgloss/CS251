
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include <assert.h>

/* Take an already existing Vector as a parameter. (Do not create a Vector
 inside this function. Set its internal memory size to match the value given in
 the parameter, allocate an appropriately-sized C array to hold the actual data,
 and initialize the size variable to 0. */
 
void init(Vector *vector, int memorySize){
  int* vec = (int*) malloc (sizeof (int) * memorySize);
  for (int i = 0; i < memorySize; i++){
    vec[i] = 0;
  }
  vector->array  = vec; 
  vector->memorySize = memorySize;
  vector->size = 0; 
}

/* Removes the array associated with the Vector. */

void cleanup(Vector *vector){
  free(vector->array);
  //free(vector); 
}

/* Print out the Vector for debugging purposes. */

void print(Vector *vector){

  for (int i = 0; i < vector->size; i++){
    printf("%i, ", vector->array[i]);
  }
  printf("\n");
  printf("Memory Size: %i", vector->memorySize);
  printf("\n");
  printf("Size: %i", vector->size); 
  printf("\n");
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
  if (location < 0 || location >= vector->memorySize){
    return 0; 
  }
  if (vector->size >= vector->memorySize){
    Vector newVector;
    init(&newVector, (vector->memorySize * 2));
    
    for (int i = 0; i < vector->size; i++){

      int newValue;
      get(vector, (vector->size - i - 1), &newValue);
      insert(&newVector, 0, newValue);

    }

    cleanup(vector); 
    *vector = newVector;

  }
  
  if (location < vector->size){
     int hold_value = vector->array[location];
     vector->array[location] = value;
     insert(vector, location + 1, hold_value);
     return 1; 
  }
  else {
    vector->size += 1;
    vector->array[location] = value;
    return 1;
  }
}

/* Obtains value at location inside the Vector. Returns 1 if success, and 0 if
 the location is invalid (less than 0, or greater than or equal to the
 size). The value itself is returned through the parameter value. */

int get(Vector *vector, int location, int *value){
  if (location < 0 || location >= vector->size){
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
  if(location >= vector->size || location < 0){
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


/*int main(){
  Vector testVec; 

  init(&testVec, 5);
  printf("%i \n",insert(&testVec, 0, 99));
  printf("%i \n",insert(&testVec, 1, 3));
  printf("%i \n", insert(&testVec, 2, 7));
  printf("%i \n", insert(&testVec, 3, 10));
  print(&testVec);
  printf("%i \n", insert(&testVec, 1, 4)); 
  printf("Big Num: %i \n", testVec.array[2]);
  print(&testVec);

  Vector testVec2; 
  init(&testVec2, 10);
  int value=0; 
  int count = 0; 
  for(int j = 0; j < 7; j++){
    insert(&testVec2, j, count);
    count++; 
  }
  // 0, 1, 2, 3, 4, 5, 6
  printf("%i\n", get(&testVec2, 6, &value)); 
  printf("Need: %i\n", testVec2.array[3]); 
  printf("Value: %i\n", value);

  for(int i = 0; i < testVec2.size; i++){
    printf("%i\n", get(&testVec2, i, &value));
  }

  //Vector *vector = (Vector *)malloc(sizeof(Vector));
  
  Vector testerTest; 
  init(&testerTest,100);
  
  for (int i=0; i < 50; i++) {
    assert(insert(&testerTest,0,i));
  }
   
  for (int i=50; i < 53; i++) {
    int status = get(&testerTest, i, &value);
    printf("%i \n", status); 
  }

  Vector doubleVector;
  init(&doubleVector,10);

  // Insert 50 numbers into it
  for (int i=0; i < 15; i++) {
      insert(&doubleVector,0,i);
  }
}*/




