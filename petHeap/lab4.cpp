// Benjamin R Spitzauer
// lab4.cpp
// 2/14/2023

/*

 Purpose: Lab orientates around the understanding of heaps and their invariants
 it is here to teach basic functions of the ADT like percolate up and down;
 as well as public accessor functions which allow the user to see the state of
 the heap. This could mean seeing the size (amount of pets) of the heap, 
 the actual pets inside the heap and their prioitization. As well as 
 means to add to the heap a new animal.

*/

#include <vector> 
#include <string> 
#include <iostream> 
#include <random>

using namespace std; 


// ~~~~~~~~~~~~~~~~~~~~~Prototypes~~~~~~~~~~~~~~~~~~~~~

/*

  Pre-Condition: Pet's name and age.
  Post-Condition: A container for the traits of an animal which is used in
  PetHeap's class as the main key of the heap.

*/
struct Pet;

/*

  Pre-Condition: None.
  Post-Condition: Will act as if it's a heap that contains Pets of varying ages
  and names. It contains a vector<Pet*> container which allows for easy
  accession of elements in heap and removal. This vector can be added to from
  the user in its public functions; but can only remove the first element which
  should have the invariant of always being the highest prioritied Pet in the
  shelter based on a max-heap property.

*/
class PetHeap;

// ~~~~~~~~~~~~~~~~~END Prototypes~~~~~~~~~~~~~~~~~~~~~


struct Pet
{  

  string name;  
  int age;  

  Pet(string petName, int petAge)
  {
    this->name = petName;
    this->age = petAge;
  }

}; 
 
class PetHeap 
{ 

  private:           

    vector<Pet*> petHeap;

    /*

      Pre-Condition: Index of element in heap.
      Post-Condition: Will place the value inside the index's element into its
      parent node if it's greater than its parent (max-heap). It will then call
      itself again to check oncemore at this new index if it's in the right
      spot in the heap according to its priority.

    */
    void percolateUp(int index); 

    /*

      Pre-Condition: Index of element in heap.
      Post-Condition: Will place the value inside the index's element into its
      greatest prioritied child node if it's greater than its child (max-heap).
      It will then call itself again to check oncemore at this new index if 
      it's in the right spot in the heap according to its priority.

    */
    void percolateDown(int index); 

    /*

      Pre-Condition: None.
      Post-Condition: Removes root element and swaps it with the last element
      in the heap. This element is then checked with percolate down to make
      sure it's added in the right spot of the heap to maintain the heap
      property.

    */
    void deleteMax(); 

    /*

      Pre-Condition: None.
      Post-Condition: Returns index 0 in the vector which should be highest
      prioritied pet and returns it as a pointer. Else, return nullptr if
      it's empty.

    */
    Pet* peekMax(); 

    /*

      Pre-Condition: Index of child to swap; index of parent to swap w/ child.
      Post-Condition: Will swap the Nodes positions with one another.
      Invariant: Should always be indexes in heap.

    */
    void swap(int childIndex, int parentIndex);

  public: 
    PetHeap()
    {  
      petHeap = vector<Pet*>(); // Init heap container
    } 

    /*

      Pre-Condition: A pointer to a allocated pet to add into the heap.
      Post-Condition: Inserts pet into heap at end and calls percolate
      up to find its spot in the heap.

    */
    void insert(Pet* pet); 

    /*

      Pre-Condition: None.
      Post-Condition: Returns pointer to the oldest pet in the shelter and
      removes them from heap. If the heap is empty, it will return nullptr.
      This returned Pet* pet is not freed from memory though.

    */
    Pet* adoptOldestPet(); 

    /*

      Pre-Condition: None.
      Post-Condition: Returns number of pets in the shelter's heap.

    */
    int numPets(); 

    /*

      Pre-Condition: None.
      Post-Condition: Displays for user each pet in the heap in a level-order
      traversal manner.

    */
    void displayPets(); 

}; 

 
void PetHeap::swap(int childIndex, int parentIndex)
{

  Pet* tmp = this->petHeap[parentIndex];

  this->petHeap[parentIndex] = this->petHeap[childIndex];
  this->petHeap[childIndex]= tmp;

}


