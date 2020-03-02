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
    placeForNewVal->setVal(rand()%60 - 30);
  }

  double fitnessFunction(std::vector<IntegerOwn*> * inValues, std::vector<IntegerOwn*> * outValues){
    double correctValue;
    double programGuess;

    correctValue = inValues->at(0)->getVal() + inValues->at(0)->getVal() * inValues->at(1)->getVal();
    programGuess = outValues->at(0)->getVal();

    return (correctValue - programGuess)*(correctValue - programGuess);
  }


int main(){
    srand(time(NULL));
/*
    CGP_Algorithm<IntegerOwn> mainCGP(2,2);
    mainCGP.addFormula(add,2);
    mainCGP.addFormula(multiply,2);
    mainCGP.enoughFormulas();

    mainCGP.setOrganism(4,3,2);
    std::cout << "Inicjalizacja zakonczona skucesem" << std::endl;
    mainCGP.listOrganisms('a');

    mainCGP.testCopyingPrograms(1,2);
    mainCGP.testMutate(8);
    mainCGP.listOrganisms('a');
*/
    CGP_Algorithm<IntegerOwn> mainCGP(1,3);
    mainCGP.addFormula(add,2);
    mainCGP.addFormula(multiply,2);
    mainCGP.enoughFormulas();
    mainCGP.addFitnessFunction(fitnessFunction);
    mainCGP.addRandomFiller(getRandomInput);

    mainCGP.setOrganism(2,2,1);
    std::cout << "Inicjalizacja zakonczona skucesem" << std::endl;

    //mainCGP.listOrganisms('a');
    mainCGP.doCGP(1000,100);
    //mainCGP.listOrganisms('a');

    //mainCGP.testCopyingPrograms(1,2);
    //mainCGP.testMutate(8);
    //mainCGP.listOrganisms('a');

    //mainCGP.addFormula(add);
    //mainCGP.addFormula(subtract);
    //mainCGP.addFormula(multiply);

    std::cout << "Get to the choppa" << std::endl;
}
