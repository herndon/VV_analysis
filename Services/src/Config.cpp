#include <string>
#include "Services/include/Exception.h"
#include "Services/include/Config.h"

vvana::Config::Config(std::ifstream & configfile,int genData,int eventNumberForEventDisplay=-1):
    _debugLevel(1) {

    _initConfig(configfile);

}

void vvana::Config::_initConfig(std::ifstream & configfile) {


    if (!configfile) {
        throw Exception("Config::_initConfig: can't open configfile.txt file");
    }


    std::string configString;


    while(configfile) {
      configfile >> configString;

      if (configString == "DebugLevel") {
	configfile >> _debugLevel;
      } else if (configString == "RootFileName") {
	configfile >> _rootFileName;
      } else {
	throw Exception("Config::_initConfig: Unrecognized parameter in configfile.txt");
      }


    } // endl while loop through configfile

}



void vvana::Config::printConfig(std::ostream& out) const {

    out << "Program configuration information" << std::endl;

        out << "Debug Level: " << _debugLevel << std::endl;
        out << "Root file name " << _rootFileName << std::endl;

}
