#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

void greedyAlg1(vector<vector<int>> weightParam, vector<vector<int>> profitParam, string outputFileName, vector<int> problemCapacities);
void printOut(string outputFileName, int size, int maxProfit, double ms);
vector<int> greedyAlg1Return(vector<int> weights, vector<int> profits, int capacity);
void greedyAlg2(vector<vector<int>> weightParam, vector<vector<int>> profitParam, string outputFileName, vector<int> problemCapacities);
vector<int> greedyAlg2Return(vector<int> weights, vector<int> profits, int capacity);
int KWF2(int i, int weight, int profit, vector<int> weights, vector<int> profits, int capacity);
bool promising(int i, int profit, int weight, vector<int> weights, vector<int> profits, int capacity, int *maxProfit);
void knapsack(int i, int profit, int weight, vector<int> weights, vector<int> profits, int capacity, vector<int> *bestSet, vector<int> include, int *maxProfit);
void backtracking(vector<vector<int>> weightParam, vector<vector<int>> profitParam, string outputFileName, vector<int> problemCapacities);

int main(int argc, char** argv) {

  if (argc != 4) {
    perror("Error. Needs 4 inputs.");
    return 1;
  }
  string inputFileName = argv[1];
  string outputFileName = argv[2];
  string algNum = argv[3];


  ifstream infile(inputFileName);

  ofstream outfile;
  outfile.open(outputFileName, ofstream::out | ofstream::trunc);

  int numOfProblems = 0;
  vector<vector<int>> problemWeights; //stores all of the indiviual problem weight vectors
  vector<vector<int>> problemProfits; //stores all of the individual problem profit vectors
  vector<int> problemCapacities; //stores the limits for each problem

  if (infile.is_open()) {
    string line;

    while (!infile.eof()) {
      numOfProblems++;
      getline(infile, line);


      stringstream ss1(line);
      vector<int> individualProblemWeight; //stores each individual problem's weight
      vector<int> individualProblemProfit; //stores each individual problem's profit
      string token;

      getline(ss1, token, ' ');
      int numOfElements = stoi(token);
      getline(ss1, token, ' ');
      problemCapacities.push_back(stoi(token));
      for (int i = 0; i < numOfElements; i++) {
        getline(infile, line);

        stringstream ss2(line);
        string token1;

        getline(ss2, token1, ' ');
        individualProblemWeight.push_back(stoi(token1));
        getline(ss2, token1, ' ');
        if (token1.empty()) {
          getline(ss2, token1, ' ');
        }
        individualProblemProfit.push_back(stoi(token1));

      }
      problemWeights.push_back(individualProblemWeight);
      problemProfits.push_back(individualProblemProfit);
    }
  }
  infile.close();

  switch (stoi(algNum))
  {
    case 0: greedyAlg1(problemWeights, problemProfits, outputFileName, problemCapacities);
            break;
    case 1: greedyAlg2(problemWeights, problemProfits, outputFileName, problemCapacities);
            break;
    case 2: backtracking(problemWeights, problemProfits, outputFileName, problemCapacities);
            break;
  }

}

void printOut(string outputFileName, int size, int maxProfit, double ms) {
  ofstream outfile;
  outfile.open(outputFileName, ofstream::out | ofstream::app);
  if (outfile.is_open()) {
    outfile << size << " " << maxProfit << " " << ms * 1000 << endl;
  }
  outfile.close();
}


void greedyAlg1(vector<vector<int>> weightParam, vector<vector<int>> profitParam, string outputFileName, vector<int> problemCapacities) {
  for (long unsigned int i = 0; i < weightParam.size(); i++) {
    clock_t begin, end;
    begin = clock();
    vector<int> weights = weightParam[i];
    vector<int> profits = profitParam[i];
    int size = weights.size();
    vector<int> profitPerWeight;
    for (int j = 0; j < size; j++) {
      profitPerWeight.push_back(profits[j]/weights[j]);
    }

    for (int j = 0; j < size - 1; j++) {
      int max = j;
      for (int k = j + 1; k < size; k++) {
        if (profitPerWeight[max] < profitPerWeight[k]) {
          max = k;
        }
      }
      int key = profitPerWeight[max];
      int prof = profits[max];
      int weig = weights[max];
      while (max > j) {
        profitPerWeight[max] = profitPerWeight[max - 1];
        profits[max] = profits[max - 1];
        weights[max] = weights[max - 1];
        max--;
      }
      profitPerWeight[j] = key;
      profits[j] = prof;
      weights[j] = weig;
    }


    vector<int> vec;
    int capacity = problemCapacities[i];
    for (int j = 0; j < size; j++) {
      vec.push_back(0);
    }

    int index = 0;
    int weight = 0;
    while (weight < capacity && index <= size) {
      if (weight + weights[index] <= capacity) {
        vec[index] = 1;
        weight += weights[index];
      } else {
        vec[index] = (capacity - weight) / weights[index];
      }
      index++;
    }
    int maxProfit = 0;
    for (int j = 0; j < size; j++) {
      maxProfit += vec[j] * profits[j];
    }
    end = clock();
    double duration = double(end - begin) / double(CLOCKS_PER_SEC);
    printOut(outputFileName, size, maxProfit, duration);
  }
}

