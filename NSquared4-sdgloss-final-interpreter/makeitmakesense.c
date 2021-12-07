Value *evalSet2(Value *args, Frame *frame){
  Value *returnValue = talloc(sizeof(Value));
  Value->type = VOID_TYPE;

  Value *setSymbol = car(args);
  Value *setValue = eval(car(cdr(args)), frame);
  Value *setBinding = cons(setSymbol, cons(setValu, makeNull()));

  

  while(frame != NULL){
    Value *newBindings = makeNull();
    if(frame->bindings->type == CONS_TYPE){
      Value *holdBindings = frame->bindings;
      while(holdBindings->type != NULL_TYPE){
        Value *oldBinding = car(holdBindings);
        Value *oldSymbol = car(car(holdBindings));

        if(oldSymbol->type == SYMBOL_TYPE){
          if(!strcmp(oldSymbol->s, setSymbol->s)){
            oldBinding = setBinding);
          }
        }

        newBindings = cons(oldBinding, newBindings);
        holdBindings = cdr(holdBindings);
      }
    }

    frame->bindings = newBindings;
    frame = frame->parent; 
  }

  return returnValue;
}