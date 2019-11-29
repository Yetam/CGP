#include "CGPlib.h"
#include <iostream>
#include <cstdlib>

using namespace CGP;
/* ==== IMPLEMENTACJA KLASY Operational 0000 */


/* ==== IMPLEMENTACJA KLASY CGP_Algorithm ==== */

//template class CGP_Algorithm<int>;
//template class CGP_Algorithm<bool>;

/* ==== IMPLEMENTACJA KLASY BLOCK<T> ==== */

//KONSTRUKTORY
/*
Block::Block(int ID, Block * inA, Block * inB){
  inputA = inA;
  inputB = inB;
  uniqueIdentifier = ID;
  isInputBlock = false;
  isCrucial = false;
}

//SET/GET
int Block::getID(){
  return uniqueIdentifier;
}
Block * Block::getInputA(){
  return inputA;
}
Block * Block::getInputB(){
  return inputB;
}

//METODY PODRZĘDNE
void Block::print(){
  if(!isInputBlock){
    std::cout << "Self_ID: " << uniqueIdentifier << " A_ID: "  << std::endl;
  }

}
*/


/**
Algorytm CGP składa się z n bloków.
Każdy blok ma trzy zmienne:
  -input1
  -input2
  -formulaType


**/
