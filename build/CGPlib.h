#ifndef __CGPlib__
#define __CGPlib__

#include<list>
#include<iostream>
#include<cstdlib>
#include<vector>
#include<time.h>

namespace CGP{

/* =============================================================================
     ____   _____   ______  _____          _______  _____  ____   _   _            _
    / __ \ |  __ \ |  ____||  __ \     /\ |__   __||_   _|/ __ \ | \ | |    /\    | |
   | |  | || |__) || |__   | |__) |   /  \   | |     | | | |  | ||  \| |   /  \   | |
   | |  | ||  ___/ |  __|  |  _  /   / /\ \  | |     | | | |  | || . ` |  / /\ \  | |
   | |__| || |     | |____ | | \ \  / ____ \ | |    _| |_| |__| || |\  | / ____ \ | |____
    \____/ |_|     |______||_|  \_\/_/    \_\|_|   |_____|\____/ |_| \_|/_/    \_\|______|

============================================================================= */


  //KLASA ABSTRAKCYJNA, PIERWOWZOR DO TWORZENIA OBIEKTÓW WARTOSCI NA KTÓRYCH OPERUJE CGP
  // ================================\/ OBSOLETE \/=================================
  // ================================\/ OBSOLETE \/=================================
  // ================================\/ OBSOLETE \/=================================
  class Operational{
  public:
    Operational(){};
    virtual void print() = 0; //metoda czysto wirtualna wypisująca jasno i zwięźle wartość obiektu
    virtual void copy(Operational * src, Operational * dst) = 0;
  };
  // ================================/\ OBSOLETE /\=================================
  // ================================/\ OBSOLETE /\=================================
  // ================================/\ OBSOLETE /\=================================

/* =============================================================================
    ____   _       ____    _____  _  __
   |  _ \ | |     / __ \  / ____|| |/ /
   | |_) || |    | |  | || |     | ' /
   |  _ < | |    | |  | || |     |  <
   | |_) || |____| |__| || |____ | . \
   |____/ |______|\____/  \_____||_|\_\

============================================================================= */
  template <typename T>
  class Block{

  private:
    //POLA DEFINIUJACE
    T * value;        //wartosc zmiennej bloku
    std::vector<Block<T> *> inputs;
    std::vector<T*> inputValues;
    int id;
    int type;     //(-1)=input, (0)=func, (1)=output
    int formulaId;

    typedef void (* formula)(std::vector<T*> * funcInputs, T *funcOutput);
    formula myFunction;

    //POLA FUNKCYJNE

  public:
//KONSTRUKTORY
    Block(int blockID, int blockType): id(blockID), type(blockType){
      //  std::cout << "\t\tAdded block ID: " << blockID << std::endl;
        value = new T();
    }
    ~Block(){
      delete value;
    }

    int getId(){return id;}
    int getType(){return type;}
    int getFormulaId(){return formulaId;}
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
    void setNthInput(int n,Block<T> * newNthInput){
      inputs.at(n) = newNthInput;
      inputValues.at(n) = newNthInput->value;
    }
    T * returnValueOfBlock(){
      return value;
    }
    void setFormulaId(int newFormulaId){formulaId = newFormulaId;}
    void addInput( Block<T> * nextInput ){
      inputs.push_back(nextInput);
      inputValues.push_back(nextInput->value);
    }

    void setMyFormula(formula myNewFormula){
      myFunction = myNewFormula;
    }

    //MUTACJE BLOKU

    //LISTING
    void printBlock(){
      std::cout << "\t"  << id << "\t->\t";
      for(int i=0 ; i<inputs.size() ; i++){
        std::cout << inputs.at(i)->getId() <<" ";
      }
      std::cout << std::endl;
    }
  };

/* =============================================================================
    _____   _____    ____    _____  _____             __  __
   |  __ \ |  __ \  / __ \  / ____||  __ \     /\    |  \/  |
   | |__) || |__) || |  | || |  __ | |__) |   /  \   | \  / |
   |  ___/ |  _  / | |  | || | |_ ||  _  /   / /\ \  | |\/| |
   | |     | | \ \ | |__| || |__| || | \ \  / ____ \ | |  | |
   |_|     |_|  \_\ \____/  \_____||_|  \_\/_/    \_\|_|  |_|

============================================================================= */

  template <typename T>
  class Program{

    std::vector<Block<T> *> genes;
    int inputs;
    int formulas;
    int outputs;
    //int blockInputs;
    int maxInputsToFurmula;   //ile maksymnalnie wejsć mają funkcje z formulasList
    int id;

    typedef void (* formula)(std::vector<T*> * funcInputs, T *funcOutput);
    std::vector<formula> formulasList;

    //wskazniki na funkcje potrzebne do funkcji celu
    typedef double (* calcFitness)(std::vector<T*> * inputs, std::vector<T*> * outputs);               //sama funkcja celu, przyjmuje inputy jako wzór i outputy jako przedmiot oceny
    typedef void (* getRandomInput)( T * whereToWriteValue);        //funkcja zapewniajaca zestaw danych wejściowych do programu, wykorzystywana przy obliczaniu fitnessu

