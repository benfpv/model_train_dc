int get_random_target_index(int arraySize, int currentIndex, int direction);

int get_closest_zero_int(int myArraySize, int myArray[]);
int get_closest_zero_index(int myArraySize, int myArray[]);
void get_closest_zero_indices(int myArraySize, int myArray[], int closestZeroIndices[]);
int limit_int(int x, int min, int max);

int get_polarity_of_int(int integer);

int get_index_of_int(int arrayOfIntsSize, int arrayOfInts[], int intToFind);
void get_indices_of_int(int arrayOfIntsSize, int arrayOfInts[], int intToFind, int outputIndices[]);
int get_index_of_string(int arrayOfStringsSize, String arrayOfStrings[], String stringToFind);
void get_indices_of_string(int arrayOfStringsSize, String arrayOfStrings[], String stringToFind, int outputIndices[]);
int get_index_of_char(int arrayOfCharsSize, char arrayOfChars[], char charToFind);
void get_indices_of_char(int arrayOfCharsSize, char arrayOfChars[], char charToFind, int outputIndices[]);

void get_strings_unique_from_dir_name_indices(String allNameArrays[][10], int dirIndicesSize, int dirIndices[], int nameIndices[], String names[]);
void get_strings_unique_indices_from_dir_name_indices(String allNameArrays[][10], int dirIndicesSize, int dirIndices[], int nameIndices[], String names[], int newDirIndices[], int newNameIndices[]);
void get_strings_chars_unique_indices_from_dir_name_indices(String allNameArrays[][10], char allCharArrays[][10], int dirIndicesSize, int dirIndices[], int nameIndices[], String names[], char chars[], int newDirIndices[], int newNameIndices[]);

void set_int_array_value(int myArraySize, int myArray[], int newValue);
void set_double_array_value(int myArraySize, double myArray[], double newValue);
void set_string_array_value(int myArraySize, String myArray[], String newValue);
void set_char_array_value(int myArraySize, char myArray[], char newValue);

void roll_int_array(int myArraySize, int myArray[], int direction, int newArray[]);
void roll_string_array(int myArraySize, String myArray[], int direction, String newArray[]);
void roll_char_array(int myArraySize, char myArray[], int direction, char newArray[]);

int get_sizeof_int_array_positive_values(int myArraySize, int myArray[]);
int get_sizeof_string_array_not_none(int myArraySize, String myArray[]);

void transfer_int_array_values(int beginArraySize, int beginArray[], int targetArraySize, int targetArray[]);

void print_array_values(int myArraySize, int myArray[]);
void print_array_doubles(int myArraySize, double myArray[]);
void print_array_strings(int myArraySize, String myArray[]);
void print_array_chars(int myArraySize, char myArray[]);

void get_single_relevant_dir_name_indices(int allNameArraysSize, String allNameArrays[][10], String currentName, int dirIndices[], int ordinateIndices[]);
void get_identicals_between_int_arrays(int currentArraySize, int currentArray[], int targetArraySize, int targetArray[], int identicalsArray[]);
void get_identicals_between_int_arrays_relay_name_indices(int currentDirIndicesSize, int currentDirIndices[], int currentNameIndices[], int targetDirIndicesSize, int targetDirIndices[], int targetNameIndices[], int identicalsArray[], int identicalCurrentNameIndices[], int identicalTargetNameIndices[]);
void get_subordinate_dir_name_indices(String allNameArrays[][10], String currentName, int relevantIndicesSize, int dirIndices[], int ordinateIndices[], int subordinateDirIndices[], int subordinateNameIndices[]);
void get_superordinate_dir_name_indices(String allNameArrays[][10], String currentName, int relevantIndicesSize, int dirIndices[], int ordinateIndices[], int superordinateDirIndices[], int superordinateNameIndices[]);

void get_simple_name_index_differences(int identicalDirIndicesSize, int currentNameIndices[], int targetNameIndices[], int pathfindSimpleNameIndexDifferences[]);
void get_path_indices_current_to_target(int identicalCurrentNameIndex, int shortestPathfindSimple, int pathIndices[]);
void get_path_names_current_to_target(String allNameArrays[][10], int pathDirIndex, int pathNameIndicesSize, int pathNameIndices[], String pathNames[]);