// ==========================================================================
// robot.hpp
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

# ifndef ROBOT_HPP
# define ROBOT_HPP

#include "servo.hpp"
#include "PIRsensor.hpp"
#include "oledScreen.hpp"
#include "internalBleSensors.hpp" 
#include "buzzer.hpp"
#include "microLidar.hpp"

/**
 * @brief enum class ROBOT_STATES, all posible robot states
 * 
 */
enum class ROBOT_STATES{
    IDLE,
    REMINDER_BREAK,
    REMINDER_WATER,
    REMINDER_WALK,
    WEATHER_STATION,
    INTERACTIVE_MODE,
    OFF
};

/**
 * @brief main Robot class supporting the social robotics robot.
 * 
 */
class Robot{
private:
    Servo &head_servo;
    Servo &neck_servo;
    PIRSensor &pir_sensor;
    MicroLidar &lidar;
    InternalBLESensors internal_sensors;
    OledScreen &face_screen;
    Buzzer buzzer;

    unsigned int break_time = 2 * HOUR;
    unsigned int walk_time = 30 * MINUTE;
    unsigned int water_time = 50 * HOUR;
    unsigned int shutdown_after = 5 * MINUTE;
    unsigned int weather_station_time = 5 * MINUTE;

    unsigned int break_time_duration = 15 * MINUTE;
    unsigned int walk_time_duration = 5 * MINUTE;
    unsigned int water_time_duration = 30 * SECOND;
    unsigned int interactive_mode_duration = 20 * SECOND;
    unsigned int weather_station_duration = 5 * SECOND;

    bool break_time_active = true;
    bool walk_time_active = true;
    bool water_time_active = true;
    bool weather_time_active = true;

    unsigned long start_time_timer_water;
    unsigned long start_time_timer_walk;
    unsigned long start_time_timer_break;
    unsigned long start_time_timer_weather;
    unsigned long current_time_difference;

    ROBOT_STATES current_state;
    ROBOT_STATES prev_state;
    ROBOT_STATES temp_state;

    unsigned int distance_found_object;
    unsigned int found_object_x;
    unsigned int found_object_y;
    unsigned int tmp_x_coordinate;
    unsigned int tmp_y_coordinate;
    

    /**
     * @brief tries to follow a found object by making a spiraling motion when hand is removed. 
     * 
     * @return true 
     * @return false 
     */
    bool followClosestObject();

    /**
     * @brief Idle state of the robot; idle animation.
     * 
     */
    void idleState();
    /**
     * @brief Break reminder state of the robot; attention movement, break animation
     * 
     */
    void reminderBreak();
    /**
     * @brief Walk reminder state of the robot; attention movement, notification, walk animation
     * 
     */
    void reminderWalk();
    /**
     * @brief Water reminder state of the robot; attention movement, notification, water animation
     * 
     */
    void reminderWater();
    /**
     * @brief Weather station state of the robot; weatherStation animation
     * 
     */
    void showWeatherStation();
    /**
     * @brief Makes the robot search out the user and look at hiim/follow him, return true after succes.
     * 
     * @return true 
     * @return false 
     */
    bool interactiveMode();
public:
    /**
     * @brief Construct a new Robot object
     * 
     * @param head_servo : Servo
     * @param neck_servo : Servo
     * @param pir_sensor : PIRSensor
     * @param lidar : MicroLidar
     * @param buzzer_pin : Buzzer
     */
    Robot(  Servo & head_servo,
            Servo & neck_servo,
            PIRSensor & pir_sensor,
            MicroLidar & lidar,
            OledScreen & face_screen,
            int buzzer_pin ):
            head_servo(head_servo),
            neck_servo(neck_servo),
            pir_sensor(pir_sensor),
            lidar(lidar),
            face_screen(face_screen),
            buzzer( Buzzer(buzzer_pin) )
        {}
    /**
     * @brief setup the robot, call before use of other functions
     * 
     */
    void setup();

    /**
     * @brief main run function of the robot
     * 
     */
    void run();


    /**
     * @brief Get the Break Time loop interval
     * 
     * @return unsigned int 
     */
    unsigned int getBreakTime();
    /**
     * @brief Get the Walk Time loop interval
     * 
     * @return unsigned int 
     */
    unsigned int getWalkTime();
    /**
     * @brief Get the Water Time loop interval
     * 
     * @return unsigned int 
     */
    unsigned int getWaterTime();
    /**
     * @brief Get the Weather Station Time object
     * 
     * @return unsigned int 
     */
    unsigned int getWeatherStationTime();

