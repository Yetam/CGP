#include "CGPlib.h"
#include <iostream>

int verbose_level = 0;

using namespace CGP;

int N = 4;

void AND(std::vector<bool*> * funcInputs, bool *tuncOutput){        //2 ARGUMENTY
  bool val = *(funcInputs->at(0)) && *(funcInputs->at(1));
  *tuncOutput = val;
}
void OR(std::vector<bool*> * funcInputs, bool *tuncOutput){        //2 ARGUMENTY
  bool val =  *(funcInputs->at(0)) || *(funcInputs->at(1));
  *tuncOutput = val;
}
void XOR(std::vector<bool*> * funcInputs, bool *tuncOutput){        //2 ARGUMENTY
  bool val;// = *(funcInputs->at(0)) ;
  if(*(funcInputs->at(0)) == *(funcInputs->at(1))){
    val = false;
  }
  else{
    val = true;
  }
  *tuncOutput = val;
}
void NOT(std::vector<bool*> * funcInputs, bool *tuncOutput){        //2 ARGUMENTY
  bool val =  !*(funcInputs->at(0));
  *tuncOutput = val;
}

double fitnessFunction(std::vector<bool*> * inValues, std::vector<bool*> * outValues){

  return -0.2;

}

/*
* Poniższą funkcję programuje użytkownik i ona zwraca dopasowanie w formie double
*/
double fitness(Program<bool> * p){

    std::vector<bool> v_input;
    std::vector<bool*> *v_output;

    double wynik = 0;
    int max_N = 1 << N;

    for(int l=0 ; l<max_N ; l++){
      v_input.clear();
      for(int ii=0;ii<N;ii++){
        v_input.push_back( (bool)(l&(1<<ii)) );
      }
      //std::cout << v_input.at(0) << " " << v_input.at(1) << " " << v_input.at(2) << " " << v_input.at(3) << " " << std::endl;

      v_output = p->calcFitnessValue( &v_input );
      bool val = *v_output->at(0);
      //std::cout << val << std::endl;

      if(l==max_N-1){
        if(val == false){
          wynik += 1;
        }
      }
      else{
        if(val == true){
          wynik += 1./max_N;
        }
      }

    }

    return wynik;

}


void getRandomInput(bool * placeForNewVal){
  *placeForNewVal =  (bool)(rand()%2);
}

int main(int argc, char** argv){
  N = atoi(argv[1]);

  srand(time(NULL));
  CGP_Algorithm<bool> mainCGP(N,50,1,fitness,getRandomInput);

  //mainCGP.addFormula(AND,2);
  mainCGP.addFormula(OR,2);
  //mainCGP.addFormula(XOR,2);
  mainCGP.addFormula(NOT,1);

  mainCGP.doCGP(10000000);


  return 0;
}
