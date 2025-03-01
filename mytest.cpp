// UMBC - CMSC 341 - Spring 2024 - Proj3
#include "rqueue.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
#include <ctime>     //used to get the current time
using namespace std;
// We can use the Random class to generate the test data randomly!
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};
// Priority functions compute an integer priority for a student.  Internal
// computations may be floating point, but must return an integer.
int priorityFn1(const Student &student);
int priorityFn2(const Student &student);
// a name database for testing purposes
// we can use this DB to generate student names randomly instead of generating
// random strings as names (from Random class)
const int NUMNAMES = 20;
string nameDB[NUMNAMES] = {
    "Ismail Carter", "Lorraine Peters", "Marco Shaffer", "Rebecca Moss",
    "Lachlan Solomon", "Grace Mclaughlin", "Tyrese Pruitt", "Aiza Green", 
    "Addie Greer", "Tatiana Buckley", "Tyler Dunn", "Aliyah Strong", 
    "Alastair Connolly", "Beatrix Acosta", "Camilla Mayo", "Fletcher Beck",
    "Erika Drake", "Libby Russo", "Liam Taylor", "Sofia Stewart"
};
class Tester{
    // implement your test functions in this class
    public:
      bool testNormalMinHeapInsertion();
      bool testNormalMinHeapRemoval();
      bool traverseMin(Node* node, prifn_t priFn);
      bool testNormalMaxHeapInsertion();
      bool testNormalMaxHeapRemoval();
      bool traverseMax(Node* node, prifn_t priFn);
      bool testLeftist();
      bool nplHelper(Node* node);
      bool testPriorFuncChange();
      bool isPriFn(Node* node, prifn_t priFn, RQueue& queue);
      bool testStructureChange();
      bool isStructure(Node* node, prifn_t priFn, RQueue& queue);
      bool testMergeQueueEdge();
      bool traverseMerged(Node* q1, Node* q2);
      bool testMergeQueueError();
      bool testCopyConstructorNormal();
      bool testCopyConstructorEdge();
      bool testAssignOperatorNormal();
      bool testAssignOperatorEdge();
      bool constructorHelper(Node* h1, Node* h2);
      bool testDequeueError();
};
int main(){
  Tester tester;
  
  { //Test insertion for a normal case of min-heap
    cout << "Insertion: Normal case of min-heap" << endl;
    
    if (tester.testNormalMinHeapInsertion()) { 
      cout << "\tTest passed!" << endl;
    } else {
      cout << "\tTest failed!" << endl;
    }
    cout << endl;
  }
  
  { //Test removal for a normal case of min-heap
    cout << "Removal: Normal case of min-heap" << endl;
    
    if (tester.testNormalMinHeapRemoval()) { 
      cout << "\tTest passed!" << endl;
    } else {
      cout << "\tTest failed!" << endl;
    }
    cout << endl;
  }
  
  { //Test insertion and removal for a normal case of max-heap
    cout << "Insertion: Normal case of max-heap" << endl;
    
    if (tester.testNormalMaxHeapInsertion()) { 
      cout << "\tTest passed!" << endl;
    } else {
      cout << "\tTest failed!" << endl;
    }
    cout << endl;
  }
  
  { //Test removal for a normal case of min-heap
    cout << "Removal: Normal case of max-heap" << endl;
    
    if (tester.testNormalMaxHeapRemoval()) { 
      cout << "\tTest passed!" << endl;
    } else {
      cout << "\tTest failed!" << endl;
    }
    cout << endl;
  }

  { //Test for correct NPL values
    cout << "Tests for correct NPL values of leftist heap" << endl;
    
    if (tester.testLeftist()) { 
      cout << "\tTest passed!" << endl;
    } else {
      cout << "\tTest failed!" << endl;
    }
    cout << endl;
  }
  
  { //Test that correct heap is built after changing priority function
    cout << "Correct heap rebuild: priority function" << endl;
    
    if (tester.testPriorFuncChange()) { 
      cout << "\tTest passed!" << endl;
    } else {
      cout << "\tTest failed!" << endl;
    }
    
    cout << endl;
  }
  
  { //Test that correct heap is built after changing structure function
    cout << "Correct heap rebuild: structure function" << endl;
    
    if (tester.testStructureChange()) { 
      cout << "\tTest passed!" << endl;
    } else {
      cout << "\tTest failed!" << endl;
    }
    
    cout << endl;
  }
  
  { //Test merge: edge case (empty queue -> normal queue)
    cout << "Merging two queues: Edge case" << endl;
    
    if (tester.testMergeQueueEdge()) { 
      cout << "\tTest passed!" << endl;
    } else {
      cout << "\tTest failed!" << endl;
    }
    
    cout << endl;
  }
  
  { //Test merge: error case (should throw domain_error exception)
    cout << "Merging two queues: Error case" << endl;
    
    if (tester.testMergeQueueError()) { 
      cout << "\tTest passed!" << endl;
    } else {
      cout << "\tTest failed!" << endl;
    }
    
    cout << endl;
  }
  
  { //Test Copy Constructor: Normal case
    cout << "Copy Constructor: Normal case" << endl;

    if (tester.testCopyConstructorNormal()) { 
      cout << "\tTest passed!" << endl;
    } else {
      cout << "\tTest failed!" << endl;
    }
    
    cout << endl;
  }
  
  { //Test Copy Constructor: Edge case
    cout << "Copy Constructor: Edge case" << endl;

    if (tester.testCopyConstructorEdge()) { 
      cout << "\tTest passed!" << endl;
    } else {
      cout << "\tTest failed!" << endl;
    }
    
    cout << endl;
  }
  
  { //Test Assignment Operator: Normal case
    cout << "Assignment Operator: Normal case" << endl;

    if (tester.testAssignOperatorNormal()) { 
      cout << "\tTest passed!" << endl;
    } else {
      cout << "\tTest failed!" << endl;
    }
    
    cout << endl;
  }
  
  { //Test Assignment Operator: Normal case
    cout << "Assignment Operator: Normal case" << endl;

    if (tester.testAssignOperatorEdge()) { 
      cout << "\tTest passed!" << endl;
    } else {
      cout << "\tTest failed!" << endl;
    }
    
    cout << endl;
  }
  
  { //Test dequeue error, should throw out_of_range exception
    cout << "Dequeue" << endl;

    if (tester.testDequeueError()) { 
      cout << "\tTest passed!" << endl;
    } else {
      cout << "\tTest failed!" << endl;
    }
    
    cout << endl;
  }
}

