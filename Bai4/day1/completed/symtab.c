/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object *obj);
void freeScope(Scope *scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab *symtab;
Type *intType;
Type *charType;

/******************* Type utilities ******************************/

Type *makeIntType(void)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type *makeCharType(void)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type *makeArrayType(int arraySize, Type *elementType)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type *duplicateType(Type *type)
{
  // TODO
  Type *newType = (Type *)malloc(sizeof(Type));
  newType->typeClass = type->typeClass;
  newType->arraySize = type->arraySize;
  newType->elementType = type->elementType;

  return newType;
}

int compareType(Type *type1, Type *type2)
{
  // TODO
  if (type1->typeClass == type2->typeClass)
  {
    if (type1->arraySize == TP_ARRAY)
    {
      if (type1->arraySize == type2->arraySize && compareType(type1->elementType, type2->elementType))
      {
        return 1;
      }
      else
        return 0;
    }
  }
  return 0;
}

// Giải phóng kiểu
void freeType(Type *type)
{
  // TODO
  if (type->elementType != NULL)
  {
    freeType(type->elementType);
  }
  free(type);
  type = NULL;
}

/******************* Constant utility ******************************/

ConstantValue *makeIntConstant(int i)
{
  // TODO
  ConstantValue *c = (ConstantValue *)malloc(sizeof(ConstantValue));
  c->type = TP_INT;
  c->intValue = i;
  return c;
}

ConstantValue *makeCharConstant(char ch)
{
  // TODO
  ConstantValue *c = (ConstantValue *)malloc(sizeof(ConstantValue));
  c->type = TP_CHAR;
  c->charValue = ch;

  return c;
}

ConstantValue *duplicateConstantValue(ConstantValue *v)
{
  // TODO
  ConstantValue *c = (ConstantValue *)malloc(sizeof(ConstantValue));
  c->type = v->type;
  c->charValue = v->charValue;
  c->intValue = v->intValue;

  return c;
}

/******************* Object utilities ******************************/

Scope *createScope(Object *owner, Scope *outer)
{
  Scope *scope = (Scope *)malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

// Tạo một đối tượng chương trình
Object *createProgramObject(char *programName)
{
  Object *program = (Object *)malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes *)malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = createScope(program, NULL);
  symtab->program = program;

  return program;
}

// Tạo một đối tượng hằng số
Object *createConstantObject(char *name)
{
  // TODO
  Object *constObj = (Object *)malloc(sizeof(Object));
  strcpy(constObj->name, name);
  constObj->kind = OBJ_CONSTANT;

  constObj->constAttrs = (ConstantAttributes *)malloc(sizeof(ConstantAttributes));
  constObj->constAttrs->value = NULL;

  return constObj;
}

// Tạo một đối tượng kiểu
Object *createTypeObject(char *name)
{
  // TODO
  Object *typeObj = (Object *)malloc(sizeof(Object));
  strcpy(typeObj->name, name);
  typeObj->kind = OBJ_TYPE;

  typeObj->typeAttrs = (TypeAttributes *)malloc(sizeof(TypeAttributes));
  typeObj->typeAttrs->actualType = NULL;

  return typeObj;
}

// Tạo một đối tượng biến
Object *createVariableObject(char *name)
{
  // TODO
  Object *variaObj = (Object *)malloc(sizeof(Object));
  strcpy(variaObj->name, name);
  variaObj->kind = OBJ_VARIABLE;

  variaObj->varAttrs = (VariableAttributes *)malloc(sizeof(VariableAttributes));
  variaObj->varAttrs->type = NULL;
  variaObj->varAttrs->scope = symtab->currentScope;

  return variaObj;
}

// Tạo một đối tượng hàm
Object *createFunctionObject(char *name)
{
  // TODO
  Object *funcObj = (Object *)malloc(sizeof(Object));
  strcpy(funcObj->name, name);
  funcObj->kind = OBJ_FUNCTION;

  funcObj->funcAttrs = (FunctionAttributes *)malloc(sizeof(FunctionAttributes));
  funcObj->funcAttrs->paramList = NULL;
  funcObj->funcAttrs->returnType = NULL;
  funcObj->funcAttrs->scope = createScope(funcObj, symtab->currentScope);

  return funcObj;
}

// Tạo một đối tượng thủ tục
Object *createProcedureObject(char *name)
{
  // TODO
  Object *procedureObj = (Object *)malloc(sizeof(Object));
  strcpy(procedureObj->name, name);
  procedureObj->kind = OBJ_PROCEDURE;

  procedureObj->procAttrs = (ProcedureAttributes *)malloc(sizeof(ProcedureAttributes));
  procedureObj->procAttrs->paramList = NULL;
  procedureObj->procAttrs->scope = createScope(procedureObj, symtab->currentScope);

  return procedureObj;
}

// Tạo một đối tượng tham số hình thức
Object *createParameterObject(char *name, enum ParamKind kind, Object *owner)
{
  // TODO
  Object *parameterObj = (Object *)malloc(sizeof(Object));
  strcpy(parameterObj->name, name);
  parameterObj->kind = OBJ_PARAMETER;

  parameterObj->paramAttrs = (ParameterAttributes *)malloc(sizeof(ParameterAttributes));
  parameterObj->paramAttrs->function = owner;
  parameterObj->paramAttrs->kind = kind;
  parameterObj->paramAttrs->type = NULL;

  return parameterObj;
}

// Giải phóng đối tượng
void freeObject(Object *obj)
{
  // TODO
  if (obj != NULL)
  {
    if (obj->constAttrs != NULL)
    {
      switch (obj->kind)
      {
      case OBJ_CONSTANT:
        if (obj->constAttrs->value != NULL)
        {
          free(obj->constAttrs->value);
          obj->varAttrs->type = NULL;
        }
        break;
      case OBJ_VARIABLE:
        if (obj->varAttrs->type != NULL)
        {
          free(obj->varAttrs->type);
          obj->varAttrs->type = NULL;
        }
        break;
      case OBJ_TYPE:
        if (obj->typeAttrs->actualType != NULL)
        {
          free(obj->typeAttrs->actualType);
          obj->typeAttrs->actualType = NULL;
        }
        break;
      case OBJ_FUNCTION:
        freeScope(obj->funcAttrs->scope);
        break;
      case OBJ_PROCEDURE:
        freeScope(obj->procAttrs->scope);
        break;
      case OBJ_PROGRAM:
        if (obj->progAttrs->scope != NULL)
        {
          freeScope(obj->progAttrs->scope);
          obj->progAttrs->scope = NULL;
        }
        break;
      case OBJ_PARAMETER:
        if (obj->paramAttrs->type != NULL)
        {
          free(obj->paramAttrs->type);
          obj->paramAttrs->type = NULL;
        }
        break;
      default:
        break;
      }
      free(obj->constAttrs);
      obj->constAttrs = NULL;
    }
    free(obj);
    obj = NULL;
  }
}

// Giải phóng block
void freeScope(Scope *scope)
{
  // TODO
  if (scope != NULL)
  {
    freeObjectList(scope->objList);
    free(scope);
    scope = NULL;
  }
}

// Giải phóng danh sách đối tượng
void freeObjectList(ObjectNode *objList)
{
  // TODO
  if (objList != NULL)
  {
    freeObject(objList->object);
    freeObjectList(objList->next);
    objList = NULL;
  }
}

// Giải phóng danh sách đối tượng
void freeReferenceList(ObjectNode *objList)
{
  // TODO
  if (objList != NULL)
  {
    freeObject(objList->object);
    freeReferenceList(objList->next);
    objList = NULL;
  }
}

void addObject(ObjectNode **objList, Object *obj)
{
  ObjectNode *node = (ObjectNode *)malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL)
    *objList = node;
  else
  {
    ObjectNode *n = *objList;
    while (n->next != NULL)
      n = n->next;
    n->next = node;
  }
}

Object *findObject(ObjectNode *objList, char *name)
{
  // TODO
  ObjectNode *currentNode = objList;
  while (currentNode != NULL)
  {
    if (strcmp(currentNode->object->name, name) == 0)
    {
      return currentNode->object;
    }
    currentNode = currentNode->next;
  }
  return NULL;
}

/******************* others ******************************/

void initSymTab(void)
{
  Object *obj;
  Object *param;

  symtab = (SymTab *)malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;

  obj = createFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEI");
  param = createParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  addObject(&(obj->procAttrs->paramList), param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITEC");
  param = createParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  addObject(&(obj->procAttrs->paramList), param);
  addObject(&(symtab->globalObjectList), obj);

  obj = createProcedureObject("WRITELN");
  addObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void)
{
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope *scope)
{
  symtab->currentScope = scope;
}

void exitBlock(void)
{
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object *obj)
{
  if (obj->kind == OBJ_PARAMETER)
  {
    Object *owner = symtab->currentScope->owner;
    switch (owner->kind)
    {
    case OBJ_FUNCTION:
      addObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      addObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }

  addObject(&(symtab->currentScope->objList), obj);
}
