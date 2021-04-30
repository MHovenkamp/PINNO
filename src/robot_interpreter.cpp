#include <robot_interpreter.hpp>

void Interpreter::run(){
    // check for repl mode or full file mode.
    if(mode == INTERPRETER_MODES::IDLE){
        String choice = "";
        Serial.println("Enter mode to which to program robot with: ");
        Serial.println("1. REPL, Enter a single command and instantly see the result");
        Serial.println("2. FILE, upload a file with your written code");
        Serial.println("3. EXIT, exit current program");
        while(choice != "3"){
            Serial.println("waiting for input");
            while(choice == ""){
                choice = Serial.readStringUntil('\n');
            }
            Serial.println("chosen option: " + choice);
            if( choice == "2"){ 
                Serial.println("FILE chosen"); 
                mode = INTERPRETER_MODES::FILE; 
                choice = "";
                break; 
            }
            else if( choice == "1"){
                Serial.println("REPL chosen"); 
                mode = INTERPRETER_MODES::REPL; 
                choice = "";
                break; 
            } else{
                Serial.println("Unknow option");
                Serial.println("Enter mode to which to program robot with: ");
                Serial.println("1. REPL, Enter a single command and instantly see the result");
                Serial.println("2. FILE, upload a file with your written code");
                Serial.println("3. EXIT, exit current program");
                choice = "";
            }
        }
    }
    if(mode == INTERPRETER_MODES::REPL){
        repl();
        mode = INTERPRETER_MODES::IDLE;
    } else if(mode == INTERPRETER_MODES::FILE){
        file();
        mode = INTERPRETER_MODES::IDLE;
    }
};

void Interpreter::file(){
    Serial.println("file");
}

void Interpreter::repl(){
    String command = "";
    while(command != "EXIT"){
        Serial.println("Enter command. Or \"EXIT\" to quit");
        while(command == ""){
            command = Serial.readStringUntil('\n');
        }
        if(command == "EXIT"){
            Serial.println(command +" "+ String(command.length()));
            break;
        }
        Serial.println("given input: " + command);
        std::unique_ptr<Node> node_ptr = parseCommand(command);
        node_ptr->print(); 
        node_ptr->execute(robot);
        node_ptr.reset();
        command = "";
        rtos::ThisThread::sleep_for(MS(5000));
    }
}

