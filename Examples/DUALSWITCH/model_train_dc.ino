#include "Train_DC.h"
#include "Rail_DC.h"
#include "funcs_arduino.h"

#include "Proximity_IR.h"
#include "Rail_Switch_DC.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// TO DO:
// Create Travel Management System

// Demo
int demoMode = 1; // 0 = not demo mode, 1 = simulate departure and move to 3rd rail section forward, 2 = simulate hitting fullstop sensor.

// Rail_Parser Output Parameters Begin
String masterDirName = "RAIL_DUALSWITCH";
String allRailSectionsDirs[5][10] = 
{
{"ASIA"}, 
{"ASIA", "CYTZ"}, {"MAIN"}, {"MAIN", "CYTZ"}, {"MAIN", "CYYZ"}
};
String allRailSectionsNames[] = 
{
"ASIA", "CYTZ", "MAIN", "CYTZ", "CYYZ"
};
String allSubordinateNames[5][3] = 
{
{"CYTZ", "NONE", "NONE"}, 
{"NONE", "NONE", "NONE"}, {"CYTZ", "CYYZ", "NONE"}, {"NONE", "NONE", "NONE"}, {"NONE", "NONE", "NONE"}
};
String allSubordinateSwitchesNames[] = 
{
"MAIN"
};
int allSubordinateSwitchesPositions[][3] = 
{
{-1, 1, 0}
};
String allSuperordinateNames[5][3] = 
{
{"NONE", "NONE", "NONE"}, 
{"ASIA", "MAIN", "NONE"}, {"NONE", "NONE", "NONE"}, {"ASIA", "MAIN", "NONE"}, {"MAIN", "NONE", "NONE"}
};
String allSuperordinateSwitchesNames[] = 
{
"CYTZ"
};
int allSuperordinateSwitchesPositions[][3] = 
{
{-1, 1, 0}
};
// Rail_Parser Output Parameters End

// User Parameters - Rail Section Begin/End Detectors (IR Proximity Sensors) - They must correspond by index to the rail section names (index 0 of pins == index 0 of section names)
int beginProxPins_IN[] = {22, 24, 26, 24, 30}; // Digital In
int endProxPins_IN[] = {23, 25, 27, 25, 31}; // Digital In

// User Parameters - Rail Fullstop Detectors (IR Proximity Sensors)
// You can declare some pins (belonging to begin/endProxPins) as fullstop pins by putting their numbers in this array.
int fullstopProxPins_IN[] = {22, 25, 26, 31}; // Digital in

// User Parameters - Rail Stations (Train Stops via Servo + Distance Sensor Combos)
String allRailStationsNames[] = 
{
"MAIN", "CYYZ", "CYTZ", "ASIA"
};

// User Parameters - Rail Station Detectors (IR Proximity Sensors) - These must correspond by index to the rail station names (index 0 of pins == index 0 of station names)
int stationProxPins_IN[] = {44, 45, 46, 47};

// User Parameters - Subordinate Rail Switches (if using switches) - These must correspond by index to each subordinate switch in allSubordinateSwitchesNames (index 0 of pins == index 0 of allSubordinateSwitchesNames)
int subordinateRailSwitchPins_EN[] = {5};
int subordinateRailSwitchPins_IN1[] = {6};
int subordinateRailSwitchPins_IN2[] = {7};

// User Parameters - Superordinate Rail Switches (if using switches) - These must correspond by index to each superordinate switch in allSuperordinateSwitchesNames (index 0 of pins == index 0 of allSuperordinateSwitchesNames)
int superordinateRailSwitchPins_EN[] = {8};
int superordinateRailSwitchPins_IN1[] = {9};
int superordinateRailSwitchPins_IN2[] = {10};

// User Parameters - Trains
String trainNames[] = {"EF81"}; // Train names in Strings, size of 4.
int trainNavigationTypes[] = {0}; // 0 Random, 1 Queue Sequential
int trainPins_EN[] = {2};
int trainPins_IN1[] = {3};
int trainPins_IN2[] = {4};
int trainTargetIdleTimes[] = {9000}; // Time in milliseconds to stay idle at a station upon arriving.
int trainMotorSpeedSlows[] = {30}; // Speed limit in motorspeed (/255) when travelling in target rail section.
int trainMotorSpeedMaxs[] = {34}; // Speed limit in motorspeed (/255) when travelling in non-target rail section.
int trainMotorAccelerationStoppings[] = {2}; // Acceleration/deceleration limit per loop when stopping normally.
int trainMotorAccelerationMaxs[] = {5}; // Acceleration/deceleration limit per loop when emergency stopping.
String trainStartingRailSectionNames[] = {"JNRT"}; // The name of the rail station that each train will start from.
String trainTargetRailStationsNamesQueue[][10] =
{
  {"CYYZ", "MAIN", "CYTZ", "ASIA", "CYTZ", "MAIN"}
};
int trainNumberOfTargetRailStationsInQueue[] = 
{
  6
};

// Initialize Rail
Rail_DC rail(masterDirName);
// Initialize Rail Fullstops, Sections & Stations (Proximity IR)
Proximity_IR fullstopProx[] = 
{
  Proximity_IR(fullstopProxPins_IN[0]), 
  Proximity_IR(fullstopProxPins_IN[1]),
  Proximity_IR(fullstopProxPins_IN[2]),
  Proximity_IR(fullstopProxPins_IN[3])
};
Proximity_IR beginProx[] = 
{
  Proximity_IR(beginProxPins_IN[0]), 
  Proximity_IR(beginProxPins_IN[1]),
  Proximity_IR(beginProxPins_IN[2]),
  Proximity_IR(beginProxPins_IN[3]),
  Proximity_IR(beginProxPins_IN[4])
};
Proximity_IR endProx[] = 
{
  Proximity_IR(endProxPins_IN[0]), 
  Proximity_IR(endProxPins_IN[1]),
  Proximity_IR(endProxPins_IN[2]),
  Proximity_IR(endProxPins_IN[3]),
  Proximity_IR(endProxPins_IN[4])
};
Proximity_IR stationProx[] = 
{
  Proximity_IR(stationProxPins_IN[0]), 
  Proximity_IR(stationProxPins_IN[1]),
  Proximity_IR(stationProxPins_IN[2]),
  Proximity_IR(stationProxPins_IN[3])
};
// Initialize Trains
Train_DC trains[] = 
{
  Train_DC(trainNames[0], 0, trainNavigationTypes[0], trainPins_EN[0], trainPins_IN1[0], trainPins_IN2[0], trainTargetIdleTimes[0], trainMotorSpeedSlows[0], trainMotorSpeedMaxs[0], trainMotorAccelerationStoppings[0], trainMotorAccelerationMaxs[0])
};
// Initialize Rail Switches
Rail_Switch_DC subordinateRailSwitches[] =
{
  Rail_Switch_DC(subordinateRailSwitchPins_EN[0], subordinateRailSwitchPins_IN1[0], subordinateRailSwitchPins_IN2[0])
};
Rail_Switch_DC superordinateRailSwitches[] =
{
  Rail_Switch_DC(superordinateRailSwitchPins_EN[0], superordinateRailSwitchPins_IN1[0], superordinateRailSwitchPins_IN2[0])
};

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(128, 64, &Wire, -1);

