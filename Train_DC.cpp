/*
    Train_DC.h - Library for running a DC train.
    Created by benfpv, 09/25/2023.
    Released into the public domain.
*/

#include "Arduino.h"
#include "Train_DC.h"
#include "funcs_arduino.h"

Train_DC::Train_DC(String trainName, int trainIndex, int trainNavigationType, int pin_EN, int pin_IN1, int pin_IN2, int targetIdleTime, int motorSpeedSlow, int motorSpeedMax, int motorAccelerationStopping, int motorAccelerationMax)
{   
    // Attributes - Train Identity
    _trainName = trainName;
    _trainIndex = trainIndex;
    _trainNavigationType = trainNavigationType;
    // Pin Inputs
    _pin_EN = pin_EN;
    _pin_IN1 = pin_IN1;
    _pin_IN2 = pin_IN2;
    // Attributes - Train User Input Characteristics
    _targetIdleTime = targetIdleTime;
    _motorSpeedSlow = motorSpeedSlow;
    _motorSpeedMax = motorSpeedMax;
    _motorAccelerationStopping = motorAccelerationStopping;
    _motorAccelerationMax = motorAccelerationMax;
}

void Train_DC::begin()
{
    pinMode(_pin_EN, OUTPUT);
    pinMode(_pin_IN1, OUTPUT);
    pinMode(_pin_IN2, OUTPUT);
}

void Train_DC::init_target_rail_section_queue(int numberOfTargetRailSectionsInQueue, String targetRailSectionsNamesQueue[]) {
  _numberOfTargetRailSectionsInQueue = numberOfTargetRailSectionsInQueue;
  // Serial.print("_numberOfTargetRailSectionsInQueue: "); Serial.println(_numberOfTargetRailSectionsInQueue);
  
  set_string_array_value(10, _targetRailSectionsNamesQueue, "NONE");
  for (int i = 0; i < _numberOfTargetRailSectionsInQueue; i++) {
    _targetRailSectionsNamesQueue[i] = targetRailSectionsNamesQueue[i];
  }

  // Serial.print("_targetRailSectionNamesQueue: ");
  // print_array_strings(_numberOfTargetRailSectionsInQueue, _targetRailSectionsNamesQueue);
}

void Train_DC::clear_current_localization_attributes()
{
  // Attributes - Rail Localization - Current
  _currentRailSectionName = "NONE";
  // _currentRailSectionIndex = -1;
  _currentRailSectionIndexInPlanned = -1;
  _currentRailSectionSwitch = 'N';
}

void Train_DC::clear_target_planned_localization_attributes()
{
  // Attributes - Rail Localization - Target (Must Reflect Present Truth)
  _targetRailSectionName = "NONE";
  // _targetRailSectionIndex = -1;
  // Attributes - Rail Localization - Planned
  _numberOfPlannedRailSections = 0;
  set_string_array_value(10, _plannedRailSectionsNames, "NONE");
  // set_int_array_value(10, _plannedRailSectionsDirIndices, -1);
  // set_int_array_value(10, _plannedRailSectionsNameIndices, -1);
  set_int_array_value(10, _plannedRailSectionsSwitches, 0);
}

void Train_DC::clear_subordinate_superordinate_localization_attributes()
{
  // Attributes - Rail Localization - Subordinates and Superordinates
  _numberOfSubordinateRailSections = 0;
  set_string_array_value(3, _subordinateRailSectionsNames, "NONE");
  // set_int_array_value(3, _subordinateRailSectionsDirIndices, -1);
  // set_int_array_value(3, _subordinateRailSectionsNameIndices, -1);
  set_int_array_value(3, _subordinateRailSectionsSwitches, 0);
  _numberOfSuperordinateRailSections = 0;
  set_string_array_value(3, _superordinateRailSectionsNames, "NONE");
  // set_int_array_value(3, _superordinateRailSectionsDirIndices, -1);
  // set_int_array_value(3, _superordinateRailSectionsNameIndices, -1);
  set_int_array_value(3, _superordinateRailSectionsSwitches, 0);
}

void Train_DC::print_localization() {
  Serial.print("["); Serial.print(_trainName); Serial.print("] ");
  Serial.print(_currentRailStationName); Serial.print(" | "); Serial.print(_currentRailSectionName); Serial.print("->"); Serial.println(_targetRailSectionName);
  // Serial.print("[Subs] ");
  // print_array_strings(_numberOfSubordinateRailSections, _subordinateRailSectionsNames);
  // Serial.print("[Supers] ");
  // print_array_strings(_numberOfSuperordinateRailSections, _superordinateRailSectionsNames);
  Serial.print("[Plan] "); print_array_strings(_numberOfPlannedRailSections, _plannedRailSectionsNames);
  Serial.print("[R]"); Serial.print(_motorReverser); Serial.print("/"); Serial.print(_plannedDirection); Serial.print(" [V]"); Serial.print(_motorSpeed); Serial.print("/"); Serial.print(_motorAccelerationTarget); Serial.print("/"); Serial.print(_motorSpeedTarget); 
  Serial.print(" [X]");
  if (_idleStatus == true) {
    Serial.print("I ");
  }
  if (_readyToGenerateTarget == true) {
    Serial.print("P1 ");
  }
  if (_readyToPathfindTarget == true) {
    Serial.print("P2 ");
  }
  if (_readyToMoveStatus == true) {
    Serial.print("M ");
  }
  if (_emergencyStopStatus == true) {
    Serial.print("EMER ");
  }
  if (_switchDirectionStatus == true) {
    Serial.print("SWD ");
  }
  if (_arriveStation == true) {
    Serial.print("ARR ");
  }
  if (_departStation == true) {
    Serial.print("DEP ");
  }
  Serial.println("");
}

