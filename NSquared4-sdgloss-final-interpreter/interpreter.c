#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "tokenizer.h"
#include "value.h"
#include "linkedlist.h"
#include "talloc.h"
#include "tokenizer.h"
#include "parser.h"
#include "interpreter.h"

void evaluateError(int i){
  printf("Evaluation error %i\n", i);
}

void printToken(Value *token){
  //gets a token and then prints it
  //printf("Type: %u\n", token->type);
  if(token->type == INT_TYPE){
    printf("%i \n", token->i);
  }
  else if(token->type == DOUBLE_TYPE){
    printf("%f \n", token->d); 
  }
  else if(token->type == STR_TYPE){
    printf("\"%s\" \n", token->s);
  }
  else if(token->type == BOOL_TYPE){
    if(token->i == 1){
      printf("#t\n");
    }
    else if(token->i == 0){
      printf("#f\n");
    }
    else{
      printf("Bool Error\n");
    }
  }
  else if(token->type == SYMBOL_TYPE){
    if(!strcmp(token->s, "error")){
      evaluateError(1);
    }
    else{
      printf("%s ", token->s);
    }
  }
  else if(token->type == CONS_TYPE){
    Value *carToken = car(token);
    Value *cdrToken = cdr(token);

    printf("(");
    printToken(carToken);
    if(cdr(token)->type != CONS_TYPE){
      printf(". ");
      printToken(cdr(token));
    }
    else{
      while(cdrToken->type == CONS_TYPE){
        printToken(car(cdrToken));
        cdrToken = cdr(cdrToken);
      }
    }
    printf(")");

    //print (
    //print car(token);
    //check if cdr is a CONS_TYPE
    //no = print a .
    //yes = print a space (or do nothing)
    //recursively keep printing the cdr
  }
  else if(token->type == NULL_TYPE){
    printf("()");
  }
  else if(token->type != NULL_TYPE){
    printf("%s \n", token->s);
  }
}

Value *findEqual(Value *trueArg, Frame *frame){
  //finds the right value that has the same value as trueArg.
  Frame *holdFrame = frame;
  Value *rEqual = talloc(sizeof(Value));
  rEqual->type = SYMBOL_TYPE;
  rEqual->s = "error";
  

  while (holdFrame->bindings != NULL){
    if(holdFrame->bindings->type == CONS_TYPE){
      Value *hold = holdFrame->bindings;

      while(hold->type != NULL_TYPE){
        Value *symbol = car(car(hold)); 
        Value *val = car(cdr(car(hold)));

        
        if(symbol->type == SYMBOL_TYPE){ 
          if(!strcmp(symbol->s, trueArg->s)){
            rEqual = val;
            return rEqual;
          }
        }
        hold = cdr(hold);
      }
    }
    holdFrame = holdFrame->parent;
  }

  return rEqual;
}

Value *evalIf(Value *args, Frame *frame){
  Value *condition = eval(car(args), frame);
  Value *tReturn = cdr(args);
  Value *fReturn = cdr(tReturn);
  Value *returnArgs;
  if(condition->i == 1){
    returnArgs = eval(tReturn->c.car, frame);
  }
  else if(condition->i == 0){
    returnArgs = eval(fReturn->c.car, frame);
  }
  else if(condition->type == SYMBOL_TYPE){
    returnArgs = findEqual(condition, frame);

    if(returnArgs->i == 1){
      if(tReturn->c.car->type == SYMBOL_TYPE){
        returnArgs = findEqual(tReturn->c.car, frame);
      }
      else{
        returnArgs = tReturn->c.car;
      }
    }
    else if(returnArgs->i == 0){
      if(fReturn->c.car->type == SYMBOL_TYPE){
      returnArgs = findEqual(fReturn->c.car, frame);
      }
      else{
        returnArgs = fReturn->c.car;
      }
    }
  }
  else{
    returnArgs = makeNull();
  }
  
  if(returnArgs->type == CONS_TYPE){
    Value *secIf = car(returnArgs);
    Value *secArgs = cdr(returnArgs);

    if(!strcmp(secIf->s, "if")){
      returnArgs = evalIf(secArgs, frame);
    }
    else if(!strcmp(secIf->s, "quote")){
      returnArgs = car(secArgs);
    }
  }
  return returnArgs;
}


Value *createClosure(Value *args, Frame *parent){
  Value *closure = talloc(sizeof(Value));
  
  Value *formalParam = car(args);
  Value *fxnCode = car(cdr(args));
  
  closure->type = CLOSURE_TYPE;
  closure->cl.paramNames = formalParam;
  closure->cl.functionCode = fxnCode;
  closure->cl.frame = parent;

  return closure;
}

