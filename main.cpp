#include <bits/stdc++.h>
using namespace std;

// map to store the no of params required for executed command
map<string, int> commandToParamsMap;

// Template for storing registration number and age
class PersonDetails {
public:
  string registrationNumber;
  int age;
  // constructors
  PersonDetails() {}
  PersonDetails(int age, string registrationNumber) {
    this->registrationNumber = registrationNumber;
    this->age = age;
  }

  // getter functions
  string getRegistrationNumber() {
    return this->registrationNumber;
  }
  
  int getAge() {
    return this->age;
  }
};

// Template to store parking lot details - It store details of parked vechicle in each slot.
class ParkingLot {
  public:
  vector<PersonDetails*> slots;
  int capacity;
  int filledSlots;

  // constructors
  ParkingLot() {}
  ParkingLot(int capacity) {
    this->capacity = capacity;
    for(int i=0; i<capacity; i++) {
      this->slots.push_back(NULL);
    }
    filledSlots = 0;
  }

  // function to assign an empty slot to a person, it return the slot position if alloted else return -1
  int occupySlot(PersonDetails* person) {
    if (this->capacity > this->filledSlots) {
      for(int i=0; i<this->capacity; i++) {
        if(!this->slots[i]) {
          this->slots[i] = person;
          this->filledSlots += 1;
          return i+1;
        }
      }
    }
    return -1;
  }

  // function to free the slot occupied, returns the details of vehicle
  PersonDetails* freeSlot(int slotNo) {
    if(this->filledSlots == 0) {
      cout<<"Parking Lot is empty";
      return NULL;
    }

    if(slotNo < 1 || slotNo > this->capacity) {
      cout<<"Invalid Slot Position entered.";
      return NULL;
    }

    if(!slots[slotNo-1]) {
      cout<<"Slot is empty.";
      return NULL;
    }

    PersonDetails* person = this->slots[slotNo-1];
    this->slots[slotNo-1] = NULL;
    this->filledSlots -= 1;
    return person;
  }

  // function to get the capacity of parking lot
  int getCapacity() {
    return this->capacity;
  }

  // function to get the slots of all vehicles occupied by a particular age
  vector<int> getSlotsByAge(int age) {
    vector<int> result;
    for(int i=0; i<this->capacity; i++) {
      if(this->slots[i] && this->slots[i]->getAge() == age) {
        result.push_back(i+1);
      }
    }
    return result;
  }

  // function to get the registration number of all vehicles by a particular age
  vector<string> getRegsistrationNumbersByAge(int age) {
    vector<string> result;
    for(int i=0; i<this->capacity; i++) {
      if(this->slots[i] && this->slots[i]->getAge() == age) {
        result.push_back(this->slots[i]->getRegistrationNumber());
      }
    }
    return result;
  }

  // function to get the slot occupied by vehicle registartion number
  int getSlotByRegistrationNumber(string resgistrationNumber) {
    for(int i=0; i<this->capacity; i++) {
      if(this->slots[i] && this->slots[i]->getRegistrationNumber() == resgistrationNumber) {
        return i+1;
      }
    }
    return -1;
  }
};

// Template to manage the parking lot
class ParkingHandler: public ParkingLot, public PersonDetails {
  public:
  // constructor
  ParkingHandler(int capacity) : ParkingLot(capacity){}

  // function to park a vehicle and return its slot position
  int parkVehicle(PersonDetails* person) {
    return ParkingLot::occupySlot(person);
  }

  // function to free a occupied slot and return vehicle details
  PersonDetails* freeParking(int slotNo) {
    return ParkingLot::freeSlot(slotNo);
  }

  // function to get all slots occupied by persons of particular age
  vector<int> getSlotNumbersForAge(int age) {
    return ParkingLot::getSlotsByAge(age);
  }

  // function to get registration number of all vehicles occupied by persons of particular age
  vector<string> getRegistrationNumberForAge(int age) {
    return ParkingLot::getRegsistrationNumbersByAge(age);
  }

  // function to get all slots occupied by a particular vehicle
  int getSlotForRegistrationNumber(string registrationNumber) {
    return ParkingLot::getSlotByRegistrationNumber(registrationNumber);
  }
  
};

ParkingHandler* parkingHandlerObj = NULL;

// function to split params by space
vector<string> getParams(string command) {
  istringstream ss(command);
  string param;
  vector<string> params;
  while (ss >> param) {
    params.push_back(param);
  }
  return params;
}

