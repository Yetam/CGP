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

  void add(IntegerOwn *a, IntegerOwn *b, IntegerOwn *out){
    out->setVal( a->getVal() + b->getVal() );
  }
  void subtract(IntegerOwn *a, IntegerOwn *b, IntegerOwn *out){
    out->setVal( a->getVal() - b->getVal() );
  }
  void multiply(IntegerOwn *a, IntegerOwn *b, IntegerOwn *out){
    out->setVal( a->getVal() - b->getVal() );
  }


int main(){
    CGP_Algorithm<IntegerOwn> mainCGP(1,1);
    mainCGP.setOrganism(2,12,1);
    //mainCGP.addFormula(add);
  //  mainCGP.addFormula(subtract);
    //mainCGP.addFormula(multiply);


}
