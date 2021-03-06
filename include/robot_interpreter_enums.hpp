// ==========================================================================
// robot_interpreter_enums.hpp
// Copyright (C) 2021  Maaike Hovenkamp

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
// ==========================================================================

# ifndef ROBOT_INTEPRETER_ENUMS_HPP
# define ROBOT_INTEPRETER_ENUMS_HPP

/**
 * @brief PARSE_WORDS enum class for the Interpreter
 * 
 */
struct PARSE_WORDS{
    String weatherstation = "weatherstation";
    String walk_reminder = "walk_reminder";
    String water_reminder = "water_reminder";
    String break_reminder = "break_reminder";
    String shut_down_after = "shut_down_after";
    String STATE = "STATE";
    String IDLE = "IDLE";
    String REMINDER_BREAK = "REMINDER_BREAK";
    String REMINDER_WATER = "REMINDER_WATER";
    String REMINDER_WALK = "REMINDER_WALK";
    String WEATHER_STATION = "WEATHER_STATION";
    String INTERACTIVE_MODE = "INTERACTIVE_MODE";
    String OFF = "OFF";
    String ACTIVE = "ACTIVE";
    String NON_ACTIVE = "NON_ACTIVE";
    String PERIOD = "PERIOD";
    String DURATION = "DURATION";
    String MINUTE_ = "MINUTE";
    String HOUR_ = "HOUR";
    String SECOND_ = "SECOND";
    String MILLI_SECOND_ = "MILLI_SECOND";
    String WAIT = "WAIT";
    String interactive_mode = "interactive_mode";
    String shutdown = "shutdown";
    String move_head = "move_head";
    String move_neck = "move_neck";
    String showWeatherStation = "show_weather_station";
    String getBreakTime = "getBreakTime";
    String getWalkTime = "getWalkTime";
    String getWaterTime = "getWaterTime";
    String getWeatherStationTime = "getWeatherStationTime";
    String getState = "getState";
    String getHeadPos = "getHeadPos";
    String getNeckPos = "getNeckPos";
    String getDistance = "getDistance";
    String getLastMovementDetected = "getLastMovementDetected";
    String rngMovement = "rng_movement";
    String returnToStartPos = "return_to_start_pos";
    String playSound = "playSound";
    String start_up = "start_up";
    String shut_down = "shut_down";
    String notification = "notification";
    String SETUP = "SETUP:";
    String LOOP = "LOOP:";
    String EOF_ = "EOF:";
    String IF = "IF";
    String WHILE = "WHILE";
};

/**
 * @brief NODE_TYPES enum class for the Interpreter
 * 
 */
enum class NODE_TYPES{
    NODE,
    STATE_SETTER,
    SETTER,
    COMMAND,
    WAIT,
    ERROR,
    IFNODE,
    INTEGER_NODE,
    MATH_NODE,
    WHILENODE
};

enum class MATH_TYPES{
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE
};


#endif