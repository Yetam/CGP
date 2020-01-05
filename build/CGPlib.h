#ifndef __CGPlib__
#define __CGPlib__

#include<list>
#include<iostream>
#include<cstdlib>
#include<vector>
#include<time.h>

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
        std::cout << "\t\tAdded block ID: " << blockID << std::endl;
        value = new T();
    }
    ~Block(){
      delete value;
    }

    int getId(){return id;}
    int getType(){return type;}
    int getNofInputs(){return inputs.size();}
    Block<T> * getNthInput(int n){
      if(n<inputs.size()){
        return inputs.at(n);
      }
      else{
        std::cerr << "inputs list index out of range" << std::endl;
        return NULL;
      }
    }
    void addInput( Block<T> * nextInput ){inputs.push_back(nextInput);}


//METODY KOPIUJĄCE

//LISTING
    void printBlock(){
      std::cout << "\t"  << id << "\t->\t";
      for(int i=0 ; i<inputs.size() ; i++){
        std::cout << inputs.at(i)->getId() <<" ";
      }
      std::cout << std::endl;
    }
  };

//============================================================================//
//==========================       klasa Program      ========================//
//============================================================================//

  template <typename T>
  class Program{

    std::vector<Block<T> *> genes;
    int inputs;
    int formulas;
    int outputs;
    int id;

      public:
    Program(int progID, int nOfInputs, int nOfFormulas, int nOfOutputs): id(progID), inputs(nOfInputs), outputs(nOfOutputs), formulas(nOfFormulas){
      std::cout << "\tProgram() Id:" << progID << std::endl;

      int currentID=0;
      Block<T> * blockToBeAdded;

      //dodawanie bloków wejściowych
      std::cout << "\t/dodawanie bloków wejściowych/" << std::endl;
      for(int i=0 ; i<inputs ; i++){
        blockToBeAdded = new Block<T>(currentID,-1);
        genes.push_back(blockToBeAdded);
        currentID++;
      }
      //dodawanie bloków funkcyjnych
      std::cout << "\t/dodawanie bloków funkcyjnych/" << std::endl;
      for(int i=0 ; i<formulas ; i++){
        blockToBeAdded = new Block<T>(currentID,0);
        genes.push_back(blockToBeAdded);
        currentID++;
      }
      //dodawanie bloków wyjściowych
      std::cout << "\t/dodawanie bloków wyjsciowych/" << std::endl;
      for(int i=0 ; i<outputs ; i++){
        blockToBeAdded = new Block<T>(currentID,1);
        genes.push_back(blockToBeAdded);
        currentID++;
      }
      blockToBeAdded=NULL; //tak w razie czego :)

      //random fill inputów genów
      for(int i=inputs ; i<genes.size() ; i++){                                 //dla każdego genu począwszy od pierwszego niewejściowego
        int inputsNumber = rand()%i;                                            //wybierz liczbę g z przedziału (0,i). Będzie to liczba inputów tego genu
        for(int g=0 ; g<inputsNumber ; g++)                                     //dodaj g inputów
          genes.at(i)->addInput( genes.at(rand()%i) );                          //dodawanie jednego z wcześniejszych genów jako inputu. UWAGA - inputy mogą być swoimi duplikatami
      }
    };

    ~Program(){
      std::cout << "\tUsuwam Program ID: "<< id << std::endl;
      for(int i=0 ; i<genes.size() ; i++){
        std::cout << "\tUsuwam gen ID: " << i << std::endl;
        delete genes[i];
      }
    }

    int getNumberOfGenes(){return genes.size();}
    int getID(){return id;}
    void setID(int newID){id = newID;}
    Block<T> * getGeneAt(int n){return genes.at(n);}
    int getInputsOfGeneAt( int n ){ return genes.at(n)->getNofInputs(); }
    void addGene(Block<T> * blockToAdd){genes.push_back(blockToAdd);}
    void dropGenes(){
      for(int i=0 ; i<genes.size()  ; i++){
        delete genes[i];
      }
      genes.clear();
    }




    void copyProgram(Program * src, Program * dst){
      /*
          Algorytm kopiowania Programu:
            -Wyczyść bloki w dst
            -Przepisuj bloki w kolejności od wejściowych
              -dla każdego bloku stworz nowa liste inputsow tmp
      */
      dst->dropGenes();                                                         //usuwanie listy genów w dst

      //Dodawanie takiej lic )zby genów do dst jak jest w src
      int currentID=0;
      Block<T> * blockToBeAdded;
      for(int i=0 ; i<src->getNumberOfGenes() ; i++){
        blockToBeAdded = new Block<T>(currentID,-1);
        dst->addGene(blockToBeAdded);
        currentID++;
      }
      //przepisywanie połączeń dla każdego genu
      for(int currGene=0 ; currGene<dst->getNumberOfGenes() ; currGene++){
        int inputsOfcurrGene = src->getGeneAt(currGene)->getNofInputs(); //src->getNofInputs();                             //wydobycie liczby połączeń do akurat iterowanego genu
        for(int currInput=0 ; currInput<inputsOfcurrGene ; currInput++){        //iterowanie po kolejnych inputach danego genu
          dst->getGeneAt(currGene)->addInput( dst->getGeneAt( src->getGeneAt(currGene)->getNthInput(currInput)->getId() ) );
          //w dst w genie currGene dodaj input będący genem z dst o takim ID jakie ma input currInput z genu currGene w src
        }
      }

      //przepisywanie ID
      dst->setID( src->getID() );

    }

    //LISTING
    void printProgram(){
      for(int i=0 ; i<genes.size() ; i++){
        genes.at(i)->printBlock();
      }
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
    std::vector<Program<T> *> organisms;

    //DEKLARACJE FUNKCJI ROZRODCZYCH
    void makeOffspring(){
      for(int i=0 ; i<organisms.size() ; i++){
        delete organisms[i];
      }
    }
    void copyProgram(Program<T> * src, Program<T> * dst){
      src->copyProgram(src,dst);
    }

    public:
      CGP_Algorithm(int muVal, int lambdaVal): mu(muVal), lambda(lambdaVal){
        std::cout << "CGP_Algorithm constructor" << std::endl;
      }
      void setOrganism( int nOfInputs, int nOfOutputs, int nOfFormulas ){

        Program<T> * programToBeAdded;
        std::cout << "CGP_Algorithm::setOrganism(): adding parents" << std::endl;
        for(int i=1 ; i<=mu ; i++ ){
          programToBeAdded = new Program<T>((-1)*rand(),nOfInputs, nOfOutputs, nOfFormulas);
          organisms.push_back(programToBeAdded);
        }
        std::cout << "CGP_Algorithm::setOrganism(): adding offspring" << std::endl;
        for(int i=1 ; i<=lambda ; i++ ){
          programToBeAdded = new Program<T>(rand(),nOfInputs, nOfOutputs, nOfFormulas);
          organisms.push_back(programToBeAdded);
        }
        programToBeAdded = NULL;
        std::cout << "Zostal wygenerowany obiekt CGP_Algorithm" << std::endl;
      }
      void testCopyingPrograms(int srcIndex, int dstIndex){
        copyProgram(organisms.at(srcIndex), organisms.at(dstIndex));
      }
      ~CGP_Algorithm(){

      }

      void listOrganisms(char mode){
        if(mode = 'a'){
          std::cout << "WYPISUJE ID PROGRAMÓW:" << std::endl;
          for(int i=0 ; i<organisms.size() ; i++){
            std::cout << organisms.at(i)->getID()<< std::endl;
            organisms.at(i)->printProgram();
          }
        }
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