      public:
    Program(int progID, int nOfInputs, int nOfFormulas,  int nOfOutputs,  std::vector<formula> formulasListPointer, int maxInputs): id(progID), inputs(nOfInputs), formulas(nOfFormulas), outputs(nOfOutputs), formulasList(formulasListPointer), maxInputsToFurmula(maxInputs) /*blockInputs(nOfBlockInputs)*/{
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
        //int inputsNumber = rand()%i;
        int inputsNumber = maxInputsToFurmula;                                            //wybierz liczbę g z przedziału (0,i). Będzie to liczba inputów tego genu
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
    void returnDataIntoVectors(std::vector<T*> * inputsVector, std::vector<T*> * outputsVector){
      for(int i=0 ; i<inputs ; i++){
        inputsVector.push_back( genes.at(i).returnValueOfBlock() );
      }
      for(int i=inputs+formulas ; i<genes.size() ; i++){
        outputsVector.push_back( genes.at(i).returnValueOfBlock() );
      }
    }
    void dropGenes(){
      for(int i=0 ; i<genes.size()  ; i++){
        delete genes[i];
      }
      genes.clear();
    }

    //==========================================================================
    //    KOPIOWANIE PROGRAMU
    //==========================================================================
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
    //==========================================================================
    //    MUTOWANIE PROGRAMU
    //==========================================================================
    bool isMutate(int denominatorOfChance){
      return rand()%denominatorOfChance==0;
    }
    void mutateProgram(int denominatorOfChance){
      for(int currGene=inputs; currGene<genes.size() ; currGene++){             //iterowanie po kolejnych chromosomach
        for(int currInput = 0 ; currInput<genes.at(currGene)->getNofInputs() ; currInput ++){           //iterowanie po kolejnych inputach chromosomu
          if(isMutate(denominatorOfChance)){                                        //jeżeli ma zajść mutacja połączenia
            genes.at(currGene)->setNthInput(currInput , genes.at( rand()%currGene ) ); //zmutuj losowy input
          }
        }
        if(isMutate(denominatorOfChance)){                                        //jeżeli ma zajść mutacja formuły
          genes.at(currGene)->setFormulaId( rand()%formulas  );
        }
      }
    }

    //==========================================================================
    //    Obliczanie wartosci w genach
    //==========================================================================
    void propagateForwardFull(){
      for(int currGene = inputs ; currGene<genes.size() ; currGene++){
        formulasList.at(genes.at(currGene)->getFormulaId())(&genes.at(currGene)->inputValues , genes.at(currGene)->value);
      }
    }
    //==========================================================================
    //    Obliczanie funkcji dopasowania
    //==========================================================================
    void calculateFitness(){
      //ustaw wartosci poczatkowe
      for(int currGene = inputs ; currGene<genes.size() ; currGene++){

      }
    }


    //LISTING
    void printProgram(){
      for(int i=0 ; i<genes.size() ; i++){
        genes.at(i)->printBlock();
      }
    }
  };

/* =============================================================================
     _____  _____  _____                    _                      _  _    _
    / ____|/ ____||  __ \                  | |                    (_)| |  | |
   | |    | |  __ | |__) |            __ _ | |  __ _   ___   _ __  _ | |_ | |__   _ __ ___
   | |    | | |_ ||  ___/            / _` || | / _` | / _ \ | '__|| || __|| '_ \ | '_ ` _ \
   | |____| |__| || |               | (_| || || (_| || (_) || |   | || |_ | | | || | | | | |
    \_____|\_____||_|                \__,_||_| \__, | \___/ |_|   |_| \__||_| |_||_| |_| |_|
                            ______              __/ |
                           |______|            |___/

============================================================================= */

  //klasa CGP jest główną klasą przechowującą metody i dane związane z implementacją CGP
  template <typename T>
  class CGP_Algorithm{

  //DEKLARACJE DOT. FUNKCJI UŻYWANYCH PRZEZ CGP
    typedef void (* formula)(std::vector<T*> * funcInputs, T *funcOutput);    //definicja typu wskaznika na fcje
    std::vector<formula> formulasList;                          //lista do ktorej uzytkownik wrzuca zdefiniowane przez siebie funkcje
    std::vector<int> formulasInputsList;
    int maxFormulaInputs = 0;

    typedef double (* calcFitness)(std::vector<T*> * inputs, std::vector<T*> * outputs);               //sama funkcja celu, przyjmuje inputy jako wzór i outputy jako przedmiot oceny
    typedef void (* getRandomInput)( T * whereToWriteValue);
    calcFitness fitFunc;
    std::vector<double> fitnessValues;

    int mu;         //wielkość populacji rodziców
    int lambda;      //wielkośc populacji potomstwa