// (PROBABLY) NO USER CHANGES REQUIRED FROM HERE ON OUT
// Automated Parameters Characterization
const int numberOfRailSections = sizeof(allRailSectionsNames)/sizeof(*allRailSectionsNames);
const int numberOfRailStations = sizeof(allRailStationsNames)/sizeof(*allRailStationsNames);
const int numberOfFullstopProx = sizeof(fullstopProx)/sizeof(*fullstopProx);
const int numberOfBeginProx = sizeof(beginProx)/sizeof(*beginProx);
const int numberOfEndProx = sizeof(endProx)/sizeof(*endProx);
const int numberOfStationProx = sizeof(stationProx)/sizeof(*stationProx);
const int numberOfSubordinateRailSwitches = sizeof(subordinateRailSwitches)/sizeof(*subordinateRailSwitches);
const int numberOfSuperordinateRailSwitches = sizeof(superordinateRailSwitches)/sizeof(*superordinateRailSwitches);
const int numberOfTrains = sizeof(trainNames)/sizeof(*trainNames);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A0));

  Serial.println(" ");
  Serial.println("----- SETUP BEGIN -----");
  Serial.println("--- Setup: Auto Parameters Details ---");
  Serial.print("#DemoMode: "); Serial.println(demoMode);
  Serial.print("#RailSections: "); Serial.println(numberOfRailSections);
  Serial.print("#RailStations: "); Serial.println(numberOfRailStations);
  Serial.print("#FullstopIRProx: "); Serial.println(numberOfFullstopProx);
  Serial.print("#BeginIRProx: "); Serial.println(numberOfBeginProx);
  Serial.print("#EndIRProx: "); Serial.println(numberOfEndProx);
  Serial.print("#StationIRProx: "); Serial.println(numberOfStationProx);
  Serial.print("#SubordinateRailSwitches: "); Serial.println(numberOfSubordinateRailSwitches);
  Serial.print("#SuperordinateRailSwitches: "); Serial.println(numberOfSuperordinateRailSwitches);
  Serial.print("#Trains: "); Serial.println(numberOfTrains);
  // Setup Rail
  Serial.println("--- Setup: Auto Rail ---");
  rail.clear_attributes();
  String thisName = "NONE";
  for (int i = 0; i < numberOfTrains; i++) {
    thisName = trainStartingRailSectionNames[i];
    int indexSection = get_index_of_string(numberOfRailSections, allRailSectionsNames, thisName);
    if (indexSection != -1) {
      rail.add_rail_section_occupied_by_name(numberOfRailSections, allRailSectionsNames, thisName);
    }
    int indexStation = get_index_of_string(numberOfRailStations, allRailStationsNames, thisName);
    if (indexStation != -1) {
      rail.add_rail_station_occupied_by_name(numberOfRailStations, allRailStationsNames, thisName);
    }
  }
  // Setup Proximity IR
  Serial.println("--- Setup: Auto Proximity IR ---");
  for (int i = 0; i < numberOfFullstopProx; i++) {
    fullstopProx[i].begin();
    fullstopProx[i].clear_attributes();
  }
  for (int i = 0; i < numberOfBeginProx; i++) {
    beginProx[i].begin();
    beginProx[i].clear_attributes();
  }
  for (int i = 0; i < numberOfEndProx; i++) {
    endProx[i].begin();
    endProx[i].clear_attributes();
  }
  for (int i = 0; i < numberOfStationProx; i++) {
    stationProx[i].begin();
    stationProx[i].clear_attributes();
  }
  // Setup Trains
  Serial.println("--- Setup: Auto Trains ---");
  int numberOfTargetRailSectionsInQueue;
  for (int i = 0; i < numberOfTrains; i++) {
    trains[i].begin();
    if (trains[i]._trainNavigationType == 1) {
      trains[i].init_target_rail_section_queue(trainNumberOfTargetRailStationsInQueue[i], trainTargetRailStationsNamesQueue[i]);
    }
    trains[i].clear_current_localization_attributes();
    trains[i].clear_target_planned_localization_attributes();
    trains[i].clear_subordinate_superordinate_localization_attributes();
    trains[i].set_starting_rail_section(numberOfRailSections, allRailSectionsNames, numberOfRailStations, allRailStationsNames, trainStartingRailSectionNames[i]);
    trains[i].set_subordinate_rail_sections(numberOfRailSections, allRailSectionsDirs, false);
    trains[i].set_superordinate_rail_sections(numberOfRailSections, allRailSectionsDirs, false);
    trains[i]._readyToGenerateTarget = true;
  }
  // Setup Rail Switches
  for (int i = 0; i < numberOfSubordinateRailSwitches; i++) {
    subordinateRailSwitches[i].begin();
    subordinateRailSwitches[i].set_switch(1);
  }
  for (int i = 0; i < numberOfSuperordinateRailSwitches; i++) {
    superordinateRailSwitches[i].begin();
    superordinateRailSwitches[i].set_switch(1);
  }
  // OLED Display
  Serial.println("--- Setup: Auto OLED Display ---");
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Preemptive Setup Update
  Serial.println("--- Setup: Pre-Plan Trains' Localizations ---");
  bool pathfindSuccessful;
  if (demoMode == 1) {
    for (int t = 0; t < numberOfTrains; t++) {
      trains[t].print_localization();
    }
  }

  Serial.println("--- Setup: Auto Preemptive Update ---");
  if (demoMode == 0) {
    bool passfail = false;
    while (passfail == false) {
      passfail = test_prox();
    }
    update_prox();
  }

  while (true) {  
    pathfindSuccessful = true;
    update_rail();
    update_station_sensors();
    update_trains();
    update_rail_switches();
    for (int t = 0; t < numberOfTrains; t++) {
      if (trains[t]._readyToMoveStatus == false) {
        pathfindSuccessful = false;
      }
    }
    if (pathfindSuccessful == true) {
      break;
    }
  }
  
  Serial.println("------ SETUP END ------");

  // Delays
  if (demoMode == 0) {
    Serial.println("--- Setup: Delay Countdown ---");
    setup_countdown(10000);
  }
  else if (demoMode == 1) {
    delay(1000000);
  }

  // Start Looping
  Serial.println("------ LOOP BEGIN ------");
}