std::unique_ptr<Node> Interpreter::parseCommand(String command){
    int amount_of_spaces = 0;
    for(unsigned int i = 0; i < command.length(); i++){
        if( command[i] == ' '){
            amount_of_spaces++;
        }
    }
    String string_array[amount_of_spaces+1] = {};
    int current_array_index = 0;
    int start_index = 0;
    for(unsigned int i = 0; i < command.length(); i++){
        if( command[i] == ' '){
            string_array[current_array_index] = command.substring(start_index,i);
            current_array_index++;
            start_index = i+1;
        }
        if( i == command.length()-1){
            string_array[current_array_index] = command.substring(start_index,i+1);
        }
    }

    switch (amount_of_spaces+1)
    {
    case 1:
        return std::unique_ptr<Node> (new CommandNode(command, string_array[0], "", false));
        break;
    case 2:
        return std::unique_ptr<Node> (new CommandNode(command, string_array[0], string_array[1], true));
        break;
    case 3:
        if(string_array[1] == "="){
            // STATE = IDLE
            if( string_array[0] == parse_words.STATE && string_array[1] == "="){
                if(string_array[2] == parse_words.IDLE){
                    return std::unique_ptr<Node> (new SetStateNode(command, parse_words.IDLE));
                } else if(string_array[2] == parse_words.REMINDER_BREAK){
                    return std::unique_ptr<Node> (new SetStateNode(command, parse_words.REMINDER_BREAK));
                } else if(string_array[2] == parse_words.REMINDER_WATER){
                    return std::unique_ptr<Node> (new SetStateNode(command, parse_words.REMINDER_WATER));
                } else if(string_array[2] == parse_words.REMINDER_WALK){
                    return std::unique_ptr<Node> (new SetStateNode(command, parse_words.REMINDER_WALK));
                } else if(string_array[2] == parse_words.WEATHER_STATION){
                    return std::unique_ptr<Node> (new SetStateNode(command, parse_words.WEATHER_STATION));
                } else if(string_array[2] == parse_words.INTERACTIVE_MODE){
                    return std::unique_ptr<Node> (new SetStateNode(command, parse_words.INTERACTIVE_MODE));
                } else {
                    return std::unique_ptr<Node> (new ErrorNode(command, "unknown state"));
                }
            }
            // weatherstation = ACTIVE
            if(string_array[2] == parse_words.ACTIVE){
                return std::unique_ptr<Node> (new SetterNode(command, string_array[0],parse_words.ACTIVE, 0 ,parse_words.MILLI_SECOND_ ));
            } else if(string_array[2] == parse_words.NON_ACTIVE){
                return std::unique_ptr<Node> (new SetterNode(command, string_array[0],parse_words.NON_ACTIVE, 0 , parse_words.MILLI_SECOND_ ));
            }
        } else { 
            // WAIT 10 MINUTES
            if(string_array[0] == parse_words.WAIT){
                char letter;
                for(unsigned int i =0 ; i < string_array[1].length()-1; i++){
                    letter = string_array[1][i];
                    if(!isDigit(letter)){
                        return std::unique_ptr<Node> (new ErrorNode(command, "time value not a digit"));
                        break;
                    }
                }
                if(string_array[2] == parse_words.MINUTE_){
                    return std::unique_ptr<Node> (new WaitNode(command, string_array[1].toInt(), parse_words.MINUTE_));
                } else if(string_array[2] == parse_words.HOUR_){
                    return std::unique_ptr<Node> (new WaitNode(command, string_array[1].toInt(), parse_words.HOUR_));
                } else if (string_array[2] == parse_words.SECOND_){
                    return std::unique_ptr<Node> (new WaitNode(command, string_array[1].toInt(), parse_words.SECOND_));
                } else if (string_array[2] == parse_words.MILLI_SECOND_){
                    return std::unique_ptr<Node> (new WaitNode(command, string_array[1].toInt(), parse_words.MILLI_SECOND_));
                } else {
                    return std::unique_ptr<Node> (new ErrorNode(command, "Unknown time measurement type"));
                }
            }
        }
        return std::unique_ptr<Node> (new ErrorNode(command, "unknown command"));
    case 4:
        //shut_down_after = 10 MINUTE
        if(string_array[0] == parse_words.shut_down_after && string_array[1] == "="){
            char letter;
            for(unsigned int i =0 ; i < string_array[2].length()-1; i++){
                letter = string_array[2][i];
                if(!isDigit(letter)){
                    return std::unique_ptr<Node> (new ErrorNode(command, "time value not a digit"));
                    break;
                }
            }
            if(string_array[3] == parse_words.MINUTE_){
                    return std::unique_ptr<Node> (new SetterNode(command, string_array[0] , parse_words.shut_down_after, string_array[2].toInt(), parse_words.MINUTE_));
                } else if(string_array[3] == parse_words.HOUR_){
                    return std::unique_ptr<Node> (new SetterNode(command, string_array[0] , parse_words.shut_down_after, string_array[2].toInt(), parse_words.HOUR_));
                } else if (string_array[3] == parse_words.SECOND_){
                    return std::unique_ptr<Node> (new SetterNode(command, string_array[0] , parse_words.shut_down_after, string_array[2].toInt(), parse_words.SECOND_));
                } else if (string_array[3] == parse_words.MILLI_SECOND_){
                    return std::unique_ptr<Node> (new SetterNode(command, string_array[0] , parse_words.shut_down_after, string_array[2].toInt(), parse_words.MILLI_SECOND_));
                } else {
                    return std::unique_ptr<Node> (new ErrorNode(command, "Unknown time measurement type"));
                }
        }
        return std::unique_ptr<Node> (new ErrorNode(command, "unknown command"));
    case 5:
        // weatherstation PERIOD = 10 MINUTE
        if((string_array[1] == parse_words.PERIOD || string_array[1] == parse_words.DURATION) && string_array[2] == "="){
            char letter;
            for(unsigned int i =0 ; i < string_array[3].length()-1; i++){
                letter = string_array[3][i];
                if(!isDigit(letter)){
                    return std::unique_ptr<Node> (new ErrorNode(command, "time value not a digit"));
                    break;
                }
            }
            
            return std::unique_ptr<Node> (new SetterNode(command, string_array[0] , string_array[1], string_array[3].toInt(), string_array[4]));
        }
        return std::unique_ptr<Node> (new ErrorNode(command, "unknown command"));
    default:
        return std::unique_ptr<Node> (new ErrorNode(command, "unknown command"));   
        Serial.println("Unknown command: " + command);
        break;
    }
    return std::unique_ptr<Node> (new ErrorNode(command, "unknown command"));
}

void Node::execute(Robot & robot){
    Serial.println(original_string);
}

unsigned int SetterNode::getTime(){
    if(time_measurements == parse_words.MINUTE_){
        return time_period * MINUTE;
    } else if(time_measurements == parse_words.HOUR_){
        return time_period * HOUR;
    } else if(time_measurements == parse_words.SECOND_){
        return time_period * SECOND;
    } else if(time_measurements == parse_words.MILLI_SECOND_){
        return time_period * MIllI_SECOND;
    } else {
        return 0;
    }
}

