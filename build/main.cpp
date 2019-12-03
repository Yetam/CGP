#include "CGPlib.h"
#include <iostream>


using namespace CGP;


  class IntegerOwn : public Operational{
    int val; //integer value

  public:
    //co tu sie dzieje - Zmienne tworza N
    //Integer(int startVal);

    IntegerOwn(){val = 0;}
    IntegerOwn(int newVal){val=newVal;}
    void setVal(int newVal){val = newVal;}
    int getVal(){return val;}
    void print(){
      std::cout << val << std::endl;
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

    CGP_Algorithm<IntegerOwn> mainCGP(4,4);
    CGP_Algorithm<IntegerOwn>::addFormula(add);

    IntegerOwn A(2);
    IntegerOwn B(5);
    IntegerOwn C(0);

    add(&A,&B,&C);
    C.print();
    //Operational * a = new Operational();
}
