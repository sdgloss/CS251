 /*Value *setSymbol = car(args);
  printf("setSymbol: %s\n", setSymbol->s);
  Value *setValue = eval(car(cdr(args)), frame);
  Value *setBinding = cons(setSymbol, cons(setValue, makeNull()));*/

  /*Frame *oldFrame = frame;
  Frame *updatedFrame = talloc(sizeof(frame));

  Value *newBindings = makeNull();

  while(oldFrame != NULL){
    if(oldFrame->bindings->type == CONS_TYPE){
      Value *holdBinding = oldFrame->bindings;
      while(holdBinding->type != NULL_TYPE){
        Value *oldBinding = car(holdBinding);
        Value *oldSymbol = car(car(holdBinding));
        printf("OldSymbol: %s\n", oldSymbol->s);
        
        if(oldSymbol->type == SYMBOL_TYPE){
          if(!strcmp(oldSymbol->s, setSymbol->s)){
            oldBinding = setBinding;
          }
        }
        newBindings = cons(oldBinding, newBindings);
        holdBinding = cdr(holdBinding);
      }
    }

    Frame *newSingleFrame = talloc(sizeof(frame));
    newSingleFrame->bindings = newBindings;
    newSingleFrame->parent = updatedFrame;

    updatedFrame = newSingleFrame;
    
    oldFrame = oldFrame->parent;
  }
  //printf("parent is not established\n");

  Frame *testUpdate = updatedFrame;
  if(testUpdate->bindings == NULL){
    printf("Here it is\n");
  }
  //("Update Binding Type: %u\n", updatedFrame->bindings->type);
  while(testUpdate != NULL){
    Value *updatedBindings = updatedFrame->bindings;
    while(updatedBindings->type != NULL_TYPE){
      printf("updatedBindings Type: %u\n", updatedBindings->type);
      Value *symbol = car(car(updatedBindings));
      printf("Updated Symbol: %s\n", symbol->s);
      Value *value = car(cdr(car(updatedBindings)));
      
      if(value->type == INT_TYPE){
        printf("Value: %i\n", value->i);
      }
      else{
        printf("Not Int Type\n");
      }

      updatedBindings = cdr(updatedBindings);
    }

    testUpdate = testUpdate->parent; 
  }

  frame = updatedFrame;*/