#include <Arduino.h>

class Logger{


    public :

    Logger(){
        Serial.begin(9600);
    }

    void Log(String message = ""){
        Serial.println(message);
    }

    void LogInline(String message){
    Serial.print(message);
    }
    
};