int priorityFn1(const Student & student) {
    //this function works with a MAXHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [0-10]
    //level + major + group
    //the highest priority would be 3+4+3 = 10
    //the lowest priority would be 0+0+0 = 0
    //the larger value means the higher priority
    int priority = student.getLevel() + student.getMajor() + student.getGroup();
    return priority;
}

int priorityFn2(const Student & student) {
    //this function works with a MINHEAP
    //priority value is determined based on some criteria
    //priority value falls in the range [0-10]
    //race + gender + income + highschool
    //the highest priority would be 0+0+0+0 = 0
    //the lowest priority would be 2+2+4+2 = 10
    //the smaller value means the higher priority
    int priority = student.getRace() + student.getGender() + student.getIncome() + student.getHighschool();
    return priority;
}


/************************
***TEST FUNCTIONS HERE***
************************/


bool Tester::testNormalMinHeapInsertion() {
  Random nameGen(97,122);Random levelGen(0,3);
  Random majorGen(0,4);Random groupGen(0,3);
  Random raceGen(0,2);Random genderGen(0,2);
  Random incomeGen(0,4);Random highschoolGen(0,2);
  RQueue minHeap(priorityFn2, MINHEAP, LEFTIST);
  
  for (int i = 0; i < 300; i++){
    Student student(nameGen.getRandString(5), levelGen.getRandNum(),
        majorGen.getRandNum(), groupGen.getRandNum(),
        raceGen.getRandNum(), genderGen.getRandNum(),
        incomeGen.getRandNum(), highschoolGen.getRandNum());
      minHeap.insertStudent(student);
  }
  
  //test insertion
  prifn_t priFn = minHeap.getPriorityFn();
  
  bool pass = traverseMin(minHeap.m_heap, priFn);
  
  return pass;
}


