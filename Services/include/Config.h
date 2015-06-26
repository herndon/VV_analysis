#ifndef Services_Config_hh
#define Services_Config_hh
#include <fstream>
#include <iostream>
//============================================================================
// Config.hh
// header for Config class
// General config class for loading config information adapted from  fc project programs
//
// Author Matt Herndon, University of Wisconsin,
//                       Fermi National Accelerator Laborator
// 2014-06-17
//============================================================================
#include <fstream>
#include <iostream>
//#include <stream>

namespace vvana {

///
/// Class Config
/// Author Matt Herndon, University of Wisconsin, Fermi National Accelerator Laborator 2014-06-17
/// General config class for loading config information for fc project programs
///

class Config {


public:

  Config(std::ifstream&);

    int debugLevel() const {
        return _debugLevel;   //!< Returns debug level: 0 nothing, 1 progress infrmation; 2 data object printout; 3-5 basic-detailed debug
    }


    std::string const& debugFileName() const {
        return _debugFileName;    //!< Returns the name of text file for debug output
    }

    std::string const& rootFileName() const {
        return _rootFileName;    //!< Returns the name of the root file to be read.
    }

    std::string const& lheFileName() const {
        return _lheFileName;    //!< Returns the name of the lhe file to be read.
    }


    void printConfig(std::ostream& out) const;


private:

  int _debugLevel;

  std::string _debugFileName;

  std::string _rootFileName;
  std::string _lheFileName;

  void _initConfig(std::ifstream&);

};
} // end namespace vvana 

#endif // Services_Config_hh
