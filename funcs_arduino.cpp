#include "Arduino.h"
#include "funcs_arduino.h"

int get_random_target_index(int arraySize, int currentIndex, int direction) {
  int targetIndex = currentIndex;
  if (direction == 0) {
    while (targetIndex == currentIndex) {
      targetIndex = random(arraySize);
    }
  }
  else if (direction > 0) {
    while (targetIndex <= currentIndex) {
      targetIndex = random(arraySize);
    }
  }
  else if (direction < 0) {
    while (targetIndex >= currentIndex) {
      targetIndex = random(arraySize);
    }
  }
  return targetIndex;
}

int get_closest_zero_int(int myArraySize, int myArray[]) {
  int thisInt;
  int closestPositiveInt = 999;
  int closestNegativeInt = -999;
  for (int i = 0; i < myArraySize; i++) {
    thisInt = myArray[i];
    if (thisInt == 0) {
      return thisInt;
    }
    else if (thisInt > 0) {
      if (myArray[i] < closestPositiveInt) {
        closestPositiveInt = myArray[i];
      }
    }
    else if (thisInt < 0) {
      if (myArray[i] > closestNegativeInt) {
        closestNegativeInt = myArray[i];
      }
    }
  }
  if (closestPositiveInt < abs(closestNegativeInt)) {
    return closestPositiveInt;
  }
  else if (abs(closestNegativeInt) < closestPositiveInt) {
    return closestNegativeInt;
  }
}

int get_closest_zero_index(int myArraySize, int myArray[]) {
  int thisInt;
  int closestPositiveInt = 999;
  int closestNegativeInt = -999;
  int closestPositiveIndex;
  int closestNegativeIndex;
  for (int i = 0; i < myArraySize; i++) {
    thisInt = myArray[i];
    if (thisInt == 0) {
      return i;
    }
    else if (thisInt > 0) {
      if (thisInt < closestPositiveInt) {
        closestPositiveInt = thisInt;
        closestPositiveIndex = i;
      }
    }
    else if (thisInt < 0) {
      if (thisInt > closestNegativeInt) {
        closestNegativeInt = thisInt;
        closestNegativeIndex = i;
      }
    }
  }
  if (closestPositiveInt < abs(closestNegativeInt)) {
    return closestPositiveIndex;
  }
  else if (abs(closestNegativeInt) < closestPositiveInt) {
    return closestNegativeIndex;
  }
}

void get_closest_zero_indices(int myArraySize, int myArray[], int closestZeroIndices[]) {
  int thisInt;
  bool thisNewClosest;
  int closestPositiveInt = 999;
  int closestNegativeInt = -999;
  int closestPositiveIndex;
  int closestNegativeIndex;
  for (int i = 0; i < myArraySize; i++) {
    thisNewClosest = false;
    thisInt = myArray[i];
    if (thisInt == 0) {
      closestZeroIndices[i] = i;
    }
    else if (thisInt > 0) {
      if (thisInt <= closestPositiveInt) {
        closestPositiveInt = thisInt;
        closestPositiveIndex = i;
        thisNewClosest = true;
      }
    }
    else if (thisInt < 0) {
      if (thisInt >= closestNegativeInt) {
        closestNegativeInt = thisInt;
        closestNegativeIndex = i;
        thisNewClosest = true;
      }
    }
    if (thisNewClosest == true) {
      if (closestPositiveInt < abs(closestNegativeInt)) {
        closestZeroIndices[i] = i;
      }
      else if (abs(closestNegativeInt) < closestPositiveInt) {
        closestZeroIndices[i] = i;
      }
    }
  }
}

int limit_int(int x, int min, int max) {
    if (x < min) {
        return min;
    }
    else if (x > max) {
        return max;
    }
    return x;
}

int get_polarity_of_int(int integer) {
  if (integer == 0) {
    return 0;
  }
  else if (integer > 0) {
    return 1;
  }
  else {
    return -1;
  }
}

int get_index_of_int(int arrayOfIntsSize, int arrayOfInts[], int intToFind) {
  for (int i = 0; i < arrayOfIntsSize; i++) {
    if (arrayOfInts[i] == intToFind) {
        return i;
    }
  }
  return -1;
}

void get_indices_of_int(int arrayOfIntsSize, int arrayOfInts[], int intToFind, int outputIndices[]) {
  int numberOfOutputIndices = 0;
  for (int i = 0; i < arrayOfIntsSize; i++) {
      if (arrayOfInts[i] == intToFind) {
          outputIndices[numberOfOutputIndices] = i;
          numberOfOutputIndices += 1;
      }
  }
}