void Train_DC::set_starting_rail_section(int numberOfRailSections, String allRailSectionsNames[], int numberOfRailStations, String allRailStationsNames[], String currentRailSectionName) {
  // Serial.print("currentRailSectionName: ");
  // Serial.println(currentRailSectionName);
  _currentRailSectionName = currentRailSectionName;
  // _currentRailSectionIndex = get_index_of_string(numberOfRailSections, allRailSectionsNames, currentRailSectionName);
  int thisRailStationIndex = get_index_of_string(numberOfRailStations, allRailStationsNames, currentRailSectionName);
  if (thisRailStationIndex != -1) {
    _currentRailStationName = currentRailSectionName;
  }
}

void Train_DC::set_target_rail_section(int numberOfRailSections, String allRailSectionsNames[], String targetRailSectionName) {
    _targetRailSectionName = targetRailSectionName;
    // _targetRailSectionIndex = get_index_of_string(numberOfRailSections, allRailSectionsNames, targetRailSectionName);
}

void Train_DC::set_current_rail_section(int numberOfRailSections, String allRailSectionsNames[], String currentRailSectionName) {
  // Serial.print("currentRailSectionName: ");
  // Serial.println(currentRailSectionName);
  _currentRailSectionName = currentRailSectionName;
  // _currentRailSectionIndex = get_index_of_string(numberOfRailSections, allRailSectionsNames, currentRailSectionName);
}

void Train_DC::set_target_rail_section_randomly(int numberOfRailSections, String allRailSectionsNames[], int numberOfRailStations, String allRailStationsNames[], int direction) {
  if (_readyToGenerateTarget == true) {
    bool targetContainsStation = false;
    int currentRailSectionIndex = get_index_of_string(numberOfRailSections, allRailSectionsNames, _currentRailSectionName);
    int targetRailSectionIndex;
    while (_targetRailSectionName.equals(_currentRailSectionName) || targetContainsStation == false) {
      targetRailSectionIndex = get_random_target_index(numberOfRailSections, currentRailSectionIndex, direction);
      _targetRailSectionName = allRailSectionsNames[targetRailSectionIndex];
      for (int i = 0; i < numberOfRailStations; i++) {
        if (_targetRailSectionName.equals(allRailStationsNames[i])) {
          targetContainsStation = true; // Break loop if target section has a station
        }
      }
    }
    if (_targetRailSectionName != "NONE") {
      _readyToGenerateTarget = false;
      _readyToPathfindTarget = true;
    }
  }
}

void Train_DC::set_target_rail_section_by_queue(int numberOfRailStations, String allRailStationsNames[]) {
  if (_readyToGenerateTarget == true) {
    bool targetContainsStation = false;
    while (_targetRailSectionName.equals(_currentRailSectionName) || targetContainsStation == false) {
      if (_targetRailSectionIndexInQueue >= _numberOfTargetRailSectionsInQueue) {
        _targetRailSectionIndexInQueue = 0;
      }
      _targetRailSectionName = _targetRailSectionsNamesQueue[_targetRailSectionIndexInQueue];
      // Serial.print("- Candidate _targetRailSectionName: "); Serial.println(_targetRailSectionName);
      for (int i = 0; i < numberOfRailStations; i++) {
        if (_targetRailSectionName.equals(allRailStationsNames[i])) {
          targetContainsStation = true; // Break loop if target section has a station
        }
      }
      _targetRailSectionIndexInQueue += 1;
    }
    if (_targetRailSectionName != "NONE") {
      _readyToGenerateTarget = false;
      _readyToPathfindTarget = true;
    }
  }
}

