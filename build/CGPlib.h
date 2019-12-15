#ifndef __CGPlib__
#define __CGPlib__

#include<list>
#include<iostream>
#include<cstdlib>
#include<vector>

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
    //POLA DEFINIUJACE
    T * value;        //wartosc zmiennej bloku
    std::vector<Block<T> *> inputs;
    int id;
    int type;     //(-1)=input, (0)=func, (1)=output

    //POLA FUNKCYJNE

  public:
//KONSTRUKTORY
    Block(int blockID, int blockType): id(blockID), type(blockType){
        std::cout << "Added block ID: " << blockID << std::endl;
        value = new T();
    }
    ~Block(){
      delete value;
    }

    void BlockSetup(Block ** inputsTab){
      inputs = inputsTab;
    }

//METODY GET
    void copyBlock(Block * src, Block * dst){

    }
  };

//============================================================================//
//==========================       klasa Program      ========================//
//============================================================================//

  template <typename T>
  class Program{
  public:
    std::vector<Block<T> *> genes;
    int inputs;
    int formulas;
    int outputs;
    int id;

    Program(int nOfInputs, int nOfFormulas, int nOfOutputs): inputs(nOfInputs), outputs(nOfOutputs), formulas(nOfFormulas){
      std::cout << "Program()" << std::endl;
      int currentID=0;

      //dodawanie bloków wejściowych
      std::cout << "/dodawanie bloków wejściowych/" << std::endl;

      Block<T> * blockToBeAdded;
      for(int i=0 ; i<inputs ; i++){
        blockToBeAdded = new Block<T>(currentID,-1);
        genes.push_back(blockToBeAdded);
        currentID++;
      }
      //dodawanie bloków funkcyjnych
      std::cout << "/dodawanie bloków funkcyjnych/" << std::endl;
      for(int i=0 ; i<formulas ; i++){
        blockToBeAdded = new Block<T>(currentID,0);
        genes.push_back(blockToBeAdded);
        currentID++;
      }
      //dodawanie bloków wyjściowych
      std::cout << "/dodawanie bloków wyjsciowych/" << std::endl;
      for(int i=0 ; i<outputs ; i++){
        blockToBeAdded = new Block<T>(currentID,1);
        genes.push_back(blockToBeAdded);
        currentID++;
      }

    };

    ~Program(){

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
    typedef void (* formula)(std::vector<T> * funcInputs, T *funcOutput);    //definicja typu wskaznika na fcje
    std::vector<formula> formulasList;                          //lista do ktorej uzytkownik wrzuca zdefiniowane przez siebie funkcje

    int mu;         //wielkość populacji rodziców
    int lambda;      //wielkośc populacji potomstwa

    //DEKLARACJE PUL ORGANIZMÓW UŻYWANYCH PRZEZ CGP
    std::vector<Program<T>> organisms;

    //DEKLARACJE FUNKCJI ROZRODCZYCH
    void makeOffspring(){

    }

    public:
      CGP_Algorithm(int muVal, int lambdaVal): mu(muVal), lambda(lambdaVal){
        std::cout << "CGP_Algorithm constructor" << std::endl;
      }
      void setOrganism( int nOfInputs, int nOfOutputs, int nOfFormulas ){
        std::cout << "CGP_Algorithm::setOrganism(): adding parents" << std::endl;
        for(int i=0 ; i<mu ; i++ ){
          Program<T> * programToBeAdded = new Program<T>(nOfInputs, nOfOutputs, nOfFormulas);
          organisms.push_back(*programToBeAdded);
        }
        std::cout << "CGP_Algorithm::setOrganism(): adding offspring" << std::endl;
        for(int i=0 ; i<lambda ; i++ ){
          Program<T> * programToBeAdded = new Program<T>(nOfInputs, nOfOutputs, nOfFormulas);
          organisms.push_back(*programToBeAdded);
        }
        std::cout << "Zostal wygenerowany obiekt CGP_Algorithm" << std::endl;
      }

      //funkcja dodajaca formula to listy FormulaList
      void addFormula(formula f){
        formulasList.push_back(f);
      }

      //funkcja  poswiadczajaca, ze wszystkie dodano wszystie przewidziane funkcje
      void enoughFormulas(){

      }


  };


}


#endif