int get_index_of_string(int arrayOfStringsSize, String arrayOfStrings[], String stringToFind) {
    // Serial.print("arrayOfStringsSize: ");
    // Serial.println(arrayOfStringsSize);
    // Serial.print("stringToFind: ");
    // Serial.println(stringToFind);
    for (int i = 0; i < arrayOfStringsSize; i++) {
        // Serial.print("arrayOfStrings[i]: ");
        // Serial.println(arrayOfStrings[i]);
        if (arrayOfStrings[i].equals(stringToFind)) {
            // Serial.print("indexOfString: ");
            // Serial.println(i);
            return i;
        }
    }
    // Serial.println("indexOfString: Not found.");
    return -1;
}

void get_indices_of_string(int arrayOfStringsSize, String arrayOfStrings[], String stringToFind, int outputIndices[]) {
  int numberOfOutputIndices = 0;
  for (int i = 0; i < arrayOfStringsSize; i++) {
    if (arrayOfStrings[i].equals(stringToFind)) {
      outputIndices[numberOfOutputIndices] = i;
      numberOfOutputIndices += 1;
    }
  }
  // Serial.println("indexOfString: Not found.");
}

int get_index_of_char(int arrayOfCharsSize, char arrayOfChars[], char charToFind) {
    for (int i = 0; i < arrayOfCharsSize; i++) {
        if (arrayOfChars[i] == charToFind) {
            return i;
        }
    }
    return -1;
}

void get_indices_of_char(int arrayOfCharsSize, char arrayOfChars[], char charToFind, int outputIndices[]) {
  int numberOfOutputIndices = 0;
  for (int i = 0; i < arrayOfCharsSize; i++) {
    if (arrayOfChars[i] == charToFind) {
        outputIndices[numberOfOutputIndices] = i;
        numberOfOutputIndices += 1;
    }
  }
}

void get_strings_unique_from_dir_name_indices(String allNameArrays[][10], int dirIndicesSize, int dirIndices[], int nameIndices[], String names[]) {
  int dirIndex;
  int nameIndex;
  String thisName = "NONE";
  bool addThisName;
  int nameCounter = 0;
  for (int i = 0; i < dirIndicesSize; i++) {
    dirIndex = dirIndices[i];
    nameIndex = nameIndices[i];
    thisName = allNameArrays[dirIndex][nameIndex];
    addThisName = true;
    for (int x = 0; x < nameCounter; x++) {
      if (thisName.equals(names[x])) {
        addThisName = false;
      }
    }
    if (addThisName == true) {
      names[nameCounter] = thisName;
      nameCounter += 1;
    }
  }
}

void get_strings_unique_indices_from_dir_name_indices(String allNameArrays[][10], int dirIndicesSize, int dirIndices[], int nameIndices[], String names[], int newDirIndices[], int newNameIndices[]) {
  int dirIndex;
  int nameIndex;
  String thisName = "NONE";
  bool addThisName;
  int nameCounter = 0;
  for (int i = 0; i < dirIndicesSize; i++) {
    dirIndex = dirIndices[i];
    nameIndex = nameIndices[i];
    thisName = allNameArrays[dirIndex][nameIndex];
    addThisName = true;
    for (int x = 0; x < nameCounter; x++) {
      if (thisName.equals(names[x])) {
        addThisName = false;
      }
    }
    if (addThisName == true) {
      names[nameCounter] = thisName;
      newDirIndices[nameCounter] = dirIndex;
      newNameIndices[nameCounter] = nameIndex;
      nameCounter += 1;
    }
  }
}

void get_strings_chars_unique_indices_from_dir_name_indices(String allNameArrays[][10], char allCharArrays[][10], int dirIndicesSize, int dirIndices[], int nameIndices[], String names[], char chars[], int newDirIndices[], int newNameIndices[]) {
  int dirIndex;
  int nameIndex;
  String thisName = "NONE";
  char thisChar;
  bool addThisName;
  int nameCounter = 0;
  for (int i = 0; i < dirIndicesSize; i++) {
    addThisName = true;
    dirIndex = dirIndices[i];
    nameIndex = nameIndices[i];
    thisName = allNameArrays[dirIndex][nameIndex];
    for (int x = 0; x < nameCounter; x++) {
      if (thisName.equals(names[x])) {
        addThisName = false;
      }
    }
    if (addThisName == true) {
      thisChar = allCharArrays[dirIndex][nameIndex];
      names[nameCounter] = thisName;
      chars[nameCounter] = thisChar;
      newDirIndices[nameCounter] = dirIndex;
      newNameIndices[nameCounter] = nameIndex;
      nameCounter += 1;
    }
  }
}