void Train_DC::set_pathfind_current_to_target(int numberOfRailSections, String allRailSectionsDirs[][10], String allRailSectionsNames[], bool printDebug) {
  if (_readyToPathfindTarget == true) {
    if (printDebug == true) {
      Serial.println("set_pathfind_current_to_target()");
    }
    // Pathfind To Target - Get Relevant Dir & Name Indices
    int currentDirIndices[numberOfRailSections];
    set_int_array_value(numberOfRailSections, currentDirIndices, -1);
    int currentNameIndices[numberOfRailSections];
    set_int_array_value(numberOfRailSections, currentNameIndices, -1);
    get_single_relevant_dir_name_indices(numberOfRailSections, allRailSectionsDirs, _currentRailSectionName, currentDirIndices, currentNameIndices);
    int targetDirIndices[numberOfRailSections];
    set_int_array_value(numberOfRailSections, targetDirIndices, -1);
    int targetNameIndices[numberOfRailSections];
    set_int_array_value(numberOfRailSections, targetNameIndices, -1);
    get_single_relevant_dir_name_indices(numberOfRailSections, allRailSectionsDirs, _targetRailSectionName, targetDirIndices, targetNameIndices);
    int currentDirIndicesSize = get_sizeof_int_array_positive_values(numberOfRailSections, currentDirIndices);
    int targetDirIndicesSize = get_sizeof_int_array_positive_values(numberOfRailSections, targetDirIndices);
    if (printDebug == true) {
      Serial.print("currentDirIndices: ");
      print_array_values(currentDirIndicesSize, currentDirIndices);
      Serial.print("currentNameIndices: ");
      print_array_values(currentDirIndicesSize, currentNameIndices);
      Serial.print("targetDirIndices: ");
      print_array_values(targetDirIndicesSize, targetDirIndices);
      Serial.print("targetNameIndices: ");
      print_array_values(targetDirIndicesSize, targetNameIndices);
    }
    // Get only dir indices that contain both current and target indices
    int identicalDirIndicesSize = 0;
    if (targetDirIndicesSize < currentDirIndicesSize) {
      identicalDirIndicesSize = targetDirIndicesSize;
    }
    else {
      identicalDirIndicesSize = currentDirIndicesSize;
    }
    int identicalDirIndices[identicalDirIndicesSize];
    set_int_array_value(identicalDirIndicesSize, identicalDirIndices, -1);
    int identicalCurrentNameIndices[identicalDirIndicesSize];
    set_int_array_value(identicalDirIndicesSize, identicalCurrentNameIndices, -1);
    int identicalTargetNameIndices[identicalDirIndicesSize];
    set_int_array_value(identicalDirIndicesSize, identicalTargetNameIndices, -1);
    get_identicals_between_int_arrays_relay_name_indices(currentDirIndicesSize, currentDirIndices, currentNameIndices, targetDirIndicesSize, targetDirIndices, targetNameIndices, identicalDirIndices, identicalCurrentNameIndices, identicalTargetNameIndices);
    identicalDirIndicesSize = get_sizeof_int_array_positive_values(identicalDirIndicesSize, identicalDirIndices);
    if (printDebug == true) {
      Serial.print("identicalDirIndices: ");
      print_array_values(identicalDirIndicesSize, identicalDirIndices);
      Serial.print("identicalCurrentNameIndices: ");
      print_array_values(identicalDirIndicesSize, identicalCurrentNameIndices);
      Serial.print("identicalTargetNameIndices: ");
      print_array_values(identicalDirIndicesSize, identicalTargetNameIndices);
    }
    if (identicalDirIndicesSize == 0) {
      Serial.print("["); Serial.print(_trainName); Serial.print("] No path for: "); Serial.print(_currentRailSectionName); Serial.print("->"); Serial.print(_targetRailSectionName); Serial.print(". Finding new target.");
      _readyToGenerateTarget = true;
      _readyToPathfindTarget = false;
    }
    else {
      // Attempt to pathfind Simplest Path first (either directly forward or directly backward, without crossing start/finish rail section)
      int pathfindSimpleNameIndexDifferences[identicalDirIndicesSize];
      set_int_array_value(identicalDirIndicesSize, pathfindSimpleNameIndexDifferences, -1);
      get_simple_name_index_differences(identicalDirIndicesSize, identicalCurrentNameIndices, identicalTargetNameIndices, pathfindSimpleNameIndexDifferences);
      if (printDebug == true) {
        Serial.print("pathfindSimpleNameIndexDifferences: ");
        print_array_values(identicalDirIndicesSize, pathfindSimpleNameIndexDifferences);
      }
      int shortestPathfindSimple = get_closest_zero_int(identicalDirIndicesSize, pathfindSimpleNameIndexDifferences);
      int shortestPathfindSimpleIndex = get_closest_zero_index(identicalDirIndicesSize, pathfindSimpleNameIndexDifferences);
      int shortestPathfindSimpleDirIndex = identicalDirIndices[shortestPathfindSimpleIndex];
      if (printDebug == true) {
        Serial.print("shortestPathfindSimple: ");
        Serial.println(shortestPathfindSimple);
        Serial.print("shortestPathfindSimpleDirIndex: ");
        Serial.println(shortestPathfindSimpleDirIndex);
        Serial.print("shortestPathfindSimpleIndex: ");
        Serial.println(shortestPathfindSimpleIndex);
      }
      int shortestPathfindSimpleIndicesLong[identicalDirIndicesSize];
      set_int_array_value(identicalDirIndicesSize, shortestPathfindSimpleIndicesLong, -1);
      get_closest_zero_indices(identicalDirIndicesSize, pathfindSimpleNameIndexDifferences, shortestPathfindSimpleIndicesLong);
      int shortestPathfindSimpleIndicesSize = get_sizeof_int_array_positive_values(identicalDirIndicesSize, shortestPathfindSimpleIndicesLong);
      int shortestPathfindSimpleIndices[shortestPathfindSimpleIndicesSize];
      transfer_int_array_values(identicalDirIndicesSize, shortestPathfindSimpleIndicesLong, shortestPathfindSimpleIndicesSize, shortestPathfindSimpleIndices);
      if (printDebug == true) {
        Serial.print("shortestPathfindSimpleIndices: ");
        print_array_values(shortestPathfindSimpleIndicesSize, shortestPathfindSimpleIndices);
      }
      // Choose first index to path to, for now (i.e., use the 0 (first) index; decision-making for which path to take, based on rail availability, goes here)
      int simplePathNameIndicesSize = abs(shortestPathfindSimple);
      int simplePathNameIndices[simplePathNameIndicesSize];
      get_path_indices_current_to_target(identicalCurrentNameIndices[0], shortestPathfindSimple, simplePathNameIndices);
      if (printDebug == true) {
        Serial.print("simplePathNameIndices: ");
        print_array_values(simplePathNameIndicesSize, simplePathNameIndices);
      }
      int simplePathNamesSize = simplePathNameIndicesSize;
      String simplePathNames[simplePathNamesSize];
      get_path_names_current_to_target(allRailSectionsDirs, shortestPathfindSimpleDirIndex, simplePathNameIndicesSize, simplePathNameIndices, simplePathNames);
      if (printDebug == true) {
        Serial.print("simplePathNames: ");
        print_array_strings(simplePathNamesSize, simplePathNames);
      }
      // Set Planned Rail Sections
      _numberOfPlannedRailSections = abs(shortestPathfindSimple);
      // set_int_array_value(10, _plannedRailSectionsNameIndices, -1);
      set_string_array_value(10, _plannedRailSectionsNames, "NONE");
      for (int i = 0; i < _numberOfPlannedRailSections; i++) {
        _plannedRailSectionsNames[i] = simplePathNames[i];
        // _plannedRailSectionsDirIndices[i] = shortestPathfindSimpleDirIndex; // This line changes with different navigation technique
        // _plannedRailSectionsNameIndices[i] = simplePathNameIndices[i];
      }
      // Set Planned Direction and Acquire Switches
      // FIND THE PROPER SWITCH DIRECTION OF EACH PLANNED RAIL FORWARD, THEN REVERSE LR'S IF GOING BACKWARD
      _plannedDirection = get_polarity_of_int(shortestPathfindSimple);
      // int dirIndex;
      // int nameIndex;
      // int thisSwitch;
      // String thisRailSectionName;
      // for (int i = 0; i < _numberOfPlannedRailSections; i++) {
        // thisRailSectionName = _plannedRailSectionsNames[i];
        // Serial.println("thisRailSectionName: ");
        // Serial.println(thisRailSectionName);
        // dirIndex = _plannedRailSectionsDirIndices[i];
        // nameIndex = _plannedRailSectionsNameIndices[i];
        //thisSwitch = allRailSectionsDirsSwitches[dirIndex][nameIndex];
        //_plannedRailSectionsSwitches[i] = thisSwitch;
      // }
      // Print
      if (printDebug == true) {
        Serial.print("_plannedDirection: ");
        Serial.println(_plannedDirection);
        Serial.print("_numberOfPlannedRailSections: ");
        Serial.println(_numberOfPlannedRailSections);
        // Serial.print("_plannedRailSectionsDirIndices: ");
        // print_array_values(10, _plannedRailSectionsDirIndices);
        // Serial.print("_plannedRailSectionsNameIndices: ");
        // print_array_values(10, _plannedRailSectionsNameIndices);
        Serial.print("_plannedRailSectionsNames: ");
        print_array_strings(10, _plannedRailSectionsNames);
        // Serial.print("_plannedRailSectionsSwitches: ");
        // print_array_chars(10, _plannedRailSectionsSwitches);
      }
      // Pathfind Complete
      _readyToPathfindTarget = false;
      _readyToMoveStatus = true;
    }
  }
  else {
    Serial.println("Not ready to pathfind to target.");
    _readyToGenerateTarget = true;
    _readyToPathfindTarget = false;
  }
}

