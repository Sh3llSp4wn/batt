#include <iostream>
#include <boost/json.hpp>

#include "pretty.hpp"

#include <sys/ioctl.h>
#include <machine/apmvar.h>
#include <fcntl.h>

// you will pry switch statements from my cold dead hands
const std::string get_ac_state(unsigned char state){
  switch(state){
    case APM_AC_OFF:
      return "APM_AC_OFF";
    case APM_AC_ON:
      return "APM_AC_ON";
    case APM_AC_BACKUP:
      return "APM_AC_BACKUP";
    case APM_AC_UNKNOWN:
      return "APM_AC_UNKNOWN";
    default:
      return "";
  }
}
const std::string get_batt_state(unsigned char state){
  switch(state){
    case APM_BATT_HIGH:
      return "APM_BATT_HIGH";
    case APM_BATT_LOW:
      return "APM_BATT_LOW";
    case APM_BATT_CRITICAL: 
      return "APM_BATT_CRITICAL";
    case APM_BATT_CHARGING:
      return "APM_BATT_CHARGING";
    case APM_BATT_UNKNOWN:
      return "APM_BATT_UNKNOWN";
    case APM_BATTERY_ABSENT:
      return "APM_BATT_ABSENT";
    default:
      return "";
  }
}

int main(){
  // set up vars
  boost::json::object obj;
  int fd;
  struct apm_power_info apm;

  // open the power managment device
  fd = open("/dev/apm", O_RDONLY);
  if(ioctl(fd, APM_IOC_GETPOWER, &apm) == -1) return 1;
  
  /*
   *struct apm_power_info { 
   * u_char battery_state; 
   * u_char ac_state; 
   * u_char battery_life; 
   * u_char spare1; 
   * u_int minutes_left; 
   * u_int spare2[6]; 
   * };
   */

  // construct the output object
  obj["batt_info"] = {
    {"battery_state", get_batt_state(apm.battery_state)},
    {"ac_state", get_ac_state(apm.ac_state)},
    {"battery_life", apm.battery_life },
    {"spare1", apm.spare1}, // idk what the hell this does
    {"minutes_left",static_cast<int>(apm.minutes_left) },
    {"spare2", 
      // ???
      {apm.spare2[0], 
      apm.spare2[1], 
      apm.spare2[2], 
      apm.spare2[3], 
      apm.spare2[4], 
      apm.spare2[5]}}
  };

  // print the constructed object
  printers::pretty_print(std::cout, boost::json::value(obj));

  return 0;
}
