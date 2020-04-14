#ifndef __CGPlib__
#define __CGPlib__

#include<list>
#include<iostream>
#include<cstdlib>
#include<vector>
#include<ctime>
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

    //zwraca ID bloku
    int getId(){return id;}
    //zwraca typ bloku
    int getType(){return type;}
    //zwraca ID formuly przypisanej do tego bloku
    int getFormulaId(){return formulaId;}
    //zwraca wskaznik na wektor wskazników na wartości w blokach połączonych z tym blokiem
    std::vector<T*> * getInputValuesPointer(){return &inputValues;}
    //zwraca liczbę bloków połaczonych z tym blokiem
    int getNofInputs(){return inputs.size();}
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
    std::vector<T*> thisProgramOutputValues;
    int inputs;
    int formulas;
    int outputs;
    int maxInputsToFurmula;   //ile maksymnalnie wejsć mają funkcje z formulasList
    int id;

    typedef void (* formula)(std::vector<T*> * funcInputs, T *funcOutput);      //definicja wskaznika na funkcje formuly
    std::vector<formula> formulasList;    //lista formuł - dostarczana przez CGP_Algorithm

    //wskazniki na funkcje potrzebne do funkcji celu
    typedef double (* calcFitness)(std::vector<T*> * inputs, std::vector<T*> * outputs);               //sama funkcja celu, przyjmuje inputy jako wzór i outputy jako przedmiot oceny
    typedef void (* getRandomInput)( T * whereToWriteValue);        //funkcja zapewniajaca zestaw danych wejściowych do programu, wykorzystywana przy obliczaniu fitnessu

      public:
    Program(int progID, int nOfInputs, int nOfFormulas,  int nOfOutputs,  std::vector<formula> formulasListPointer, int maxInputs): id(progID), inputs(nOfInputs), formulas(nOfFormulas), outputs(nOfOutputs), formulasList(formulasListPointer), maxInputsToFurmula(maxInputs) /*blockInputs(nOfBlockInputs)*/{
      //std::cout << "\tProgram() Id:" << progID << std::endl;

      int currentID=0;
      Block<T> * blockToBeAdded;

      //dodawanie bloków wejściowych
      //std::cout << "\t/dodawanie bloków wejściowych/" << std::endl;
      for(int i=0 ; i<inputs ; i++){
        blockToBeAdded = new Block<T>(currentID,-1);
        genes.push_back(blockToBeAdded);
        currentID++;
      }
      //dodawanie bloków funkcyjnych
      //std::cout << "\t/dodawanie bloków funkcyjnych/" << std::endl;
      for(int i=0 ; i<formulas ; i++){
        blockToBeAdded = new Block<T>(currentID,0);
        genes.push_back(blockToBeAdded);
        currentID++;
      }
      //dodawanie bloków wyjściowych
      //std::cout << "\t/dodawanie bloków wyjsciowych/" << std::endl;
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

      for(int currOutGene=(inputs+formulas) ; currOutGene<genes.size() ; currOutGene++){
        thisProgramOutputValues.push_back(genes.at(currOutGene)->returnValueOfBlock());
      }

  };

    ~Program(){
      //std::cout << "\tUsuwam Program ID: "<< id << std::endl;
      for(int i=0 ; i<genes.size() ; i++){
        //std::cout << "\tUsuwam gen ID: " << i << std::endl;
        delete genes[i];
      }
    }

    int getNumberOfGenes(){return genes.size();}
    int getNofInputGenes(){return inputs;}
    int getID(){return id;}
    void setID(int newID){id = newID;}
    Block<T> * getGeneAt(int n){return genes.at(n);}
    int getInputsOfGeneAt( int n ){ return genes.at(n)->getNofInputs(); }
    void addGene(Block<T> * blockToAdd){genes.push_back(blockToAdd);}
    void returnDataIntoVectors(std::vector<T*> * inputsVector, std::vector<T*> * outputsVector){
      for(int i=0 ; i<inputs ; i++){
        inputsVector->push_back( genes.at(i)->returnValueOfBlock() );
      }
      for(int i=inputs+formulas ; i<genes.size() ; i++){
        outputsVector->push_back( genes.at(i)->returnValueOfBlock() );
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
          genes.at(currGene)->setFormulaId( rand()%formulasList.size()  );
        }
      }
    }

    //==========================================================================
    //    Obliczanie wartosci w genach
    //==========================================================================
    void propagateForwardFull(){
      for(int currGene = inputs ; currGene<genes.size() ; currGene++){
//std::cout << "(121) " << currGene << " from " << genes.size() << " " << genes.at(currGene)->getInputValuesPointer()->size() << " formula " << genes.at(currGene)->getFormulaId() << " from " << formulasList.size() << std::endl;
        formulasList.at(genes.at(currGene)->getFormulaId())(
                          genes.at(currGene)->getInputValuesPointer(),
                          genes.at(currGene)->returnValueOfBlock()
                        );
//std::cout << "(122)" << std::endl;
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
    /*void calculateFitness(){
      //ustaw wartosci poczatkowe
      for(int currGene = inputs ; currGene<genes.size() ; currGene++){

      }
    }*/

    std::vector<T*> * calcFitnessValue( std::vector<T> * inputValues ){
        //ustaw wartości w organiźmie
        if(inputValues->size() != inputs)
          std::cerr << "Fitness function input list size not equal to program input list size" << std::endl;
        for(int currGene = 0 ; currGene<inputs ; currGene++){
          genes.at(currGene)->setValueOfBlock( inputValues->at(currGene) );
        }
        propagateForwardFull();
        return &thisProgramOutputValues;
    }

    void setupFunctionsInGenes(){
      for(int currGene=0;currGene<genes.size();currGene++){
        genes.at(currGene)->setFormulaId( rand()%formulasList.size()  );
      }
    }

    //LISTING
    void printProgram(){
      //std::cout << "ID_programu: " << getID() << std::endl;
      for(int i=0 ; i<genes.size() ; i++){
        genes.at(i)->printBlock();
      }
    }

    void printInputValues(){
      //std::cout << "in of ID: " << id << std::endl;
      for(int i=0;i<inputs;i++){
        std::cout <<  genes.at(i)->returnValueOfBlock()->getVal() << "\t" << std::endl;
      }
    }
    void printOutputValues(){
      //std::cout << "out of ID: " << id << std::endl;
      for(int i=inputs+formulas ; i<genes.size() ; i++){
        std::cout <<  genes.at(i)->returnValueOfBlock()->getVal() << "\t" << std::endl;
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
    typedef void (* getRandomInput)( T * whereToWriteValue);  //def typu wsk na fcje zwracającą losową wartość dla programu
    getRandomInput randVal;                                   //wsk na fcje zdef powyżej
    typedef double (* calcFitness)(Program<T> * p);           //def typu wsk na fcje realizującą ocenę programu
    calcFitness fitFunc;                                      //wsk na fcje zdef powyżej
    std::vector<double> fitnessValues;                        //wartosci fitness z danej epoki
    bool zeroFitnessPanic = false;                            //jeżeli true to osiągnięto próg dopasowania

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

    //DEKLARACJE FUNKCJI ROZRODCZYCH


    public:
    /*
      Główny konstruktor klasy CGP_ALgorithm. Jako wejście przyjmuje:
      -(int) liczba genów wejściowych
      -(int) liczba genów procesujących
      -(int) liczba genów procesujących wyjściowych
      -(func pointer) wskaźnik na funkcję celu
      -(func pointer) wskaźnik na funkcję losującą
    */
      CGP_Algorithm(int nOfInputs, int nOfFormulas,  int nOfOutputs, calcFitness fitnessFunctionPointer, getRandomInput randomInputFunctionPointer): numberOfInputGenes(nOfInputs), numberOfProcessingGenes(nOfFormulas), numberOfOutputGenes(nOfOutputs), fitFunc(fitnessFunctionPointer), randVal(randomInputFunctionPointer){
        //std::cout << "CGP_Algorithm constructor" << std::endl;
      }

    /*
      Funkcja inicjalizująca organizmy (programy) do CGP_Algorithm. NIe jest zaimnplementowana w konstruktorze, gdyż
      uzytkownik musi miec czas aby przekazac wskazniki na funkcje pomiedzy utworzeniem instancji klasy a jej wewnetrzna inicjalizacja
    */
      void setOrganism(){

        Program<T> * programToBeAdded;
        if(verbose_level)
          std::cout << "CGP_Algorithm::setOrganism(): adding parents" << std::endl;
        for(int i=1 ; i<=mu ; i++ ){
          programToBeAdded = new Program<T>((-1)*rand(),numberOfInputGenes, numberOfProcessingGenes,  numberOfOutputGenes,formulasList , maxFormulaInputs);
          organisms.push_back(programToBeAdded);
        }
        if(verbose_level)
          std::cout << "CGP_Algorithm::setOrganism(): adding offspring" << std::endl;
        for(int i=1 ; i<=lambda ; i++ ){
          programToBeAdded = new Program<T>(rand(),numberOfInputGenes, numberOfProcessingGenes, numberOfOutputGenes,formulasList, maxFormulaInputs);
          organisms.push_back(programToBeAdded);
        }

        for(int i=0 ; i<organisms.size() ; i++){
          organisms.at(i)->setupFunctionsInGenes();
        }

        programToBeAdded = NULL;
        if(verbose_level)
          std::cout << "Zostal wygenerowany obiekt CGP_Algorithm" << std::endl;
      }
      /*
        Destruktor klasy, brak kluczowych obiektów do ręcznego usunięcia
      */
      ~CGP_Algorithm(){}


      /*
         Funkcja wywolywana przez uzytkownika w celu uruchomienia dzialania algorytmu. Jako argument przyjmuje:
          -(int) maksymalną liczbe epok po której algorytm powinien sie zatrzymac
      */
      void doCGP(int epochs){
        setOrganism();        //wstepnie ustawia organizmy (programy) uzywane przez algorytm
        //listOrganisms('a');   //wypisuje organizmy do konsoli
        time_me(true);
                                                  //GŁÓWNA PĘTLA ALGORYTMU
        for(int i=0 ; i<epochs ; i++){
          calculateFitnessForAllOrganisms(1);     //oblicz dopasowanie wszystkich organizmów
          chooseBestNewParents();                 //wyznacz najlepsze organizmy które od teraz stają się rodzicami, a reszta staje się dziećmi
          createOffspring();                      //wypelnij luki po dzieciach nowymi dziecmi z nowych rodzicow
          mutateOffspring();                      //zmutuj zgodnie z regula wszystkie dzieci
          showProgress(i,epochs);                 //wypisz postęp

          if(zeroFitnessPanic)                    //wyjdz z petli jesli osiagnieto próg dopasowania
            break;
        }
        time_me(false);
      }
      /*
      *   NARZĘDZIE GŁÓWNEGO WYKONANIA (wykorzystywane przez doCGP())
      */

      void evaluateOrganisms(){
        //losowo wypelnij inputy
        for(int org=0 ; org<organisms.size() ; org++){
          for(int inp=0; inp<organisms.at(org)->getNofInputGenes() ; inp++ ){
            randVal(organisms.at(org)->getGeneAt(inp)->returnValueOfBlock());  //zapisz losowa wartosc do inp-owego inputu organizmu ,,org,,
          }
        }
        //przepropaguj inputy do outputow
        for(int org=0 ; org<organisms.size() ; org++){
            organisms.at(org)->propagateForwardFull();
        }
      }

      void calculateFitnessForAllOrganisms(double averagingRounds){

        for(int i=0 ; i<organisms.size() ; i++){
          double newFitValue = 0;     //miejsce na obliczana wartosc funkcji celu
          newFitValue = (fitFunc( organisms.at(i) )); //oblicz funkcje celu z uwzglednieniem usredniania
          if(fitnessValues.size() < organisms.size()){
            fitnessValues.push_back(newFitValue);
          }
          else{
            fitnessValues.at(i) += newFitValue;
          }

        }
    }
    void chooseBestNewParents(){
      //ustaw wszytskim ID na dodatnie (jako dzieci) oraz ustaw fitnessValue na 0
      for(int i=0 ; i<organisms.size() ; i++){
        organisms.at(i)->setID( std::abs( organisms.at(i)->getID() ) );
      }

      //znajdz wartosc najmniejsza fitnessFunction i jej indeks
      double minFitVal = DBL_MAX;
      int minFitIndex =0;
      for(int i=0 ; i<fitnessValues.size() ; i++){
        if( fitnessValues.at(i) < minFitVal ){
          minFitVal = fitnessValues.at(i);
          minFitIndex = i;
        }
      }


      //zmien ID na ujemne, bo to nowy rodzic
      organisms.at(minFitIndex)->setID( organisms.at(minFitIndex)->getID() * (-1) );
      if(verbose_level)
        std::cout << "Miniimum fitness at organism: " << minFitIndex << " and is " << minFitVal << std::endl;
      if(minFitVal < 0.0001)
        zeroFitnessPanic = true;
      //Jezeli liczba rodzicow >1 to trzeba dodac tutaj petle, ktora wyrzuci s fitnessValues minimlane,
      //znowu znajdzie minimum i zmieni mu ID na ujemne
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
    void showProgress(int i, int epochs){

      if( i%(epochs/100) == 0 ){
        showedProgress ++;
        if(verbose_level)
          std::cout << "Progress: " << (i*100)/epochs << "%" << std::endl;
      }
      else{
        showedProgress = 0;
      }
    }

      /*
      *   NARZĘDIA SETUPOWE I TESTOWE
      */

      void listOrganisms(char mode){
          std::cout << "WYPISUJE ID PROGRAMÓW: of " << organisms.size() << std::endl;
          for(int i=0 ; i<organisms.size() ; i++){ //nOfOutputs,formulasList
            std::cout << "Organism index: " << i << std::endl;
            organisms.at(i)->printProgram();
          }
      }

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

      //funkcja  poswiadczajaca, ze wszystkie dodano wszystie przewidziane funkcje
      /*void enoughFormulas(){
        for(int i=0;i<formulasInputsList.size();i++){
          if(formulasInputsList.at(i) > maxFormulaInputs){
            maxFormulaInputs = formulasInputsList.at(i);
          }
        }
      }*/


      int getNumberOfFormulas(){
        return formulasList.size();
      }

      void time_me(bool isStart){
        if(!isStart){
          //std::cout << "Runtime: " << difftime(time(NULL),CGP_runtime) << " s" << std::endl;
          std::cout << difftime(time(NULL),CGP_runtime) << std::endl;
        }

        CGP_runtime = time(NULL);
      }

  };


}


#endif