void Train_DC::set_subordinate_rail_sections(int numberOfRailSections, String allRailSectionsDirs[][10], bool printDebug) {
  if (printDebug == true) {
    Serial.println("set_subordinate_rail_sections()");
  }
  // Get Subordinates/Superordinates - Get Relevant Dir & Name Indices
  int dirIndices[numberOfRailSections];
  set_int_array_value(numberOfRailSections, dirIndices, -1);
  int nameIndices[numberOfRailSections];
  set_int_array_value(numberOfRailSections, nameIndices, -1);
  get_single_relevant_dir_name_indices(numberOfRailSections, allRailSectionsDirs, _currentRailSectionName, dirIndices, nameIndices);
  int dirIndicesSize = get_sizeof_int_array_positive_values(numberOfRailSections, dirIndices);
  int nameIndicesSize = get_sizeof_int_array_positive_values(numberOfRailSections, nameIndices);
  if (printDebug == true) {
    Serial.println("dirIndices: ");
    print_array_values(dirIndicesSize, dirIndices);
    Serial.println("nameIndices: ");
    print_array_values(nameIndicesSize, nameIndices);
  }
  // Get Subordinates - Search for Subordinate Dir & Name Indices
  int subordinateDirIndices[dirIndicesSize];
  set_int_array_value(dirIndicesSize, subordinateDirIndices, -1);
  int subordinateNameIndices[dirIndicesSize];
  set_int_array_value(dirIndicesSize, subordinateNameIndices, -1);
  get_subordinate_dir_name_indices(allRailSectionsDirs, _currentRailSectionName, dirIndicesSize, dirIndices, nameIndices, subordinateDirIndices, subordinateNameIndices);
  int subordinateDirIndicesSize = get_sizeof_int_array_positive_values(dirIndicesSize, subordinateDirIndices);
  if (printDebug == true) {
    Serial.print("subordinateDirIndicesSize: ");
    Serial.println(subordinateDirIndicesSize);
  }
  // Get Names Only
  get_strings_unique_from_dir_name_indices(allRailSectionsDirs, subordinateDirIndicesSize, subordinateDirIndices, subordinateNameIndices, _subordinateRailSectionsNames);
  //get_strings_chars_unique_indices_from_dir_name_indices(allRailSectionsDirs, allRailSectionsDirsSwitches, subordinateDirIndicesSize, subordinateDirIndices, subordinateNameIndices, _subordinateRailSectionsNames, _subordinateRailSectionsSwitches, _subordinateRailSectionsDirIndices, _subordinateRailSectionsNameIndices);
  // Finalize
  _numberOfSubordinateRailSections = get_sizeof_string_array_not_none(3, _subordinateRailSectionsNames);
  if (printDebug == true) {
    Serial.print("_numberOfSubordinateRailSections: ");
    Serial.println(_numberOfSubordinateRailSections);
    // Serial.print("_subordinateRailSectionsDirIndices: ");
    // print_array_values(3, _subordinateRailSectionsDirIndices);
    // Serial.print("_subordinateRailSectionsNameIndices: ");
    // print_array_values(3, _subordinateRailSectionsNameIndices);
    Serial.print("_subordinateRailSectionsNames: ");
    print_array_strings(3, _subordinateRailSectionsNames);
    // Serial.print("_subordinateRailSectionsSwitches: ");
    // print_array_chars(3, _subordinateRailSectionsSwitches);
  }
}

