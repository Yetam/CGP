#include "CGPlib.h"
#include <iostream>
#include <math.h>

int verbose_level = 0;

using namespace CGP;

class Automata{

private:
  CGP::Program<bool> * rule;          //wskaznik na program bedacy aktualnie w uzyciu przez Automat
  std::vector<bool> world;            //jedna z dwoch siatek danych (przestrzen automatu)
  std::vector<bool> world2;           //jedna z dwoch siatek danych (przestrzen automatu)
  std::vector<bool> * currentWorld;   //wskaznik na ta siatke danych ktora zawiera aktualny stan automatu
  std::vector<bool> * nextWorld;      //wskaznik na siatke danych do ktorych zapisuje sie przyszly stan automatu
  int size = 64;                      //rozmiar automatu

  /*
    Funkcja zwracajace n-ty element aktualnej siatki danych. Zapewnia torusowe warunki brzegowe
  */
  bool getElement(int n){
    if( n<size && n>=0 ){           //jezeli podane n jest w granicach rozmiaru to po prostu zwroc element
      return currentWorld->at(n);
    }
    else{                           //jezeli podane n wykracza poza granice siatki to wykonaj operacje "warunkow brzegowych"
      return currentWorld->at(abs(n)%size);
    }
  }
  void setElement(int index, bool newVal){
    currentWorld->at(index) = newVal;
  }

  /*
    Funkcja resetuje aktualna siatke danych do stanu przypadkowego tzn do siatki przypadkowej
  */
  void setupAutomata(){
    for(int i=0;i<size;i++){        //dla kazdej komorki ustaw przypadkowa wartosc true lib false
      if(rand()%2 == 1){
        setElement(i,true);
      }else{
        setElement(i,false);
      }
    }
  }

  /*
      Podany z zewnatrz program zostanie uzyty jako program reguły automatu
      *p musi istniec
  */
  void setProgramAsCurrent(Program<bool> * newProgram){
    rule = newProgram;
  }

  /*
    Dla elementu siatki danych podanego jako x wykonuje operacje opisaną aktualnie uzywanym przez automat programem
    KOnstrukcja danych wejsciowych i wyjsciowych mocno zalezna od implementacji CGP
  */
  void processByRule(int x){
    std::vector<bool> v_input;      //wektor danych wejsciowych do programu
    std::vector<bool*> *v_output;   //wektor danych wyjsciowych z programu

    v_input.push_back( getElement(x-2) );   //pobieranie danych do wektora
    v_input.push_back( getElement(x-1) );   //pobieranie danych do wektora
    v_input.push_back( getElement(x) );     //pobieranie danych do wektora
    v_input.push_back( getElement(x+1) );   //pobieranie danych do wektora
    v_input.push_back( getElement(x+2) );   //pobieranie danych do wektora

    v_output = rule->proceedProgram( &v_input );  //przeprocesowanie danych wejsciowych przez program = obliczenie wyniku reguly
    nextWorld->at( x ) = *(v_output->at(0));      //pogram zwraca tylko jedna wartosc, wiec pobieram ja sobie i zapisuje do nasteopnej iteracji
    v_input.clear();    //czyszcze wektor zeby sie nie musial komputer tym martwic
    v_output->clear();  //czyszcze wektor zeby sie nie musial komputer tym martwic
  }

  /*
    Wykonaj n epok dla automatu. Jedna epoka to wykonanie reguly na kazdej komorce
  */
  void run(int n){  //wykonaj N epok

    for(int epoch=0 ; epoch<n ; epoch++){ //tyle ile epok
      for(int i=0 ; i<size ; i++)   //dla kazdej komorki
        processByRule(i);           //wykonaj ewaluacje

        //podmiana swiatow -> kolejna iteracja staje sie ta aktualna
      if(currentWorld == &world){
        currentWorld = &world2;
        nextWorld = & world;
      }
      else{
        currentWorld = &world;
        nextWorld = &world2;
      }
    }
  }

  /*
    Obliczanie entropii n-wyrazowej
  */
  double calculate_nWordEntropy(int wordLength){

    double meanEntropy=0;                                                       //tu zapisuje wartosc entropii
    for(int word=0 ; word<size ; word++){                                       //dla kazdego wyrazu, a wyrazow tyle co komorek
      int ones=0;                                                                 //tu bede zliczac true z ciagu
      for(int letter=0 ; letter < wordLength ; letter++){                         //dla kazdej z "liter" wyrazu
        if(getElement(word+letter) == true)                                         //jezeli ta litera==true to dolicz do ones
          ones++;
      }
      double p;                                                                 //prawdop. znalezienia jedynki
      if( ones==0 || ones==wordLength){                                         //warunek naliczania entropii. Jeżeli nie ma true albo wsyzstkie to true to dolicz 0 do entropii
        meanEntropy += 0.;
      }
      else{                                                                     //jezeli cos pomiedzy to oblicz skladnik p*log(p) i dolicz go do calkowitej entropii
        p = (double)ones;
        p /= (double)wordLength;
        meanEntropy = meanEntropy - p*log2(p) - (1-p)*log2(1-p);
      }
    }
    return (meanEntropy/size);                                                  //zwroc unormowana entropie
  }

public:

  Automata(){
    for(int i=0;i<size;i++){
      world.push_back(true);
      world2.push_back(true);
      currentWorld = &world;
      nextWorld = &world2;
    }
    setupAutomata();
  }

  /* kak to ma działać
    Tworze obiekt automatu
    Tworzefunkcje
  */

  double runAutomata(Program<bool> * p){

    int tresholdIterations = 64;
    int testIterations = 64;
    int testRuns = 10;
    double entropy=0;

    for(int runs=0 ; runs<testRuns ; runs++){
      setupAutomata();  //ustaw automat do pozycji wyjsciowej (przypadkowej)
      setProgramAsCurrent(p); //ustaw program z argumentu jako aktualnie rozpatrywany program
      run(tresholdIterations);  //rozwiewanie stanów nieustalonych :)

      for(int i=0;i<testIterations;i++){       //wykoywanie wlasciwych pomiarow
        run(1);
        entropy += calculate_nWordEntropy(6);
      }
    }
    entropy /= (double)(testRuns*testIterations);  //usrednianie
    if(p->getID() <0){
      std::cout << "meanEntropy: " << entropy << std::endl;
    }


    return entropy;
  }




};

Automata automat;

double fitness(Program<bool> * p){
//std::cout << "1" << std::endl;

  double fitnessValue = automat.runAutomata(p);
  //std::cout << p->getHash() << std::endl;



  fitnessValue = (0.3-fitnessValue)*(0.3 -fitnessValue);

  if(p->getID() <0){
  //  std::cout << "fit: " << fitnessValue << std::endl;
  }
  return fitnessValue;
//std::cout << "2" << std::endl;
}

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

int main(int argc, char** argv){

  srand(time(NULL));
  //srand(7);
  CGP_Algorithm<bool> mainCGP(5,10,1,fitness);

  mainCGP.addFormula(AND,2);
  mainCGP.addFormula(OR,2);
  mainCGP.addFormula(XOR,2);
  mainCGP.addFormula(NOT,1);

  mainCGP.doCGP("SETUP");

  //mainCGP.doCGP("DIEHARD");
  mainCGP.doCGP("PROCEED",50);

  return 0;
}