bool Tester::testNormalMinHeapRemoval() {
  bool pass = false;

  Random nameGen(97,122);Random levelGen(0,3);
  Random majorGen(0,4);Random groupGen(0,3);
  Random raceGen(0,2);Random genderGen(0,2);
  Random incomeGen(0,4);Random highschoolGen(0,2);
  
  RQueue minHeap(priorityFn2, MINHEAP, SKEW);
  
  for (int i = 0; i < 300; i++){
    Student student(nameGen.getRandString(5), levelGen.getRandNum(),
        majorGen.getRandNum(), groupGen.getRandNum(),
        raceGen.getRandNum(), genderGen.getRandNum(),
        incomeGen.getRandNum(), highschoolGen.getRandNum());
      minHeap.insertStudent(student);
  }
  
  //test removal
  int lastPri = MIN;
  
  prifn_t priFn = minHeap.getPriorityFn();
  
  //checks that it is the right student being removed
  while (minHeap.numStudents() > 0) {
    Student curr = minHeap.getNextStudent();
    int currPri = priFn(curr);
    
    if (currPri < lastPri) {
      pass = false;
      break;
    }
    
    lastPri = currPri;
  }

  return pass;
}


bool Tester::traverseMin(Node* node, prifn_t priFn) {
  if (node == nullptr) {return true;}
  
  if (node->m_left != nullptr && priFn(node->m_left->getStudent()) < priFn(node->getStudent())) {
    return false;
  }
  
  if (node->m_right != nullptr && priFn(node->m_right->getStudent()) < priFn(node->getStudent())) {
    return false;
  }
  
  return traverseMin(node->m_left, priFn) && traverseMin(node->m_right, priFn);
}



bool Tester::testNormalMaxHeapInsertion() {
  Random nameGen(97, 122);Random levelGen(0,3);
  Random majorGen(0,4);Random groupGen(0,3);
  Random raceGen(0,2);Random genderGen(0,2);
  Random incomeGen(0,4);Random highschoolGen(0,2);
  RQueue maxHeap(priorityFn1, MAXHEAP, SKEW);
  
  for (int i = 0; i < 300; i++){
    Student student(nameGen.getRandString(5), levelGen.getRandNum(),
        majorGen.getRandNum(), groupGen.getRandNum(),
        raceGen.getRandNum(), genderGen.getRandNum(),
        incomeGen.getRandNum(), highschoolGen.getRandNum());
      maxHeap.insertStudent(student);
  }
  
  //test insertion
  prifn_t priFn = maxHeap.getPriorityFn();
  
  bool pass = traverseMax(maxHeap.m_heap, priFn);

  return pass;  
}


bool Tester::testNormalMaxHeapRemoval() {
  bool pass = false;

  Random nameGen(85,133);Random levelGen(0,3);
  Random majorGen(0,4);Random groupGen(0,3);
  Random raceGen(0,2);Random genderGen(0,2);
  Random incomeGen(0,4);Random highschoolGen(0,2);
  RQueue maxHeap(priorityFn1, MAXHEAP, SKEW);
  
  for (int i = 0; i < 300; i++){
    Student student(nameGen.getRandString(5), levelGen.getRandNum(),
        majorGen.getRandNum(), groupGen.getRandNum(),
        raceGen.getRandNum(), genderGen.getRandNum(),
        incomeGen.getRandNum(), highschoolGen.getRandNum());
      maxHeap.insertStudent(student);
  }
  
  //test removal
  int lastPri = MAX;
  
  prifn_t priFn = maxHeap.getPriorityFn();
  
  //checks that it is the correct student being removed
  while (maxHeap.numStudents() > 0) {
    Student curr = maxHeap.getNextStudent();
    int currPri = priFn(curr);
    
    if (currPri < lastPri) {
      pass = false;
    }
    
    lastPri = currPri;
  }

  return pass;  
}