void Train_DC::get_subordinate_rail_sections(int numberOfRailSections, String allRailSectionsDirs[][10], String currentRailSectionName, int numberOfSubordinateRailSections, String subordinateRailSectionsNames[3], bool printDebug) {
  if (printDebug == true) {
    Serial.println("get_subordinate_rail_sections()");
  }
  // Get Subordinates/Superordinates - Get Relevant Dir & Name Indices
  int dirIndices[numberOfRailSections];
  set_int_array_value(numberOfRailSections, dirIndices, -1);
  int nameIndices[numberOfRailSections];
  set_int_array_value(numberOfRailSections, nameIndices, -1);
  get_single_relevant_dir_name_indices(numberOfRailSections, allRailSectionsDirs, currentRailSectionName, dirIndices, nameIndices);
  int dirIndicesSize = get_sizeof_int_array_positive_values(numberOfRailSections, dirIndices);
  int nameIndicesSize = get_sizeof_int_array_positive_values(numberOfRailSections, nameIndices);
  if (printDebug == true) {
    Serial.println("dirIndices: ");
    print_array_values(dirIndicesSize, dirIndices);
    Serial.println("nameIndices: ");
    print_array_values(nameIndicesSize, nameIndices);
  }
  // Get Subordinates - Search for Subordinate Dir & Name Indices
  int subordinateDirIndices[dirIndicesSize];
  set_int_array_value(dirIndicesSize, subordinateDirIndices, -1);
  int subordinateNameIndices[dirIndicesSize];
  set_int_array_value(dirIndicesSize, subordinateNameIndices, -1);
  get_subordinate_dir_name_indices(allRailSectionsDirs, currentRailSectionName, dirIndicesSize, dirIndices, nameIndices, subordinateDirIndices, subordinateNameIndices);
  int subordinateDirIndicesSize = get_sizeof_int_array_positive_values(dirIndicesSize, subordinateDirIndices);
  if (printDebug == true) {
    Serial.print("subordinateDirIndicesSize: ");
    Serial.println(subordinateDirIndicesSize);
  }
  // Get Names Only
  get_strings_unique_from_dir_name_indices(allRailSectionsDirs, subordinateDirIndicesSize, subordinateDirIndices, subordinateNameIndices, subordinateRailSectionsNames);
  //get_strings_chars_unique_indices_from_dir_name_indices(allRailSectionsDirs, allRailSectionsDirsSwitches, subordinateDirIndicesSize, subordinateDirIndices, subordinateNameIndices, _subordinateRailSectionsNames, _subordinateRailSectionsSwitches, _subordinateRailSectionsDirIndices, _subordinateRailSectionsNameIndices);
  // Finalize
  numberOfSubordinateRailSections = get_sizeof_string_array_not_none(3, subordinateRailSectionsNames);
  if (printDebug == true) {
    Serial.print("numberOfSubordinateRailSections: ");
    Serial.println(numberOfSubordinateRailSections);
    // Serial.print("_subordinateRailSectionsDirIndices: ");
    // print_array_values(3, _subordinateRailSectionsDirIndices);
    // Serial.print("_subordinateRailSectionsNameIndices: ");
    // print_array_values(3, _subordinateRailSectionsNameIndices);
    Serial.print("subordinateRailSectionsNames: ");
    print_array_strings(numberOfSubordinateRailSections, subordinateRailSectionsNames);
    // Serial.print("_subordinateRailSectionsSwitches: ");
    // print_array_chars(3, _subordinateRailSectionsSwitches);
  }
}