// function to handle command according to the type
void commandParser(string command) {
  vector<string> params = getParams(command);
  string command_type = params[0];
  transform(command_type.begin(), command_type.end(), command_type.begin(), ::toupper);
  // if the requested command is not found, error message is printed
  if(commandToParamsMap.find(command_type) == commandToParamsMap.end()) {
    cout<<"Command Not allowed!"<<endl;
  }
  // if the no of required params do not match, error message is printed
  if(commandToParamsMap[command_type] != params.size()) {
    cout<<"Invalid number of parameters passed"<<endl;
    cout<<"----------------------------------------------------------------------"<<endl;
    return;
  }
  // if the parking lot is not instantiated, error message is printed
  if(command_type != "CREATE_PARKING_LOT" && parkingHandlerObj == NULL) {
    cout<<"No parking lot exists, first create a parking lot."<<endl;
    cout<<"----------------------------------------------------------------------"<<endl;
    return;
  }

  if (command_type == "CREATE_PARKING_LOT") {
    int slots = stoi(params[1]);
    parkingHandlerObj = new ParkingHandler(slots);
    cout<<"Created parking lot with " + params[1] + " slots"<<endl;
  }
  else if(command_type == "PARK") {
    PersonDetails* person = new PersonDetails(stoi(params[3]), params[1]);
    int slot = parkingHandlerObj->parkVehicle(person);
    cout<<"Parked "<<params[1]<<" on slot number "<<slot<<endl;
  }
  else if(command_type == "LEAVE") {
    PersonDetails* person = parkingHandlerObj->freeParking(stoi(params[1]));
    if(person) {
        cout<<person->getRegistrationNumber()<<" left the parking slot number "<<params[1]<<endl;
    }
  }
  else if(command_type == "SLOT_NUMBER_FOR_CAR_WITH_NUMBER") {
    int slot = parkingHandlerObj->getSlotForRegistrationNumber(params[1]);
    if(slot == -1) {
      cout<<"No car with registration number "<<params[1]<<" found"<<endl;
    }
    else{
      cout<<"Car with registration number "<<params[1]<<" parked on slot number "<<slot<<endl;
    }
  }
  else if(command_type == "SLOT_NUMBERS_FOR_DRIVER_OF_AGE") {
    vector<int> slotNumbers = parkingHandlerObj->getSlotNumbersForAge(stoi(params[1]));
    if(slotNumbers.size() == 0) {
      cout<<"No slots found for drivers with age "<<params[1]<<endl;
    }
    else {
      cout<<"Slots occupied by drivers with age "<<params[1]<<": ";
      for(int i=0; i<slotNumbers.size(); i++) {
        if(i == slotNumbers.size()-1) cout<<slotNumbers[i];
        else cout<<slotNumbers[i]<<", ";
      }
      cout<<endl;
    }
  }
  else if(command_type == "VEHICLE_REGISTRATION_NUMBER_FOR_DRIVER_OF_AGE") {
    vector<string> registrationNumbers = parkingHandlerObj->getRegistrationNumberForAge(stoi(params[1]));
    if(registrationNumbers.size() == 0) {
      cout<<"No vehicle found for drivers with age "<<params[1]<<endl;
    }
    else {
      cout<<"Registration Numbers of vehicle occupied by drivers with age "<<params[1]<<": ";
      for(int i=0; i<registrationNumbers.size(); i++) {
        if(i == registrationNumbers.size()-1) cout<<registrationNumbers[i];
        else cout<<registrationNumbers[i]<<", ";
      }
      cout<<endl;
    }
  }
  cout<<"----------------------------------------------------------------------"<<endl;
}

// main function
int main() {
    // setting the size of required params for each command
    commandToParamsMap["CREATE_PARKING_LOT"] = 2;
    commandToParamsMap["PARK"] = 4;
    commandToParamsMap["SLOT_NUMBERS_FOR_DRIVER_OF_AGE"] = 2;
    commandToParamsMap["SLOT_NUMBER_FOR_CAR_WITH_NUMBER"] = 2;
    commandToParamsMap["LEAVE"] = 2;
    commandToParamsMap["VEHICLE_REGISTRATION_NUMBER_FOR_DRIVER_OF_AGE"] = 2;

    // read the input file
    string filename("input.txt");
    string line;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr <<"Could not open the input file."<< endl;
        return -1;
    }

    // parse each command
    while (getline(input_file, line)){
      commandParser(line);
    }

    // close the input file
    input_file.close();
    return 1;
}