void setup_countdown(int countdownTime_ms) {
  int startTime_ms = millis();
  int currentTime_ms = 0;
  int maxDisplayedText = 1;
  if (numberOfTrains > 3) {
    maxDisplayedText = 3;
  }
  else {
    maxDisplayedText = numberOfTrains;
  }
  while (currentTime_ms < countdownTime_ms) {
    currentTime_ms = millis() - startTime_ms;
    display.clearDisplay();
    display.setCursor(0,0);
    display.println("Countdown");
    display.setCursor(0,10);
    display.print("[T]");
    for (int i = 0; i < maxDisplayedText; i++) {
      display.print(" "); display.print(trains[i]._trainName);
    }
    display.setCursor(0,20);
    display.print("[C]");
    for (int i = 0; i < maxDisplayedText; i++) {
      display.print(" "); display.print(trains[i]._currentRailSectionName);
    }
    display.setCursor(0,30);
    display.print("[T]");
    for (int i = 0; i < maxDisplayedText; i++) {
      display.print(" "); display.print(trains[i]._targetRailSectionName);
    }
    display.setCursor(0,40);
    display.print("[tBegin] ");
    display.print(countdownTime_ms - currentTime_ms);
    display.display();
  }
}

int loopTimeTargetMinimum_ms = 250; // WARNING: Directly related to train acceleration. Default 250. Raise floor of loop time to stabilize it.
int loopTimeStart_ms;
int loopTimeDuration_ms;
int loopTimeDurationFinal_ms;
int loopTimeDurationFinalCorrection_ms = 0;
int loopTimeToDelay_ms;
int loopCount = 0;
void loop() {
  loopTimeStart_ms = millis();
  Serial.print("----- LOOP BEGIN #"); Serial.print(loopCount); Serial.println(" -----");
  randomSeed(analogRead(A0));

  update_prox();
  update_rail();
  update_station_sensors();
  update_trains();
  update_rail_switches();

  update_oled_display();

  loopCount += 1;
  loopTimeDuration_ms = millis() - loopTimeStart_ms;
  //Serial.print("loopTimeDuration: "); Serial.println(loopTimeDuration_ms);
  if (loopTimeDuration_ms < loopTimeTargetMinimum_ms) {
    loopTimeToDelay_ms = loopTimeTargetMinimum_ms - loopTimeDuration_ms;
    //Serial.print("loopTimeToDelay: "); Serial.println(loopTimeToDelay_ms);
    delay(loopTimeToDelay_ms - loopTimeDurationFinalCorrection_ms);
  }
  loopTimeDurationFinal_ms = millis() - loopTimeStart_ms;
  Serial.print("LOOP END TIME: "); Serial.println(loopTimeDurationFinal_ms);
  // if (loopCount == 0) {
  //   loopTimeDurationFinalCorrection_ms = loopTimeDurationFinal_ms - loopTimeTargetMinimum_ms;
  // }
  if (demoMode != 0) {
    delay(1000);
  }
}

bool test_prox() {
  // Serial.println("test_prox()");
  for (int i = 0; i < numberOfFullstopProx; i++) {
    fullstopProx[i].read();
    if (fullstopProx[i]._currentStatus == 0) {
      Serial.print("fullstopProx Triggered: "); Serial.println(i);
      Serial.println("(!) TRACK IS NOT CLEAR! PLEASE CLEAR ALL NON-STATIONED IR SENSORS!");
      delay(10000);
      return false;
    }
  }
  for (int i = 0; i < numberOfBeginProx; i++) {
    beginProx[i].read();
    if (beginProx[i]._currentStatus == 0) {
      Serial.print("beginProx Triggered: "); Serial.println(i);
      Serial.println("(!) TRACK IS NOT CLEAR! PLEASE CLEAR ALL NON-STATIONED IR SENSORS!");
      delay(10000);
      return false;
    }
  }
  for (int i = 0; i < numberOfEndProx; i++) {
    endProx[i].read();
    if (endProx[i]._currentStatus == 0) {
      Serial.print("endProx Triggered: "); Serial.println(i);
      Serial.println("(!) TRACK IS NOT CLEAR! PLEASE CLEAR ALL NON-STATIONED IR SENSORS!");
      delay(10000);
      return false;
    }
  }
  for (int i = 0; i < numberOfStationProx; i++) {
    stationProx[i].read();
    if (stationProx[i]._currentStatus == 0) {
      bool stationFalselyTriggered = true;
      for (int t = 0; t < numberOfTrains; t++) {
        if (trains[t]._currentRailStationName.equals(allRailStationsNames[i])) {
          Serial.print("Train "); Serial.print(trains[t]._trainName); Serial.print(" Correctly Occupying "); Serial.println(allRailStationsNames[i]);
          stationFalselyTriggered = false;
        }
      }
      if (stationFalselyTriggered == true) {
        Serial.print("stationProx Triggered: "); Serial.println(i);
        Serial.println("(!) TRACK IS NOT CLEAR! PLEASE CLEAR ALL NON-STATIONED IR SENSORS!");
        delay(10000);
        return false;
      }
    }
  }
  return true;
}