void PetHeap::percolateUp(int index)
{ 
  if(index == 0){
    return;
  }

  int parentIndex = (index-1)/2;

  if(this->petHeap[index]->age > this->petHeap[parentIndex]->age){ // If child is greater than parent
    this->swap(index, parentIndex);
  }
  else{ // If in it's correct spot, stop
    return;
  }

  percolateUp(parentIndex); // Now check this spot, and it's parent

} 

 
void PetHeap::percolateDown(int index)
{ 

  int leftChildIndex = (index*2)+1;
  int rightChildIndex = (index*2)+2;
  int swapChildIndex; // Contains index of child for next recursive call

  if(((leftChildIndex) >= ((int)this->petHeap.size())) && ((rightChildIndex) >= ((int)this->petHeap.size()))){ // Leaf
    return;
  }
  else if(((rightChildIndex) >= ((int)this->petHeap.size()))){ // One left child

    if(this->petHeap[index]->age < this->petHeap[leftChildIndex]->age){

      this->swap(index, leftChildIndex);
      swapChildIndex = leftChildIndex; // Set for recursive call

    }
    else{ // Else in correct spot
      return;
    }
  }
  else{ // Two children
  
    // Get greatest child index
    if(this->petHeap[leftChildIndex]->age >= this->petHeap[rightChildIndex]->age){
      swapChildIndex = leftChildIndex;
    }
    else{
      swapChildIndex = rightChildIndex;
    }

    if(this->petHeap[index]->age < this->petHeap[swapChildIndex]->age){
      swap(index, swapChildIndex);
    }
    else{ // Else node is in correct spot
      return;
    }
  }

  percolateDown(swapChildIndex); // Now check new spot and its children

}

 
void PetHeap::deleteMax()
{ 

  this->swap(0, this->petHeap.size()-1);

  this->petHeap.pop_back(); // Remove max

  // Check new root if right spot in heap
  this->percolateDown(0);

} 

 
Pet* PetHeap::peekMax()
{ 

  if(((int)this->petHeap.size()) == 0){ // If empty
    return nullptr;
  }

  return this->petHeap[0];

} 

 
void PetHeap::insert(Pet* pet)
{ 
    
  this->petHeap.push_back(pet); // Add to end

  // Check element's parent if just in heap (size-1 as index starts at 0)
  this->percolateUp(this->petHeap.size()-1);

} 

 
Pet* PetHeap::adoptOldestPet()
{ 

  if(((int)this->petHeap.size()) == 0){
    cout << "Sorry the shelter is currently empty.\n";
    return nullptr;
  }

  Pet* oldest = this->peekMax();

  this->deleteMax(); // Now remove max

  return oldest; 

} 

 
int PetHeap::numPets()
{ 

  return this->petHeap.size(); // Thank you std::vector :)

} 

 
void PetHeap::displayPets()
{ 


  if(((int)this->petHeap.size()) == 0){
    cout << "Sorry the shelter is currently empty.\n";
    return;
  }

  cout << "Displaying pets in heap of shelter in level-order: \n";

  for(int i = 0; i < ((int)this->petHeap.size()); i++){

    cout << this->petHeap[i]->name << " age "
      << this->petHeap[i]->age << ", ";


  }

  cout << '\n';

} 

/*

  Pre-Condition: None.
  Post-Condition: Tests functionality of PetHeap class. This is done by adding
  12 pets to the shelter, displaying the heap, removing 6 pets from the heap,
  displaying the heap after and the size, as well as adopting the rest in the
  shelter, and finally adopting when no more animals are left in the heap. Also
  will test duplicates and how they are treated in the heap.

*/
void runTests()
{

  PetHeap shelter;

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  cout << "\nAdding 12 pets of various ages...\n";

  cout << "Adding Qambo, age 15.\n";
  shelter.insert(new Pet("Qambo", 15));

  cout << "Adding Fambo, age 1.\n";
  shelter.insert(new Pet("Fambo", 1));

  cout << "Adding Tambo, age 5.\n";
  shelter.insert(new Pet("Tambo", 5));

  cout << "Adding Lambo, age 75.\n";
  shelter.insert(new Pet("Lambo", 75));

  cout << "Adding Sambo, age 5.\n";
  shelter.insert(new Pet("Sambo", 5));

  cout << "Adding Pambo, age 45.\n";
  shelter.insert(new Pet("Pambo", 45));

  cout << "Adding Iambo, age 25.\n";
  shelter.insert(new Pet("Iambo", 25));

  cout << "Adding Aambo, age 63.\n";
  shelter.insert(new Pet("Aambo", 63));

  cout << "Adding Rambo, age 23.\n";
  shelter.insert(new Pet("Rambo", 23));

  cout << "Adding Wambo, age 11.\n";
  shelter.insert(new Pet("Wambo", 11));

  cout << "Adding Zambo, age 19.\n";
  shelter.insert(new Pet("Zambo", 19));

  cout << "Adding Xambo, age 14.\n";
  shelter.insert(new Pet("Xambo", 14));

  cout << "Done adding 12 pets!\n\n";

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  shelter.displayPets();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  cout << "\nAdopting 6 pets (should result in a size 6 heap): \n";

  // Hold current pet state
  Pet* currentPet = nullptr;

  for(int i = 0; i < 6; i++){
  
    currentPet = shelter.adoptOldestPet();

    cout << "Pet adopted is " << currentPet->name << " with age " << currentPet->age << '\n';

    delete currentPet;
  
  }

  currentPet = nullptr; // Set back to null as will be pointing to freed space

  cout << "Done adopting 6 pets.\n\n";

  cout << "Shelter's heap size is: " << shelter.numPets() << ".\n\n";

  shelter.displayPets();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  cout << "\nRemoving rest of pets...\n";

  for(int i = 0; i < 6; i++){
  
    currentPet = shelter.adoptOldestPet();

    cout << "Pet adopted is " << currentPet->name << " with age " << currentPet->age << '\n';

    delete currentPet;
  
  }

  cout << "Done adopting all pets.\n\n";

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  cout << "Attempting to adopt when 0 animals are left to adopt"
  << " (empty message should occur 6 times):\n";

  for(int i = 0; i < 6; i++){

    shelter.adoptOldestPet();  

  }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  cout << "\nInserting duplicate aged pets (age 50) with a heap root of 100 "
  << "and children of 49 (expect one age 50 to swap spaces with one 49)"
  << " and the other stays at end of ordering (wont / doesn't need to swap).";

  shelter.insert(new Pet("Qambo", 100));

  shelter.insert(new Pet("Qambo", 49));
  shelter.insert(new Pet("Qambo", 49));

  shelter.insert(new Pet("Qambo", 50));
  shelter.insert(new Pet("Qambo", 50));

  cout << "\n\nDone adding duplicates\n\n";

  shelter.displayPets();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  cout << "\nDeleting max; should bring both 50's to front as last will become "
  << "first and percolate down will keep its spot as root:\n";

  delete shelter.adoptOldestPet();

  shelter.displayPets();

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // Free open spots left
  while(shelter.numPets() != 0){
    delete shelter.adoptOldestPet();
  }

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

}