bool Tester::traverseMax(Node* node, prifn_t priFn) {
  if (node == nullptr) {return true;}
  
  if (node->m_left != nullptr && priFn(node->m_left->getStudent()) > priFn(node->getStudent())) {
    return false;
  }
  
  if (node->m_right != nullptr && priFn(node->m_right->getStudent()) > priFn(node->getStudent())) {
    return false;
  }
  
  return traverseMax(node->m_left, priFn) && traverseMax(node->m_right, priFn);
}


bool Tester::testLeftist() {
  Random nameGen(97, 122);Random levelGen(0,3);
  Random majorGen(0,4);Random groupGen(0,3);
  Random raceGen(0,2);Random genderGen(0,2);
  Random incomeGen(0,4);Random highschoolGen(0,2);
  
  RQueue leftist(priorityFn2, MINHEAP, LEFTIST);
  
  for (int i = 0; i < 20; i++){
    Student student(nameGen.getRandString(5), levelGen.getRandNum(),
        majorGen.getRandNum(), groupGen.getRandNum(),
        raceGen.getRandNum(), genderGen.getRandNum(),
        incomeGen.getRandNum(), highschoolGen.getRandNum());
      leftist.insertStudent(student);
  }
  
  return nplHelper(leftist.m_heap);
  
}


bool Tester::nplHelper(Node* node) {
  if (node == nullptr) {return true;}
  
  //post-order traversal
  bool left = nplHelper(node->m_left);
  bool right = nplHelper(node->m_right);

  //check if NPL property satisfied at current node
  bool curr = (node->m_left == nullptr || node->m_right == nullptr || node->m_left->getNPL() >= node->m_right->getNPL());
  
  return left && right && curr;
}



bool Tester::testPriorFuncChange() {
  Random nameGen(97, 122);Random levelGen(0,3);
  Random majorGen(0,4);Random groupGen(0,3);
  Random raceGen(0,2);Random genderGen(0,2);
  Random incomeGen(0,4);Random highschoolGen(0,2);
  
  RQueue aQueue(priorityFn2, MINHEAP, SKEW);

  for (int i = 0; i < 50; i++){
    Student student(nameGen.getRandString(5), levelGen.getRandNum(),
        majorGen.getRandNum(), groupGen.getRandNum(),
        raceGen.getRandNum(), genderGen.getRandNum(),
        incomeGen.getRandNum(), highschoolGen.getRandNum());
      aQueue.insertStudent(student);
  }
  
  aQueue.setPriorityFn(priorityFn1, MAXHEAP);
  
  prifn_t priFn = aQueue.getPriorityFn();
  
  if (priFn != priorityFn1) {return false;}
  
  return isPriFn(aQueue.m_heap, priFn, aQueue);
}


bool Tester::isPriFn(Node* node, prifn_t priFn, RQueue& queue) {
  if (node == nullptr) {return true;}
  
  //was struggling to figure this one out
  
  return false;
}


bool Tester::testStructureChange() {
  Random nameGen(97, 122);Random levelGen(0,3);
  Random majorGen(0,4);Random groupGen(0,3);
  Random raceGen(0,2);Random genderGen(0,2);
  Random incomeGen(0,4);Random highschoolGen(0,2);
  RQueue aQueue(priorityFn2, MINHEAP, LEFTIST);
 
  for (int i = 0; i < 50; i++){
    Student student(nameGen.getRandString(5), levelGen.getRandNum(),
        majorGen.getRandNum(), groupGen.getRandNum(),
        raceGen.getRandNum(), genderGen.getRandNum(),
        incomeGen.getRandNum(), highschoolGen.getRandNum());
      aQueue.insertStudent(student);
  }
  
  aQueue.setStructure(SKEW);
  
  prifn_t priFn = aQueue.getPriorityFn();
  
  if (aQueue.m_structure == LEFTIST) {return false;}
  
  return isStructure(aQueue.m_heap, priFn, aQueue);
}


