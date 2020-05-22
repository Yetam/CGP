#ifndef __CGPlib__
#define __CGPlib__

#include<list>
#include<string>
#include<iostream>
#include<cstdlib>
#include<vector>
#include<ctime>
#include<map>
#include<bits/stdc++.h>

namespace CGP{

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
    T * value;                        //wartosc zmiennej bloku
    std::vector<Block<T> *> inputs;   //lista bloków przekazujących dane do tego bloku
    std::vector<T*> inputValues;      //lista wartości z bloków które są połączone do tego bloku
    int id;
    int formulaId;
    bool importanceOfBlock;

    typedef void (* formula)(std::vector<T*> * funcInputs, T *funcOutput);
    formula myFunction;

    //POLA FUNKCYJNE

  public:
//KONSTRUKTORY
    Block(int blockID, int blockType): id(blockID){
        value = new T();
        importanceOfBlock = false;
    }
    ~Block(){
      delete value;
    }


    int getId(){return id;}                                                     //zwraca ID bloku
    int getFormulaId(){return formulaId;}                                       //zwraca ID formuly przypisanej do tego bloku
    std::vector<T*> * getInputValuesPointer(){return &inputValues;}             //zwraca wskaznik na wektor wskazników na wartości w blokach połączonych z tym blokiem
    int getNofInputs(){return inputs.size();}                                   //zwraca liczbę bloków połaczonych z tym blokiem

        //zwraca wskaznik na N-ty blok połączony z tym blokiem
    Block<T> * getNthInput(int n){
      if(n<inputs.size()){
        return inputs.at(n);
      }
      else{
        std::cerr << "inputs list index out of range" << std::endl;
        return NULL;
      }
    }
    //ustawia n-te połączenie bloku na wskazany blok
    void setNthInput(int n,Block<T> * newNthInput){
      inputs.at(n) = newNthInput;
      inputValues.at(n) = newNthInput->value;
    }
    //zwraca wskaznik na wartosc zapisana w tym bloku
    T * returnValueOfBlock(){
      return value;
    }
    void setValueOfBlock(T newValue){
      *value = newValue;
    }
    //ustawia ID formuly przypisanej do tego bloku
    void setFormulaId(int newFormulaId){formulaId = newFormulaId;}
    //dodaje wejscie na koncu listy wejsc polaczonych z tym blokiem
    void addInput( Block<T> * nextInput ){
      inputs.push_back(nextInput);
      inputValues.push_back(nextInput->value);
    }
    void setId(int newId){
      this->id = newId;
    }
    bool getImportance(){
      return importanceOfBlock;
    }
    void setImportance(bool newImportance){
      importanceOfBlock = newImportance;
    }
    //