void Train_DC::set_superordinate_rail_sections(int numberOfRailSections, String allRailSectionsDirs[][10], bool printDebug) {
  if (printDebug == true) {
    Serial.println("get_superordinate_rail_sections()");
  }
  // Get Subordinates/Superordinates - Get Relevant Dir & Name Indices
  int dirIndices[numberOfRailSections];
  set_int_array_value(numberOfRailSections, dirIndices, -1);
  int nameIndices[numberOfRailSections];
  set_int_array_value(numberOfRailSections, nameIndices, -1);
  get_single_relevant_dir_name_indices(numberOfRailSections, allRailSectionsDirs, _currentRailSectionName, dirIndices, nameIndices);
  int dirIndicesSize = get_sizeof_int_array_positive_values(numberOfRailSections, dirIndices);
  int nameIndicesSize = get_sizeof_int_array_positive_values(numberOfRailSections, nameIndices);
  if (printDebug == true) {
    Serial.println("dirIndices: ");
    print_array_values(dirIndicesSize, dirIndices);
    Serial.println("nameIndices: ");
    print_array_values(nameIndicesSize, nameIndices);
  }
  // Get Superordinates - Search for Superordinate Dir & Name Indices
  int superordinateDirIndices[dirIndicesSize];
  set_int_array_value(dirIndicesSize, superordinateDirIndices, -1);
  int superordinateNameIndices[dirIndicesSize];
  set_int_array_value(dirIndicesSize, superordinateNameIndices, -1);
  get_superordinate_dir_name_indices(allRailSectionsDirs, _currentRailSectionName, dirIndicesSize, dirIndices, nameIndices, superordinateDirIndices, superordinateNameIndices);
  int superordinateDirIndicesSize = get_sizeof_int_array_positive_values(dirIndicesSize, superordinateDirIndices);
  if (printDebug == true) {
    Serial.print("superordinateDirIndicesSize: ");
    Serial.println(superordinateDirIndicesSize);
  }
  // Get Names Only
  get_strings_unique_from_dir_name_indices(allRailSectionsDirs, superordinateDirIndicesSize, superordinateDirIndices, superordinateNameIndices, _superordinateRailSectionsNames);
  //get_strings_chars_unique_indices_from_dir_name_indices(allRailSectionsDirs, allRailSectionsDirsSwitches, superordinateDirIndicesSize, superordinateDirIndices, superordinateNameIndices, _superordinateRailSectionsNames, _superordinateRailSectionsSwitches, _superordinateRailSectionsDirIndices, _superordinateRailSectionsNameIndices);
  // Finalize
  _numberOfSuperordinateRailSections = get_sizeof_string_array_not_none(3, _superordinateRailSectionsNames);
  if (printDebug == true) {
    Serial.print("_numberOfSuperordinateRailSections: ");
    Serial.println(_numberOfSuperordinateRailSections);
    // Serial.print("_superordinateRailSectionsDirIndices: ");
    // print_array_values(3, _superordinateRailSectionsDirIndices);
    // Serial.print("_superordinateRailSectionsNameIndices: ");
    // print_array_values(3, _superordinateRailSectionsNameIndices);
    Serial.print("_superordinateRailSectionsNames: ");
    print_array_strings(_numberOfSuperordinateRailSections, _superordinateRailSectionsNames);
    // Serial.print("_superordinateRailSectionsSwitches: ");
    // print_array_chars(3, _superordinateRailSectionsSwitches);
  }
}

void Train_DC::get_superordinate_rail_sections(int numberOfRailSections, String allRailSectionsDirs[][10], String currentRailSectionName, int numberOfSuperordinateRailSections, String superordinateRailSectionsNames[3], bool printDebug) {
  if (printDebug == true) {
    Serial.println("get_superordinate_rail_sections()");
  }
  // Get Subordinates/Superordinates - Get Relevant Dir & Name Indices
  int dirIndices[numberOfRailSections];
  set_int_array_value(numberOfRailSections, dirIndices, -1);
  int nameIndices[numberOfRailSections];
  set_int_array_value(numberOfRailSections, nameIndices, -1);
  get_single_relevant_dir_name_indices(numberOfRailSections, allRailSectionsDirs, currentRailSectionName, dirIndices, nameIndices);
  int dirIndicesSize = get_sizeof_int_array_positive_values(numberOfRailSections, dirIndices);
  int nameIndicesSize = get_sizeof_int_array_positive_values(numberOfRailSections, nameIndices);
  if (printDebug == true) {
    Serial.println("dirIndices: ");
    print_array_values(dirIndicesSize, dirIndices);
    Serial.println("nameIndices: ");
    print_array_values(nameIndicesSize, nameIndices);
  }
  // Get Superordinates - Search for Superordinate Dir & Name Indices
  int superordinateDirIndices[dirIndicesSize];
  set_int_array_value(dirIndicesSize, superordinateDirIndices, -1);
  int superordinateNameIndices[dirIndicesSize];
  set_int_array_value(dirIndicesSize, superordinateNameIndices, -1);
  get_superordinate_dir_name_indices(allRailSectionsDirs, currentRailSectionName, dirIndicesSize, dirIndices, nameIndices, superordinateDirIndices, superordinateNameIndices);
  int superordinateDirIndicesSize = get_sizeof_int_array_positive_values(dirIndicesSize, superordinateDirIndices);
  if (printDebug == true) {
    Serial.print("superordinateDirIndicesSize: ");
    Serial.println(superordinateDirIndicesSize);
  }
  // Get Names Only
  get_strings_unique_from_dir_name_indices(allRailSectionsDirs, superordinateDirIndicesSize, superordinateDirIndices, superordinateNameIndices, superordinateRailSectionsNames);
  //get_strings_chars_unique_indices_from_dir_name_indices(allRailSectionsDirs, allRailSectionsDirsSwitches, superordinateDirIndicesSize, superordinateDirIndices, superordinateNameIndices, _superordinateRailSectionsNames, _superordinateRailSectionsSwitches, _superordinateRailSectionsDirIndices, _superordinateRailSectionsNameIndices);
  // Finalize
  numberOfSuperordinateRailSections = get_sizeof_string_array_not_none(3, superordinateRailSectionsNames);
  if (printDebug == true) {
    Serial.print("numberOfSuperordinateRailSections: ");
    Serial.println(numberOfSuperordinateRailSections);
    // Serial.print("_superordinateRailSectionsDirIndices: ");
    // print_array_values(3, _superordinateRailSectionsDirIndices);
    // Serial.print("_superordinateRailSectionsNameIndices: ");
    // print_array_values(3, _superordinateRailSectionsNameIndices);
    Serial.print("superordinateRailSectionsNames: ");
    print_array_strings(numberOfSuperordinateRailSections, superordinateRailSectionsNames);
    // Serial.print("_superordinateRailSectionsSwitches: ");
    // print_array_chars(3, _superordinateRailSectionsSwitches);
  }
}