void get_chars_from_dir_name_indices(char allCharArrays[][10], int dirIndicesSize, int dirIndices[], int nameIndices[], char chars[]) {
  int dirIndex;
  int nameIndex;
  char thisChar;
  bool addThisChar;
  int charCounter = 0;
  for (int i = 0; i < dirIndicesSize; i++) {
    addThisChar = true;
    dirIndex = dirIndices[i];
    nameIndex = nameIndices[i];
    thisChar = allCharArrays[dirIndex][nameIndex];
    for (int x = 0; x < charCounter; x++) {
      if (thisChar == chars[x]) {
        addThisChar = false;
      }
    }
    if (addThisChar == true) {
      chars[charCounter] = thisChar;
      charCounter += 1;
    }
  }
}

void set_int_array_value(int myArraySize, int myArray[], int newValue) {
  for (int i = 0; i < myArraySize; i++) {
    myArray[i] = newValue;
  }
}

void set_double_array_value(int myArraySize, double myArray[], double newValue) {
  for (int i = 0; i < myArraySize; i++) {
    myArray[i] = newValue;
  }
}

void set_string_array_value(int myArraySize, String myArray[], String newValue) {
  for (int i = 0; i < myArraySize; i++) {
    myArray[i] = newValue;
  }
}

void set_char_array_value(int myArraySize, char myArray[], char newValue) {
  for (int i = 0; i < myArraySize; i++) {
    myArray[i] = newValue;
  }
}

void roll_int_array(int myArraySize, int myArray[], int direction, int newArray[]) {
  // direction -1 means shift leftward, direction +1 means shift rightward
  if (direction == -1) { //roll leftmost to rightmost; good for queue elimination
    newArray[myArraySize-1] = myArray[0];
    for (int i = 0; i < myArraySize-1; i++) {
      newArray[i] = myArray[i+1];
    }
  }
  else if (direction == 1) { //roll rightmost to leftmost; good for timeseries updates
    newArray[0] = myArray[myArraySize-1];
    for (int i = 0; i < myArraySize-1; i++) {
      newArray[i+1] = myArray[i];
    }
  }
}

void roll_string_array(int myArraySize, String myArray[], int direction, String newArray[]) {
  // direction -1 means shift leftward, direction +1 means shift rightward
  if (direction == -1) { //roll leftmost to rightmost; good for queue elimination
    newArray[myArraySize-1] = myArray[0];
    for (int i = 0; i < myArraySize-1; i++) {
      newArray[i] = myArray[i+1];
    }
  }
  else if (direction == 1) { //roll rightmost to leftmost; good for timeseries updates
    newArray[0] = myArray[myArraySize-1];
    for (int i = 0; i < myArraySize-1; i++) {
      newArray[i+1] = myArray[i];
    }
  }
}

void roll_char_array(int myArraySize, char myArray[], int direction, char newArray[]) {
  // direction -1 means shift leftward, direction +1 means shift rightward
  if (direction == -1) { //roll leftmost to rightmost; good for queue elimination
    newArray[myArraySize-1] = myArray[0];
    for (int i = 0; i < myArraySize-1; i++) {
      newArray[i] = myArray[i+1];
    }
  }
  else if (direction == 1) { //roll rightmost to leftmost; good for timeseries updates
    newArray[0] = myArray[myArraySize-1];
    for (int i = 0; i < myArraySize-1; i++) {
      newArray[i+1] = myArray[i];
    }
  }
}

int get_sizeof_int_array_positive_values(int myArraySize, int myArray[]) {
  int sizeCounter = 0;
  for (int i = 0; i < myArraySize; i++) {
    if (myArray[i] >= 0) {
      sizeCounter += 1;
    }
    else {
      break;
    }
  }
  return sizeCounter;
}

int get_sizeof_string_array_not_none(int myArraySize, String myArray[]) {
  int sizeCounter = 0;
  for (int i = 0; i < myArraySize; i++) {
    if (!myArray[i].equals("NONE")) {
      sizeCounter += 1;
    }
    else {
      break;
    }
  }
  return sizeCounter;
}