void SetterNode::execute(Robot & robot){
    if(to_set == parse_words.weatherstation){
        if(setter_type == parse_words.ACTIVE){
            robot.setWeatherStationActive(true);
            Serial.println("Weather station activity set to : TRUE");
        } else if(setter_type == parse_words.NON_ACTIVE){
            robot.setWeatherStationActive(false);
            Serial.println("Weather station activity set to : FALSE");
        } else if(setter_type == parse_words.DURATION){
            robot.setWeatherStationDuration(getTime());
            Serial.println("Weather station duration set to: " + String(getTime()) + "milliseconds");
        } else if(setter_type == parse_words.PERIOD){
            robot.setWeatherStationTime(getTime());
            Serial.println("Weather station loop time set to: " + String(getTime()) + "milliseconds");
        }
    } else if(to_set == parse_words.walk_reminder){
        if(setter_type == parse_words.ACTIVE){
            robot.setWalkTimeActive(true);
            Serial.println("Walk reminder activity set to : TRUE");
        } else if(setter_type == parse_words.NON_ACTIVE){
            robot.setWalkTimeActive(false);
            Serial.println("Walk reminder activity set to : FALSE");
        } else if(setter_type == parse_words.DURATION){
            robot.setWalkTimeDuration(getTime());
            Serial.println("Walk reminder duration set to: " + String(getTime()) + "milliseconds");
        } else if(setter_type == parse_words.PERIOD){
            robot.setWalkTime(getTime());
            Serial.println("Walk reminder loop time set to: " + String(getTime()) + "milliseconds");
        }
    } else if(to_set == parse_words.water_reminder){
        if(setter_type == parse_words.ACTIVE){
            robot.setWaterTimeActive(true);
            Serial.println("Water reminder activity set to : TRUE");
        } else if(setter_type == parse_words.NON_ACTIVE){
            robot.setWaterTimeActive(false);
            Serial.println("Water reminder activity set to : FALSE");
        } else if(setter_type == parse_words.DURATION){
            robot.setWaterTimeDuration(getTime());
            Serial.println("Water reminder duration set to: " + String(getTime()) + "milliseconds");
        } else if(setter_type == parse_words.PERIOD){
            robot.setWaterTime(getTime());
            Serial.println("Water reminder loop time set to: " + String(getTime()) + "milliseconds");
        }
    } else if(to_set == parse_words.break_reminder){
        if(setter_type == parse_words.ACTIVE){
            robot.setBreakTimeActive(true);
            Serial.println("Break reminder activity set to : TRUE");
        } else if(setter_type == parse_words.NON_ACTIVE){
            robot.setBreakTimeActive(false);
            Serial.println("Break reminder activity set to : FALSE");
        } else if(setter_type == parse_words.DURATION){
            robot.setBreakTimeDuration(getTime());
            Serial.println("Break reminder duration set to: " + String(getTime()) + "milliseconds");
        } else if(setter_type == parse_words.PERIOD){
            robot.setBreakTime(getTime());
            Serial.println("Break reminder loop time set to: " + String(getTime()) + "milliseconds");
        }
    } else if(to_set == parse_words.shut_down_after){
        robot.setShutdownAfter(getTime());
        Serial.println("shut down timer set to: " + String(getTime()) + "milliseconds");
    } else if(to_set == parse_words.interactive_mode){
        if(setter_type == parse_words.DURATION){
            robot.setInteractiveModeDuration(getTime());
            Serial.println("Interactive mode duration time set to: " + String(getTime()) + "milliseconds");
        }
    } else {
        Serial.println("Unknown setter: " + original_string);
    }
}

void SetStateNode::execute(Robot & robot){
    if(state == parse_words.IDLE){
        robot.setState(ROBOT_STATES::IDLE);
        Serial.println("robot state set to : IDLE");
    } else if(state == parse_words.REMINDER_BREAK){
        robot.setState(ROBOT_STATES::REMINDER_BREAK);
        Serial.println("robot state set to : REMINDER_BREAK");
    } else if(state == parse_words.REMINDER_WATER){
        robot.setState(ROBOT_STATES::REMINDER_WATER);
        Serial.println("robot state set to : REMINDER_WATER");
    } else if(state == parse_words.REMINDER_WALK){
        robot.setState(ROBOT_STATES::REMINDER_WALK);
        Serial.println("robot state set to : REMINDER_WALK");
    } else if(state == parse_words.WEATHER_STATION){
        robot.setState(ROBOT_STATES::WEATHER_STATION);
        Serial.println("robot state set to : WEATHER_STATION");
    } else if(state == parse_words.INTERACTIVE_MODE){
        robot.setState(ROBOT_STATES::INTERACTIVE_MODE);
        Serial.println("robot state set to : INTERACTIVE_MODE");
    } else {
        Serial.println("Unknown state " + original_string);
    }
}