void update_prox() {
  // Serial.println("update_prox()");
  for (int i = 0; i < numberOfFullstopProx; i++) {
    fullstopProx[i]._statusTemp = fullstopProx[i]._statusHold;
    fullstopProx[i].readHold();
    if (fullstopProx[i]._statusHold == 0 && fullstopProx[i]._statusTemp == 1) { // Activated
      Serial.println("(!) Fullstop Proximity Sensor Triggered (!)");
      Serial.println("(!) EMERGENCY STOPPING ALL TRAINS (!)");
      Serial.print("fullstopProx Triggered: "); Serial.println(i);
      for (int t = 0; t < numberOfTrains; t++) {
        trains[t].emergency_stop();
      }
    }
  }
  rail.clear_prox_loop_attributes();
  for (int i = 0; i < numberOfBeginProx; i++) {
    beginProx[i]._statusTemp = beginProx[i]._statusHold;
    beginProx[i].readHold();
    if (beginProx[i]._statusHold == 0 && beginProx[i]._statusTemp == 1) { // Activated
      rail._beginProxIndicesActivatedThisLoop[rail._numberOfBeginProxActivatedThisLoop] = i;
      rail._numberOfBeginProxActivatedThisLoop += 1;
      // Serial.print("beginProx Activated: "); Serial.println(i);
    }
    else if (beginProx[i]._statusHold == 1 && beginProx[i]._statusTemp == 0) { // Deactivated
      rail._beginProxIndicesDeactivatedThisLoop[rail._numberOfBeginProxDeactivatedThisLoop] = i;
      rail._numberOfBeginProxDeactivatedThisLoop += 1;
      // Serial.print("beginProx Deactivated: "); Serial.println(i);
    }
  }
  for (int i = 0; i < numberOfEndProx; i++) {
    endProx[i]._statusTemp = endProx[i]._statusHold;
    endProx[i].readHold();
    if (endProx[i]._statusHold == 0 && endProx[i]._statusTemp == 1) { // Activated
      rail._endProxIndicesActivatedThisLoop[rail._numberOfEndProxActivatedThisLoop] = i;
      rail._numberOfEndProxActivatedThisLoop += 1;
      // Serial.print("endProx Activated: "); Serial.println(i);
    }
    else if (endProx[i]._statusHold == 1 && endProx[i]._statusTemp == 0) { // Deactivated
      rail._endProxIndicesDeactivatedThisLoop[rail._numberOfEndProxDeactivatedThisLoop] = i;
      rail._numberOfEndProxDeactivatedThisLoop += 1;
      // Serial.print("endProx Deactivated: "); Serial.println(i);
    }
  }
  for (int i = 0; i < numberOfStationProx; i++) {
    stationProx[i].readHold();
  }
}

void update_rail() {
  // Serial.println("update_rail()");
  // Add Rail Sections Occupied by Prox Activated
  rail.clear_rail_loop_attributes();
  String thisRailSectionName = "NONE";
  int thisIndex;
  // Begin Prox Activation Check & Add
  // Serial.print("Begin Prox Activation Check "); Serial.println(rail._numberOfBeginProxActivatedThisLoop);
  for (int i = 0; i < rail._numberOfBeginProxActivatedThisLoop; i++) {
    thisIndex = rail._beginProxIndicesActivatedThisLoop[i];
    thisRailSectionName = allRailSectionsNames[thisIndex];
    // Serial.print("- "); Serial.println(thisRailSectionName);
    rail.add_rail_section_occupied_by_name(numberOfRailSections, allRailSectionsNames, thisRailSectionName);
  }
  // End Prox Activation Check & Add
  // Serial.print("End Prox Activation Check "); Serial.println(rail._numberOfEndProxActivatedThisLoop);
  for (int i = 0; i < rail._numberOfEndProxActivatedThisLoop; i++) {
    thisIndex = rail._endProxIndicesActivatedThisLoop[i];
    thisRailSectionName = allRailSectionsNames[thisIndex];
    // Serial.print("- "); Serial.println(thisRailSectionName);
    rail.add_rail_section_occupied_by_name(numberOfRailSections, allRailSectionsNames, thisRailSectionName);
  }
  // Print Information
  // rail.print_rail_sections_occupied(allRailSectionsNames);
  // rail.print_new_rail_sections_occupied_this_loop(allRailSectionsNames);
  // rail.print_rail_stations_occupied(allRailStationsNames);
  // rail.print_new_rail_stations_occupied_this_loop(allRailStationsNames);
  // Begin Prox Deactivation Check & Pop
  bool popThisRailSection = true;
  for (int i = 0; i < rail._numberOfBeginProxDeactivatedThisLoop; i++) {
    thisIndex = rail._beginProxIndicesDeactivatedThisLoop[i];
    thisRailSectionName = allRailSectionsNames[thisIndex];
    popThisRailSection = true;
    for (int t = 0; t < numberOfTrains; t++) {
      if (thisRailSectionName.equals(trains[t]._currentRailSectionName)) {
        popThisRailSection = false;
        break;
      }
    }
    if (popThisRailSection == true) {
      rail.pop_rail_section_occupied_by_name(numberOfRailSections, allRailSectionsNames, thisRailSectionName);
    }
  }
  // End Prox Deactivation Check & Pop
  for (int i = 0; i < rail._numberOfEndProxDeactivatedThisLoop; i++) {
    thisIndex = rail._endProxIndicesDeactivatedThisLoop[i];
    thisRailSectionName = allRailSectionsNames[thisIndex];
    popThisRailSection = true;
    for (int t = 0; t < numberOfTrains; t++) {
      if (thisRailSectionName.equals(trains[t]._currentRailSectionName)) {
        popThisRailSection = false;
        break;
      }
    }
    if (popThisRailSection == true) {
      rail.pop_rail_section_occupied_by_name(numberOfRailSections, allRailSectionsNames, thisRailSectionName);
    }
  }
  // Add Rail Station if Train Occupies a Rail Section with Station
  for (int i = 0; i < rail._numberOfRailSectionsOccupied; i++) {
    thisIndex = rail._railSectionsIndicesOccupied[i];
    if (thisIndex != -1) {
      thisRailSectionName = allRailSectionsNames[thisIndex];
      for (int t = 0; t < numberOfTrains; t++) {
        if (((trains[t]._departStation == true) || (trains[t]._arriveStation == true)) && trains[t]._currentRailSectionName.equals(thisRailSectionName)) {
          rail.add_rail_station_occupied_by_name(numberOfRailStations, allRailStationsNames, thisRailSectionName);
          trains[t]._currentRailStationName = thisRailSectionName;
        }
      }
    }
  }

  // Rail Sections Check & Pop - The reason this is needed is in case where setups do not have all tracks tracked/documented (e.g., station to station, within-switch rails not tracked, etc.).
  for (int i = 0; i < rail._numberOfRailSectionsOccupied; i++) {
    thisIndex = rail._railSectionsIndicesOccupied[i];
    thisRailSectionName = allRailSectionsNames[thisIndex]; 
    popThisRailSection = true;
    for (int t = 0; t < numberOfTrains; t++) {
      if (trains[t]._currentRailSectionName.equals(thisRailSectionName) || trains[t]._plannedRailSectionsNames[0].equals(thisRailSectionName)) {
        popThisRailSection = false;
        break;
      }
    }
    if (popThisRailSection == true) {
      rail.pop_rail_section_occupied_by_name(numberOfRailSections, allRailSectionsNames, thisRailSectionName);
    }
  }
  // Rail Stations Check & Pop
  for (int i = 0; i < rail._numberOfRailStationsOccupied; i++) {
    thisIndex = rail._railStationsIndicesOccupied[i];
    thisRailSectionName = allRailStationsNames[thisIndex]; // For efficiency, called "rail section name" even though it is station
    popThisRailSection = true;
    for (int t = 0; t < numberOfTrains; t++) {
      if ((trains[t]._departStation == true || trains[t]._arriveStation == true || trains[t]._idleStatus == true || trains[t]._readyToGenerateTarget == true || trains[t]._readyToPathfindTarget == true) && trains[t]._currentRailStationName.equals(thisRailSectionName)) { // If train is departing and occupies station, do not pop this station
        popThisRailSection = false;
        break;
      }
    }
    if (popThisRailSection == true) {
      rail.pop_rail_station_occupied_by_name(numberOfRailStations, allRailStationsNames, thisRailSectionName);
    }
  }
  // DEBUG
  // if (rail._numberOfRailSectionsOccupied > 1) {
  //   Serial.println("numberOfRailSectionsOccupied > 1");
  //   delay(500);
  // }
  // if (rail._numberOfRailStationsOccupied > 1) {
  //   Serial.println("numberOfRailStationsOccupied > 1");
  //   delay(1000);
  // }
  rail.print_rail_sections_occupied(allRailSectionsNames);
  rail.print_new_rail_sections_occupied_this_loop(allRailSectionsNames);
  rail.print_rail_sections_popped_this_loop(allRailSectionsNames);
  rail.print_rail_stations_occupied(allRailStationsNames);
  rail.print_new_rail_stations_occupied_this_loop(allRailStationsNames);
  rail.print_rail_stations_popped_this_loop(allRailStationsNames);
}

