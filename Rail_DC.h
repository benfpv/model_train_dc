/*
    Rail_DC.h - Library for handling DC Rail logic.
    Created by benfpv.
    Released into the public domain.

    Assumptions:
    - Trains must fit into all stations.
    - Trains must start behind any gates+distance sensors.

*/

#ifndef Rail_DC_h
#define Rail_DC_h

#include "Arduino.h"

class Rail_DC
{
  public:
    // Attributes - Identity
    String _railName;
    // Attributes - Prox Changes
    int _numberOfBeginProxActivatedThisLoop = 0;
    int _beginProxIndicesActivatedThisLoop[10];
    int _numberOfEndProxActivatedThisLoop = 0;
    int _endProxIndicesActivatedThisLoop[10];
    
    int _numberOfBeginProxDeactivatedThisLoop = 0;
    int _beginProxIndicesDeactivatedThisLoop[10];
    int _numberOfEndProxDeactivatedThisLoop = 0;
    int _endProxIndicesDeactivatedThisLoop[10];
    // Attributes - Occupied Rail Sections/Stations
    int _numberOfRailSectionsOccupied = 0;
    int _railSectionsIndicesOccupied[10];
    int _numberOfNewRailSectionsOccupiedThisLoop = 0;
    int _newRailSectionsIndicesOccupiedThisLoop[10];
    int _numberOfRailSectionsPoppedThisLoop = 0;
    int _railSectionsIndicesPoppedThisLoop[10];

    int _numberOfRailStationsOccupied = 0;
    int _railStationsIndicesOccupied[10];
    int _numberOfNewRailStationsOccupiedThisLoop = 0;
    int _newRailStationsIndicesOccupiedThisLoop[10];
    int _numberOfRailStationsPoppedThisLoop = 0;
    int _railStationsIndicesPoppedThisLoop[10];
    // Functions - Setup
    Rail_DC(String railName);
    void clear_attributes();
    void clear_prox_loop_attributes();
    void clear_rail_loop_attributes();
    // Functions - Print
    void print_rail_sections_occupied(String allRailSectionsNames[]);
    void print_new_rail_sections_occupied_this_loop(String allRailSectionsNames[]);
    void print_rail_sections_popped_this_loop(String allRailSectionsNames[]);
    
    void print_rail_stations_occupied(String allRailStationsNames[]);
    void print_new_rail_stations_occupied_this_loop(String allRailStationsNames[]);
    void print_rail_stations_popped_this_loop(String allRailStationsNames[]);
    // Functions - Prox Changes
    
    // Functions - Add/Remove Occupieds
    void add_rail_section_occupied_by_name(int numberOfRailSections, String allRailSectionsNames[], String currentRailSectionName);
    void pop_rail_section_occupied_by_name(int numberOfRailSections, String allRailSectionsNames[], String currentRailSectionName);
    void add_rail_station_occupied_by_name(int numberOfRailStations, String allRailStationsNames[], String currentRailStationName);
    void pop_rail_station_occupied_by_name(int numberOfRailStations, String allRailStationsNames[], String currentRailStationName);
};

#endif