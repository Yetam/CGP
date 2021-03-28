
/*
 * =============================================================================
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 * =============================================================================
 *
 *   CGPlib.h is a Cartesian Genetic Programming library written in C++.
 *   Its purpose is to incorporate Genetic Programming algorithms.
 *
 *   It was created and is developed in the course of creating my
 *   master thesis on Faculty of Physics on Warsaw University of Technology
 *   (Wydział Fizyki Politechniki Warszawskiej) with title:
 *   "Implementation and application of the Cartesian Genetic Programming algorithm to evolution of cellular automata"
 *   and my supervisor is Maciej Mrowiński
 *
 *   I was doing my best to optimize it, though it can still be polished more :)
 *
 *   2019, 2020, 2021 Mateusz Niewiadomski
 * =============================================================================
*/

#ifndef __CGPlib__
#define __CGPlib__

#include<list>
#include<string>
#include<iostream>
#include<cstdlib>
#include<vector>
#include<ctime>
#include <chrono>
#include<thread>
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

    T * value;                        //value of block
    std::vector<Block<T> *> inputs;   //list of blocks connected TO this block
    std::vector<T*> inputValues;      //list of values from blocks connected TO this block
    int id;                           //id of this block
    int formulaId;                    //id of formula realized by this block
    bool importanceOfBlock;           //is block used during program evaluation

    typedef void (* formula)(std::vector<T*> * funcInputs, T *funcOutput);
    formula myFunction;               //pointer to formula realized by this block


  public:

    //==========================================================================
    //    CONSTRUCTOR AND DESTRUCTOR
    //==========================================================================
    Block(int blockID, int blockType): id(blockID){
        value = new T();
        importanceOfBlock = false;
    }
    ~Block(){
      delete value;
    }

    //==========================================================================
    //    GETTERS
    //==========================================================================
    int getId(){return id;}                                                     //returns block id
    int getFormulaId(){return formulaId;}                                       //returns id of formula realized by this block
    std::vector<T*> * getInputValuesPointer(){return &inputValues;}             //returns pointer to vector inputValues
    int getNofInputs(){return inputs.size();}                                   //returns number of blocks connected to this block
    T * returnValueOfBlock(){return value;}                                     //returns pointer to value stored by this block
    bool getImportance(){return importanceOfBlock;}                             //returns this block importance
    Block<T> * getNthInput(int n){                                              //returns pointer to the n-th block connected to this block
      if(n<inputs.size()){
        return inputs.at(n);
      }
      else{
        std::cerr << "inputs list index out of range" << std::endl;
        return NULL;
      }
    }

    //==========================================================================
    //    SETTERS
    //==========================================================================
    void setValueOfBlock(T newValue){*value = newValue;}                        //sets value of this block to new value
    void setFormulaId(int newFormulaId){formulaId = newFormulaId;}              //sets formula id realized by this block. Does not change the formula pointer!
    void setId(int newId){this->id = newId;}                                    //sets this block id
    void setImportance(bool newImportance){importanceOfBlock = newImportance;}  //sets this block importance
    void setNthInput(int n,Block<T> * newNthInput){                             //sets n-th input as specified by argument
      inputs.at(n) = newNthInput;
      inputValues.at(n) = newNthInput->value;
    }
    void addInput( Block<T> * nextInput ){                                      //adds nw input to this block at the end of the list of inputs
      inputs.push_back(nextInput);
      inputValues.push_back(nextInput->value);
    }


    //==========================================================================
    //  functions used to list blocks or get information about them
    //==========================================================================

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
      std::string output = " ";
      output.append(std::to_string( this->getFormulaId() ));
      output.append("");
      for(int i=0 ; i<this->getNofInputs() ; i++){
        output.append( std::to_string(inputs.at(i)->getId()) );
        output.append(" ");
      }
      return output;
    }
    long getNumericHash(){
      long numHash=0;
      int ii=0;
      for(ii ; ii<this->getNofInputs() ; ii++){
        numHash += (1+ii)*inputs.at(ii)->getId();
      }
      numHash *= (this->getFormulaId()+1);
      return numHash;
    }
    std::string saveGeneToString(){
      std::string geneString = "";
      geneString.append(std::to_string(this->getId())); //id
      geneString.append(" ");
      geneString.append(std::to_string(this->getFormulaId())); //funkcja
      geneString.append(" ");
      geneString.append(std::to_string(this->getNofInputs())); //liczba wejsc
      geneString.append(" ");

      for(int i=0 ; i<this->getNofInputs() ; i++){
        geneString.append( std::to_string(inputs.at(i)->getId()) );
        geneString.append(" ");
      }
      return geneString;
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

    std::vector<Block<T> *> genes;              //list of program genes
    std::vector<T*> thisProgramOutputValues;    //vector of output values
    int inputs;                                 //number of input genes (getting valus from "outer world")
    int formulas;                               //number of formula genes (genes which are processing values)
    int outputs;                                //number of output genes (from which the output value of program is taken "to the outer world"). These can still process data
    int maxInputsToFurmula;                     //maximal number of inputs that a formula from formula list can take
    int id;

    typedef void (* formula)(std::vector<T*> * funcInputs, T *funcOutput);      //definition of formula pointer
    std::vector<formula> formulasList;    //list of formulas - delivered by CGP_Algorithm

  public:
    //Program
    Program(){
      //nothing :(
    };
    Program(int progID, int nOfInputs, int nOfFormulas,  int nOfOutputs,  std::vector<formula> formulasListPointer, int maxInputs): id(progID), inputs(nOfInputs), formulas(nOfFormulas), outputs(nOfOutputs), formulasList(formulasListPointer), maxInputsToFurmula(maxInputs) /*blockInputs(nOfBlockInputs)*/{

      int currentID=0;
      Block<T> * blockToBeAdded;

      for(int i=0 ; i<inputs ; i++){                      //adding input genes
        blockToBeAdded = new Block<T>(currentID,-1);
        genes.push_back(blockToBeAdded);
        currentID++;
      }
      for(int i=0 ; i<formulas ; i++){                    //adding formula genes
        blockToBeAdded = new Block<T>(currentID,0);
        genes.push_back(blockToBeAdded);
        currentID++;
      }
      for(int i=0 ; i<outputs ; i++){                     //adding output genes
        blockToBeAdded = new Block<T>(currentID,1);
        genes.push_back(blockToBeAdded);
        currentID++;
      }
      blockToBeAdded=NULL; //just in case

      //random filling of genes input
      for(int i=inputs ; i<genes.size() ; i++){           //dla każdego genu począwszy od pierwszego niewejściowego //for each gene starting from first noninput gene
        int inputsNumber = maxInputsToFurmula;            //chose number g from (0 to i). It will be a number of input to that gene
        for(int g=0 ; g<inputsNumber ; g++)               //add g inputs to this gene
          genes.at(i)->addInput( genes.at(rand()%i) );    //adding one of previous genes as input to this one. WARNING - duplicates allowed
      }

      //preliminary write of output genes values to vector of output values
      for(int currOutGene=(inputs+formulas) ; currOutGene<genes.size() ; currOutGene++){
        thisProgramOutputValues.push_back(genes.at(currOutGene)->returnValueOfBlock());
      }

      setupFunctionsInGenes();  //setup of functions in genes of this program

  };

  public:
    //destructor of class Program. Calls destructors of Block class on each of block/genes list
    ~Program(){
      for(int i=0 ; i<genes.size() ; i++){
        delete genes[i];
      }
    }



  public:
    int getNumberOfGenes(){return genes.size();}                                //returns number of genes in this program
    //int getNofInputGenes(){return inputs;}          //
    int getID(){return id;}                                                     //returns this program id
    void setID(int newID){id = newID;}                                          //sets this program id
    int getInputs(){return this->inputs;}                                       //return number of input genes
    int getFormulas(){return this->formulas;}                                   //return number of formula genes
    int getOutputs(){return this->outputs;}                                     //return nuber of output genes
    int getMaxInputsToFurmula(){return this->maxInputsToFurmula;}               //returns maximal number of inputs that any of formulas from formula list can take
    std::vector<formula> getFormulasList(){return this->formulasList;}          //returns the list of formulas

  public:
    Block<T> * getGeneAt(int n){return genes.at(n);}                            //returns n-th gene of this program (starting from first input gene)
    //int getInputsOfGeneAt( int n ){ return genes.at(n)->getNofInputs(); }
  public:
    void addGene(Block<T> * blockToAdd){genes.push_back(blockToAdd);}           //adds gene to the genes list at the end

  public:
    void dropGenes(){
      for(int i=0 ; i<genes.size()  ; i++){
        delete genes[i];
      }
      genes.clear();
    }

    //==========================================================================
    //    PROGRAM COPYING
    //==========================================================================
  public:
    void copyProgram(Program * src, Program * dst){
      /*
          Program copying algorithm
            -clear blocks in dst
            -add blocks in dst starting from input genes
              -for each gene recreate input list of that gene
      */
      dst->dropGenes();                                                         //clear blocks in dst

      //Dodawanie takiej liczby genów do dst jak jest w src
      int currentID=0;
      Block<T> * blockToBeAdded;
      for(int i=0 ; i<src->getNumberOfGenes() ; i++){
        blockToBeAdded = new Block<T>(currentID,-1);
        dst->addGene(blockToBeAdded);
        //dst->setFormulaId( src->getFormulaId() );
        currentID++;
      }
      //przepisywanie połączeń dla każdego genu
      for(int currGene=0 ; currGene<dst->getNumberOfGenes() ; currGene++){      //iterating over all genes from src
        dst->getGeneAt(currGene)->setFormulaId( src->getGeneAt(currGene)->getFormulaId() );
        int inputsOfcurrGene = src->getGeneAt(currGene)->getNofInputs();        //acquiring number of inputs of current gene
        dst->getGeneAt(currGene)->setImportance(src->getGeneAt(currGene)->getImportance()); //rewriting the importance of the gene
        for(int currInput=0 ; currInput<inputsOfcurrGene ; currInput++){        //iterating over inputs of current gene
          dst->getGeneAt(currGene)->addInput( dst->getGeneAt( src->getGeneAt(currGene)->getNthInput(currInput)->getId() ) );
          //in currGene gene of dst add an input which is a gene from dst with the same ID as input currInput from currGene in src
        }
      }

      dst->setID( src->getID() );                                               //rewriting IDs
    }

    //==========================================================================
    //    PROGRAM MUTATION
    //==========================================================================
  private:
    bool isMutate(int denominatorOfChance){                                     //decides if mutation occurs
      return rand()%denominatorOfChance==0;
    }
  public:
    void mutateProgram(int denominatorOfChance){
      bool organismMutated = false;
      int mutations = 0;
      for(int currGene=inputs; currGene<genes.size() ; currGene++){             //iterating over genes
        for(int currInput = 0 ; currInput<genes.at(currGene)->getNofInputs() ; currInput ++){           //iterating over currGene inputs
          if(isMutate(denominatorOfChance)){                                        //if mutation must occur then
            genes.at(currGene)->setNthInput(currInput , genes.at( rand()%currGene ) ); //mutate random input
            organismMutated = true;
            mutations++;
          }
        }
        if(isMutate(denominatorOfChance)){                                      //if the formula mutation must occur
          genes.at(currGene)->setFormulaId( rand()%formulasList.size()  );      //change the formula randomly
          mutations++;
        }
      }
      if(organismMutated){
        this->updateImportanceOfGenes();                                        //update information of genes importance
      }
      //std::cout << "muations: " << mutations << std::endl;

    }

    //==========================================================================
    //    Calculating values in genes
    //==========================================================================
  private:
    void propagateForwardFull(){
      for(int currGene = inputs ; currGene<genes.size() ; currGene++){
        if( getGeneAt(currGene)->getImportance() ){
          formulasList.at(genes.at(currGene)->getFormulaId())(
                            genes.at(currGene)->getInputValuesPointer(),
                            genes.at(currGene)->returnValueOfBlock()
                          );
        }
      }

      //update output values vector
      thisProgramOutputValues.clear();
      for(int currOutGene=(inputs+formulas) ; currOutGene<genes.size() ; currOutGene++){
        thisProgramOutputValues.push_back(genes.at(currOutGene)->returnValueOfBlock());
      }
    }
    //==========================================================================
    //    Setting program input values
    //==========================================================================
  public:
    void setProgramInputValue(int inputIndex, T newValue){
      if(inputIndex >= inputs ){
        std::cerr << "Requested access to noninput Block while setting up block. Requested "<< inputIndex <<"while max Input index is " << inputs-1 <<   std::endl;
      }
      else{
        genes.at(inputIndex)->setValueOfBlock( newValue );
      }
    }

    //==========================================================================
    //    Calculating fitness
    //==========================================================================

  private:
    void setupFunctionsInGenes(){
        for(int currGene=0;currGene<genes.size();currGene++){
          genes.at(currGene)->setFormulaId( rand()%formulasList.size()  );
          //std::cout << genes.at(currGene)->getFormulaId() << std::endl;
        }
      }

  public:
    std::vector<T*> * proceedProgram( std::vector<T> * inputValues ){
        //ustaw wartości w organiźmie
        if(inputValues->size() != inputs)
          std::cerr << "Fitness function input list size not equal to program input list size, passed: "<< inputValues->size() << " required: " << inputs << std::endl;
        for(int currGene = 0 ; currGene<inputs ; currGene++){
          genes.at(currGene)->setValueOfBlock( inputValues->at(currGene) );
        }
        propagateForwardFull();
        return &thisProgramOutputValues;
    }
    std::vector<T*> * proceedProgram(){
        //ustaw wartości w organiźmie
        //if(inputValues->size() != inputs)
      //    std::cerr << "Fitness function input list size not equal to program input list size, passed: "<< inputValues->size() << " required: " << inputs << std::endl;
      //  for(int currGene = 0 ; currGene<inputs ; currGene++){
      //    genes.at(currGene)->setValueOfBlock( inputValues->at(currGene) );
      //  }
        propagateForwardFull();
        return &thisProgramOutputValues;
    }

    //==========================================================================
    //    calculating importance of genes
    //==========================================================================
  public:
    void updateImportanceOfGenes(){
      for(int currGene=0 ; currGene<genes.size() ; currGene++ ){                //set all importance to false
        genes.at(currGene)->setImportance(false);
      }
      for(int currGene=0 ; currGene<genes.size() ; currGene++ ){                //set true to all input and outputs
        if(currGene<inputs || currGene>=(inputs+formulas) )
          genes.at(currGene)->setImportance(true);
      }
      for(int currGene=genes.size()-1 ; currGene>=inputs ; currGene--){                     //iteratong bact-to-front
        if( genes.at(currGene)->getImportance() ){                                          //if currGene is important
          for(int inputGene=0; inputGene<genes.at(currGene)->getNofInputs() ; inputGene++){ //iterate over all inputs of that gene
            genes.at(currGene)->getNthInput(inputGene)->setImportance(true);                //set each input importance to true
          }
        }
      }
    }

    //==========================================================================
    //    Printing program
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
        //output.append(std::to_string( this->getID() ));
        for(int i=0;i<genes.size();i++){
          output.append(genes.at(i)->getHash());
          //output.append("\n");
        }
        return output;
    }

    Program<T> * makeNewProgramFrom(Program<T> * src){

      /*
      Program(
      int progID,
      int nOfInputs,
      int nOfFormulas,
      int nOfOutputs,
      std::vector<formula> formulasListPointer,
      int maxInputs):
      */
      Program<T> * newProgram = new Program<T>(
        src->getID(),
        src->getInputs(),
        src->getFormulas(),
        src->getOutputs(),
        src->getFormulasList(),
        src->getMaxInputsToFurmula()
      );

      newProgram->copyProgram(src,newProgram);
/*
            std::cout <<  newProgram->getID() << " " <<
                    newProgram->getInputs()<< " " <<
                    newProgram->getFormulas()<< " " <<
                    newProgram->getOutputs()<< " " <<
                    newProgram->getFormulasList().size()<< " " <<
                    newProgram->getMaxInputsToFurmula() << std::endl;
*/
      if(src != NULL){
        return newProgram;
      }
      else{
        std::cerr << "COpied program was null" << std::endl;
        return NULL;
      }

    }

  std::string saveProgramToString(){
    std::string savedString = " ";
    savedString.append( std::to_string(inputs) );
    savedString.append( " " );
    savedString.append( std::to_string(formulas) );
    savedString.append( " " );
    savedString.append( std::to_string(outputs) );
    savedString.append( " " );
    savedString.append( std::to_string(maxInputsToFurmula) );
    savedString.append( " " );
    savedString.append( std::to_string(id) );

    for(int i=0;i<genes.size();i++){
      savedString.append(genes.at(i)->saveGeneToString());
      savedString.append("\n");
    }

  }

  long getNumericHash(){
    long numHash = (inputs+formulas+outputs+maxInputsToFurmula);
    for(int i=0;i<genes.size();i++){
      numHash += (i+1)*genes.at(i)->getNumericHash();
    }

    if(this->id < 0) numHash *= (-1);
    return numHash;
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
CLass proceeding the CGP algorithms
================================================================================ */
  template <typename T>
  class CGP_Algorithm{

  private:
    //===============================================
    //  BASIC PARAMETERS OF CGP Algorithm
    //===============================================

    const int mu = 1;                                                           //number of parents in each epoch (DO NOT CHANGE! PROBABLY FIXED)
    const int lambda = 4;                                                       //number of children in each epoch (DO NOT CHANGE! PROBABLY FIXED)

    int numberOfInputGenes;                                                     //number of genes in each program that get input
    int numberOfProcessingGenes;                                                //number of genes in each program that proceed formulas and values but do not return value as program output
    int numberOfOutputGenes;                                                    //number of genes in each program that proceed formulas and values and return value as program output

    //===============================================
    // GENES FUNCTION
    //===============================================
    typedef void (* formula)(std::vector<T*> * funcInputs, T *funcOutput);      //definition of function pointer to formulas that process values
    std::vector<formula> formulasList;                                          //vector storing formulas defined by user
    std::vector<int> formulasInputsList;                                        //vector storing number of inputs to each formula
    int maxFormulaInputs = 0;                                                   //maximal number of formula inputs )from all defined formulas)

    //===============================================
    //  FITNESS AND DRAWING FUNCTIONS
    //===============================================
    typedef float (* calcFitness)(Program<T> * p);                              //definition of pointer to fitness function that calculates fitness value
    calcFitness fitFunc;                                                        //pointer to fitness funtion
    std::vector<float> fitnessValues;                                           //valus of fitness from current epoch
    bool zeroFitnessPanic = false;                                              //if true then the algorithm i stoppes (i.e given treshold of fitness achieved)
    int minimumFitnessOrganismIndex;                                            //index of organism with best fitness
    float minimumFitnessValue = DBL_MAX;                                        //value of best fitness
    int mutationChanceDenominator =100;                                         //chance of mutation (if you want mutation chance of 1/100 then set to 100 if 1/30 then set to 30)

    //===============================================
    //  PROGRAMS POOL AND ITS FUNCTIONS
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
    //  CGP modes of run
    //===============================================
    std::string setup_flag = "SETUP";                                           //flag to setup the algorithm with given parameters
    std::string proceed_flag = "PROCEED";                                       //flag to run CGP algorithm as long as specified by parameter
    std::string diehard_flag = "DIEHARD";                                       //flag to run CGP in diehard mode - stops only if given treshold of fitness is achieved (may run endlessly!!)

    //===============================================
    //  LEFTOVERS
    //===============================================
    long showedProgress = 0;
    bool verbose_level = false;
    time_t CGP_runtime;


    //===============================================
    //  SETTING ORGANISMS (PROGRAMS)
    //
    //  Function initialize organisms (programs) for CGO_Algorithm. It is intentionally not implemented ic constructor so thisProgramOutputValues
    //  user has a moment to pass formulas pointers to CGP_Algorithm between its declaration and initialization
    //===============================================
      void setOrganism(){

        organisms.clear();
        Program<T> * programToBeAdded;
        for(int i=1 ; i<=mu ; i++ ){
          programToBeAdded = new Program<T>((-1)*rand(),numberOfInputGenes, numberOfProcessingGenes,  numberOfOutputGenes,formulasList , maxFormulaInputs);
          organisms.push_back(programToBeAdded);
        }
        for(int i=1 ; i<=lambda ; i++ ){
          programToBeAdded = new Program<T>(rand(),numberOfInputGenes, numberOfProcessingGenes, numberOfOutputGenes,formulasList, maxFormulaInputs);
          organisms.push_back(programToBeAdded);
        }
        programToBeAdded = NULL;
      }

    //===============================================
    // METHODS RUN BY CGP
    //===============================================
      void calculateFitnessForAllOrganisms(){
        float newFitValue = 0;                                                  //place to store newely calculated fitness value
        for(int i=0 ; i<organisms.size() ; i++){
          newFitValue = fitFunc( organisms.at(i) );                             //calculating fitness value (with averaging)
          if(fitnessValues.size() < organisms.size())
            fitnessValues.push_back(newFitValue);
          else
            fitnessValues.at(i) = newFitValue;
        }

    }
    bool chooseBestNewParents(){
      short lastParentdID;
      float parentFitness;
      float meanOffspring =0;
      for(int i=0 ; i<organisms.size() ; i++){                                  //set all programs ID to positive value (indicatng they are children)
        if(organisms.at(i)->getID() < 0){
          lastParentdID = i;
        }
        else{
          meanOffspring += organisms.at(i)->getID();
        }
        organisms.at(i)->setID( std::abs( organisms.at(i)->getID() ) );
      }
      meanOffspring /=4;

      //find program index with lowest value of ftness
      float localMinFitnessValue = DBL_MAX;
      int localMinFitnessIndex = 0;
      for(int i=0 ; i<fitnessValues.size() ; i++){
        if( fitnessValues.at(i) < localMinFitnessValue ){
          localMinFitnessValue = fitnessValues.at(i);
          localMinFitnessIndex = i;
        }
      }
      minimumFitnessOrganismIndex = localMinFitnessIndex;
      minimumFitnessValue = localMinFitnessValue;

      //set the program ID with minimum fitness to negative value (indicatong it is a new parent)
      organisms.at(minimumFitnessOrganismIndex)->setID( organisms.at(minimumFitnessOrganismIndex)->getID() * (-1) );


      bool doOffspring = false;
      if(lastParentdID == minimumFitnessOrganismIndex){
        if(minimumFitnessValue < 0.8*meanOffspring){
          doOffspring = true;
        }
        else{
          doOffspring = false;
        }
      }
      else{
        if(fitnessValues.at((lastParentdID)-fitnessValues.at(minimumFitnessOrganismIndex) < fitnessValues.at(lastParentdID)/25.) ){
          doOffspring = false;
        }
        else{
            doOffspring = true;
        }
      }
      fitnessValues.clear();
      return doOffspring;
  }
    void createOffspring(bool doesParentChanged){
      //if the parent has changed then make offspring out of it, otherwise do nothing
      if(doesParentChanged){
        //find program with negatove index (parent)
        int parentIndex=0;
        for(int i=0 ; i<organisms.size() ; i++){
          if(organisms.at(i)->getID() < 0){
            parentIndex = i;
            break;
          }
        }

        //clone it everywhere except itself
        for(int i=0 ; i<organisms.size() ; i++){
          if( i != parentIndex ){
            copyProgram(organisms.at(parentIndex), organisms.at(i));
            organisms.at(i)->setID( abs( organisms.at(i)->getID() ) );
          }
        }
      }
      else{
        //DO NOTHING
      }
    }
    void mutateOffspring(){
      //mutateProgram(int denominatorOfChance)
      for(int i=0 ; i<organisms.size() ; i++){
        if( organisms.at(i)->getID() >= 0 ){
            organisms.at(i)->mutateProgram(mutationChanceDenominator);
        }
      }
    }
    void calculateImportanceOfGenes(){
      for(int i=0 ; i<organisms.size() ; i++){
        organisms.at(i)->updateImportanceOfGenes();
      }
    }

    bool diehardChecker(float treshold){
      if(minimumFitnessValue < treshold){
        return true;
      }
      else{
        return false;
      }
    }

    //==========================================================================
    //  CREATING AND RUNNING CGP Algorithm
    //==========================================================================
    public:

    /*
      Main constructor od CGP Algorithms. As parameters it must get:
      -(int) number of input genes
      -(int) number of processing genes (non-ouptut)
      -(int) number of output genes (which can still do operations)
      -(func pointer) pointer to fitness function
      -(func pointer) pointer to drawing (used for mutation purposes) function
    */
      CGP_Algorithm(int nOfInputs, int nOfFormulas,  int nOfOutputs, calcFitness fitnessFunctionPointer,float mutationChanceDenominatorParam ): numberOfInputGenes(nOfInputs), numberOfProcessingGenes(nOfFormulas), numberOfOutputGenes(nOfOutputs), fitFunc(fitnessFunctionPointer), mutationChanceDenominator(mutationChanceDenominatorParam){
        mutationChanceDenominator = 1./mutationChanceDenominatorParam;
      }

    /*
      Class destructor
    */
    ~CGP_Algorithm(){}

    /*
      Function called by user to start the algorithm. As arguments it must get:
        -(string) flag informing of mode in which the doCGP is called. Possible modes are:
          --"SETUP" - sets parameters and organisms (programs). MUST be called first
          --"PROCEED" - proceeds given number of epochs
          --"DIEHARD" - proceeds epochs untill given fitness treshold is achieved
        -(int) max epochs. After that exits
      Returns:
        -0 if given treshold was not achieved
        -1 if treshold was achieved
    */
    int doCGP(std::string s,int epochs=0, float fitTreshold = 0.00001){
      int return_value = -1;

      if(!s.compare(setup_flag)){                                               //IF RUN AT SETUP FLAG
        setOrganism();                                                          //basic setup of organisms before the run commences
        calculateImportanceOfGenes();                                           //calculating importance of (still) dummy programs
        return_value = 0;
      }
      else if(!s.compare(proceed_flag)){                                        //IF RUN AT PROCEED FLAG
        return_value = 0;
        for(int i=0 ; i<epochs ; i++){                                          //MAIN PROGRAM LOOP
          calculateFitnessForAllOrganisms();                                    //calculate all programs fitness
          createOffspring(chooseBestNewParents());                              //create offspring based on current (or new) parents
          mutateOffspring();                                                    //mutate children
          calculateImportanceOfGenes();                                         //calculate importance of genes in programs
        }
      }
      else if(!s.compare(diehard_flag)){                                        //IF RUN AT DIEHARD FLAG
        return_value = 0;
        for(int i=0 ; i<epochs ; i++){                                          //MAIN PROGRAM LOOP
          calculateFitnessForAllOrganisms();                                    //calculate all programs fitness
          createOffspring(chooseBestNewParents());                              //create offspring based on current (or new) parents
          mutateOffspring();                                                    //mutate children
          calculateImportanceOfGenes();                                         //calculate importance of genes in programs
          return_value++;
          if(diehardChecker(fitTreshold)){                                      //check if treshold was achieved
            break;
          }
        }
      }
      return return_value;
    }

    //===============================================
    //  MODIFYING AND READING FROM CGP_ALgorithm
    //===============================================

      void addFormula(formula f, int numberOfFormulaInputs){                    //adding formula to formula list
        formulasList.push_back(f);
        formulasInputsList.push_back(numberOfFormulaInputs);

        for(int i=0;i<formulasInputsList.size();i++){
          if(formulasInputsList.at(i) > maxFormulaInputs){
            maxFormulaInputs = formulasInputsList.at(i);
          }
        }
      }
      float getFitness(){                                                       //returns fitness of pest fitted organism
        return minimumFitnessValue;
      }
      Program<T> * getBestOrganism(){                                           //returns pointer to the best fitted organism
        return organisms.at(minimumFitnessOrganismIndex);
      }
      Program<T> * getSpecifiedOrganism(int i){                                 //returns pointer to specified organism
        if(i<organisms.size()) return organisms.at(i);
        else return NULL;
      }
  };


}


#endif