void Train_DC::update_travel_plan(int numberOfRailSections, String allRailSectionsNames[])
{
  // If Following a Travel Plan
  if (_readyToPathfindTarget == false && _numberOfPlannedRailSections > 0) {
    _currentRailSectionIndexInPlanned = get_index_of_string(_numberOfPlannedRailSections, _plannedRailSectionsNames, _currentRailSectionName);
    if (_currentRailSectionIndexInPlanned != -1) { // If current rail section is part of planned rail section
      if (_departStation == true) { // If departing, but following planned route, can be considered on route and therefore not departing.
        _departStation = false;
      }
      String newPlannedRailSectionsNames[10];
      // int newPlannedRailSectionsDirIndices[10];
      // int newPlannedRailSectionsNameIndices[10];
      int newPlannedRailSectionsSwitches[10];
      set_string_array_value(10, newPlannedRailSectionsNames, "NONE");
      // set_int_array_value(10, newPlannedRailSectionsDirIndices, -1);
      // set_int_array_value(10, newPlannedRailSectionsNameIndices, -1);
      set_int_array_value(10, newPlannedRailSectionsSwitches, 0);
      _plannedRailSectionsNames[0] = "NONE";
      // _plannedRailSectionsDirIndices[0] = -1;
      // _plannedRailSectionsNameIndices[0] = -1;
      _plannedRailSectionsSwitches[0] = 'N';
      // Roll Planneds
      roll_string_array(10, _plannedRailSectionsNames, -1, newPlannedRailSectionsNames);
      // roll_int_array(10, _plannedRailSectionsDirIndices, -1, newPlannedRailSectionsDirIndices);
      // roll_int_array(10, _plannedRailSectionsNameIndices, -1, newPlannedRailSectionsNameIndices);
      roll_int_array(10, _plannedRailSectionsSwitches, -1, newPlannedRailSectionsSwitches);
      // Reset existing _planneds
      set_string_array_value(10, _plannedRailSectionsNames, "NONE");
      // set_int_array_value(10, _plannedRailSectionsDirIndices, -1);
      // set_int_array_value(10, _plannedRailSectionsNameIndices, -1);
      set_int_array_value(10, _plannedRailSectionsSwitches, 0);
      // Set New Planneds
      for (int i = 0; i < 10; i++) {
        _plannedRailSectionsNames[i] = newPlannedRailSectionsNames[i];
        // _plannedRailSectionsDirIndices[i] = newPlannedRailSectionsDirIndices[i];
        // _plannedRailSectionsNameIndices[i] = newPlannedRailSectionsNameIndices[i];
        _plannedRailSectionsSwitches[i] = newPlannedRailSectionsSwitches[i];
      }
      _numberOfPlannedRailSections -= 1;
      // Serial.println("New _plannedRailSectionsNames: ");
      // print_array_strings(10, _plannedRailSectionsNames);
      // Serial.println("New _plannedRailSectionsDirIndices: ");
      // print_array_values(10, _plannedRailSectionsDirIndices);
      // Serial.println("New _plannedRailSectionsNameIndices: ");
      // print_array_values(10, _plannedRailSectionsNameIndices);
      // Serial.println("New _plannedRailSectionsSwitches: ");
      // print_array_chars(10, _plannedRailSectionsSwitches);
      // delay(10000);
    }
    else { //If current rail section is not part of planned rail sections
      if (_plannedDirection == 1) {
        int indexOfCurrentRailSectionName = get_index_of_string(_numberOfSuperordinateRailSections, _superordinateRailSectionsNames, _currentRailSectionName);
        if (indexOfCurrentRailSectionName != -1) {
          _readyToMoveStatus = false;
          _switchDirectionStatus = true;
          _motorSpeedTarget = 0;
        }
        else {
          Serial.println("Current rail section is not part of planned travel route! Will try to pathfind again.");
          _readyToMoveStatus = false;
          _readyToPathfindTarget = true;
        }
      }
      else if (_plannedDirection == -1) {
        int indexOfCurrentRailSectionName = get_index_of_string(_numberOfSubordinateRailSections, _subordinateRailSectionsNames, _currentRailSectionName);
        if (indexOfCurrentRailSectionName != -1) {
          _readyToMoveStatus = false;
          _switchDirectionStatus = true;
          _motorSpeedTarget = 0;
        }
        else {
          Serial.println("Current rail section is not part of planned travel route! Will try to pathfind again.");
          _readyToMoveStatus = false;
          _readyToPathfindTarget = true;
        }
      }
    }
  }
  // If Target Rail Section Reached -- Rail Stations Docking does NOT activate if not a Target Rail Section.
  if (_currentRailSectionName == _targetRailSectionName) {
    _arriveStation = true;
    _currentRailStationName = _currentRailSectionName;
  }
}