bool Tester::isStructure(Node* node, prifn_t priFn, RQueue& queue) {
  if (node == nullptr) {return true;}
  
  if (queue.m_structure == SKEW) {
    if (node->m_left != nullptr && priFn(node->m_left->m_student) < priFn(node->m_student)) {
      return false;
    }
  } else if (queue.m_structure == LEFTIST) {
    if (node->m_right != nullptr && priFn(node->m_right->m_student) < priFn(node->m_student)) {
      return false;
    }
  }
  
  return isStructure(node->m_left, priFn, queue) && isStructure(node->m_right, priFn, queue);
}


bool Tester::testMergeQueueEdge() {
  Random nameGen(97,122);Random levelGen(0,3);
  Random majorGen(0,4);Random groupGen(0,3);
  Random raceGen(0,2);Random genderGen(0,2);
  Random incomeGen(0,4);Random highschoolGen(0,2);
  
  RQueue q1(priorityFn2, MINHEAP, SKEW); //first queue
  
  //populate the first queue
  for (int i = 0; i < 20; i++){
    Student student(nameGen.getRandString(5), levelGen.getRandNum(),
        majorGen.getRandNum(), groupGen.getRandNum(),
        raceGen.getRandNum(), genderGen.getRandNum(),
        incomeGen.getRandNum(), highschoolGen.getRandNum());
      q1.insertStudent(student);
  }
  
  RQueue q2(priorityFn2, MINHEAP, SKEW); //second queue, empty
  
  q2.mergeWithQueue(q1); //merges populated queue with empty queue
  
  return traverseMerged(q1.m_heap, q2.m_heap);
}


bool Tester::traverseMerged(Node* q1, Node* q2) {
  if (q1 == nullptr || q2 == nullptr) {return true;}
  
  bool left = traverseMerged(q1->m_left, q2->m_left);
  bool right = traverseMerged(q1->m_right, q2->m_right);
  
  //checks that current nodes are the same
  bool curr = (q1 == q2);
  
  return left && right && curr;
}


bool Tester::testMergeQueueError() {
  Random nameGen(97, 122);Random levelGen(0,3);
  Random majorGen(0,4);Random groupGen(0,3);
  Random raceGen(0,2);Random genderGen(0,2);
  Random incomeGen(0,4);Random highschoolGen(0,2);
  
  RQueue q1(priorityFn1, MAXHEAP, SKEW); //first queue
  
  //populate the first queue
  for (int i = 0; i < 20; i++){
    Student student(nameGen.getRandString(5), levelGen.getRandNum(),
        majorGen.getRandNum(), groupGen.getRandNum(),
        raceGen.getRandNum(), genderGen.getRandNum(),
        incomeGen.getRandNum(), highschoolGen.getRandNum());
      q1.insertStudent(student);
  }
  
  RQueue q2(priorityFn2, MINHEAP, SKEW); //second queue, different priority fn
  
  //populate the second queue
  for (int i = 0; i < 10; i++){
    Student student(nameGen.getRandString(5), levelGen.getRandNum(),
        majorGen.getRandNum(), groupGen.getRandNum(),
        raceGen.getRandNum(), genderGen.getRandNum(),
        incomeGen.getRandNum(), highschoolGen.getRandNum());
      q2.insertStudent(student);
  }
  
  try {
    q1.mergeWithQueue(q2);
  } catch (domain_error &e) {
    cout << e.what() << endl;
    return true;
  }
  
  return false;
}


