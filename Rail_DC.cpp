/*
    Rail_DC.h - Library for handling DC Rail logic.
    Created by benfpv, 09/25/2023.
    Released into the public domain.
*/

#include "Arduino.h"
#include "Rail_DC.h"
#include "funcs_arduino.h"

Rail_DC::Rail_DC(String railName) {
    _railName = railName;
}

void Rail_DC::clear_attributes() {
  _numberOfRailSectionsOccupied = 0;
  set_int_array_value(10, _railSectionsIndicesOccupied, -1);
  _numberOfRailStationsOccupied = 0;
  set_int_array_value(10, _railStationsIndicesOccupied, -1);
}

void Rail_DC::clear_prox_loop_attributes() {
  _numberOfBeginProxActivatedThisLoop = 0;
  set_int_array_value(10, _beginProxIndicesActivatedThisLoop, -1);
  _numberOfEndProxActivatedThisLoop = 0;
  set_int_array_value(10, _endProxIndicesActivatedThisLoop, -1);
  _numberOfBeginProxDeactivatedThisLoop = 0;
  set_int_array_value(10, _beginProxIndicesDeactivatedThisLoop, -1);
  _numberOfEndProxDeactivatedThisLoop = 0;
  set_int_array_value(10, _endProxIndicesDeactivatedThisLoop, -1);
}

void Rail_DC::clear_rail_loop_attributes() {
  _numberOfNewRailSectionsOccupiedThisLoop = 0;
  set_int_array_value(10, _newRailSectionsIndicesOccupiedThisLoop, -1);
  _numberOfNewRailStationsOccupiedThisLoop = 0;
  set_int_array_value(10, _newRailStationsIndicesOccupiedThisLoop, -1);
  _numberOfRailSectionsPoppedThisLoop = 0;
  set_int_array_value(10, _railSectionsIndicesPoppedThisLoop, -1);
  _numberOfRailStationsPoppedThisLoop = 0;
  set_int_array_value(10, _railStationsIndicesPoppedThisLoop, -1);
}

void Rail_DC::print_rail_sections_occupied(String allRailSectionsNames[]) {
  Serial.print("[Rail Occ] ");
  int thisNameIndex;
  for (int i = 0; i < _numberOfRailSectionsOccupied; i++) {
    thisNameIndex = _railSectionsIndicesOccupied[i];
    Serial.print(allRailSectionsNames[thisNameIndex]);
    Serial.print(" ");
  }
  Serial.println(" ");
}

void Rail_DC::print_new_rail_sections_occupied_this_loop(String allRailSectionsNames[]) {
  Serial.print("[Rail (+)] ");
  int thisNameIndex;
  for (int i = 0; i < _numberOfNewRailSectionsOccupiedThisLoop; i++) {
    thisNameIndex = _newRailSectionsIndicesOccupiedThisLoop[i];
    Serial.print(allRailSectionsNames[thisNameIndex]);
    Serial.print(" ");
  }
  Serial.println(" ");
}

void Rail_DC::print_rail_sections_popped_this_loop(String allRailSectionsNames[]) {
  Serial.print("[Rail (-)] ");
  int thisNameIndex;
  for (int i = 0; i < _numberOfRailSectionsPoppedThisLoop; i++) {
    thisNameIndex = _railSectionsIndicesPoppedThisLoop[i];
    Serial.print(allRailSectionsNames[thisNameIndex]);
    Serial.print(" ");
  }
  Serial.println(" ");
}

void Rail_DC::print_rail_stations_occupied(String allRailStationsNames[]) {
  Serial.print("[Stat Occ] ");
  int thisNameIndex;
  for (int i = 0; i < _numberOfRailStationsOccupied; i++) {
    thisNameIndex = _railStationsIndicesOccupied[i];
    Serial.print(allRailStationsNames[thisNameIndex]);
    Serial.print(" ");
  }
  Serial.println(" ");
}

void Rail_DC::print_new_rail_stations_occupied_this_loop(String allRailStationsNames[]) {
  Serial.print("[Stat (+)] ");
  int thisNameIndex;
  for (int i = 0; i < _numberOfNewRailStationsOccupiedThisLoop; i++) {
    thisNameIndex = _newRailStationsIndicesOccupiedThisLoop[i];
    Serial.print(allRailStationsNames[thisNameIndex]);
    Serial.print(" ");
  }
  Serial.println(" ");
}

void Rail_DC::print_rail_stations_popped_this_loop(String allRailStationsNames[]) {
 Serial.print("[Stat (-)] ");
  int thisNameIndex;
  for (int i = 0; i < _numberOfRailStationsPoppedThisLoop; i++) {
    thisNameIndex = _railStationsIndicesPoppedThisLoop[i];
    Serial.print(allRailStationsNames[thisNameIndex]);
    Serial.print(" ");
  }
  Serial.println(" ");
}