void update_station_sensors() {
  // Serial.println("update_station_sensors()");
  // For each occupied rail station, get their inputs
  int thisRailIndex;
  String thisRailStationName;
  for (int i = 0; i < rail._numberOfRailStationsOccupied; i++) {
    thisRailIndex = rail._railStationsIndicesOccupied[i];
    thisRailStationName = allRailStationsNames[thisRailIndex];
    for (int t = 0; t < numberOfTrains; t++) {
      if (trains[t]._arriveStation == true && trains[t]._currentRailSectionName.equals(thisRailStationName)) {
        if (stationProx[thisRailIndex]._statusHold == 1) { // Did Not Hit Station
          if (trains[t]._motorSpeedTarget != 0) {
            trains[t]._motorSpeedTarget = trains[t]._motorSpeedSlow;
          }
        }
        else if (stationProx[thisRailIndex]._statusHold == 0) { // Hit Station
          trains[t]._triggeredStation = true;
          trains[t]._motorAccelerationTarget = trains[t]._motorAccelerationStopping;
          trains[t]._motorSpeedTarget = 0;
        }
        // If Stopped
        if (trains[t]._motorSpeed == 0) {
          if (stationProx[thisRailIndex]._statusHold == 0) {
            trains[t].set_idle();
            Serial.print("***** Train "); Serial.print(trains[t]._trainName);
            Serial.print(" has arrived at Station "); Serial.print(thisRailStationName); Serial.println(" *****");
          }
          else { // Overshot
            if (trains[t]._triggeredStation == true) {
              trains[t]._switchDirectionStatus = true;
            }
          }
        }
      }
    }
  }
}

void update_trains() {
  // Serial.println("update_trains()");
  for (int t = 0; t < numberOfTrains; t++) {
    // Idle Timer
    if (trains[t]._idleStatus == true) {
      trains[t]._arriveStation = false;
      trains[t]._departStation = false;
      trains[t].set_motor_reverser(0);
      trains[t]._currentIdleTime = millis() - trains[t]._idleStartTime_ms; // CHANGE TO TIME VERSION
      if (trains[t]._currentIdleTime > trains[t]._targetIdleTime) { // Idle complete!
        trains[t].set_ready();
      }
    }
    // Generate Next Target
    if (trains[t]._readyToGenerateTarget == true) {
      if (trains[t]._trainNavigationType == 0) {
        trains[t].set_target_rail_section_randomly(numberOfRailSections, allRailSectionsNames, numberOfRailStations, allRailStationsNames, 0);
      }
      else if (trains[t]._trainNavigationType == 1) {
        trains[t].set_target_rail_section_by_queue(numberOfRailStations, allRailStationsNames);
      }
    }
    if (trains[t]._readyToPathfindTarget == true) {
      trains[t].set_pathfind_current_to_target(numberOfRailSections, allRailSectionsDirs, allRailSectionsNames, false);
      // trains[t].pathfind_current_to_target(numberOfRailSections, allRailSectionsDirs, true);
      if (trains[t]._readyToMoveStatus == true) {
        trains[t]._departStation = true;
      }
    }
    // Prepare To Depart Station - Requires _departStation which is derived from rail check that planned rails for this train are clear
    if (trains[t]._readyToMoveStatus == true && trains[t]._departStation == true && trains[t]._motorReverser == 0 && trains[t]._switchDirectionStatus == false) {
      trains[t].set_motor_reverser(trains[t]._plannedDirection);
      trains[t]._motorAccelerationTarget = 1;
      trains[t]._motorSpeedTarget = trains[t]._motorSpeedMax;
    }
  }
  // Update Current Localization
  int thisRailSectionIndex;
  String thisRailSectionName = "NONE";
  for (int i = 0; i < rail._numberOfNewRailSectionsOccupiedThisLoop; i++) {
    thisRailSectionIndex = rail._newRailSectionsIndicesOccupiedThisLoop[i];
    thisRailSectionName = allRailSectionsNames[thisRailSectionIndex];
    for (int t = 0; t < numberOfTrains; t++) {
      if (trains[t]._readyToMoveStatus == true) {
        bool currentLocalizationUpdated = false;
        // Subordinate
        for (int rs = 0; rs < trains[t]._numberOfSubordinateRailSections; rs++) {
          if (trains[t]._subordinateRailSectionsNames[rs].equals(thisRailSectionName)) {
            Serial.print("***** Train Name: "); Serial.print(trains[t]._trainName);
            Serial.print("; New Current (Subordinate): "); Serial.print(thisRailSectionName); Serial.println(" *****");
            trains[t].set_current_rail_section(numberOfRailSections, allRailSectionsNames, thisRailSectionName);
            trains[t].update_travel_plan(numberOfRailSections, allRailSectionsNames);
            currentLocalizationUpdated = true;
          }
        }
        // Superordinate
        for (int rs = 0; rs < trains[t]._numberOfSuperordinateRailSections; rs++) {
          if (trains[t]._superordinateRailSectionsNames[rs].equals(thisRailSectionName)) {
            Serial.print("***** Train Name: "); Serial.print(trains[t]._trainName);
            Serial.print("; New Current (Superordinate): "); Serial.print(thisRailSectionName); Serial.println(" *****");
            trains[t].set_current_rail_section(numberOfRailSections, allRailSectionsNames, thisRailSectionName);
            trains[t].update_travel_plan(numberOfRailSections, allRailSectionsNames);
            currentLocalizationUpdated = true;
          }
        }
        // Update Subordinate/Superordinate
        if (currentLocalizationUpdated == true) {
          trains[t].clear_subordinate_superordinate_localization_attributes();
          trains[t].set_subordinate_rail_sections(numberOfRailSections, allRailSectionsDirs, false);
          trains[t].set_superordinate_rail_sections(numberOfRailSections, allRailSectionsDirs, false);
        }
      }
    }
  }
  // Update Popped Localizations
  for (int i = 0; i < rail._numberOfRailSectionsPoppedThisLoop; i++) {
    thisRailSectionIndex = rail._railSectionsIndicesPoppedThisLoop[i];
    thisRailSectionName = allRailSectionsNames[thisRailSectionIndex];
    for (int t = 0; t < numberOfTrains; t++) {
      if (trains[t]._currentRailSectionName.equals(thisRailSectionName)) {
        trains[t]._currentRailSectionName = "NONE";
      }
    }
  }
  // Update Popped Stations
  for (int i = 0; i < rail._numberOfRailStationsPoppedThisLoop; i++) {
    thisRailSectionIndex = rail._railStationsIndicesPoppedThisLoop[i];
    thisRailSectionName = allRailStationsNames[thisRailSectionIndex];
    for (int t = 0; t < numberOfTrains; t++) {
      if (trains[t]._currentRailStationName.equals(thisRailSectionName)) {
        trains[t]._currentRailStationName = "NONE";
      }
    }
  }
  // Finalize Update to Trains
  for (int t = 0; t < numberOfTrains; t++) {
    // If Switching Direction
    if (trains[t]._switchDirectionStatus == true || trains[t]._switchRealPositiveDirectionStatus == true) {
      trains[t]._motorSpeedTarget = 0;
      if (trains[t]._motorSpeed == 0) {
        trains[t]._emergencyStopStatus = false;
        if (trains[t]._switchDirectionStatus == true) {
          if (trains[t]._motorReverser == 1) {
            trains[t].set_motor_reverser(-1);
          }
          else {
            trains[t].set_motor_reverser(1);
          }
          trains[t]._switchDirectionStatus = false;
        }
        if (trains[t]._switchRealPositiveDirectionStatus == true) {
          if (trains[t]._realPositiveDirection == 1) {
            trains[t]._realPositiveDirection = -1;
          }
          else {
            trains[t]._realPositiveDirection = 1;
          }
          trains[t]._switchRealPositiveDirectionStatus = false;
          trains[t].set_motor_reverser(trains[t]._plannedDirection);
        }
        trains[t]._readyToMoveStatus = true;
        trains[t]._motorAccelerationTarget = 1;
        trains[t]._motorSpeedTarget = trains[t]._motorSpeedMax;
      }
    }
    // Finalize Target Speed & Increment
    if (trains[t]._readyToMoveStatus == false) {
      trains[t]._motorSpeedTarget = 0;
    }
    trains[t].print_localization();
    trains[t].increment_motor_speed();
  }
}