void Train_DC::set_idle() {
  _readyToMoveStatus = false;
  _idleStatus = true;
  _idleStartTime_ms = millis();
  set_motor_reverser(0);
}

void Train_DC::set_ready() {
  _idleStatus = false;
  _currentIdleTime = 0;
  _readyToGenerateTarget = true;
  clear_target_planned_localization_attributes();
}

void Train_DC::set_motor_reverser(int motorReverserTargetInt)
{
  /* 
  Inputs: 
  - reverserTargetInt: Int to determine the target direction of travel:
  (1) Forward (Clockwise)
  (0) Neutral
  (-1) Backward (Counter-Clockwise)

  Outputs:
  - Int (-1 to 1):
  (1) Forward (Clockwise)
  (0) Neutral
  (-1) Backward (Counter-Clockwise)
  */
  if (_motorSpeed == 0) {
    if (motorReverserTargetInt == 1){
        _motorReverser = 1 * _realPositiveDirection;
        // return 1;
    }
    else if (motorReverserTargetInt == 0){
        _motorReverser = 0;
        // return 0;
    }
    else if (motorReverserTargetInt == -1){
        _motorReverser = -1 * _realPositiveDirection;
        // return -1;
    }
  }
  else {
    Serial.println("Can not switch Motor Reverser since Motor Speed is not 0!");
  }
  if (_motorReverser == 1) {
    digitalWrite(_pin_IN1, HIGH);
    digitalWrite(_pin_IN2, LOW);
  }
  else if (_motorReverser == -1) {
    digitalWrite(_pin_IN1, LOW);
    digitalWrite(_pin_IN2, HIGH);
  }
  else if (_motorReverser == 0) {
    digitalWrite(_pin_IN1, LOW);
    digitalWrite(_pin_IN2, LOW);
  }
}

void Train_DC::increment_motor_speed()
{
  /*
  Inputs:
  - accelerationTargetInt: Int to determine the rate of motor acceleration to reach motorSpeedTargetInt:
  (0 to 255) 0 is Slowest. 255 is Fastest.
  - motorSpeedTargetInt: Int to determine the target motor out:
  (0 to 255) 0 is Slowest. 255 is Fastest.

  Outputs:
  - Int (0 to 255): The newest output speed of the motor.
  */

  // Ensure motorAccelerationTargetInt is within limit.
  _motorAccelerationTarget = limit_int(_motorAccelerationTarget, 0, _motorAccelerationMax);
  _motorSpeedTarget = limit_int(_motorSpeedTarget, 0, _motorSpeedMax);

  // Positive Accelerate to motorTargetInt
  if (_motorSpeed < _motorSpeedTarget) {
    if (_motorSpeed + _motorAccelerationTarget < _motorSpeedTarget){
        _motorSpeed += _motorAccelerationTarget;
        analogWrite(_pin_EN, _motorSpeed);
        // return _motorSpeed;
    }
    else{
        _motorSpeed = _motorSpeedTarget;
        analogWrite(_pin_EN, _motorSpeed);
        // return _motorSpeed;
    }
  }
  // Negative Accelerate (Decelerate) to Target Velocity
  else if (_motorSpeed > _motorSpeedTarget){
    if (_motorSpeed - _motorAccelerationTarget > _motorSpeedTarget){
        _motorSpeed -= _motorAccelerationTarget;
        analogWrite(_pin_EN, _motorSpeed);
        // return _motorSpeed;
    }
    else{
        _motorSpeed = _motorSpeedTarget;
        analogWrite(_pin_EN, _motorSpeed);
        // return _motorSpeed;
    }
  }
  // If No Acceleration/Deceleration Required, Return Current Velocity.
  // return _motorSpeed;
}

void Train_DC::emergency_stop()
{
  /*
  Inputs: None
  Outputs:
  - Int (0): The newest output velocity of the motor.
  */
  if (_emergencyStopStatus == false) {
    _emergencyStopStatus = true;
    _motorSpeedTarget = 0;
    _motorAccelerationTarget = _motorAccelerationMax;
    _readyToMoveStatus = false;
    _switchRealPositiveDirectionStatus = true;
    _switchDirectionStatus = true;
  }
  //_motorSpeed = 0;
  //analogWrite(_pin_EN, 0);
  // return 0;
}