/*

  Pre-Condition: None.
  Post-Condition: Runs a random public functions of the PetHeap class 50 times.
  If it is to insert, it will add a random aged pet from 0-2000 in age.

*/
void runRandomTests()
{

  std::random_device rand;   // Making seeded gen
  std::mt19937 seed(rand()); // Seeding with quicker gen with seed
  std::uniform_int_distribution<int> randDegree(1, 4); // Range of values
  std::uniform_int_distribution<int> randAge(0, 2000); // Range of values

  PetHeap* shelter = new PetHeap();

  for(int i = 0; i < 50; i++){

    switch(randDegree(seed)){


      case 1:
        cout << "\n\nInserted pet...\n\n";
        shelter->insert(new Pet("Bombas", randAge(seed)));
        break;
      case 2:
      {
        Pet* pet = shelter->adoptOldestPet();

        if(!pet){ // No pets to adopt
          break;
        }

        cout << "\nPet adopted is " << pet->name << " with age " << pet->age << '\n';

        delete pet; // Free memory

        break;
      }
      case 3:
        cout << "\nThere are " << shelter->numPets() << " pet(s) in the shelter.\n"; 
        break;
      case 4:
        shelter->displayPets();
        break;
    }

  }

  // Free open spots left by user
  while(shelter->numPets() != 0){
    delete shelter->adoptOldestPet();
  }

  delete shelter;


}


/*

  Pre-Condition: PetHeap* to be used by user.
  Post-Condition: Asks user for input of 1-7 which corresponds to different
  functions of the PetHeap DT. It will return a boolean to be used in the
  outer main while loop to help respond to when the user wants to leave the
  interface of the given shelter.

*/
bool request(PetHeap*& shelter)
{

  int userResponse;

  std::cout << "\nWelcome to the ShelterBST heap (lab 3),"
  << " What would you like to do?\n"
  << "1. Insert a Pet\n"
  << "2. Adopt a Pet\n"
  << "3. Get number of pets in shelter\n"
  << "4. Display all pets in shelter\n"
  << "5. Run Tests\n"
  << "6. Run Random Tests\n"
  << "7. Exit\n"
  << "Reponse (1-7): ";

  std::cin >> userResponse;

  switch(userResponse){


    case 1:
    { 
      std::string petsName;
      std::cout << "Whats the name of the Pet and Age? ";
      std::cin >> petsName >> userResponse;
      shelter->insert(new Pet(petsName, userResponse));
      break;
    }
    case 2:
    {
      Pet* pet = shelter->adoptOldestPet();

      if(!pet){ // No pets to adopt
        break;
      }

      cout << "Pet adopted is " << pet->name << " with age " << pet->age << '\n';

      delete pet; // Free memory

      break;
    }
    case 3:
      cout << "There are " << shelter->numPets() << " pet(s) in the shelter.\n"; 
      break;
    case 4:
      shelter->displayPets();
      break;
    case 5:
      runTests();
      break;
    case 6:
      runRandomTests();
      break;
    case 7:
      cout << "\n\n"; // Clear from sys prompts
      return false; // Exit
  }

  return true;

};


int main()
{


  PetHeap* userShelter = new PetHeap();
  bool loopFlag = true;

  while(loopFlag){
  
    loopFlag = request(userShelter);
  
  }

  // Free open spots left by user
  while(userShelter->numPets() != 0){
    delete userShelter->adoptOldestPet();
  }

  delete userShelter;

}