    //LISTING
    void printBlock(){
      std::cout << "\t"  << id << "\t->\t";
      for(int i=0 ; i<inputs.size() ; i++){
        std::cout << inputs.at(i)->getId() <<" ";
      }
      std::cout << " fun: " << formulaId;
      std::cout << std::endl;
    }
    std::string blockToString(){
      std::string output = "";
      output.append(std::to_string( this->getId() ));
      output.append(" ");
      output.append(std::to_string( this->getFormulaId() ));
      for(int i=0 ; i<this->getNofInputs() ; i++){
        output.append(" ");
        output.append( std::to_string(inputs.at(i)->getId()) );
      }
      return output;
    }
    std::string getHash(){
      std::string output = "";
      output.append(std::to_string( this->getFormulaId() ));
      for(int i=0 ; i<this->getNofInputs() ; i++){
        output.append( std::to_string(inputs.at(i)->getId()) );
      }
      return output;
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

  private:

    std::vector<Block<T> *> genes;              //lista bloków (genów) danego programu
    std::vector<T*> thisProgramOutputValues;    //wektor wielkości wyjściowych (okno na świat dla programu)
    int inputs;                                 //liczba bloków (genów) wejściowych, przyjmujących dane z zewnątrz
    int formulas;                               //liczba bloków (genów) procedujących dane
    int outputs;                                //liczba bloków (genów) procedujących dane i zwracających je na zewnątrz
    int maxInputsToFurmula;                     //ile maksymnalnie wejsć mają funkcje z formulasList
    int id;

    typedef void (* formula)(std::vector<T*> * funcInputs, T *funcOutput);      //definicja wskaznika na funkcje formuly
    std::vector<formula> formulasList;    //lista formuł - dostarczana przez CGP_Algorithm

  public:
    Program(int progID, int nOfInputs, int nOfFormulas,  int nOfOutputs,  std::vector<formula> formulasListPointer, int maxInputs): id(progID), inputs(nOfInputs), formulas(nOfFormulas), outputs(nOfOutputs), formulasList(formulasListPointer), maxInputsToFurmula(maxInputs) /*blockInputs(nOfBlockInputs)*/{
      //std::cout << "\tProgram() Id:" << progID << std::endl;

      int currentID=0;
      Block<T> * blockToBeAdded;

      for(int i=0 ; i<inputs ; i++){                      //dodawanie bloków przyjmujacych dane z zewnątrz
        blockToBeAdded = new Block<T>(currentID,-1);
        genes.push_back(blockToBeAdded);
        currentID++;
      }
      for(int i=0 ; i<formulas ; i++){                    //dodawanie bloków procesuących dane
        blockToBeAdded = new Block<T>(currentID,0);
        genes.push_back(blockToBeAdded);
        currentID++;
      }
      for(int i=0 ; i<outputs ; i++){                     //dodawanie bloków procesujących przekazujących dane na zewnątrz
        blockToBeAdded = new Block<T>(currentID,1);
        genes.push_back(blockToBeAdded);
        currentID++;
      }
      blockToBeAdded=NULL; //tak w razie czego :)

      //random fill inputów genów
      for(int i=inputs ; i<genes.size() ; i++){           //dla każdego genu począwszy od pierwszego niewejściowego
        int inputsNumber = maxInputsToFurmula;            //wybierz liczbę g z przedziału (0,i). Będzie to liczba inputów tego genu
        for(int g=0 ; g<inputsNumber ; g++)               //dodaj g inputów
          genes.at(i)->addInput( genes.at(rand()%i) );    //dodawanie jednego z wcześniejszych genów jako inputu. UWAGA - inputy mogą być swoimi duplikatami
      }

      //wstępne wpisanie wartości z bloków wyjsciowych do wektora danych wyjsciowych
      for(int currOutGene=(inputs+formulas) ; currOutGene<genes.size() ; currOutGene++){
        thisProgramOutputValues.push_back(genes.at(currOutGene)->returnValueOfBlock());
      }

      setupFunctionsInGenes();  //ustawienie funkcji w genach

  };

  public:
    //Destruktor klasy Program. Wywołuje destruktor klasy Blok na każdym obiekcie z listy bloków
    ~Program(){
      for(int i=0 ; i<genes.size() ; i++){
        delete genes[i];
      }
    }
  public:
    int getNumberOfGenes(){return genes.size();}                                //zwraca liczbę genów
    //int getNofInputGenes(){return inputs;}          //
    int getID(){return id;}                                                     //zwraca ID programu
    void setID(int newID){id = newID;}                                          //ustawia ID programu

  public:
    Block<T> * getGeneAt(int n){return genes.at(n);}                            //zwraca n-ty gen programu
    //int getInputsOfGeneAt( int n ){ return genes.at(n)->getNofInputs(); }

  public:
    void addGene(Block<T> * blockToAdd){genes.push_back(blockToAdd);}           //dodaje gen do puli genów programu

  public:
    void dropGenes(){
      for(int i=0 ; i<genes.size()  ; i++){
        delete genes[i];
      }
      genes.clear();
    }

    //==========================================================================
    //    KOPIOWANIE PROGRAMU
    //==========================================================================
  public:
    void copyProgram(Program * src, Program * dst){
      /*
          Algorytm kopiowania Programu:
            -Wyczyść bloki w dst
            -Przepisuj bloki w kolejności od wejściowych
              -dla każdego bloku stworz nowa liste inputsow tmp
      */
      dst->dropGenes();                                                         //usuwanie listy genów w dst

      //Dodawanie takiej liczby genów do dst jak jest w src
      int currentID=0;
      Block<T> * blockToBeAdded;
      for(int i=0 ; i<src->getNumberOfGenes() ; i++){
        blockToBeAdded = new Block<T>(currentID,-1);
        dst->addGene(blockToBeAdded);
        currentID++;
      }
      //przepisywanie połączeń dla każdego genu
      for(int currGene=0 ; currGene<dst->getNumberOfGenes() ; currGene++){      //iterowanie po wszystkich genach
        int inputsOfcurrGene = src->getGeneAt(currGene)->getNofInputs();        //wydobycie liczby połączeń do akurat iterowanego genu
        dst->getGeneAt(currGene)->setImportance(src->getGeneAt(currGene)->getImportance()); //przepisanie ważności danego genu
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
  private:
    bool isMutate(int denominatorOfChance){
      return rand()%denominatorOfChance==0;
    }
  public:
    void mutateProgram(int denominatorOfChance){
      bool organismMutated = false;
      for(int currGene=inputs; currGene<genes.size() ; currGene++){             //iterowanie po kolejnych chromosomach
        for(int currInput = 0 ; currInput<genes.at(currGene)->getNofInputs() ; currInput ++){           //iterowanie po kolejnych inputach chromosomu
          if(isMutate(denominatorOfChance)){                                        //jeżeli ma zajść mutacja połączenia
            genes.at(currGene)->setNthInput(currInput , genes.at( rand()%currGene ) ); //zmutuj losowy input
            organismMutated = true;
          }
        }
        if(isMutate(denominatorOfChance)){                                        //jeżeli ma zajść mutacja formuły
          genes.at(currGene)->setFormulaId( rand()%formulasList.size()  );
        }
      }
      if(organismMutated){
        this->updateImportanceOfGenes();
      }

    }

    //==========================================================================
    //    Obliczanie wartosci w genach
    //==========================================================================
  private:
    void propagateForwardFull(){
      for(int currGene = inputs ; currGene<genes.size() ; currGene++){          //na każdym
        if( getGeneAt(currGene)->getImportance() ){
          formulasList.at(genes.at(currGene)->getFormulaId())(
                            genes.at(currGene)->getInputValuesPointer(),
                            genes.at(currGene)->returnValueOfBlock()
                          );
        }
      }

      //aktualizacja wektora wartosci wyjsciowych
      thisProgramOutputValues.clear();
      for(int currOutGene=(inputs+formulas) ; currOutGene<genes.size() ; currOutGene++){
        thisProgramOutputValues.push_back(genes.at(currOutGene)->returnValueOfBlock());
      }
    }
    //==========================================================================
    //    Obliczanie funkcji dopasowania
    //==========================================================================

  private:
    void setupFunctionsInGenes(){
        for(int currGene=0;currGene<genes.size();currGene++){
          genes.at(currGene)->setFormulaId( rand()%formulasList.size()  );
        }
      }

  public:
    std::vector<T*> * proceedProgram( std::vector<T> * inputValues ){
        //ustaw wartości w organiźmie
        if(inputValues->size() != inputs)
          std::cerr << "Fitness function input list size not equal to program input list size" << std::endl;
        for(int currGene = 0 ; currGene<inputs ; currGene++){
          genes.at(currGene)->setValueOfBlock( inputValues->at(currGene) );
        }
        propagateForwardFull();
        return &thisProgramOutputValues;
    }

    //==========================================================================
    //    Skracanie programu
    //==========================================================================
  public:
    void updateImportanceOfGenes(){
      for(int currGene=0 ; currGene<genes.size() ; currGene++ ){                   //zerowanie ważności, wszystko na FALSE
        genes.at(currGene)->setImportance(false);
      }
      for(int currGene=0 ; currGene<genes.size() ; currGene++ ){    //ustawianie na TRUE wszystkich outputów i inputów
        if(currGene<inputs || currGene>=(inputs+formulas) )
          genes.at(currGene)->setImportance(true);
      }
      for(int currGene=genes.size()-1 ; currGene>=inputs ; currGene--){                    //iterowanie od tyłu w celu znalezienia kluczowych genów
        if( genes.at(currGene)->getImportance() ){                                                      //jeżeli aktualny gen X jest ważny
          for(int inputGene=0; inputGene<genes.at(currGene)->getNofInputs() ; inputGene++){ //przeiteruj po wszystkich wejsciach tego genu
            genes.at(currGene)->getNthInput(inputGene)->setImportance(true);  //ustaw ważność n-tego inputu genu X na true

          }
        }
      }
    }

    //==========================================================================
    //    Wypisywanie programu
    //==========================================================================
  private:
    void printProgram(){
      //std::cout << "ID_programu: " << getID() << std::endl;
      for(int i=0 ; i<genes.size() ; i++){
        genes.at(i)->printBlock();
      }
    }
  public:
    std::string programToString(){
      std::string output="";
      for(int i=0 ; i<genes.size() ; i++){
        output.append(genes.at(i)->blockToString());
        output.append("\n");
      }
      return output;
    }
    std::string getHash(){
      std::string output="";
        output.append(std::to_string( this->getID() ));
        for(int i=0;i<genes.size();i++){
          output.append(genes.at(i)->getHash());
        }
        return output;
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

================================================================================
Klasa procedująca algorytm kartezjanskiego programowania genetycznego.
================================================================================ */
  template <typename T>
  class CGP_Algorithm{

  private:
    //===============================================
    //  PODSTAWOWE PARAMETRY ALGORYTMU CGP
    //===============================================

    const int mu = 1;         //wielkość populacji rodziców  (NIE ZMIENIAC! CHYBA ZAHARDKODOWANE)
    const int lambda = 4;     //wielkośc populacji potomstwa (NIE ZMIENIAC! CHYBA ZAHARDKODOWANE)

    int numberOfInputGenes;       //liczba genów które przyjmuja dane z zewnatrz
    int numberOfProcessingGenes;  //liczba genów które przeprocesowują wartości ale nie zwracają wyniku na zewnątworz
    int numberOfOutputGenes;      //liczba genów które przeprocesowują wartości i zwracają ich wartość na zewnątrz

    //===============================================
    // FUNKCJE GENÓW
    //===============================================
    typedef void (* formula)(std::vector<T*> * funcInputs, T *funcOutput);    //definicja typu wskaznika na fcje formuły która procesuje wielkości
    std::vector<formula> formulasList;    //wektor skladujacy formuly zdefiniowane przez uzytkownika
    std::vector<int> formulasInputsList;  //wektor skladujacy liczbe wejsc korespondującej formuły
    int maxFormulaInputs = 0;             //maksymalna liczba wejść (spośród wszystkich formuł)

    //===============================================
    //  FUNKCJE OCENIAJĄCE I LOSUJĄCE
    //===============================================
    typedef double (* calcFitness)(Program<T> * p);           //def typu wsk na fcje realizującą ocenę programu
    calcFitness fitFunc;                                      //wsk na fcje zdef powyżej
    std::vector<double> fitnessValues;                        //wartosci fitness z danej epoki
    bool zeroFitnessPanic = false;                            //jeżeli true to osiągnięto próg dopasowania
    int minimumFitnessOrganismIndex;
    double minimumFitnessValue = DBL_MAX;

    //===============================================
    //  PULA ORGANIZMÓW I JEJ FUNKCJE
    //===============================================
    std::vector<Program<T> *> organisms;    //wektor organizmów

    void makeOffspring(){
      for(int i=mu ; i<organisms.size() ; i++){
        delete organisms[i];
      }
    }
    void copyProgram(Program<T> * src, Program<T> * dst){
      src->copyProgram(src,dst);
    }

    //===============================================
    //  pozostałe
    //===============================================
    long showedProgress = 0;
    bool verbose_level = false;
    time_t CGP_runtime;
    std::string setup_flag = "SETUP";
    std::string proceed_flag = "PROCEED";
    std::string diehard_flag = "DIEHARD";

    //DEKLARACJE FUNKCJI ROZRODCZYCH

    /*
      Funkcja inicjalizująca organizmy (programy) do CGP_Algorithm. NIe jest zaimnplementowana w konstruktorze, gdyż
      uzytkownik musi miec czas aby przekazac wskazniki na funkcje pomiedzy utworzeniem instancji klasy a jej wewnetrzna inicjalizacja
    */
      void setOrganism(){

        Program<T> * programToBeAdded;
        for(int i=1 ; i<=mu ; i++ ){
          programToBeAdded = new Program<T>((-1)*rand(),numberOfInputGenes, numberOfProcessingGenes,  numberOfOutputGenes,formulasList , maxFormulaInputs);
          organisms.push_back(programToBeAdded);
        }
        for(int i=1 ; i<=lambda ; i++ ){
          programToBeAdded = new Program<T>(rand(),numberOfInputGenes, numberOfProcessingGenes, numberOfOutputGenes,formulasList, maxFormulaInputs);
          organisms.push_back(programToBeAdded);
        }
/*
        for(int i=0 ; i<organisms.size() ; i++){
          organisms.at(i)->setupFunctionsInGenes();
        }
*/
        programToBeAdded = NULL;
      }

    //===============================================
    // METODY WYKONYWANE PRZEZ doCGP()
    //===============================================
      void calculateFitnessForAllOrganisms(double averagingRounds){
        double newFitValue = 0;     //miejsce na obliczana wartosc funkcji celu
        for(int i=0 ; i<organisms.size() ; i++){
          newFitValue = (fitFunc( organisms.at(i) )); //oblicz funkcje celu z uwzglednieniem usredniania

          if(fitnessValues.size() < organisms.size())
            fitnessValues.push_back(newFitValue);
          else
            fitnessValues.at(i) += newFitValue;

        }
    }
    void chooseBestNewParents(){
      //ustaw wszytskim ID na dodatnie (jako dzieci) oraz ustaw fitnessValue na 0
      for(int i=0 ; i<organisms.size() ; i++){
        organisms.at(i)->setID( std::abs( organisms.at(i)->getID() ) );
      }
      //znajdz wartosc najmniejsza fitnessFunction i jej indeks
      double localMinFitnessValue = DBL_MAX;
      int localMinFitnessIndex = 0;
      for(int i=0 ; i<fitnessValues.size() ; i++){
        if( fitnessValues.at(i) < localMinFitnessValue ){
          localMinFitnessValue = fitnessValues.at(i);
          localMinFitnessIndex = i;
        }
      }
      minimumFitnessOrganismIndex = localMinFitnessIndex;
      minimumFitnessValue = localMinFitnessValue;

      //zmien ID na ujemne, bo to nowy rodzic
      organisms.at(minimumFitnessOrganismIndex)->setID( organisms.at(minimumFitnessOrganismIndex)->getID() * (-1) );
      fitnessValues.clear();
  }
    void createOffspring(){
      //znajdz program o ujemnym ID
      int parentIndex=0;
      for(int i=0 ; i<organisms.size() ; i++){
        if(organisms.at(i)->getID() < 0){
          parentIndex = i;
          break;
        }
      }

      //skopiuj go do wszystkich oprócz siebie
      for(int i=0 ; i<organisms.size() ; i++){
        if( i != parentIndex ){
          copyProgram(organisms.at(parentIndex), organisms.at(i));
          organisms.at(i)->setID( abs( organisms.at(i)->getID() ) );
        }
      }
    }
    void mutateOffspring(){
      //mutateProgram(int denominatorOfChance)
      for(int i=0 ; i<organisms.size() ; i++){
        if( organisms.at(i)->getID() >= 0 )
          organisms.at(i)->mutateProgram(numberOfProcessingGenes+numberOfOutputGenes);
      }
    }
    void calculateImportanceOfGenes(){
      for(int i=0 ; i<organisms.size() ; i++){
        organisms.at(i)->updateImportanceOfGenes();
      }
    }

    //==========================================================================
    //  TWORZENIE I URUCHAMIANIE CGP
    //==========================================================================
    public:

    /*
      Główny konstruktor klasy CGP_ALgorithm. Jako wejście przyjmuje:
      -(int) liczba genów wejściowych
      -(int) liczba genów procesujących
      -(int) liczba genów procesujących wyjściowych
      -(func pointer) wskaźnik na funkcję celu
      -(func pointer) wskaźnik na funkcję losującą
    */
      CGP_Algorithm(int nOfInputs, int nOfFormulas,  int nOfOutputs, calcFitness fitnessFunctionPointer ): numberOfInputGenes(nOfInputs), numberOfProcessingGenes(nOfFormulas), numberOfOutputGenes(nOfOutputs), fitFunc(fitnessFunctionPointer){
        //does n
      }

    /*
      Destruktor klasy, brak kluczowych obiektów do ręcznego usunięcia
    */
    ~CGP_Algorithm(){}

    /*
      Funkcja wywolywana przez uzytkownika w celu uruchomienia dzialania algorytmu. Jako argument przyjmuje:
        -(string) flaga informująca o trybie wywołania doCGP. Dostępne tryby:
          --"SETUP" - ustawia parametry i organizmy, musi być wywołana na początku
          --"PROCEED" - wykonuje zadaną liczbę epok
          --"DIEHARD" - wykonuje petle az do osiagniecia progu dopasowania
        -(int) maksymalną liczbe epok po której algorytm powinien sie zatrzymac
      Zwraca:
        -0 jeśli nie udało się zejść poniżej progu
        -1 jeżeli osiągnięto próg i iteracje zostały przerwane
    */
    int doCGP(std::string s,int epochs=0){
      int return_value = -1;

      if(!s.compare(setup_flag)){
        setOrganism();        //wstepnie ustawia organizmy (programy) uzywane przez algorytm
        return_value = 0;
      }
      else if(!s.compare(proceed_flag)){
        return_value = 0;
        //GŁÓWNA PĘTLA ALGORYTMU
        for(int i=0 ; i<epochs ; i++){
          calculateFitnessForAllOrganisms(1);     //oblicz dopasowanie wszystkich organizmów
          chooseBestNewParents();                 //wyznacz najlepsze organizmy które od teraz stają się rodzicami, a reszta staje się dziećmi
          createOffspring();                      //wypelnij luki po dzieciach nowymi dziecmi z nowych rodzicow
          mutateOffspring();                      //zmutuj zgodnie z regula wszystkie dzieci
          calculateImportanceOfGenes();           //wyznacza które bloki są ważne w danym programie
        }
      }
      return return_value;
    }

    //===============================================
    //  MODYFIKOWANIE I CZYTANIE Z CGP_ALgorithm
    //===============================================

      //funkcja dodajaca formula to listy FormulaList
      void addFormula(formula f, int numberOfFormulaInputs){
        formulasList.push_back(f);
        formulasInputsList.push_back(numberOfFormulaInputs);

        for(int i=0;i<formulasInputsList.size();i++){
          if(formulasInputsList.at(i) > maxFormulaInputs){
            maxFormulaInputs = formulasInputsList.at(i);
          }
        }
      }

      //Zwraca dopasowanie najlepszego organizmu
      double getFitness(){
        return minimumFitnessValue;
      }
      //Zwraca wskaźnik na najlepszy organizm
      Program<T> * getBestOrganism(){
        return organisms.at(minimumFitnessOrganismIndex);
      }

  };


}


#endif