    //DEKLARACJE PUL ORGANIZMÓW UŻYWANYCH PRZEZ CGP
    std::vector<Program<T> *> organisms;

    //DEKLARACJE FUNKCJI ROZRODCZYCH
    void makeOffspring(){
      for(int i=mu ; i<organisms.size() ; i++){
        delete organisms[i];
      }
    }
    void copyProgram(Program<T> * src, Program<T> * dst){
      src->copyProgram(src,dst);
    }

    public:

      /*
      *   SETUP CGP
      */

      CGP_Algorithm(int muVal, int lambdaVal): mu(muVal), lambda(lambdaVal){
        std::cout << "CGP_Algorithm constructor" << std::endl;
      }
      void setOrganism( int nOfInputs, int nOfFormulas,  int nOfOutputs ){

        Program<T> * programToBeAdded;
        std::cout << "CGP_Algorithm::setOrganism(): adding parents" << std::endl;
        for(int i=1 ; i<=mu ; i++ ){
          programToBeAdded = new Program<T>((-1)*rand(),nOfInputs, nOfFormulas,  nOfOutputs,formulasList , maxFormulaInputs);
          organisms.push_back(programToBeAdded);
        }
        std::cout << "CGP_Algorithm::setOrganism(): adding offspring" << std::endl;
        for(int i=1 ; i<=lambda ; i++ ){
          programToBeAdded = new Program<T>(rand(),nOfInputs, nOfFormulas, nOfOutputs,formulasList, maxFormulaInputs);
          organisms.push_back(programToBeAdded);
        }
        programToBeAdded = NULL;
        std::cout << "Zostal wygenerowany obiekt CGP_Algorithm" << std::endl;
      }
      void testCopyingPrograms(int srcIndex, int dstIndex){
        copyProgram(organisms.at(srcIndex), organisms.at(dstIndex));
      }
      void testMutate(int denominatorOfChance){
        organisms.at(1)->mutateProgram(denominatorOfChance);
      }
      ~CGP_Algorithm(){

      }


      /*
      *   TUTAJ CGP JUŻ COŚ ROBI
      */
      //główna funkcja procedująca algorytm CGP dla tego obiektu CGP_Algorithm
      void doCGP(int epochs){

        for(int i=0 ; i<epochs ; i++){

          //zakładając że CGP jest już skonfigurowane, czyli istnieją rodzice i dzieci jakkolwiek zainicjalizowane

          //calculateFitnessForAllOrganisms();      //oblicz dopasowanie wszystkich organizmów
          //chooseBestNewParents();                 //wyznacz najlepsze organizmy które od teraz stają się rodzicami, a reszta staje się dziećmi
          //createOffspring();                      //wypelnij luki po dzieciach nowymi dziecmi z nowych rodzicow
          //mutateOffspring();                      //zmutuj zgodnie z regula wszystkie dzieci

        }

      }
      /*
      *   NARZĘDZIE GŁÓWNEGO WYKONANIA (wykorzystywane przez doCGP())
      */
      void calculateFitnessForAllOrganisms(){
        std::vector<T*> inValues;//stworz wektor na liste wejsciowych
        std::vector<T*> outValues;//stworz wektor na liste wyjsciowych
        fitnessValues.clear();//wyczysc wektor wartosci fitnesow, bo porzeciez teraz beda nowe
        for(int i=0 ; i<organisms.size() ; i++){
          inValues.clear();//wyzeruj wektor wejsciowych
          outValues.clear();//wyzeruj wektor wyjsciowych
          organisms.at(i).returnDataIntoVectors(&inValues, &outValues);//dla itego programu pobierz wektory wejsiowe i wyjsciowe
          fitnessValues.push_back( fitFunc( &inValues, &outValues ) );//oblicz do itego fitnessValues fitness itego organizmu
        }
    }


      /*
      *   NARZĘDIA SETUPOWE I TESTOWE
      */

      void listOrganisms(char mode){
        if(mode = 'a'){
          std::cout << "WYPISUJE ID PROGRAMÓW:" << std::endl;
          for(int i=0 ; i<organisms.size() ; i++){ //nOfOutputs,formulasList
            std::cout << organisms.at(i)->getID()<< std::endl;
            organisms.at(i)->printProgram();
          }
        }
      }

      //funkcja dodajaca formula to listy FormulaList
      void addFormula(formula f, int numberOfFormulaInputs){
        formulasList.push_back(f);
        formulasInputsList.push_back(numberOfFormulaInputs);
      }

      //funkcja  poswiadczajaca, ze wszystkie dodano wszystie przewidziane funkcje
      void enoughFormulas(){
        for(int i=0;i<formulasInputsList.size();i++){
          if(formulasInputsList.at(i) > maxFormulaInputs){
            maxFormulaInputs = formulasInputsList.at(i);
          }
        }
      }
      void addFitnessFunction(calcFitness f){
        fitFunc = f;
      }

      int getNumberOfFormulas(){
        return formulasList.size();
      }



  };


}


#endif
