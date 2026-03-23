# Model_Train_DC
Model_Train_DC is an arduino project/prototype which aims to allow a DC-powered model train to sequentially navigate and travel between most to any configuration of rail and rail stations, with minimal user inputs required.
Navigation is achieved through the following:
1. The user sets up their railway configuration, with proximity sensors marking the beginning and end of each rail section, as well as train stations/stops. A couple of rules must be followed.
3. The user then creates a folder tree to reflect the organized rail structure - the folder tree acts as a sort of map of the rail sections.
4. The included python script 'Rail_Parser.py' is used to populate required vectors in the Arduino script 'Model_Train_DC.ino' according to the specific requirements of the user's railway.
5. The user manually updates some parameters in 'Model_Train_DC.ino' according to the specific requirements of their railway.
6. The 'Model_Train_DC.ino' Arduino script is uploaded to the target Arduino, and run.
7. The train starts from a predetermined rail section, and attempts to pathfind to user-declared rail stations. Once it reaches a target rail station, it idles there for some time, before pathfinding to the next target, and so forth.

# Important
- Currently, the scripts only support 1 train running on the railway. However, it was designed to be able to support multiple trains eventually. A couple of functions must be added & tested for multiple trains support (e.g., train traffic management to avoid collision).

# Known Issues
- Currently, all rail sections must be adjacent to eachother, and any stretch of rail that is not included in any rail section must be of length < the length of the shortest train on that rail.

# Future Directions
Possible future directions for this project includes:
- More complex train navigation, where train can navigate from any one station to any other station. Currently, trains can only navigate between stations that do not require direction reversal.
- Ability to run DCC trains.
- Ability to run multiple DCC trains on the same railway config. Recommends automatic traffic management functions to avoid collisions.
- Ability to run multiple DC trains on the same railway config, by powering individual rail sections accordingly. Recommends automatic traffic management functions to avoid collisions.

# Repository Contents
1. (Folder) Configs — ready-to-use track layout configurations (RAIL_SIMPLE, DUALSWITCH, MIDSPLIT). Each contains a pre-populated `model_train_dc.ino` and a matching `Rail_Parser.py`.
2. (Folder) tools — utility scripts. Run `tools/Rail_Parser.py` from the project root (see script header for details).
3. funcs_arduino.cpp / .h
4. Instructions.pdf
5. model_train_dc.ino
6. Proximity_IR.cpp / .h
7. Rail_DC.cpp / .h
8. Rail_Switch_DC.cpp / .h
9. README.md
10. Train_DC.cpp / .h

# Software Prerequisites (on typical OS)
1. Arduino IDE.
2. The following Arduino packages (for output to I2C OLED display): Wire.h, Adafruit_GFX.h, Adafruit_SSD1306.h.
3. Python (for running `tools/Rail_Parser.py`).

# Instructions for Use - Summary
For detailed instructions for use, please refer to the included `Instructions.pdf` file.
In summary, the following must be completed by the user:
1. Setup rail configuration in-real-life (IRL).
	a. Divide the railway into separate rail sections.
	b. Place rail stations within designated rail sections.
2. Create a folder tree which reflects the IRL rail configuration.
3. Edit and run `tools/Rail_Parser.py` **from the project root directory** (`cd path/to/model_train_dc`, then `python tools/Rail_Parser.py`). Alternatively, copy and run the pre-configured `Rail_Parser.py` from the matching `Configs/<LAYOUT>/` folder.
4. Edit “Model_Train_DC.ino” Arduino script.
	a. Edit the rail parameters and connect the rail-section and rail-station sensors, motor drivers for the trains, and motor drivers for the switches.
	b. Edit the train parameters.
5. Connect the I2C Display (recommended).
	a. I used an I2C OLED Display of 128x64 resolution, and the “model_train_dc.ino” script reflects this. Thus, if you would like to run without a display, or on a different display, the script needs to be changed accordingly.
6. Connect Motor Drivers Outputs to their Targets (e.g., Trains & Switches).
7. Connect Power to the Arduino and Motor Drivers (for the Trains & Switches).
8. Place the Train(s) at their Starting Rail Stations.
9. Upload the “Model_Train_DC.ino” Arduino script to the Arduino Board.
