import os
import sys
import time

# User Parameters
master_folder_name = "RAIL_MIDSPLIT" # Folder Name that contains a tree of connected rail sections, starting from the first section, clockwise.
ino_file_name = “model_train_dc.ino" # Name of the main arduino file to run
ino_params_begin_keyword = "// Rail_Parser Output Parameters Begin"
ino_params_end_keyword = "// Rail_Parser Output Parameters End"
dirs_txt_name = "dirs.txt"
pages_txt_name = "pages.txt"

# Script
class Main():
    def __init__(self):
        print("------- BEGIN -------")
        dirs, pages = Main.parse_folder(master_folder_name)
        allSubordinateNames, allSuperordinateNames = Main.get_subordinate_superordinate_names(dirs, pages)
        allSubordinateSwitchesPositions, allSubordinateSwitchesNames, allSuperordinateSwitchesPositions, allSuperordinateSwitchesNames = Main.get_subordinate_superordinate_switches_empty(dirs, pages, allSubordinateNames, allSuperordinateNames)
        Main.clear_ino()
        Main.output_to_ino(dirs, pages, allSubordinateNames, allSubordinateSwitchesPositions, allSubordinateSwitchesNames, allSuperordinateNames, allSuperordinateSwitchesPositions, allSuperordinateSwitchesNames)
        time.sleep(2)
        print("-------- END --------")

    def parse_folder(master_folder_name):
        #print("Main.parse_folder()")
        dirs, pages = Main.create_dirs_pages(master_folder_name)
        return dirs, pages
    
    def clear_ino():
        print("Main.clear_ino()")
        with open(ino_file_name, 'r') as ino_file:
            lines = ino_file.readlines()
        line_ino_begin = -1
        line_ino_end = -1
        line_count = 0
        for line in lines:
            if (ino_params_begin_keyword in line):
                line_ino_begin = line_count
            if (ino_params_end_keyword in line):
                line_ino_end = line_count
            line_count += 1
        print(" ***** ")
        print("Line Begin to End: {} to {}".format(lines[line_ino_begin], lines[line_ino_end]))
        print(" ***** ")
        if (line_ino_begin < line_ino_end) and (line_ino_begin != line_ino_end) and (line_ino_begin != -1) and (line_ino_end != -1):
            with open(ino_file_name, 'w') as new_ino_file:
                new_ino_file.writelines(lines[0:line_ino_begin+1])
                new_ino_file.writelines(lines[line_ino_end::])
        else:
            print("ERROR: line_ino_begin and/or line_ino_end lines are not found, or not in proper order.")
    
    def output_to_ino(dirs, pages, allSubordinateNames, allSubordinateSwitchesPositions, allSubordinateSwitchesNames, allSuperordinateNames, allSuperordinateSwitchesPositions, allSuperordinateSwitchesNames):
        #print("Main.output_to_ino()")
        len_dirs = len(dirs)
        len_pages = len(pages)
        line_ino_params = 1
        with open(ino_file_name, 'r') as ino_file:
            lines = ino_file.readlines()
        for line in lines:
            if (ino_params_begin_keyword in line):
                break
            line_ino_params += 1
        # Add masterDirName
        line_master_dir_name = 'String masterDirName = "' + master_folder_name + '";\n'
        print(line_master_dir_name)
        # Add Dirs
        # Demo: char dirs[][][] = { { {}, {} }, { {}, {} }, { {}, {} } }
        max_dir_len = Main.find_max_len_in_list_of_lists(dirs)
        lines_dirs = "String allRailSectionsDirs[" + str(len_dirs) + "][" + str(10) + "] = \n"
        lines_dirs += "{\n"
        dir_count = 0
        for i in dirs:
            lines_dirs += "{"
            name_count = 0
            for name in i:
                if (name_count != 0):
                    lines_dirs += ", "
                lines_dirs += '"' + name + '"'
                name_count += 1
            lines_dirs += "}"
            if (dir_count != len_dirs-1):
                lines_dirs += ", "
            if (dir_count % 4 == 0) and (dir_count != len_dirs-1):
                lines_dirs += "\n"
            dir_count += 1
        lines_dirs += "\n};\n"
        print(lines_dirs)
        # Add Pages
        lines_pages = "String allRailSectionsNames[] = \n"
        lines_pages += "{\n"
        page_count = 0
        for i in pages:
            if (page_count != 0):
                lines_pages += ", "
            lines_pages += '"' + i + '"'
            page_count += 1
        lines_pages += "\n};\n"
        print(lines_pages)
        # Add Subordinate Names
        lines_subordinateNames = "String allSubordinateNames[" + str(len_dirs) + "][3] = \n"
        lines_subordinateNames += "{\n"
        dir_count = 0
        for i in allSubordinateNames:
            lines_subordinateNames += "{"
            name_count = 0
            for name in i:
                if (name_count != 0):
                    lines_subordinateNames += ", "
                lines_subordinateNames += '"' + allSubordinateNames[dir_count][name_count] + '"'
                name_count += 1
            lines_subordinateNames += "}"
            if (dir_count != len_dirs-1):
                lines_subordinateNames += ", "
            if (dir_count % 4 == 0) and (dir_count != len_dirs-1):
                lines_subordinateNames += "\n"
            dir_count += 1
        lines_subordinateNames += "\n};\n"
        print(lines_subordinateNames)
        # Add Subordinate Switches Names
        lines_subordinateSwitchesNames = "String allSubordinateSwitchesNames[] = \n"
        lines_subordinateSwitchesNames += "{\n"
        dir_count = 0
        for i in allSubordinateSwitchesNames:
            lines_subordinateSwitchesNames += '"' + allSubordinateSwitchesNames[dir_count] + '"'
            if (dir_count != len(allSubordinateSwitchesNames)-1):
                lines_subordinateSwitchesNames += ", "
            if (dir_count % 4 == 0) and (dir_count != len(allSubordinateSwitchesNames)-1):
                lines_subordinateSwitchesNames += "\n"
            dir_count += 1
        lines_subordinateSwitchesNames += "\n};\n"
        print(lines_subordinateSwitchesNames)
        # Add Subordinate Switches
        lines_subordinateSwitches = "int allSubordinateSwitchesPositions[][3] = \n"
        lines_subordinateSwitches += "{\n"
        dir_count = 0
        for i in allSubordinateSwitchesPositions:
            value_count = 0
            lines_subordinateSwitches += "{"
            for value in i:
                if (value_count != 0):
                    lines_subordinateSwitches += ", "
                lines_subordinateSwitches += str(allSubordinateSwitchesPositions[dir_count][value_count])
                value_count += 1
            lines_subordinateSwitches += "}"
            if (dir_count != len(allSubordinateSwitchesPositions)-1):
                lines_subordinateSwitches += ", "
            if (dir_count % 4 == 0) and (dir_count != len(allSubordinateSwitchesPositions)-1):
                lines_subordinateSwitches += "\n"
            dir_count += 1
        lines_subordinateSwitches += "\n};\n"
        print(lines_subordinateSwitches)
        # Add Superordinate Names
        lines_superordinateNames = "String allSuperordinateNames[" + str(len_dirs) + "][3] = \n"
        lines_superordinateNames += "{\n"
        dir_count = 0
        for i in allSuperordinateNames:
            lines_superordinateNames += "{"
            name_count = 0
            for name in i:
                if (name_count != 0):
                    lines_superordinateNames += ", "
                lines_superordinateNames += '"' + allSuperordinateNames[dir_count][name_count] + '"'
                name_count += 1
            lines_superordinateNames += "}"
            if (dir_count != len_dirs-1):
                lines_superordinateNames += ", "
            if (dir_count % 4 == 0) and (dir_count != len_dirs-1):
                lines_superordinateNames += "\n"
            dir_count += 1
        lines_superordinateNames += "\n};\n"
        print(lines_superordinateNames)
        # Add Superordinate Switches Names
        lines_superordinateSwitchesNames = "String allSuperordinateSwitchesNames[] = \n"
        lines_superordinateSwitchesNames += "{\n"
        dir_count = 0
        for i in allSuperordinateSwitchesNames:
            lines_superordinateSwitchesNames += '"' + allSuperordinateSwitchesNames[dir_count] + '"'
            if (dir_count != len(allSubordinateSwitchesNames)-1):
                lines_superordinateSwitchesNames += ", "
            if (dir_count % 4 == 0) and (dir_count != len(allSubordinateSwitchesNames)-1):
                lines_superordinateSwitchesNames += "\n"
            dir_count += 1
        lines_superordinateSwitchesNames += "\n};\n"
        print(lines_superordinateSwitchesNames)
        # Add Superordinate Switches
        lines_superordinateSwitches = "int allSuperordinateSwitchesPositions[][3] = \n"
        lines_superordinateSwitches += "{\n"
        dir_count = 0
        for i in allSuperordinateSwitchesPositions:
            value_count = 0
            lines_superordinateSwitches += "{"
            for value in i:
                if (value_count != 0):
                    lines_superordinateSwitches += ", "
                lines_superordinateSwitches += str(allSuperordinateSwitchesPositions[dir_count][value_count])
                value_count += 1
            lines_superordinateSwitches += "}"
            if (dir_count != len(allSuperordinateSwitchesPositions)-1):
                lines_superordinateSwitches += ", "
            if (dir_count % 4 == 0) and (dir_count != len(allSuperordinateSwitchesPositions)-1):
                lines_superordinateSwitches += "\n"
            dir_count += 1
        lines_superordinateSwitches += "\n};\n"
        print(lines_superordinateSwitches)
        # Confirm Continue with User Input
        Main.user_input_confirm()
        # Add lines to .ino script/file
        if (len(lines) > line_ino_params):
            lines[line_ino_params] = line_master_dir_name
            lines[line_ino_params] += lines_dirs
            lines[line_ino_params] += lines_pages
            lines[line_ino_params] += lines_subordinateNames
            lines[line_ino_params] += lines_subordinateSwitchesNames
            lines[line_ino_params] += lines_subordinateSwitches
            lines[line_ino_params] += lines_superordinateNames
            lines[line_ino_params] += lines_superordinateSwitchesNames
            lines[line_ino_params] += lines_superordinateSwitches
            lines[line_ino_params] += ino_params_end_keyword
            lines[line_ino_params] += "\n"
        # for i in lines:
        #     print(i)
        with open(ino_file_name, 'w') as ino_file:
            ino_file.writelines(lines)
        
    def user_input_confirm():
        print("Completed parsing the folder name: {}.".format(master_folder_name))
        print("Please confirm - do the parse outputs correctly reflect your intended layout?")
        userConfirmed = False
        while (userConfirmed == False):
            userInput= input("Y = Yes, N = No:\n")
            if (userInput.upper() == "Y"):
                print("Confirmed. Outputting.")
                userConfirmed = True
            elif (userInput.upper() == "N"):
                print("No. Not outputting. Exiting.")
                exit()
            else:
                pass

    def print_lists_ab_concurrently(list_a, list_b):
        #print("print_lists_ab_concurrently()")
        len_list = len(list_a)
        for i in range(len_list):
            print("{}, {}".format(list_a[i], list_b[i]))
        
    def create_dirs_pages(master_folder_name):
        #print("Main.create_dirs_pages()")
        dirs = []
        pages = []
        if (not master_folder_name):
            return [], []
        all_files = os.listdir(master_folder_name)
        if not all_files:
            return [], []
        for this_root, this_dirs, this_files in os.walk(master_folder_name):
            try:
                folder_name = this_root.split("\\")[1::]
                page_name = this_root.split("\\")[-1]
                if folder_name:
                    dirs.append(folder_name)
                    pages.append(page_name)
            except:
                pass
        return dirs, pages

    def get_subordinate_superordinate_names(dirs, pages):
        #print("get_subordinate_superordinate_names()")
        allSubordinateNames = []
        allSuperordinateNames = []
        for name in pages:
            ordinateDirIndices = []
            ordinateNameIndices = []
            subordinateNames = []
            superordinateNames= []
            ordinateDirIndices, ordinateNameIndices = Main.get_ordinate_dir_name_indices(dirs, name)
            subordinateNames = Main.get_subordinate_names(dirs, ordinateDirIndices, ordinateNameIndices)
            superordinateNames = Main.get_superordinate_names(dirs, ordinateDirIndices, ordinateNameIndices)
            allSubordinateNames.append(subordinateNames)
            allSuperordinateNames.append(superordinateNames)
        # print("- Pages: ")
        # for i in pages:
        #     print(i)
        print("- allSubordinateNames: ")
        for i in allSubordinateNames:
            print(i)
        print("- allSuperordinateNames: ")
        for i in allSuperordinateNames:
            print(i)
        return allSubordinateNames, allSuperordinateNames
    
    def get_subordinate_superordinate_switches_empty(dirs, pages, allSubordinateNames, allSuperordinateNames):
        #print("get_subordinate_superordinate_names()")
        allSubordinateSwitchesPositions = []
        allSuperordinateSwitchesPositions = []
        allSubordinateSwitchesNames = []
        allSuperordinateSwitchesNames = []
        for i in range(len(pages)): # Subordinates
            currentRailName = pages[i]
            subsCount = allSubordinateNames[i].count("NONE")
            if (subsCount <= 1): # If this is a switch
                if (currentRailName not in allSubordinateSwitchesNames):
                    allSubordinateSwitchesNames.append(currentRailName)
                    allSubordinateSwitchesPositions.append([0,0,0])
        for i in range(len(pages)): # Superordinates
            currentRailName = pages[i]
            supersCount = allSuperordinateNames[i].count("NONE")
            if (supersCount <= 1): # If this is a switch
                if (currentRailName not in allSuperordinateSwitchesNames):
                    allSuperordinateSwitchesNames.append(currentRailName)
                    allSuperordinateSwitchesPositions.append([0,0,0])
        # print("- Pages: ")
        # for i in pages:
        #     print(i)
        print("- allSubordinateSwitchesPositions: ")
        for i in allSubordinateSwitchesPositions:
            print(i)
        print("- allSuperordinateSwitchesPositions: ")
        for i in allSuperordinateSwitchesPositions:
            print(i)
        print("- allSubordinateSwitchesNames: ")
        for i in allSubordinateSwitchesNames:
            print(i)
        print("- allSuperordinateSwitchesNames: ")
        for i in allSuperordinateSwitchesNames:
            print(i)
        return allSubordinateSwitchesPositions, allSubordinateSwitchesNames, allSuperordinateSwitchesPositions, allSuperordinateSwitchesNames
        
    def get_ordinate_dir_name_indices(dirs, name):
        #print("get_ordinate_dir_name_indices()")
        len_dirs = len(dirs)
        ordinateDirIndices = []
        ordinateNameIndices = []
        dirIndex = 0
        for thisDir in dirs:
            if (name in thisDir):
                nameIndex = thisDir.index(name)
                ordinateDirIndices.append(dirIndex)
                ordinateNameIndices.append(nameIndex)
            dirIndex += 1
        #print("- ordinateDirIndices: {}".format(ordinateDirIndices))
        #print("- ordinateNameIndices: {}".format(ordinateNameIndices))
        return ordinateDirIndices, ordinateNameIndices

    def get_subordinate_names(dirs, ordinateDirIndices, ordinateNameIndices):
        #print("get_subordinate_names()")
        len_dirs = len(dirs)
        subordinateDirIndices = []
        subordinateNameIndices = []
        subordinateNames = ["NONE","NONE","NONE"]
        subordinateCounter = 0
        dirCounter = 0
        for dirIndex in ordinateDirIndices:
            nameIndex  = ordinateNameIndices[dirCounter]
            subordinateNameIndex = nameIndex + 1
            try:
                thisName = dirs[dirIndex][subordinateNameIndex]
                subordinateDirIndices.append(dirIndex)
                subordinateNameIndices.append(nameIndex)
                if (thisName not in subordinateNames):
                    subordinateNames[subordinateCounter] = thisName
                    subordinateCounter += 1
            except:
                pass
            dirCounter += 1
        #print("- subordinateNames: {}".format(subordinateNames))
        return subordinateNames
    
    def get_superordinate_names(dirs, ordinateDirIndices, ordinateNameIndices):
        #print("get_superordinate_names()")
        len_dirs = len(dirs)
        superordinateDirIndices = []
        superordinateNameIndices = []
        superordinateNames = ["NONE","NONE","NONE"]
        superordinateCounter = 0
        dirCounter = 0
        for dirIndex in ordinateDirIndices:
            nameIndex  = ordinateNameIndices[dirCounter]
            superordinateNameIndex = nameIndex - 1
            if (nameIndex != 0):
                thisName = dirs[dirIndex][superordinateNameIndex]
                superordinateDirIndices.append(dirIndex)
                superordinateNameIndices.append(nameIndex)
                if (thisName not in superordinateNames):
                    superordinateNames[superordinateCounter] = thisName
                    superordinateCounter += 1
            dirCounter += 1
        #print("- superordinateNames: {}".format(superordinateNames))
        return superordinateNames

    def write_list_to_txt(txtName, listToWrite):
        #print("Main.write_list_to_txt()")
        open(txtName, "w").close()
        with open(txtName, "w") as rootsTxt:
            for i in listToWrite:
                rootsTxt.write(i + "\n")
        return
    
    def find_max_len_in_list_of_lists(aList):
        #print("Main.find_max_len_in_list_of_lists()")
        maxLen = 0
        for i in aList:
            lenThis = len(i)
            if (lenThis > maxLen):
                maxLen = lenThis
        return maxLen

if __name__ == "__main__":
    Main()