void transfer_int_array_values(int beginArraySize, int beginArray[], int targetArraySize, int targetArray[]) {
  for (int i = 0; i < targetArraySize; i++) {
    targetArray[i] = beginArray[i];
  }
}

void print_array_values(int myArraySize, int myArray[]) {
  for (int i = 0; i < myArraySize; i++) {
    if (i > 0) {
      Serial.print(", ");
    }
    Serial.print(myArray[i]);
  }
  Serial.print("\n");
}

void print_array_doubles(int myArraySize, double myArray[]) {
  for (int i = 0; i < myArraySize; i++) {
    if (i > 0) {
      Serial.print(", ");
    }
    Serial.print(myArray[i]);
  }
  Serial.print("\n");
}

void print_array_strings(int myArraySize, String myArray[]) {
  for (int i = 0; i < myArraySize; i++) {
    if (i > 0) {
      Serial.print(", ");
    }
    Serial.print(myArray[i]);
  }
  Serial.print("\n");
}

void print_array_chars(int myArraySize, char myArray[]) {
  for (int i = 0; i < myArraySize; i++) {
    if (i > 0) {
      Serial.print(", ");
    }
    Serial.print(myArray[i]);
  }
  Serial.print("\n");
}

void get_single_relevant_dir_name_indices(int allNameArraysSize, String allNameArrays[][10], String currentName, int dirIndices[], int ordinateIndices[]) {
  int ordinatesCounter = 0;
  // Serial.print("allNameArraysSize: ");
  // Serial.println(allNameArraysSize);
  // Iterate search each dirArray
  for (int i = 0; i < allNameArraysSize; i++) {
    int thisArraySize = sizeof(allNameArrays[i])/sizeof(allNameArrays[i][0]);
    // Serial.print("thisArraySize: ");
    // Serial.println(thisArraySize);
    if (thisArraySize > 1) {
      for (int nameIndex = 0; nameIndex < thisArraySize; nameIndex++) {
        if (allNameArrays[i][nameIndex].equals(currentName)) {
          // Serial.print("Single Name Equal & Indices (Dir, Name): ");
          // Serial.print(allNameArrays[i][nameIndex]);
          // Serial.print(", ");
          // Serial.print(i);
          // Serial.print(", ");
          // Serial.println(nameIndex);
          dirIndices[ordinatesCounter] = i;
          ordinateIndices[ordinatesCounter] = nameIndex;
          ordinatesCounter += 1;
        }
      }
    }
  }
}

void get_identicals_between_int_arrays(int currentArraySize, int currentArray[], int targetArraySize, int targetArray[], int identicalsArray[]) {
  int currentValue;
  int targetValue;
  int identicalsCounter = 0;
  // Serial.println("identicals between arrays: ");
  for (int i_current = 0; i_current < currentArraySize; i_current++) {
    for (int i_target = 0; i_target < targetArraySize; i_target++) {
      currentValue = currentArray[i_current];
      targetValue = targetArray[i_target];
      if (currentValue == targetValue) {
        identicalsArray[identicalsCounter] = currentValue;
        // Serial.print(currentValue);
        // Serial.print("&");
        // Serial.print(targetValue);
        // Serial.print(", ");
        identicalsCounter += 1;
      }
    }
  }
}

void get_identicals_between_int_arrays_relay_name_indices(int currentDirIndicesSize, int currentDirIndices[], int currentNameIndices[], int targetDirIndicesSize, int targetDirIndices[], int targetNameIndices[], int identicalDirIndices[], int identicalCurrentNameIndices[], int identicalTargetNameIndices[]) {
  int currentDirIndex;
  int targetDirIndex;
  int currentNameIndex;
  int targetNameIndex;
  int identicalsCounter = 0;
  // Serial.println("identicals between arrays: ");
  for (int i_current = 0; i_current < currentDirIndicesSize; i_current++) {
    for (int i_target = 0; i_target < targetDirIndicesSize; i_target++) {
      currentDirIndex = currentDirIndices[i_current];
      targetDirIndex = targetDirIndices[i_target];
      currentNameIndex = currentNameIndices[i_current];
      targetNameIndex = targetNameIndices[i_target];
      if (currentDirIndex == targetDirIndex) {
        identicalDirIndices[identicalsCounter] = currentDirIndex;
        identicalCurrentNameIndices[identicalsCounter] = currentNameIndex;
        identicalTargetNameIndices[identicalsCounter] = targetNameIndex;
        // Serial.print(currentValue);
        // Serial.print("&");
        // Serial.print(targetValue);
        // Serial.print(", ");
        identicalsCounter += 1;
      }
    }
  }
}