void update_rail_switches() {
  // Serial.println("update_rail_switches()");
  int thisPlannedDirection = 0;
  int thisRailSectionIndex = -1;
  String thisRailSectionName = "NONE";
  int thisSwitchIndex = -1;
  int thisAntiSwitchIndex = -1;
  String prevRailSectionName = "NONE";
  String nextRailSectionName = "NONE";
  int thisPlannedSwitchIndex = -1;
  int thisCurrentSwitchPosition = 0;
  int thisPlannedSwitchPosition = 0;
  for (int t = 0; t < numberOfTrains; t++) {
    if ((trains[t]._departStation == true) && (trains[t]._motorSpeed < trains[t]._motorSpeedSlow)) {
      thisPlannedDirection = trains[t]._plannedDirection;
      // Do Current Rail Section Switch
      thisRailSectionName = trains[t]._currentRailSectionName;
      thisRailSectionIndex = get_index_of_string(numberOfRailSections, allRailSectionsNames, thisRailSectionName);
      if (thisPlannedDirection == 1) {
        thisSwitchIndex = get_index_of_string(numberOfSubordinateRailSwitches, allSubordinateSwitchesNames, thisRailSectionName);
        thisAntiSwitchIndex = get_index_of_string(numberOfSuperordinateRailSwitches, allSuperordinateSwitchesNames, thisRailSectionName);
        // Serial.print("Dir 1, Switch/Antiswitch Index: "); Serial.print(thisSwitchIndex); Serial.print("/"); Serial.println(thisAntiSwitchIndex);
        if (thisSwitchIndex != -1) {
          // Serial.println("Dir 1, SwitchIndex Found");
          nextRailSectionName = trains[t]._plannedRailSectionsNames[0];
          // Serial.print("Post-Switch Rail Section: "); Serial.println(nextRailSectionName);
          thisPlannedSwitchIndex = get_index_of_string(3, allSubordinateNames[thisRailSectionIndex], nextRailSectionName);
          // Serial.print("Planned Switch Index: "); Serial.println(thisPlannedSwitchIndex);
          thisCurrentSwitchPosition = subordinateRailSwitches[thisSwitchIndex]._currentSwitch;
          thisPlannedSwitchPosition = allSubordinateSwitchesPositions[thisSwitchIndex][thisPlannedSwitchIndex];
          if (thisPlannedSwitchPosition != thisCurrentSwitchPosition) {;
            // Serial.print("Switch Current/Planned "); Serial.print(thisCurrentSwitchPosition); Serial.print("/"); Serial.println(thisPlannedSwitchPosition);
            subordinateRailSwitches[thisSwitchIndex].set_switch(thisPlannedSwitchPosition);
          }
        }
        if (thisAntiSwitchIndex != -1) {
          // Serial.println("Dir 1, AntiSwitchIndex Found");
          prevRailSectionName = trains[t]._currentRailSectionName;
          thisPlannedSwitchIndex = get_index_of_string(3, allSuperordinateNames[thisRailSectionIndex], prevRailSectionName);
          // Serial.print("Pre-Switch Rail Section: "); Serial.println(prevRailSectionName);
          // Serial.print("Planned Switch Index: "); Serial.println(thisPlannedSwitchIndex);
          thisCurrentSwitchPosition = superordinateRailSwitches[thisAntiSwitchIndex]._currentSwitch;
          thisPlannedSwitchPosition = allSuperordinateSwitchesPositions[thisAntiSwitchIndex][thisPlannedSwitchIndex];
          if (thisPlannedSwitchPosition != thisCurrentSwitchPosition) {
            // Serial.print("Switch Current/New "); Serial.print(thisCurrentSwitchPosition); Serial.print("/"); Serial.println(thisPlannedSwitchPosition);
            superordinateRailSwitches[thisAntiSwitchIndex].set_switch(thisPlannedSwitchPosition);
          }
        }
      }
      else if (thisPlannedDirection == -1) {
        thisSwitchIndex = get_index_of_string(numberOfSuperordinateRailSwitches, allSuperordinateSwitchesNames, thisRailSectionName);
        thisAntiSwitchIndex = get_index_of_string(numberOfSubordinateRailSwitches, allSubordinateSwitchesNames, thisRailSectionName);
        // Serial.print("Dir 1, Switch/Antiswitch Index: "); Serial.print(thisSwitchIndex); Serial.print("/"); Serial.println(thisAntiSwitchIndex);
        if (thisSwitchIndex != -1) {
          // Serial.println("Dir -1, SwitchIndex Found");
          nextRailSectionName = trains[t]._plannedRailSectionsNames[0];
          // Serial.print("Post-Switch Rail Section: "); Serial.println(nextRailSectionName);
          // Serial.print("Planned Switch Index: "); Serial.println(thisPlannedSwitchIndex);
          thisPlannedSwitchIndex = get_index_of_string(3, allSuperordinateNames[thisRailSectionIndex], nextRailSectionName);
          thisCurrentSwitchPosition = superordinateRailSwitches[thisSwitchIndex]._currentSwitch;
          thisPlannedSwitchPosition = allSuperordinateSwitchesPositions[thisSwitchIndex][thisPlannedSwitchIndex];
          if (thisPlannedSwitchPosition != thisCurrentSwitchPosition) {
            // Serial.print("Switch Current/New "); Serial.print(thisCurrentSwitchPosition); Serial.print("/"); Serial.println(thisPlannedSwitchPosition);
            superordinateRailSwitches[thisSwitchIndex].set_switch(thisPlannedSwitchPosition);
          }
        }
        if (thisAntiSwitchIndex != -1) {
          // Serial.println("Dir -1, AntiSwitchIndex Found");
          prevRailSectionName = trains[t]._currentRailSectionName;
          thisPlannedSwitchIndex = get_index_of_string(3, allSubordinateNames[thisRailSectionIndex], prevRailSectionName);
          // Serial.print("Pre-Switch Rail Section: "); Serial.println(prevRailSectionName);
          // Serial.print("Planned Switch Index: "); Serial.println(thisPlannedSwitchIndex);
          thisCurrentSwitchPosition = subordinateRailSwitches[thisAntiSwitchIndex]._currentSwitch;
          thisPlannedSwitchPosition = allSubordinateSwitchesPositions[thisAntiSwitchIndex][thisPlannedSwitchIndex];
          if (thisPlannedSwitchPosition != thisCurrentSwitchPosition) {
            // Serial.print("Switch Current/New "); Serial.print(thisCurrentSwitchPosition); Serial.print("/"); Serial.println(thisPlannedSwitchPosition);
            subordinateRailSwitches[thisAntiSwitchIndex].set_switch(thisPlannedSwitchPosition);
          }
        }
      }
      // Do Planned Rail Sections Switches
      for (int i = 0; i < trains[t]._numberOfPlannedRailSections; i++) {
        thisRailSectionName = trains[t]._plannedRailSectionsNames[i];
        // Serial.print("P"); Serial.print(trains[t]._numberOfPlannedRailSections); Serial.print(": "); Serial.println(thisRailSectionName);
        thisRailSectionIndex = get_index_of_string(numberOfRailSections, allRailSectionsNames, thisRailSectionName);
        if (thisPlannedDirection == 1) {
          thisSwitchIndex = get_index_of_string(numberOfSubordinateRailSwitches, allSubordinateSwitchesNames, thisRailSectionName);
          thisAntiSwitchIndex = get_index_of_string(numberOfSuperordinateRailSwitches, allSuperordinateSwitchesNames, thisRailSectionName);
          // Serial.print("Dir 1, Switch/Antiswitch Index: "); Serial.print(thisSwitchIndex); Serial.print("/"); Serial.println(thisAntiSwitchIndex);
          if (thisSwitchIndex != -1) {
            // Serial.println("Dir 1, SwitchIndex Found");
            if (i+1 < trains[t]._numberOfPlannedRailSections) {
              nextRailSectionName = trains[t]._plannedRailSectionsNames[i+1];
              // Serial.print("Post-Switch Rail Section: "); Serial.println(nextRailSectionName);
              thisPlannedSwitchIndex = get_index_of_string(3, allSubordinateNames[thisRailSectionIndex], nextRailSectionName);
              // Serial.print("Planned Switch Index: "); Serial.println(thisPlannedSwitchIndex);
              thisCurrentSwitchPosition = subordinateRailSwitches[thisSwitchIndex]._currentSwitch;
              thisPlannedSwitchPosition = allSubordinateSwitchesPositions[thisSwitchIndex][thisPlannedSwitchIndex];
              if (thisPlannedSwitchPosition != thisCurrentSwitchPosition) {;
                // Serial.print("Switch Current/Planned "); Serial.print(thisCurrentSwitchPosition); Serial.print("/"); Serial.println(thisPlannedSwitchPosition);
                subordinateRailSwitches[thisSwitchIndex].set_switch(thisPlannedSwitchPosition);
              }
            }
          }
          if (thisAntiSwitchIndex != -1) {
            // Serial.println("Dir 1, AntiSwitchIndex Found");
            if (i-1 >= 0) {
              prevRailSectionName = trains[t]._plannedRailSectionsNames[i-1];
            }
            else {
              prevRailSectionName = trains[t]._currentRailSectionName;
            }
            thisPlannedSwitchIndex = get_index_of_string(3, allSuperordinateNames[thisRailSectionIndex], prevRailSectionName);
            // Serial.print("Pre-Switch Rail Section: "); Serial.println(prevRailSectionName);
            // Serial.print("Planned Switch Index: "); Serial.println(thisPlannedSwitchIndex);
            thisCurrentSwitchPosition = superordinateRailSwitches[thisAntiSwitchIndex]._currentSwitch;
            thisPlannedSwitchPosition = allSuperordinateSwitchesPositions[thisAntiSwitchIndex][thisPlannedSwitchIndex];
            if (thisPlannedSwitchPosition != thisCurrentSwitchPosition) {
              // Serial.print("Switch Current/New "); Serial.print(thisCurrentSwitchPosition); Serial.print("/"); Serial.println(thisPlannedSwitchPosition);
              superordinateRailSwitches[thisAntiSwitchIndex].set_switch(thisPlannedSwitchPosition);
            }
          }
        }
        else if (thisPlannedDirection == -1) {
          thisSwitchIndex = get_index_of_string(numberOfSuperordinateRailSwitches, allSuperordinateSwitchesNames, thisRailSectionName);
          thisAntiSwitchIndex = get_index_of_string(numberOfSubordinateRailSwitches, allSubordinateSwitchesNames, thisRailSectionName);
          // Serial.print("Dir 1, Switch/Antiswitch Index: "); Serial.print(thisSwitchIndex); Serial.print("/"); Serial.println(thisAntiSwitchIndex);
          if (thisSwitchIndex != -1) {
            // Serial.println("Dir -1, SwitchIndex Found");
            if (i+1 < trains[t]._numberOfPlannedRailSections) {
              nextRailSectionName = trains[t]._plannedRailSectionsNames[i+1];
              // Serial.print("Post-Switch Rail Section: "); Serial.println(nextRailSectionName);
              // Serial.print("Planned Switch Index: "); Serial.println(thisPlannedSwitchIndex);
              thisPlannedSwitchIndex = get_index_of_string(3, allSuperordinateNames[thisRailSectionIndex], nextRailSectionName);
              thisCurrentSwitchPosition = superordinateRailSwitches[thisSwitchIndex]._currentSwitch;
              thisPlannedSwitchPosition = allSuperordinateSwitchesPositions[thisSwitchIndex][thisPlannedSwitchIndex];
              if (thisPlannedSwitchPosition != thisCurrentSwitchPosition) {
                // Serial.print("Switch Current/New "); Serial.print(thisCurrentSwitchPosition); Serial.print("/"); Serial.println(thisPlannedSwitchPosition);
                superordinateRailSwitches[thisSwitchIndex].set_switch(thisPlannedSwitchPosition);
              }
            }
          }
          if (thisAntiSwitchIndex != -1) {
            // Serial.println("Dir -1, AntiSwitchIndex Found");
            if (i-1 >= 0) {
              prevRailSectionName = trains[t]._plannedRailSectionsNames[i-1];
            }
            else {
              prevRailSectionName = trains[t]._currentRailSectionName;
            }
            thisPlannedSwitchIndex = get_index_of_string(3, allSubordinateNames[thisRailSectionIndex], prevRailSectionName);
            // Serial.print("Pre-Switch Rail Section: "); Serial.println(prevRailSectionName);
            // Serial.print("Planned Switch Index: "); Serial.println(thisPlannedSwitchIndex);
            thisCurrentSwitchPosition = subordinateRailSwitches[thisAntiSwitchIndex]._currentSwitch;
            thisPlannedSwitchPosition = allSubordinateSwitchesPositions[thisAntiSwitchIndex][thisPlannedSwitchIndex];
            if (thisPlannedSwitchPosition != thisCurrentSwitchPosition) {
              // Serial.print("Switch Current/New "); Serial.print(thisCurrentSwitchPosition); Serial.print("/"); Serial.println(thisPlannedSwitchPosition);
              subordinateRailSwitches[thisAntiSwitchIndex].set_switch(thisPlannedSwitchPosition);
            }
          }
        }
      }
    }
  }
}