void Rail_DC::add_rail_section_occupied_by_name(int numberOfRailSections, String allRailSectionsNames[], String currentRailSectionName) {
  int thisRailSectionIndex = get_index_of_string(numberOfRailSections, allRailSectionsNames, currentRailSectionName);
  if (thisRailSectionIndex != -1) {
    int thisExistingIndex;
    bool indexAlreadyExists = false;
    for (int i = 0; i < 10; i++) {
      thisExistingIndex = _railSectionsIndicesOccupied[i];
      if (thisRailSectionIndex == thisExistingIndex) {
        indexAlreadyExists = true;
      }
    }
    // Serial.print("thisRailSectionIndex: "); Serial.println(thisRailSectionIndex);
    // Serial.print("indexAlreadyExists: "); Serial.println(indexAlreadyExists);
    if (indexAlreadyExists == false) {
      _newRailSectionsIndicesOccupiedThisLoop[_numberOfNewRailSectionsOccupiedThisLoop] = thisRailSectionIndex;
      _numberOfNewRailSectionsOccupiedThisLoop += 1;
      _railSectionsIndicesOccupied[_numberOfRailSectionsOccupied] = thisRailSectionIndex;
      _numberOfRailSectionsOccupied += 1;
    }
  }
}

void Rail_DC::pop_rail_section_occupied_by_name(int numberOfRailSections, String allRailSectionsNames[], String currentRailSectionName) {
  int stringIndices[5]; // For rail sections found with the same name!
  set_int_array_value(5, stringIndices, -1);

  get_indices_of_string(numberOfRailSections, allRailSectionsNames, currentRailSectionName, stringIndices);

  int thisRailSectionIndex;
  for (int p = 0; p < 5; p++) { // For all indices corresponding to the target string
    thisRailSectionIndex = stringIndices[p];
    if (thisRailSectionIndex == -1) {
      break;
    }
    else {
      // Clear Existing Index
      for (int i = 0; i < _numberOfRailSectionsOccupied; i++) {
        if (thisRailSectionIndex == _railSectionsIndicesOccupied[i]) {
          _railSectionsIndicesOccupied[i] = -1;
        }
      }
      int newRailSectionsIndicesOccupied[10];
      set_int_array_value(10, newRailSectionsIndicesOccupied, -1);
      int thisIndex;
      int newCounter = 0;
      for (int i = 0; i < 10; i++) {
        thisIndex = _railSectionsIndicesOccupied[i];
        if (thisIndex != -1) {
          newRailSectionsIndicesOccupied[newCounter] = thisIndex;
          newCounter += 1;
        }
      }
      // Set _newRailSectionsIndicesOccupied
      for (int i = 0; i < 10; i++) {
        _railSectionsIndicesOccupied[i] = newRailSectionsIndicesOccupied[i];
      }
      _numberOfRailSectionsOccupied = newCounter;
      // Update Popped List
      _railSectionsIndicesPoppedThisLoop[_numberOfRailSectionsPoppedThisLoop] = thisRailSectionIndex;
      _numberOfRailSectionsPoppedThisLoop += 1;
    }
  }
}

void Rail_DC::add_rail_station_occupied_by_name(int numberOfRailStations, String allRailStationsNames[], String currentRailStationName) {
  int thisRailStationIndex = get_index_of_string(numberOfRailStations, allRailStationsNames, currentRailStationName);
  if (thisRailStationIndex != -1) {
    int thisExistingIndex;
    bool indexAlreadyExists = false;
    for (int i = 0; i < 10; i++) {
      thisExistingIndex = _railStationsIndicesOccupied[i];
      if (thisRailStationIndex == thisExistingIndex) {
        indexAlreadyExists = true;
      }
    }
    // Serial.print("thisRailStationIndex: "); Serial.println(thisRailStationIndex);
    // Serial.print("indexAlreadyExists: "); Serial.println(indexAlreadyExists);
    if (indexAlreadyExists == false) {
      _newRailStationsIndicesOccupiedThisLoop[_numberOfNewRailStationsOccupiedThisLoop] = thisRailStationIndex;
      _numberOfNewRailStationsOccupiedThisLoop += 1;
      _railStationsIndicesOccupied[_numberOfRailStationsOccupied] = thisRailStationIndex;
      _numberOfRailStationsOccupied += 1;
    }
  }
}

void Rail_DC::pop_rail_station_occupied_by_name(int numberOfRailStations, String allRailStationsNames[], String currentRailStationName) {
  int stringIndices[5]; // For rail stations found with the same name!
  set_int_array_value(5, stringIndices, -1);

  get_indices_of_string(numberOfRailStations, allRailStationsNames, currentRailStationName, stringIndices);

  int thisRailStationIndex;
  for (int p = 0; p < 5; p++) { // For all indices corresponding to the target string
    thisRailStationIndex = stringIndices[p];
    if (thisRailStationIndex == -1) {
      break;
    }
    else {
      // Clear Existing Index
      for (int i = 0; i < _numberOfRailStationsOccupied; i++) {
        if (thisRailStationIndex == _railStationsIndicesOccupied[i]) {
          _railStationsIndicesOccupied[i] = -1;
        }
      }
      int newRailStationsIndicesOccupied[10];
      set_int_array_value(10, newRailStationsIndicesOccupied, -1);
      int thisIndex;
      int newCounter = 0;
      for (int i = 0; i < 10; i++) {
        thisIndex = _railStationsIndicesOccupied[i];
        if (thisIndex != -1) {
          newRailStationsIndicesOccupied[newCounter] = thisIndex;
          newCounter += 1;
        }
      }
      // Set _newRailStationsIndicesOccupied
      for (int i = 0; i < 10; i++) {
        _railStationsIndicesOccupied[i] = newRailStationsIndicesOccupied[i];
      }
      _numberOfRailStationsOccupied = newCounter;
      // Update Popped List
      _railStationsIndicesPoppedThisLoop[_numberOfRailStationsPoppedThisLoop] = thisRailStationIndex;
      _numberOfRailStationsPoppedThisLoop += 1;
    }
  }
}