/*
    Train_DC.h - Library for running a DC train.
    Created by benfpv.
    Released into the public domain.
*/

#ifndef Train_DC_h
#define Train_DC_h

#include "Arduino.h"

class Train_DC
{
    public:
        // Attributes - Train Identity
        String _trainName = "NONE";
        int _trainIndex;
        int _trainNavigationType; // 0 == randomly generate target each time, 1 == follow queue, 3 == follow randomly generated from queue.
        // Attributes - User Input Train Characteristics
        int _targetIdleTime = 9000; // Idle time in milliSeconds between reaching target and readyToMoveStatus.
        int _motorSpeedSlow = 30;
        int _motorSpeedMax = 34;
        int _motorAccelerationStopping = 2;
        int _motorAccelerationMax = 5;
        // Attributes - Status
        bool _idleStatus = false;
        bool _readyToGenerateTarget = false;
        bool _readyToPathfindTarget = false;
        bool _readyToMoveStatus = false;
        bool _departStation = false;
        bool _arriveStation = false;

        bool _triggeredStation = false;
        bool _emergencyStopStatus = false;

        bool _switchRealPositiveDirectionStatus = false;
        bool _switchDirectionStatus = false;
        // Attributes - Idle
        int _idleStartTime_ms;
        int _currentIdleTime;
        // Attributes - Rail Exploration
        int _realPositiveDirection = -1; // Real Direction when Reverser is Positive (1). 1 is Forward, -1 is Backward. Modulates reverser.
        // Attributes - Rail Localization - Current & Target (Must Reflect Present Truth)
        String _currentRailSectionName;
        // int _currentRailSectionIndex;
        int _currentRailSectionIndexInPlanned;
        int _currentRailSectionSwitch;
        // Target
        String _targetRailSectionName;
        // int _targetRailSectionIndex;
        // Target Queue
        int _targetRailSectionIndexInQueue = 0;
        int _numberOfTargetRailSectionsInQueue;
        String _targetRailSectionsNamesQueue[10]; // ONLY USED in conjunction with "set_target_rail_section_by_queue", or "set_target_rail_section_randomly_from_queue", NOT "set_target_rail_section_randomly".
        // Station
        String _currentRailStationName = "NONE";
        // Attributes - Rail Localization - Planned (Must Reflect Present Truth)
        int _plannedDirection = 0; // Fwd (or towards subordinate) = 1, Back (or towards superordinate) = -1 (remember the latter needs to reverse track directions).
        int _numberOfPlannedRailSections;
        String _plannedRailSectionsNames[10];
        // int _plannedRailSectionsDirIndices[10];
        // int _plannedRailSectionsNameIndices[10];
        int _plannedRailSectionsSwitches[10];
        // Attributes - Rail Localization - Subordinates and Superordinates
        int _numberOfSubordinateRailSections;
        String _subordinateRailSectionsNames[3];
        // int _subordinateRailSectionsDirIndices[3];
        // int _subordinateRailSectionsNameIndices[3];
        int _subordinateRailSectionsSwitches[3];
        int _numberOfSuperordinateRailSections;
        String _superordinateRailSectionsNames[3];
        // int _superordinateRailSectionsDirIndices[3];
        // int _superordinateRailSectionsNameIndices[3];
        int _superordinateRailSectionsSwitches[3];
        // Attributes - Motor Management
        int _motorReverser = 0; // This is the final true direction
        int _motorAccelerationTarget = 0;
        int _motorSpeed = 0;
        int _motorSpeedTarget = 0;
        // Functions - Train Initialization
        Train_DC(String trainName, int trainIndex, int trainNavigationType, int pin_EN, int pin_IN1, int pin_IN2, int targetIdleTime, int motorSpeedSlow, int motorSpeedMax, int motorAccelerationStopping, int motorAccelerationMax);
        void begin();
        // Functions - Rail Path Initialization
        void init_target_rail_section_queue(int numberOfTargetRailSectionsInQueue, String targetRailSectionsNamesQueue[]);
        void clear_current_localization_attributes();
        void clear_target_planned_localization_attributes();
        void clear_subordinate_superordinate_localization_attributes();
        // Functions - Rail Exploration

        // Functions - Rail Localization - Current & Target
        void print_localization();
        void set_starting_rail_section(int numberOfRailSections, String allRailSectionsNames[], int numberOfRailStations, String allRailStationsNames[], String currentRailSectionName);
        void set_current_rail_section(int numberOfRailSections, String allRailSectionsNames[], String currentRailSectionName);
        // Target
        void set_target_rail_section(int numberOfRailSections, String allRailSectionsNames[], String targetRailSectionName);
        void set_target_rail_section_randomly(int numberOfRailSections, String allRailSectionsNames[], int numberOfRailStations, String allRailStationsNames[], int direction);
        void set_target_rail_section_by_queue(int numberOfRailStations, String allRailStationsNames[]);
        // Functions - Rail Localization - Pathfind Planned Rail 
        void set_pathfind_current_to_target(int numberOfRailSections, String allRailSectionsDirs[][10], String allRailSectionsNames[], bool printDebug);
        // void pathfind_current_to_target(int numberOfRailSections, String allRailSectionsDirs[][10], bool printDebug);
        // Functions - Rail Localization - Subordinates & Superordinates
        void set_subordinate_rail_sections(int numberOfRailSections, String allRailSectionsDirs[][10], bool printDebug);
        void get_subordinate_rail_sections(int numberOfRailSections, String allRailSectionsDirs[][10], String currentRailSectionName, int numberOfSubordinateRailSections, String subordinateRailSectionsNames[3], bool printDebug);
        void set_superordinate_rail_sections(int numberOfRailSections, String allRailSectionsDirs[][10], bool printDebug);
        void get_superordinate_rail_sections(int numberOfRailSections, String allRailSectionsDirs[][10], String currentRailSectionName, int numberOfSuperordinateRailSections, String superordinateRailSectionsNames[3], bool printDebug);
        // High Level Functions - Rail Localization - High-level Update Rail Section(s) Info
        void update_travel_plan(int numberOfRailSections, String allRailSectionsNames[]);
        // Functions - Process Transitions
        void set_idle();
        void set_ready();
        // Functions - Direct Controls
        void set_motor_reverser(int motorReverserTargetInt);
        void increment_motor_speed();
        void emergency_stop();
    private:
        // Arduino Pins
        int _pin_EN;
        int _pin_IN1;
        int _pin_IN2;
};

#endif