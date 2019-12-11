#ifndef __CGPlib__
#define __CGPlib__

#include<list>
#include<iostream>
#include<cstdlib>

namespace CGP{

//============================================================================//
//==========================     klasa Operational    ========================//
//============================================================================//

  //KLASA ABSTRAKCYJNA, PIERWOWZOR DO TWORZENIA OBIEKTÓW WARTOSCI NA KTÓRYCH OPERUJE CGP
  class Operational{
  public:
    Operational(){};
    virtual void print() = 0; //metoda czysto wirtualna wypisująca jasno i zwięźle wartość obiektu
    virtual void copy(Operational * src, Operational * dst) = 0;
  };

//============================================================================//
//==========================        klasa Block       ========================//
//============================================================================//

  template <typename T>
  class Block{
  private:
//POLA POMOCNICZE I OPISOWE
    int uniqueIdentifier;
    char blockType;  //czy to blok wejsciowy ('i'), wyjściowy ('o') czy zwykły ('n') czy nieokreślony (0)
    bool crucial;     //czy ma on znaczenie w propagacji sygnalu

//POLA DEFINIUJACE
    T * value;        //wartosc zmiennej bloku
    Block* inputA;  //wejscie A bloku
    Block* inputB;  //wejscie B bloku

    //POLA FUNKCYJNE

  public:
//KONSTRUKTORY
    Block(){};
    ~Block(){
      delete value;
    }

    void BlockSetup(int ID, Block * inA, Block * inB){
      inputA = inA;
      inputB = inB;
      uniqueIdentifier = ID;
      blockType = 0;  //domyślnie jako blok nieokreślony
      crucial = false;  //domyślnie blok niekodujący fenotypu
    }

//METODY GET
    Block * getInputA(){return inputA;}                                         //zwraca wskaznik na inputA
    Block * getInputB(){return inputB;}                                         //zwraca wskaznik na inputB
    int getID(){return uniqueIdentifier;}                                       //zwraca ID bloku
    bool isCrucial(){return crucial;}                                           //zwraca czy blok definiuje fenotyp
    bool isInputBlock(){return blockType;}                                      //zwraca typ bloku
//METODY SET
    //void setInputA(){}
    void setBlockType(char newType){blockType = newType;}
    void setCruciality(bool newCruciality){crucial = newCruciality;}
//METODY PODRZĘDNE
    void print();

    void copyBlock(Block * src, Block * dst){
      
    }
  };

//============================================================================//
//==========================       klasa Program      ========================//
//============================================================================//

  template <typename T>
  class Program{
  public:
    Block<T> ** genotyp;
    int nRow;
    int nCol;

    Program(){};
    void initProgram(int rows, int cols){
      nRow = rows;
      nCol = cols;

      //inicjalizuje tablice Blocków genotypu
      genotyp = new Block<T>*[nRow];
      for(int r=0 ; r<nRow ; r++){
        genotyp[r] = new Block<T>[nCol]();
      }
    }
    ~Program(){
      //czyszczenie Operational-ów wewnątrz bloków programu
        for(int r=0 ; r<nRow ; r++){
          for(int c=0 ; c<nCol ; c++){
            delete genotyp;
          }
        }
      //czyszczenie Bloków genotypu
        //KOD
      //zwalnianie pamięci zajętej przez genotyp
      for(int r=0 ; r<nRow ; r++){
        delete [] genotyp[r];
      }
      delete genotyp;
    }

    void copyProgram(Program src, Program dst){

    }
  };

//============================================================================//
//==========================    klasa CGP_Algorithm   ========================//
//============================================================================//

  //klasa CGP jest główną klasą przechowującą metody i dane związane z implementacją CGP
  template <typename T>
  class CGP_Algorithm{

    //DEKLARACJE DOT. FUNKCJI UŻYWANYCH PRZEZ CGP
    typedef void (* formula)(T *valA, T *valB, T *valOut);    //definicja typu wskaznika na fcje
    formula ** formulas;                                      //tablica dyn. alok. wsk. na fcje
    std::list<formula> formulasList;                          //lista do ktorej uzytkownik wrzuca zdefiniowane przez siebie funkcje

    int nRow;       //liczba wierszy siatki operacji
    int nCol;       //liczba kolumn siatki operacji
    int nFormulas;  //liczba możliwych zaimplementowanych operacji
    int mu;         //wielkość populacji rodziców
    int lambda;      //wielkośc populacji potomstwa

    //DEKLARACJE PUL ORGANIZMÓW UŻYWANYCH PRZEZ CGP
    Program<T> ** parentPull;    //pula rodziców
    Program<T> ** childrenPull;  //pula potomków

    //DEKLARACJE FUNKCJI ROZRODCZYCH
    void makeOffspring(){
      //czyszczenie aktualnej puli dzieci
      for(int i=0 ; i<lambda ; i++){
        delete childrenPull[i];
      }

    }

    public:
      CGP_Algorithm(int rows, int cols, int muVal, int lambdaVal): nRow(rows), nCol(cols), mu(muVal), lambda(lambdaVal){
        //tworzenie i inicjalizacja Programów z puli rodziców
        parentPull = new Program<T>*[mu];             //init puli rodziców
        for(int i=0 ; i<mu ; i++){
          parentPull[i] = new Program<T>();           //tworzy obiekt z puli rodziców
          parentPull[i]->initProgram(nRow,nCol);      //init rodzica
        }

        //tworzenie i inicjalizacja Programów z puli dzieci
        childrenPull = new Program<T>*[lambda];       //init puli dzieci
        for(int i=0 ; i<lambda ; i++){
          childrenPull[i] = new Program<T>();         //tworzy obiekt z puli dzieci
          childrenPull[i]->initProgram(nRow,nCol);    //init dziecka
        }

      }

      //funkcja dodajaca formula to listy FormulaList
      void addFormula(formula f){
        formulasList.push_back(f);
      }

      //funkcja  poswiadczajaca, ze wszystkie dodano wszystie przewidziane funkcje
      void enoughFormulas(){
        formulas = new formula*[formulasList.size()];
        nFormulas = formulasList.size();
        for(int i=0 ; i<formulasList.size() ; i++)
          formulas[i] = formulasList.pop_front();
      }


  };


}


#endif
