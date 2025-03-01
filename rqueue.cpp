// UMBC - CMSC 341 - Spring 2024 - Proj3
#include "rqueue.h"
RQueue::RQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure) : m_heap(nullptr), m_size(0), m_priorFunc(priFn), m_heapType(heapType), m_structure(structure) {
}


RQueue::~RQueue() {
  clear();
}


void RQueue::clear() {
  clearHelper(m_heap);
  
  m_heap = nullptr;
  m_size = 0;
}


RQueue::RQueue(const RQueue& rhs) {
  m_size = rhs.m_size;
  m_priorFunc = rhs.m_priorFunc;
  m_heapType = rhs.m_heapType;
  m_structure = rhs.m_structure;
  
  m_heap = nullptr;
  
  m_heap = copy(rhs.m_heap);
}


HEAPTYPE RQueue::getHeapType() const {
  return m_heapType;
}


RQueue& RQueue::operator=(const RQueue& rhs) {
  if (this != &rhs) {
    clear();
    
    m_size = rhs.m_size;
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;
    
    m_heap = copy(rhs.m_heap);
  }
  
  return *this;
}


void RQueue::mergeWithQueue(RQueue& rhs) {
  if (m_priorFunc != rhs.m_priorFunc || m_structure != rhs.m_structure || m_heapType != rhs.m_heapType || this == &rhs) {
    throw domain_error("Could not merge.");
  }
  
  m_heap = merge(m_heap, rhs.m_heap);

  rhs.m_heap = nullptr;
  
  //updates and resets the sizes
  m_size += rhs.m_size;
  rhs.m_size = 0;
}


void RQueue::insertStudent(const Student& student) {
  m_size++;
  
  //creates new node to store student information
  Node* newNode = new Node(student);

  m_heap = merge(m_heap, newNode);
}


int RQueue::numStudents() const {
 return numStudentsHelper(m_heap);
}


prifn_t RQueue::getPriorityFn() const {
  return m_priorFunc;
}


Student RQueue::getNextStudent() {
  if (m_heap == nullptr && m_size == 0) {
    throw out_of_range("Queue is empty");
  }
  
  Student student = m_heap->m_student;
  
  m_size--;
  
  Node* old = m_heap;
  
  delete old;

  m_heap = merge(m_heap->m_left, m_heap->m_right);
    
  return student;
}


void RQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
  if (m_priorFunc == priFn) {return;}

  //updates the member variables
  m_priorFunc = priFn;
  m_heapType = heapType;
  
  m_heap = rebuildHeap(m_heap);
}


void RQueue::setStructure(STRUCTURE structure){
  if (m_structure == structure) {return;}
  
  //updates structure
  m_structure = structure;
  
  m_heap = rebuildHeap(m_heap);
}


STRUCTURE RQueue::getStructure() const {
  return m_structure;
}


void RQueue::printStudentsQueue() const {
  printQueueHelper(m_heap);
}


/********************
HELPER FUNCTIONS HERE
*********************/


Node* RQueue::merge(Node* heapNode, Node* newNode) {
  if (heapNode == nullptr) {
    return newNode;
  }
  
  if (newNode == nullptr) {
    return heapNode;
  }

  //swaps the nodes based on the heap type
  if ((m_heapType == MINHEAP && m_priorFunc(newNode->getStudent()) < m_priorFunc(heapNode->getStudent()))
  || (m_heapType == MAXHEAP && m_priorFunc(newNode->getStudent()) > m_priorFunc(heapNode->getStudent()))) {
    swap(heapNode, newNode);
  }
  
  
  if (m_structure == SKEW) { //merges skew heap
    //recursively merges
    heapNode->m_right = merge(heapNode->m_right, newNode);
    
    swap(heapNode->m_left, heapNode->m_right);
    
  } else if (m_structure == LEFTIST) { //merges leftist heap
     heapNode->m_right = merge(heapNode->m_right, newNode);

    //ensures leftist heap is satisfied
    if (heapNode->m_left == nullptr || heapNode->m_left->getNPL() < heapNode->m_right->getNPL()) {
      swap(heapNode->m_left, heapNode->m_right);
    }
    
    //updates npl
    heapNode->setNPL((heapNode->m_right == nullptr) ? 0 : heapNode->m_right->getNPL() + 1);
  }

  return heapNode;
}


Node* RQueue::copy(Node* node) {
  if (node == nullptr) return nullptr;
  
  Node* newNode = new Node(node->getStudent());
  
  newNode->m_left = copy(node->m_left);
  newNode->m_right = copy(node->m_right);
  
  return newNode;
}


Node* RQueue::rebuildHeap(Node* node) {
  if (node == nullptr) {return nullptr;}

  //postorder traversal
  Node* left = rebuildHeap(node->m_left);
  Node* right = rebuildHeap(node->m_right);
  
  node->m_left = node->m_right = nullptr;
  
  if (m_heapType == MAXHEAP) {
    if (left && m_priorFunc(left->getStudent()) > m_priorFunc(node->getStudent())) {
      swap(node, left);
    }
    
    if (right && m_priorFunc(right->getStudent()) > m_priorFunc(node->getStudent())) {
      swap(node, right); 
    }
  }
  
  Node* temp = merge(left, right);
  
  return merge(temp, node);
}


void RQueue::clearHelper(Node* node) {
  if (node == nullptr) {return;}
  
  //post-order removal of nodes
  clearHelper(node->m_left);
  clearHelper(node->m_right);
  
  delete node;
}


void RQueue::printQueueHelper(Node* node) const {
  if (node == nullptr) {return;}

  //pre-order traversal
  cout << "[" << m_priorFunc(node->getStudent()) << "] " << *node << endl;
  
  printQueueHelper(node->m_left);
  printQueueHelper(node->m_right);
}


int RQueue::numStudentsHelper(Node* node) const {
  if (node == nullptr) {return 0;}
  
  return 1 + numStudentsHelper(node->m_left) + numStudentsHelper(node->m_right);
}


void RQueue::dump() const {
  if (m_size == 0) {
    cout << "Empty heap.\n" ;
  } else {
    dump(m_heap);
  }
  cout << endl;
}


void RQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    if (m_structure == SKEW)
        cout << m_priorFunc(pos->m_student) << ":" << pos->m_student.m_name;
    else
        cout << m_priorFunc(pos->m_student) << ":" << pos->m_student.m_name << ":" << pos->m_npl;
    dump(pos->m_right);
    cout << ")";
  }
}


ostream& operator<<(ostream& sout, const Student& student) {
  sout << "Student name: " << student.m_name 
        << ", Major: " << student.getMajorStr()
        << ", Gender: " << student.getGenderStr()
        << ", Level: " << student.getLevelStr();
  return sout;
}


ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.m_student;
  return sout;
}