vector<int> greedyAlg1Return(vector<int> weights, vector<int> profits, int capacity) {
  int size = weights.size();
  vector<int> profitPerWeight;
  for (int j = 0; j < size; j++) {
    profitPerWeight.push_back(profits[j]/weights[j]);
  }

  for (int j = 0; j < size - 1; j++) {
    int max = j;
    for (int k = j + 1; k < size; k++) {
      if (profitPerWeight[max] < profitPerWeight[k]) {
        max = k;
      }
    }
    int key = profitPerWeight[max];
    int prof = profits[max];
    int weig = weights[max];
    while (max > j) {
      profitPerWeight[max] = profitPerWeight[max - 1];
      profits[max] = profits[max - 1];
      weights[max] = weights[max - 1];
      max--;
    }
    profitPerWeight[j] = key;
    profits[j] = prof;
    weights[j] = weig;
  }


  vector<int> vec;
  for (int j = 0; j < size; j++) {
    vec.push_back(0);
  }

  int index = 0;
  int weight = 0;
  while (weight < capacity && index <= size) {
    if (weight + weights[index] <= capacity) {
      vec[index] = 1;
      weight += weights[index];
    } else {
      vec[index] = (capacity - weight) / weights[index];
    }
    index++;
  }

  return vec;
}

void greedyAlg2(vector<vector<int>> weightParam, vector<vector<int>> profitParam, string outputFileName, vector<int> problemCapacities) {
  for (long unsigned int l = 0; l < weightParam.size(); l++) {
    clock_t begin, end;
    begin = clock();
    vector<int> weights = weightParam[l];
    vector<int> profits = profitParam[l];
    int size = weights.size();
    int capacity = problemCapacities[l];
    vector<int> vec = greedyAlg1Return(weights, profits, capacity);
    int pmax = 0;
    for (int i = 0; i < size; i++) {
      if (weights[i] <= capacity) {
        if (profits[i] > pmax) {
          pmax = profits[i];
        }
      }
    }

    int greedyProf = 0;
    for (int i = 0; i < size; i++) {
      greedyProf += vec[i] * profits[i];
    }

    if (greedyProf > pmax) {
      end = clock();
      double duration = double(end - begin) / double(CLOCKS_PER_SEC);
      printOut(outputFileName, size, greedyProf, duration);
    } else {
      end = clock();
      double duration = double(end - begin) / double(CLOCKS_PER_SEC);
      printOut(outputFileName, size, pmax, duration);
    }
  }
}

vector<int> greedyAlg2Return(vector<int> weights, vector<int> profits, int capacity) {
  int size = weights.size();
  vector<int> vec = greedyAlg1Return(weights, profits, capacity);

  int pmax = 0;
  int index = -1;
  for (int i = 0; i < size; i++) {
    if (weights[i] <= capacity) {
      if (profits[i] > pmax) {
        pmax = profits[i];
        index = i;
      }
    }
  }

  int greedyProf = 0;
  for (int i = 0; i < size; i++) {
    greedyProf += vec[i] * profits[i];
  }

  if (greedyProf > pmax) {
    return vec;
  } else {
    vector<int> returnVec;
    for (int i = 0; i < size; i++) {
      returnVec.push_back(0);
    }
    returnVec[index] = 1;
    return returnVec;
  }
}

int KWF2(int i, int weight, int profit, vector<int> weights, vector<int> profits, int capacity) {
  int bound = profit;
  vector<int> arr;
  int size = weights.size();
  for (long unsigned int j = i; j < weights.size(); j++) {
    arr.push_back(0);
  }
  while (weight < capacity && i < size) {
    if (weight + weights[i] <= capacity) {
      arr[i] = 1;
      weight += weights[i];
      bound += profits[i];
    } else {
      arr[i] = (capacity - weight) / weights[i];
      weight = capacity;
      bound += profits[i] * arr[i];
    }
    i += 1;
  }
  return bound;
}

bool promising(int i, int profit, int weight, vector<int> weights, vector<int> profits, int capacity, int *maxProfit) {
  if (weight >= capacity) {
    return false;
  }
  int bound = KWF2(i + 1, weight, profit, weights, profits, capacity);
  return bound > (*maxProfit);
}

void knapsack(int i, int profit, int weight, vector<int> weights, vector<int> profits, int capacity, vector<int> *bestSet, vector<int> include, int *maxProfit) {
  if (weight <= capacity && profit > (*maxProfit)) {
    (*maxProfit) = profit;
    (*bestSet) = include;
  }
  if (promising(i, profit, weight, weights, profits, capacity, maxProfit)) {
    include[i + 1] = 1;
    knapsack(i + 1, profit + profits[i + 1], weight + weights[i + 1], weights, profits, capacity, bestSet, include, maxProfit);
    include[i + 1] = 0;
    knapsack(i + 1, profit, weight, weights, profits, capacity, bestSet, include, maxProfit);
  }
}



void backtracking(vector<vector<int>> weightParam, vector<vector<int>> profitParam, string outputFileName, vector<int> problemCapacities) {
  for (long unsigned int l = 0; l < weightParam.size(); l++) {
    clock_t begin, end;
    begin = clock();
    vector<int> weights = weightParam[l];
    vector<int> profits = profitParam[l];
    int size = weights.size();
    int capacity = problemCapacities[l];
    vector<int> bestSet = greedyAlg2Return(weights, profits, capacity);

    int maxProfit = 0;
    for (int i = 0; i < size; i++) {
      maxProfit += bestSet[i] * profits[i];
    }
    bestSet.insert(bestSet.begin(), 0);
    profits.insert(profits.begin(), 0);
    weights.insert(weights.begin(), 0);
    vector<int> include;
    for (int i = 0; i < size + 1; i++) {
      include.push_back(0);
    }
    knapsack(0, 0, 0, weights, profits, capacity, &bestSet, include, &maxProfit);
    end = clock();
    double duration = double(end - begin) / double(CLOCKS_PER_SEC);
    printOut(outputFileName, size, maxProfit, duration);

  }
}