Value *createDefFrame(Value *parseTree, Frame *parent){
  Value *parentBind = parent->bindings;
  Frame *parentHold = parent->parent; 
  
  Value *symbol = car(parseTree);
  Value *value = eval(car(cdr(parseTree)), parent);

  Value *defBinding = cons(symbol, cons(value, makeNull()));
  Value *newBinding = cons(defBinding, parentBind);

  parent->bindings = newBinding;

  Value *rDefine = talloc(sizeof(Value));
  rDefine->type = VOID_TYPE;

  return rDefine;
}

Value *evalEach(Value *args, Frame *frame){
  Value *holdArg = args;
  Value *argument = makeNull();

  while(holdArg->type != NULL_TYPE){
    Value * symbHold = eval(car(holdArg), frame);
    argument = cons(symbHold, argument);
    holdArg = cdr(holdArg);
  }
  argument = reverse(argument);

  return argument; 
}

Value *apply(Value *func, Value *args){
  if(func->type == PRIMITIVE_TYPE){
    return func->pf(args);
  }
  Value *closeParam = func->cl.paramNames;
  Value *holdArgs = args;
  Value *allBinding = makeNull();

  while(closeParam->type != NULL_TYPE && holdArgs->type != NULL_TYPE){
    Value *symbol = car(closeParam);
    Value *value = car(holdArgs);
    
    Value *singleBind = cons(symbol, cons(value, makeNull()));

    allBinding = cons(singleBind, allBinding);

    closeParam = cdr(closeParam);
    holdArgs = cdr(holdArgs);
  }

  Frame *newFrame = talloc(sizeof(Frame));
  newFrame->bindings = allBinding;
  newFrame->parent = func->cl.frame;

  Value *evaulated = eval(func->cl.functionCode, newFrame);

  return evaulated;
}


//bind primitives not bindings
void bind(char *name, Value *(*function)(struct Value *), Frame *frame) {
    // Add primitive functions to top-level bindings list
    Value *value = talloc(sizeof(Value));
    value->type = PRIMITIVE_TYPE;
    value->pf = function;

    Value *symbol = talloc(sizeof(Value));
    symbol->type = SYMBOL_TYPE;
    symbol->s = name;

    Value *binding = cons(symbol, cons(value, makeNull()));
    binding = cons(binding, frame->bindings);

    frame->bindings = binding;
}


//done
Value *primAdd(Value *args){
  Value *sum = talloc(sizeof(Value));
  sum->type = INT_TYPE;

  if (length(args) == 2){
    sum->i = (car(args)->i + car(cdr(args))->i);
  }
  else{
    sum->i = (car(args)->i + primAdd(cdr(args))->i);
  }

  return sum;
}

//works
Value *primMinus(Value *args){
  Value *sum = talloc(sizeof(Value));
  sum->type = INT_TYPE;

  if (length(args) == 2){
    sum->i = (car(args)->i - car(cdr(args))->i);
  }
  else{
    sum->i = (car(args)->i - primMinus(cdr(args))->i);
  }

  return sum;
}
 
//done.
Value *primNull(Value *args){
  Value *null = talloc(sizeof(Value));
  null->type = BOOL_TYPE;

  if(car(args)->type == NULL_TYPE){
    null->i = 1;
  }
  else{
    null->i = 0;
  }

  return null;
}

//done.
Value *primCar(Value* args){
  if(car(args)->type == CONS_TYPE){
    return car(car(args));
  }
  else {
    return car(args);
  }
}

//worked now. 
Value *primCdr(Value* args){
  Value *carHold = car(args);
  if(car(carHold)->type != CONS_TYPE){
    return cdr(carHold);
  }
  else{
    return cdr(car(carHold));
  }
}


Value *primCons(Value* args){
    Value *holder = makeNull();

    holder = cons(car(args), car(cdr(args)));
    
    return holder;
          
}

//works
Value *primGreater(Value* args){
  Value *rBool = talloc(sizeof(Value));
  rBool->type = BOOL_TYPE;

  if(car(args)->i > car(cdr(args))->i){
    rBool->i = 1;
  }
  else{
    rBool->i = 0;
  }

  return rBool;
}

//works
Value *primLess(Value* args){
  Value *rBool = talloc(sizeof(Value));
  rBool->type = BOOL_TYPE;

  if(car(args)->i < car(cdr(args))->i){
    rBool->i = 1;
  }
  else{
    rBool->i = 0;
  }

  return rBool;
}
//works
Value *primEqual(Value* args){
  Value *rBool = talloc(sizeof(Value));
  rBool->type = BOOL_TYPE;

  if(car(args)->i == car(cdr(args))->i){
    rBool->i = 1;
  }
  else{
    rBool->i = 0;
  }

  return rBool;
}