void update_oled_display() {
  // Serial.println("update_oled_display()");
  display.clearDisplay();

  display.setCursor(0,0);
  display.print(""); display.print(trains[0]._trainName); display.println("");
  display.setCursor(0,10);
  display.print("State: ");
  if (trains[0]._idleStatus == true) {
    display.print("IDLE "); display.print(trains[0]._currentIdleTime); display.print("/"); display.print(trains[0]._targetIdleTime); display.print(" ");
  }
  if (trains[0]._readyToGenerateTarget == true) {
    display.print("P1"); display.print(" ");
  }
  if (trains[0]._readyToPathfindTarget == true) {
    display.print("P2"); display.print(" ");
  }
  if (trains[0]._readyToMoveStatus == true) {
    display.print("M"); display.print(" ");
  }
  if (trains[0]._emergencyStopStatus == true) {
    display.print("EMER"); display.print(" ");
  }
  if (trains[0]._switchDirectionStatus == true) {
    display.print("SWD"); display.print(" ");
  }
  if (trains[0]._arriveStation == true) {
    display.print("ARR"); display.print(" ");
  }
  if (trains[0]._departStation == true) {
    display.print("DEP"); display.print(" ");
  }
  display.println("");

  display.setCursor(0,20);
  display.print("Curr: "); display.print(trains[0]._currentRailSectionName); display.print("");
  
  display.setCursor(0,30);
  if (trains[0]._departStation == true) {
    display.print("> DEPART "); display.print(trains[0]._currentRailStationName); display.println(" <");
  }
  else if (trains[0]._arriveStation == true) {
    display.print("> ARRIVE "); display.print(trains[0]._currentRailStationName); display.println(" <");
  }
  else if (trains[0]._idleStatus == true) {
    display.print("- OCCUPY "); display.print(trains[0]._currentRailStationName); display.println(" -");
  }
  else if (trains[0]._readyToMoveStatus == true) { // Simply travelling
    display.print("> ONROUTE TO "); display.print(trains[0]._targetRailSectionName); display.println(" <");
  }

  display.setCursor(0,40);
  if (trains[0]._numberOfPlannedRailSections > 0) {
    display.print(trains[0]._currentRailSectionName);
    for (int i = 0; i < 3; i++) {
      if (trains[0]._plannedRailSectionsNames[i] != "NONE") {
        display.print(" -> "); display.print(trains[0]._plannedRailSectionsNames[i]);
      }
      else {
        break;
      }
    }
  }

  display.setCursor(0,50);
  display.print("R"); display.print(trains[0]._motorReverser); display.print("/"); display.print(trains[0]._plannedDirection); display.print(" P"); display.print(trains[0]._numberOfPlannedRailSections); 
  display.print(" V"); display.print(trains[0]._motorSpeed); display.print("/A"); display.print(trains[0]._motorAccelerationTarget); display.print("/"); display.println(trains[0]._motorSpeedTarget);
  // Rail
  // display.print("[Rail] ");
  // int thisIndex;
  // for (int i = 0; i < rail._numberOfRailSectionsOccupied; i++) {
  //   thisIndex = rail._railSectionsIndicesOccupied[i];
  //   display.print(allRailSectionsNames[thisIndex]);
  //   display.print(" ");
  // }

  // Station

  // Display
  display.display();
}