void get_subordinate_dir_name_indices(String allNameArrays[][10], String currentName, int relevantIndicesSize, int dirIndices[], int ordinateIndices[], int subordinateDirIndices[], int subordinateNameIndices[]) {
  int thisNameArraySize;
  int dirIndex;
  int ordinateIndex;
  int subordinateIndex;
  int subordinateCounter = 0;
  for (int i = 0; i < relevantIndicesSize; i++) {
    dirIndex = dirIndices[i];
    thisNameArraySize = sizeof(allNameArrays[dirIndex])/sizeof(allNameArrays[dirIndex][0]);
    ordinateIndex = ordinateIndices[i];
    subordinateIndex = ordinateIndex + 1;
    if (!allNameArrays[dirIndex][subordinateIndex].equals("")) {
      subordinateDirIndices[subordinateCounter] = dirIndex;
      subordinateNameIndices[subordinateCounter] = subordinateIndex;
      subordinateCounter += 1;
      // Serial.print("dirIndex, subIndices, & Names (Ordinate, Sub): ");
      // Serial.print(dirIndex);
      // Serial.print("; ");
      // Serial.print(ordinateIndex);
      // Serial.print(", ");
      // Serial.print(subordinateIndex);
      // Serial.print(", ");
      // Serial.print(allNameArrays[dirIndex][ordinateIndex]);
      // Serial.print(", ");
      // Serial.println(allNameArrays[dirIndex][subordinateIndex]);
    }
  }
}

void get_superordinate_dir_name_indices(String allNameArrays[][10], String currentName, int relevantIndicesSize, int dirIndices[], int ordinateIndices[], int superordinateDirIndices[], int superordinateNameIndices[]) {
  int thisNameArraySize;
  int dirIndex;
  int ordinateIndex;
  int superordinateIndex;
  int superordinateCounter = 0;
  for (int i = 0; i < relevantIndicesSize; i++) {
    dirIndex = dirIndices[i];
    thisNameArraySize = sizeof(allNameArrays[dirIndex])/sizeof(allNameArrays[dirIndex][0]);
    ordinateIndex = ordinateIndices[i];
    superordinateIndex = ordinateIndex - 1;
    if (superordinateIndex >= 0) {
      superordinateDirIndices[superordinateCounter] = dirIndex;
      superordinateNameIndices[superordinateCounter] = superordinateIndex;
      superordinateCounter += 1;
      // Serial.print("dirIndex, superIndices, & Names (Super, Ordinate): ");
      // Serial.print(dirIndex);
      // Serial.print("; ");
      // Serial.print(superordinateIndex);
      // Serial.print(", ");
      // Serial.print(ordinateIndex);
      // Serial.print(", ");
      // Serial.print(allNameArrays[dirIndex][superordinateIndex]);
      // Serial.print(", ");
      // Serial.println(allNameArrays[dirIndex][ordinateIndex]);
    }
  }
}

void get_simple_name_index_differences(int identicalDirIndicesSize, int currentNameIndices[], int targetNameIndices[], int pathfindSimpleNameIndexDifferences[]) {
  int simpleNameIndexDifferences;
  for (int i = 0; i < identicalDirIndicesSize; i++) {
    pathfindSimpleNameIndexDifferences[i] = targetNameIndices[i] - currentNameIndices[i];
  }
}

void get_path_indices_current_to_target(int identicalCurrentNameIndex, int shortestPathfindSimple, int pathIndices[]) {
  for (int i = 0; i < abs(shortestPathfindSimple); i++) {
    if (shortestPathfindSimple < 0) {
      pathIndices[i] = identicalCurrentNameIndex - 1 -  i;
    }
    else if (shortestPathfindSimple > 0) {
      pathIndices[i] = identicalCurrentNameIndex + 1 + i;
    }
  }
}

void get_path_names_current_to_target(String allNameArrays[][10], int pathDirIndex, int pathNameIndicesSize, int pathNameIndices[], String pathNames[]) {
  int nameIndex;
  String thisName = "NONE";
  for (int i = 0; i < pathNameIndicesSize; i++) {
    nameIndex = pathNameIndices[i];
    thisName = allNameArrays[pathDirIndex][nameIndex];
    pathNames[i] = thisName;
  }
}