//works
Value *evalAnd(Value *args, Frame *frame){
  Value *returnVal = talloc(sizeof(Value));
  returnVal->type = BOOL_TYPE;
  returnVal-> i = 1;
  Value *holdArgs = args;

  while(holdArgs->type != NULL_TYPE){
    Value *holder = eval(car(holdArgs), frame);
    if(holder->type != BOOL_TYPE || holder->i != 1){
      returnVal->i = 0;
      break;
    }
    holdArgs = cdr(holdArgs);
  }

  return returnVal;
}

//
Value *evalOr(Value *args, Frame *frame){
  Value *returnVal = talloc(sizeof(Value));
  returnVal->type = BOOL_TYPE;
  returnVal-> i = 0;
  Value *holdArgs = args;

  while(holdArgs->type != NULL_TYPE){
    Value *holder = eval(car(holdArgs), frame);

    if(holder->type == BOOL_TYPE && holder->i == 1){
      returnVal->i = 1;
      break;
    }
    holdArgs = cdr(holdArgs);
  }

  return returnVal;
}

Value *evalSet(Value *args, Frame *frame){
  Value *returnValue = talloc(sizeof(Value));
  returnValue->type = VOID_TYPE;
  Value *setSymbol = car(args);
  Value *setValue = eval(car(cdr(args)), frame);
  //printf("setValue: %i\n", setValue->i);
  Value *setBinding = cons(setSymbol, cons(setValue, makeNull()));

  while(frame != NULL){
    Value *newBindings = makeNull();
    if(frame->bindings->type == CONS_TYPE){
      Value *holdBindings = frame->bindings;
      while(holdBindings->type != NULL_TYPE){
        Value *oldBinding = car(holdBindings);
        Value *oldSymbol = car(car(holdBindings));

        if(oldSymbol->type == SYMBOL_TYPE){
          if(!strcmp(oldSymbol->s, setSymbol->s)){
            oldBinding = setBinding;
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


//Get the Symbol and Value from let parse tree
//Create the bind of the Symbol and Value and the Frame
Frame *createFrame(Value *parseTree, Frame *parent){
  Value *holdParse = parseTree; 
  Value *subParse = car(holdParse);
  Value *rBinding = makeNull();
  
  
  while(subParse->type != NULL_TYPE){
    
    if(subParse->type == CONS_TYPE){
      Value *subSubParse = subParse;
      while(car(subSubParse)->type == CONS_TYPE){
        subSubParse = car(subSubParse);
      }
      
      Value *symbol = car(subSubParse);
      Value *value = eval(car(cdr(subSubParse)), parent);

      
      rBinding = cons(cons(symbol, cons(value, makeNull())), rBinding);
    }

    subParse = cdr(subParse);
  }

  Frame *newFrame = talloc(sizeof(Frame));
  newFrame->bindings = rBinding;
  newFrame->parent = parent;

  return newFrame;
}

//works
Frame *createLetStarFrame(Value *expr, Frame *frame){
  Value *holdParse = expr; 
  Value *subParse = car(holdParse); 
  Frame *newFrame;  

  while(subParse->type != NULL_TYPE){
    Value *rBinding = makeNull();
    newFrame = talloc(sizeof(Frame));
    if(subParse->type == CONS_TYPE){
      Value *subSubParse = subParse;
      while(car(subSubParse)->type == CONS_TYPE){
        subSubParse = car(subSubParse);
      }
      
      Value *symbol = car(subSubParse);
      //check binding, takes the binding, takes the symbol, sees if it's in there. 
      Value *value = eval(car(cdr(subSubParse)), frame);

    
      rBinding = cons(cons(symbol, cons(value, makeNull())), rBinding);
    }
    newFrame->bindings = rBinding;
    newFrame->parent = frame;
    frame = newFrame;
      
    subParse = cdr(subParse);
  }
    

  return frame;
}

//works
Frame *createLetRecFrame(Value *expr, Frame *frame){
  Frame *curFrame = talloc(sizeof(Frame));
  curFrame->parent = frame;
  Value *args = car(expr);
  Value* rBinding = makeNull();

  while(args->type == CONS_TYPE) {
    Value *curPair = car(args);
    if (car(curPair)->type == SYMBOL_TYPE) {
        Value *value = eval(car(cdr(curPair)), curFrame);
        if (value->type != NULL_TYPE) {
            Value *newBind = makeNull();
            rBinding = cons(cons(car(curPair), cons(value, makeNull())), rBinding);;
        }
    }
    args = cdr(args);
  }
  curFrame->bindings = rBinding;
  return curFrame;

}

Value *evalBegin(Value *args, Frame *frame){
  Value *holdArgs = args;
  Value *value;
  while(holdArgs->type != NULL_TYPE){
    value = talloc(sizeof(Value));
    Value *expr = car(holdArgs);
    value = eval(expr, frame);

    holdArgs = cdr(holdArgs);
  }

  return value;
}

Value *eval(Value *expr, Frame *frame){
  switch (expr->type)  {
    case INT_TYPE: {
      //printf("Here\n");
      return expr;
      break;
    }
    case DOUBLE_TYPE: {
      return expr; 
      break;
    } 
    case STR_TYPE: {
      return expr; 
      break;
    } 
    case BOOL_TYPE: {
      return expr; 
      break;
    }
    case SYMBOL_TYPE: {
      //printf("Goes into Symbol\n");
      return findEqual(expr, frame);
      break;
    }  
    case CONS_TYPE: {
      Value *first = car(expr);
      //printf("First: %s\n", first->s);
      Value *args = cdr(expr);
      Value *result = talloc(sizeof(Value));
      result->type = SYMBOL_TYPE; 
      result->s = "error";

      //printf("Symbol: %s\n", first->s);
      
      if(!strcmp(first->s, "if")){
        if(cdr(args)->type != NULL_TYPE){
          result = evalIf(args, frame);
        }
      }
      else if(!strcmp(first->s, "let*")){
        frame = createLetStarFrame(args, frame);
        //while(cdr(args)->type != NULL_TYPE){
        result = eval(car(cdr(args)), frame);
        //args = cdr(args);}
      }
      else if(!strcmp(first->s, "letrec")){
        frame = createLetRecFrame(args, frame);
        //while(cdr(args)->type != NULL_TYPE){
        result = eval(car(cdr(args)), frame);
        //args = cdr(args);}
      }    
      else if(!strcmp(first->s, "let")){
        frame = createFrame(args, frame);
        while(cdr(args)->type != NULL_TYPE){
        result = eval(car(cdr(args)), frame);
        args = cdr(args);}

      }
      else if(!strcmp(first->s, "quote")){
        if(cdr(args)->type != NULL_TYPE){
          break;
        }
        else{
          result = car(args);
        }
      }
      else if(!strcmp(first->s, "define")){
        result = createDefFrame(args, frame);
      }
      else if(!strcmp(first->s, "lambda")){
        result = createClosure(args, frame);
      }
      else if(!strcmp(first->s, "and")){
        result = evalAnd(args, frame);
      }
      else if(!strcmp(first->s, "or")){
        result = evalOr(args, frame);
      }
      else if(!strcmp(first->s, "set!")){
        result = evalSet(args, frame);
      }
      else if(!strcmp(first->s, "begin")){
        result = evalBegin(args, frame);
      }
      else{
          // If not a special form, evaluate the first, evaluate the args, then
          // apply the first to the args.
          Value *evaledOperator = eval(first, frame);
          //printf("Operator: %u\n", evaledOperator->type);
          //printf("EvalOpp Type: %u\n", evaledOperator->type);
          Value *evalArgs = evalEach(args, frame);

          if(evaledOperator->type == CLOSURE_TYPE || evaledOperator->type == PRIMITIVE_TYPE){
            result = apply(evaledOperator, evalArgs);
          }
          else{
            evaluateError(32);
          }

        }

      return result; 
      break; 
    }
    default: {
      break;
    }
    
    
  }
  
  Value *noEval = talloc(sizeof(Value));
  noEval->type = SYMBOL_TYPE;
  noEval->s = "error";
  
  return noEval; 
}



void interpret(Value *tree){
  Value *parseTree = tree;
  Frame *frame = talloc(sizeof(Frame));
  frame->bindings = makeNull();
  frame->parent = NULL;
  bind("+", primAdd, frame);
  bind("-", primMinus, frame);
  bind("<", primLess, frame);
  bind(">", primGreater, frame);
  bind("=", primEqual, frame);
  bind("car", primCar, frame);
  bind("cdr", primCdr, frame);
  bind("cons", primCons, frame);
  bind("null?", primNull, frame);
  //bind("let*", letStar, frame);
  
  while(parseTree->type != NULL_TYPE){
    Value *expression = car(parseTree);
    Value *evalResult = eval(expression, frame);

    if(evalResult->type != VOID_TYPE){
      printToken(evalResult);
      //printf("Hi");
    }

    Value *treeCdr = cdr(parseTree);
    parseTree = treeCdr;
  }

  texit(0);
}