bool Tester::testCopyConstructorNormal() {
  Random nameGen(97,122);Random levelGen(0,3);
  Random majorGen(0,4);Random groupGen(0,3);
  Random raceGen(0,2);Random genderGen(0,2);
  Random incomeGen(0,4);Random highschoolGen(0,2);
  
  RQueue q1(priorityFn2, MINHEAP, SKEW);
  
  for (int i = 0; i < 25; i++){
    Student student(nameGen.getRandString(5), levelGen.getRandNum(),
        majorGen.getRandNum(), groupGen.getRandNum(),
        raceGen.getRandNum(), genderGen.getRandNum(),
        incomeGen.getRandNum(), highschoolGen.getRandNum());
      q1.insertStudent(student);
  }
  
  RQueue copy(q1); //copy
  
  if (constructorHelper(q1.m_heap, copy.m_heap)) {
    return true;
  }
  
  return false;
}


bool Tester::testCopyConstructorEdge() {
  RQueue q1(priorityFn1, MAXHEAP, LEFTIST); //empty heap
  
  RQueue copy(q1); //copy
  
  //both heaps should be empty
  if (q1.m_heap == nullptr && copy.m_heap == nullptr) {
    return true;
  }
  
  return false;
}


bool Tester::testAssignOperatorNormal() {
  Random nameGen(97,122);Random levelGen(0,3);
  Random majorGen(0,4);Random groupGen(0,3);
  Random raceGen(0,2);Random genderGen(0,2);
  Random incomeGen(0,4);Random highschoolGen(0,2);
  RQueue q1(priorityFn2, MINHEAP, SKEW);
  
  for (int i = 0; i < 10; i++){
    Student student(nameGen.getRandString(5), levelGen.getRandNum(),
        majorGen.getRandNum(), groupGen.getRandNum(),
        raceGen.getRandNum(), genderGen.getRandNum(),
        incomeGen.getRandNum(), highschoolGen.getRandNum());
      q1.insertStudent(student);
  }
  
  RQueue assign = q1; //assignment
  
  if (constructorHelper(q1.m_heap, assign.m_heap)) {
    return true;
  }
  
  return false;
}


bool Tester::testAssignOperatorEdge() {
  Random nameGen(97, 122);Random levelGen(0,3);
  Random majorGen(0,4);Random groupGen(0,3);
  Random raceGen(0,2);Random genderGen(0,2);
  Random incomeGen(0,4);Random highschoolGen(0,2);
  
  RQueue q1(priorityFn1, MAXHEAP, SKEW); //empty heap
  
  RQueue assign = q1; //assignment
  
  //both heaps should be empty
  if (q1.m_heap == nullptr && assign.m_heap == nullptr) {
    return true;
  }
  
  return false;
}


bool Tester::constructorHelper(Node* h1, Node* h2) {
  if (h1 == nullptr && h2 == nullptr) {
    return true;
  } else if (h1 == nullptr || h2 == nullptr) {
    return false;
  }
  
  //returns true if the nodes in the first tree match up to the copied nodes
  return (h1->getStudent() == h2->getStudent()) && constructorHelper(h1->m_left, h2->m_left) && constructorHelper(h1->m_right, h2->m_right);
}


bool Tester::testDequeueError() {
  Random nameGen(97,122);Random levelGen(0,3);
  Random majorGen(0,4);Random groupGen(0,3);
  Random raceGen(0,2);Random genderGen(0,2);
  Random incomeGen(0,4);Random highschoolGen(0,2);
  RQueue aQueue(priorityFn2, MINHEAP, SKEW);
  
  for (int i = 0; i < 8; i++){
    Student student(nameGen.getRandString(5), levelGen.getRandNum(),
        majorGen.getRandNum(), groupGen.getRandNum(),
        raceGen.getRandNum(), genderGen.getRandNum(),
        incomeGen.getRandNum(), highschoolGen.getRandNum());
      aQueue.insertStudent(student);
  }
  
  try {
    for (int i = 0; i < 9; i++) {
      Student temp = aQueue.getNextStudent();
    }
  } catch (out_of_range &e) {
    cout << e.what() << endl;
    return true; //test passed
  }
  
  return false;
}