    /**
     * @brief Get the distance measured from the micro lidar in the head in mm
     * 
     * @return unsigned int 
     */
    unsigned int getMicroLidarDistance();

    /**
     * @brief Set the Break Time Active object
     * 
     * @param active 
     */
    void setBreakTimeActive(bool active);
    /**
     * @brief Set the Walk Time Active object
     * 
     * @param active 
     */
    void setWalkTimeActive(bool active);
    /**
     * @brief Set the Water Time Active object
     * 
     * @param active 
     */
    void setWaterTimeActive(bool active);
    /**
     * @brief Set the Break Time Active object
     * 
     * @param active 
     */
    void setWeatherStationActive(bool active);

    /**
     * @brief Set the Break Time loop interval
     * 
     * @param time : unsigned long, loop interval
     */
    void setBreakTime(unsigned long time);
    /**
     * @brief Set the Walk Time loop interval
     * 
     * @param time : unsigned long, loop interval
     */
    void setWalkTime(unsigned long time);
    /**
     * @brief Set the Water Time loop interval
     * 
     * @param time : unsinged long, loop interal
     */
    void setWaterTime(unsigned long time);
    /**
     * @brief Set the Shutdown After loop interval
     * 
     * @param time : unsigned long, shut down after x time of inactivity. 
     */
    void setShutdownAfter(unsigned long time);
    /**
     * @brief Set the Weather station Time loop interval
     * 
     * @param time 
     */
    void setWeatherStationTime(unsigned long time);

    /**
     * @brief Set the Break Time Duration 
     * 
     * @param time : unsinged long
     */
    void setBreakTimeDuration(unsigned long time);
    /**
     * @brief Set the Walk Time Duration 
     * 
     * @param time : unsigned long
     */
    void setWalkTimeDuration(unsigned long time);
    /**
     * @brief Set the Water Time Duration 
     * 
     * @param time : unsigned long
     */
    void setWaterTimeDuration(unsigned long time);
    /**
     * @brief Set the Interactive Mode Duration 
     * 
     * @param time : unsigned long
     */
    void setInteractiveModeDuration(unsigned long time);
    /**
     * @brief Set the Weather Station Duration
     * 
     * @param time 
     */
    void setWeatherStationDuration(unsigned long time);

    /**
     * @brief Set the State of the robot
     * 
     * @param state : ROBOT_STATES, robot state to set state to
     */
    void setState(ROBOT_STATES state);
    /**
     * @brief Get the current State of the robot
     * 
     * @return ROBOT_STATES
     */
    ROBOT_STATES getState();
    /**
     * @brief print a given state to the Serial port
     * 
     * @param state_to_print 
     */
    void printState(ROBOT_STATES state_to_print);

    /**
     * @brief move the head servo, head up and down
     * 
     * @param pos : int, between 40 and 140
     */
    void moveHead( int pos );
    /**
     * @brief move the neck servo, head left and rigth
     * 
     * @param pos pos : int, between 40 and 140
     */
    void moveNeck( int pos );

    /**
     * @brief Get the Head Position in degrees
     * 
     * @return int 
     */
    int getHeadPos();

    /**
     * @brief Get the Neck Position in degrees
     * 
     * @return int 
     */
    int getNeckPos();

    /**
     * @brief Get the Last movement detected in seconds
     * 
     * @return int 
     */
    int getLastMovementDetected();

    /**
     * @brief move the robot randomly
     * 
     */
    void rngMovement();

    /**
     * @brief return robot to starting position, 90 degrees for head servo and 90 degress for neck servo.
     * 
     */
    void returnToStartPos();

    /**
     * @brief play preprogramed song on buzzer
     * 
     * @tparam T : unsigned int, amount of notes
     * @param song : std::array<std::array<int, 2>, T>, song to be played
     */
    template <unsigned int T>
    void playSound(std::array<std::array<int, 2>, T> song){
        buzzer.playSong<T>(song);
    }

    /**
     * @brief shut down robot untill reactivate by movement on front of PIR sensor. 
     * 
     */
    void shutDown();
};

#endif
