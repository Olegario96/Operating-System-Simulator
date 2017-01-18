#ifndef BANKER_H
#define BANKER_H

#include <vector>

/*!
 *  \brief Banker.h
 *  \copyright Copyright [2016] <Lucas Joao Martins>
 *  \author Lucas Joao Martins
 *
 *  class responsible to do the banker's algorithm
 */
class Banker {
private:
  int _numberOfResources; //!< has the number of resources in algorithm
  int _numberOfProcesses; //!< has the number of processes in algorithm

  std::vector<int>* _existenceResources; //!< resources in existence
  std::vector<int>* _availableResources; //!< resources available
  std::vector<std::vector<int>>* _currentAllocation; //!< current allocation
                                                     //** matrix
  std::vector<std::vector<int>>* _processNeeds; //!< request matrix

public:
  /*!
   *  \brief default constructor
   *
   *  do nothing, because dont was implemented
   */
  Banker();

  /*!
   *  \brief constructor
   *  \param int numberOfResources is the number of resources in algorithm
   *  \param int numberOfProcesses is the number of process in algorithm
   *
   *  initialize all private members
   */
  Banker(int numberOfResources, int numberOfProcesses);

  /*!
   *  \brief destructor
   *
   *  do nothing, because dont was implemented
   */
  virtual ~Banker();

  /*!
   *  \brief getter function getExistenceResources
   *  \return a pointer to _existenceResources
   */
  std::vector<int>* getExistenceResources();

  /*!
   *  \brief getter function getAvailableResources
   *  \return a pointer to _availableResources
   */
  std::vector<int>* getAvailableResources();

  /*!
   *  \brief getter function getProcessNeeds
   *  \return a pointer to _processNeeds
   */
  std::vector<std::vector<int>>* getProcessNeeds();

  /*!
   *  \brief getter function getCurrentAllocation
   *  \return a pointer to _currentAllocation
   */
  std::vector<std::vector<int>>* getCurrentAllocation();

  /*!
   *  \brief function addExistenceResources
   *  \param int resource is the resource that will be add in simulation
   *  \param int amount says how many resources will be add
   *
   *  put amount in the right index of resource at vectors
   *    _availableResources and _existenceResources
   */
  void addExistenceResources(int resource, int amount);

  /*!
   *  \brief function addProcessNeeds
   *  \param int process is the process linked with the other params
   *  \param int resource is the resource necessary to process finish your job
   *  \param int amount says how many resources will be need to the process
   *
   *  add in _processNeeds the amount of a specified resource that a specified
   *    process need to finish your job
   */
  void addProcessNeeds(int process, int resource, int amount);

  /*!
   *  \brief function request
   *  \param int process is the process that want the request
   *  \param int resource is the resource that the process wants
   *  \param int amount says how many resources the process wants
   *  \return a boolean that says if was possible grant and do the request
   *  \sa algorithm(...)
   *
   *  if the amount be a good number (not greater than what the process need),
   *    this function will call algorithm that will check if the request will
   *    result in a safe state or not. Only ff the resulting state is safe,
   *    then the request will be accept
   */
  bool request(int process, int resource, int amount);

  /*!
   *  \brief function algorithm
   *  \param int process is the process that want the request
   *  \param int resource is the resource that the process wants
   *  \param int amount says how many resources the process wants
   *  \return boolean, says if the resulting state after the request is safe
   *
   *  this function has the banker's algorithm implemented in a similar way of
   *    his description of Tanembaum book (modern operating systems 4th, 455)
   */
  bool algorithm(int process, int resource, int amount);

  /*!
   *  \brief function free
   *  \param int process is the process
   *  \param int resource is the resource
   *
   *  if is possible (the process has at the least one resource), free one unit
   *    of the specified resource of the resource and change all variable
   *    linked with this
   */
  void free(int process, int resource);

  /*!
   *  \brief function printHelperDebug
   *
   *  print a lot of 'Debug::cout' that help a manual check of the simulation.
   *  This function is not necessary to a right work of algorithm
   */
  void printHelperDebug();
};

#endif /* BANKER_H */