void CommandNode::execute(Robot & robot){
    Serial.println(command);
    char letter;
    bool is_digit = true;
    for(unsigned int i =0 ; i < param.length()-1; i++){
        letter = param[i];
        if(!isDigit(letter)){
            is_digit = false;
            break;
        }
    }
    if(command == parse_words.shutdown){
        robot.shutDown();
    } else if(command == parse_words.interactive_mode){
        while(robot.interactiveMode()){};
    } else if(command == parse_words.break_reminder){
        Serial.println("break_reminder");
        robot.setState(ROBOT_STATES::REMINDER_BREAK);
    } else if(command == parse_words.walk_reminder){
        robot.setState(ROBOT_STATES::REMINDER_WALK);
    } else if(command == parse_words.water_reminder){
        robot.setState(ROBOT_STATES::REMINDER_WATER);
    }else if(command == parse_words.showWeatherStation){
        robot.setState(ROBOT_STATES::WEATHER_STATION);
    } else if(command == parse_words.getBreakTime){
        Serial.println("Break reminder period " + String(robot.getBreakTime())+" MS");
    } else if(command == parse_words.getWalkTime){
        Serial.println("Walk reminder period " + String(robot.getWalkTime())+" MS");
    } else if(command == parse_words.getWaterTime){
        Serial.println("Water reminder period " + String(robot.getWaterTime())+" MS");
    } else if(command == parse_words.getWeatherStationTime){
        Serial.println("weather station period " + String(robot.getWeatherStationTime())+" MS");
    } else if(command == parse_words.getState){
        ROBOT_STATES state = robot.getState();
        switch (state){
        case ROBOT_STATES::IDLE:
            Serial.println("Current state: IDLE");
            break;
        case ROBOT_STATES::INTERACTIVE_MODE:
            Serial.println("Current state: INTERACTIVE_MODE");
            break;
        case ROBOT_STATES::OFF:
            Serial.println("Current state: OFF");
            break;
        case ROBOT_STATES::REMINDER_BREAK:
            Serial.println("Current state: REMINDER_BREAK");
            break;
        case ROBOT_STATES::REMINDER_WALK:
            Serial.println("Current state: REMINDER_WALK");
            break;
        case ROBOT_STATES::REMINDER_WATER:
            Serial.println("Current state: REMINDER_WATER");
            break;
        case ROBOT_STATES::WEATHER_STATION:
            Serial.println("Current state: WEATHER_STATION");
            break;
        default:
            break;
        }
    } else if(command == parse_words.rngMovement){
        robot.rngMovement();
    } else if(command == parse_words.returnToStartPos){
        robot.returnToStartPos();  
    } else if(is_digit){
        if(command == parse_words.move_head){
            robot.moveHead(param.toInt());
        } else if(command == parse_words.move_neck){
            robot.moveNeck(param.toInt());
        } 
    } else if(!is_digit){
        if(command == parse_words.playSound){
            Songs songs;
            if(param == parse_words.start_up){
                robot.playSound<songs.start_up.size()>(songs.start_up);
            } else if(param == parse_words.shut_down){
                robot.playSound<songs.shut_down.size()>(songs.shut_down);
            } else if(param == parse_words.notification){
                robot.playSound<songs.notification.size()>(songs.notification);
            }
        }
    } else{
        ErrorNode error("Unknown command: ", original_string);
        error.execute(robot);
    }
}

void WaitNode::execute(Robot & robot){
    if(time_measurements == parse_words.MINUTE_){
        rtos::ThisThread::sleep_for(MS( time_period * MINUTE ));
    } else if(time_measurements == parse_words.HOUR_){
        rtos::ThisThread::sleep_for(MS( time_period * HOUR ));
    } else if(time_measurements == parse_words.SECOND_){
        rtos::ThisThread::sleep_for(MS( time_period * SECOND ));
    } else if(time_measurements == parse_words.MILLI_SECOND_){
        rtos::ThisThread::sleep_for(MS( time_period * MIllI_SECOND ));
    } else {
        rtos::ThisThread::sleep_for(MS(0));
    }
}

void ErrorNode::execute(Robot & robot){
    Serial.println("ERROR: " + original_string + " -> " + error_message);
}

void Node::print(){
    Serial.println("Node -> " + original_string);
}

void SetterNode::print(){
    Serial.println("SetterNode -> " +  to_set + " " + setter_type + " " + String(time_period) + " " + time_measurements);
}

void SetStateNode::print(){
    Serial.println("SetStateNode -> set state to " + state);
}

void CommandNode::print(){
    Serial.println("CommandNode -> "+ command + "(" + param + ")");
}

void WaitNode::print(){
    Serial.println("WaitNode -> wait for "+ String(time_period) + " " + time_measurements);
}

void ErrorNode::print(){
    Serial.println("ErrorNode -> " + error_message);
}