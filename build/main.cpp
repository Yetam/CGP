#include "CGPlib.h"
#include <iostream>


using namespace CGP;


  class IntegerOwn{ // : public Operational{
    int val; //integer value

  public:
    //co tu sie dzieje - Zmienne tworza N
    //Integer(int startVal);

    IntegerOwn(){val = 2;}                      //WYMAGANA
    IntegerOwn(int newVal){val=newVal;}
    void setVal(int newVal){val = newVal;}
    int getVal(){return val;}
    void print(){                               //WYMAGANA
      std::cout << val << std::endl;
    }
    void copy(IntegerOwn * src, IntegerOwn * dst){      //WYMAGANA
      dst->setVal( dst->getVal() );
    }

  };
  /* std::vector<T*> * funcInputs, T *funcOutput */
  void add(std::vector<IntegerOwn*> * funcInputs, IntegerOwn *funcOutput){
    //out->setVal( a->getVal() + b->getVal() );
    int output=0;
    for(int i=0 ; i<funcInputs->size() ; i++){
      output += funcInputs->at(i)->getVal();
    }
    funcOutput->setVal(output);
  }
  void multiply(std::vector<IntegerOwn*> * funcInputs, IntegerOwn *funcOutput){
    //out->setVal( a->getVal() + b->getVal() );
    int output=1;
    for(int i=0 ; i<funcInputs->size() ; i++){
      output *= funcInputs->at(i)->getVal();
    }
    funcOutput->setVal(output);
  }

  void getRandomInput(IntegerOwn * placeForNewVal){
    //placeForNewVal = new IntegerOwn(rand()%60 - 30);
    placeForNewVal->setVal(rand()%100);
  }

  double fitnessFunction(std::vector<IntegerOwn*> * inValues, std::vector<IntegerOwn*> * outValues){
    double correctValue;
    double programGuess;

    correctValue = inValues->at(1)->getVal() + inValues->at(0)->getVal() + inValues->at(0)->getVal() * inValues->at(1)->getVal();
    programGuess = outValues->at(0)->getVal();

    return (correctValue - programGuess)*(correctValue - programGuess);
  }


int main(){
    srand(time(NULL));
    /*
    //  Stworz obiekt CGP_ALgorithm.
    //  Argumenty:
    //    -(int) liczba genów wejściowych
    //    -(int) liczba genów procesujących
    //    -(int) liczba genów wyjściowych
    //    -(func_ptr) wskaźnik na funkcję celu
    //    -(func_ptr) wskaźnik na funkcję zwracającą losową wartość obiektu algorytmu
    */
    CGP_Algorithm<IntegerOwn> mainCGP(2,6,1,fitnessFunction,getRandomInput);

    /*
    //  Dodawanie formuł możliwych do wykorzystania w genach
    //  Argumenty:
    //    -(func_ptr) wskaźnik na funkcję dla formuły
    //    -(int) liczba argumentów którą przyjmuje dana funkcja formuły
    */
    mainCGP.addFormula(add,2);
    mainCGP.addFormula(multiply,2);

    /*
    //  Uruchomienie algorytmu CGP_Algorithm
    //  Argumenty:
    //    -(int) liczba epok algorytmu
    //    -(int) liczba uśrednień wartości fcji celu w jednej epoce
    */
    mainCGP.doCGP(100,10000);
    mainCGP.listOrganisms('a');

    std::cout << "Get to the choppa" << std::endl;
}
