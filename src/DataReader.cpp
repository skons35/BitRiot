#include "DataReader.h"
#include "MickLogger.h"

DataReader::DataReader()
{
  m_pIntFilemap = new map<string, map<string, int> >;
  m_pStrFilemap = new map<string, map<string, string> >;

  m_errorString = "";
  //m_windowHandle = NULL;
	
 // VA debug log  :
 //MickLogger::getInstance()->debug(nullptr, "DataReader() created");
	
 //MickLogger::getInstance()->debug(nullptr, std::string("DataReader() m_pIntFilemap size: ").append( std::to_string(m_pIntFilemap->size()) ) );
 //MickLogger::getInstance()->debug(nullptr, std::string("DataReader() m_pStrFilemap size: ").append( std::to_string(m_pStrFilemap->size()) ) );
}

DataReader::~DataReader()
{
  delete m_pIntFilemap;
  delete m_pStrFilemap;
	
 // VA debug log  :
 //MickLogger::getInstance()->debug(nullptr, "DataReader() deleted");
}

DataReader * DataReader::getInstance()
{
  static DataReader instance;
  return &instance;
}

/*void DataReader::setWindowHandle(HWND mainWindowHandle)
{
  m_windowHandle = mainWindowHandle;
}*/

bool DataReader::displayAnyErrors()
{
  printf("Error in DataReader: %s", m_errorString.c_str());
  return true;
  /*if (m_windowHandle != NULL)
  {
    if (m_errorString.length() == 0)
      return false;
    else
    {
            MessageBox(m_windowHandle,
        m_errorString.c_str(),
        NULL, MB_ICONEXCLAMATION);
      SendMessage(m_windowHandle,WM_CLOSE,0,0);
      return true;
    }
  } else {
    return false;
  }*/
}

void DataReader::clearCache()
{
  m_pIntFilemap->clear();
  m_pStrFilemap->clear();
}

const int DataReader::getIntFromFile(const string & constantName, const string & filename)
{
  //VA debug log : 
  //std::string infodeb = std::string("DataReader::getIntFromFile() > constantName : ").append(constantName).append(" , filename :").append(filename);
  //MickLogger::getInstance()->debug(nullptr, infodeb);		
  
  map<string, map<string, int> >::iterator filemapIterator = m_pIntFilemap->find(filename);
  if (filemapIterator == m_pIntFilemap->end())
  {
    // filename was not found in filemap, try loading it in	
    bool fileLoaded = readTextFile(filename);
    if (fileLoaded)
    {
      // use a bit of recursion (one call)
      return getIntFromFile(constantName, filename);
    }
    else
    {
      // error message set by readTextFile()
      displayAnyErrors();
      return 1;
    }
  }
  else
  {
    // filename is found, try getting constant value by constant name string
    map<string, int> & valuemap = filemapIterator->second;

    map<string, int>::iterator valuemapIterator = valuemap.find(constantName);
    if (valuemapIterator == valuemap.end())
    {
      // constant name not found in file listing
      m_errorString = "Constant: " + constantName + " not found in " + filename + "!";
      displayAnyErrors();
      return 1;
    }
    else
    {
      // constant name was found, return the value :D
      return valuemapIterator->second;
    }
  } // end if filename found
}

const string DataReader::getStringFromFile(const string & constantName, const string & filename)
{

  //VA debug log : 
  //std::string infodeb = std::string("DataReader::getStringFromFile() > constantName : ").append(constantName).append(" , filename :").append(filename);
  //MickLogger::getInstance()->debug(nullptr, infodeb);	
	
  map<string, map<string, string> >::iterator filemapIterator = m_pStrFilemap->find(filename);
  if (filemapIterator == m_pStrFilemap->end())
  {
    // filename was not found in filemap, try loading it in
    bool fileLoaded = readTextFile(filename);
    if (fileLoaded)
    {
      // use a bit of recursion (one call)
      return getStringFromFile(constantName, filename);
    }
    else
    {
      // error message set by readTextFile()
      displayAnyErrors();
      return "";
    }
  }
  else
  {
    // filename is found, try getting constant value by constant name string
    map<string, string> & valuemap = filemapIterator->second;

    map<string, string>::iterator valuemapIterator = valuemap.find(constantName);
    if (valuemapIterator == valuemap.end())
    {
      // constant name not found in file listing
      m_errorString = "Constant: " + constantName + " not found in " + filename + "!";
      displayAnyErrors();
      return "";
    }
    else
    {
      string result = valuemapIterator->second;
      //MickLogger::getInstance()->debug(this, string("DataReader getString: " + result).c_str());
      // constant name was found, return the value :D
      return result;
    }
  } // end if filename found
}

bool DataReader::readTextFile(const string & filename)
{  
  // VA debug Log : 
  //MickLogger::getInstance()->debug(nullptr, std::string("DataReader::readTextFile() called on : ").append( filename ).c_str() );
  
  // VA rewriting parsing using C code (File ,...)	: include stdio.h
  FILE *inputFile = fopen(filename.c_str(), "rb"); // read in binary mode (> no change to end lines)
  if (NULL == inputFile)
	{
		MickLogger::getInstance()->debug(nullptr, std::string("FAIL to open file :").append(filename ).c_str() );
		return false;
	}
  //MickLogger::getInstance()->debug(nullptr, std::string("File OPENed :").append(filename ).c_str() );

  // VA : read car per car using fgetc	
  //  simple file parsing & display car per car : tested ok : 
  //  int car;  // note it is a int, not a char, that returns fgetc()
  //while (1)
  //{	
  //	car = fgetc(inputFile);
  //	if ( EOF == car)
  //	{
  //		fprintf(stdout, "\n");
  //		MickLogger::getInstance()->debug(nullptr, std::string("End of File REACHED").c_str() );
  //		break;
  //	}
  //	
  //	// tempo display :
  //	fprintf(stdout, "%c", car); 
  //	
  //
  // process the caracter (as in original code) :
  //  (done in below more complete iteration)
  //
  //}
  // end of VA : read car per car using fgetc	
	
  // recreate work of original loop code  :  
  map<string, int> intValueMap;
  map<string, string> strValueMap;
  
  /* // VA : tested ok to skip comments lines and empty lines
  bool eol_detected = false; // EOL info needed in debug tempo display output
  int car;    
  car = fgetc(inputFile);
  while (1)
  {	
  	if ( EOF == car)
  	{
  		fprintf(stdout, "\n");
  		MickLogger::getInstance()->debug(nullptr, std::string("End of File REACHED").c_str() );
  		break;
  	}
	
	// pure debug display cosmetic
	if (eol_detected)
	 { fprintf(stdout, "\n"); eol_detected = false; }
		
	
	if ( '#' == car)
	{ 	  
	  while (( EOF != car) && ( '\n' != (char) car))
	  { 
		  car = fgetc(inputFile);
	  }	 
	  //runMickLogger::getInstance()->debug(nullptr, std::string("(Comment line skipped)").c_str() );
	  continue;
	}
  	
	if ( ( '\r' == (char) car) || ( '\n' == (char) car))
	{
      while ( ( EOF != car) && ( ( '\r' == (char) car) || ( '\n' == (char) car)) )
	   {
		   car = fgetc(inputFile); 
	   }	 
	  // explicit request a EOL to add to debug line output (after one or more EOL cars parsed in sequence)
	  eol_detected = true;
	  continue;
	}
	
	// At this point, we should have reached a line with 2 elements ( key value ) separated per space(s)
		
	// tempo display last acquired car (one car displayed at a time):
  	fprintf(stdout, "%c", car); 	  
	
	// prepare looping : try get next char
	car = fgetc(inputFile);
	
	// TO DO : rewrite using using fscanf() ???
	// try a key - value extraction  (value may be a numeric OR alpha type)
  }  
  */ // end of VA : tested ok to skip comments lines and empty lines
  
  
  char lineBuffer[255];
  char varNameBuffer[126];
  char varValueBuffer[126];
  
  while ( NULL != fgets(lineBuffer, sizeof(lineBuffer), inputFile) )
  {
	if ( '\n' == lineBuffer[0] || '#' == lineBuffer[0])
	  continue; // skip empty line or comment ones
	 
	// assuming a 2 param lines (var then space(s) then value (numeric or alpha))
    if ( 2 != sscanf(lineBuffer, "%s %s", varNameBuffer, varValueBuffer) )
     {
	   fprintf(stderr, "Skipping line in file:%s > unexpected format : %s \n", filename.c_str(), lineBuffer);
	   continue;
     }
	    
	// tempo display pair
	//fprintf(stdout, "%s -> %s \n", varNameBuffer, varValueBuffer);
	 
	// ok extract variable name as string  and variable value as numeric OR string otherwise
	string constantName = varNameBuffer;
	string valueString = varValueBuffer; 
	 
	// test for string or int
    if (isAlpha(valueString.at(0)))
     {
       // this is a string constant
       pair<string, string> entry(constantName, valueString);
       strValueMap.insert(entry);
     }
	else // should be a numeric 
	 {
	    if(!isNumericString(valueString))
         {		   
           string errorString = "Value after string: " + constantName + " in file: " + filename + " begins with a number but is not an int.";
		   MickLogger::getInstance()->debug(nullptr, errorString.c_str() );
           // ERROR Case exit :
		   fclose(inputFile);
           MickLogger::getInstance()->debug(nullptr, std::string("File CLOSED :").append(filename ).c_str() );
           return false; 
         }
        else
         {
           // int value is ok, add to valueMap
           stringstream ss(valueString);
           int constantValue;
           ss >> constantValue;

           pair<string, int> entry(constantName, constantValue);
           intValueMap.insert(entry);
         } 
	 }
  }      
  
  fclose(inputFile);
  //MickLogger::getInstance()->debug(nullptr, std::string("File CLOSED :").append(filename ).c_str() );
  
  // tempo debug info :
  //fprintf(stdout,"Extracted var infos : \n intMap size = %u ,  strMap size = %u \n", intValueMap.size(), strValueMap.size());
  
  // FINISH ME (add maps to existing ones...)
 
  // add the constant valueMaps to the object's filemaps
  pair<string, map<string, int> > intFilemapEntry(filename, intValueMap);
  m_pIntFilemap->insert(intFilemapEntry);
  pair<string, map<string, string> > strFilemapEntry(filename, strValueMap);
  m_pStrFilemap->insert(strFilemapEntry);
  
  // we are don, success read
  return true;
  
	
  // VA below original code hangs using inpustream and open() from this static DataReader class:		
  
  /*  // VA : start of Original Code 
  ifstream inputFile;
  
  inputFile.open(filename.c_str());
  
	
  if (!inputFile.is_open())
  {
    m_errorString = "Could not open file: " + filename;
    return false;
  }
  
  // VA DEBUG
  MickLogger::getInstance()->debug(nullptr, std::string("File opened").c_str() );
  

  // set up map<string, int> for this file
  map<string, int> intValueMap;
  map<string, string> strValueMap;

  // loop through input file as char types
  char input;
  inputFile.get(input);
  
  // VA DEBUG
  MickLogger::getInstance()->debug(nullptr, std::string("Looping in file caracter per caracter...: ").c_str() );
  
  while ((input != '\0') && inputFile)
  {
	// VA DEBUG	
    fprintf(stderr, "%c", input);
	  
    if (isWhitespace(input))
    {
      inputFile.get(input);
      continue;
    }
    if (input == '#')
    {
      // this line is a comment, ignore it
      while ((input != '\n') && (input != '\0'))
      {
        inputFile.get(input);
      }
      continue;
    }
    if (input == '\n')
    {
      inputFile.get(input);
      continue; // blank line
    }

    // get first string as constant name
    string constantName;
    while (isWhitespace(input) == false)
    {
      // add input to string, after checking for unexpected end of string or line
      if (input == '\0')
      {
        m_errorString = "Unexpected end of file: " + filename + " after string: " + constantName;
        inputFile.close();
        return false;
      }
      if (input == '\n')
      {
        m_errorString = "Unexpected end of line in file: " + filename + " after string: " + constantName;
        inputFile.close();
        return false;
      }
      constantName.push_back(input);
      inputFile.get(input);
    }

    // whitespace found, constantName is complete
    // remove inbetween whitespace
    while (isWhitespace(input))
    {
      inputFile.get(input);
      if (!inputFile)
      {
        break;
      }
    }
    // check for unexpected end of file
    if (!inputFile)
    {
      m_errorString = "Unexpected end of file: " + filename + " after string: " + constantName;
      inputFile.close();
      return false;
    }
    if (input == '\n')
    {
      m_errorString = "Unexpected end of line in file: " + filename + " after string: " + constantName;
      inputFile.close();
      return false;
    }

    // get int or string
    string valueString;

    // loop till end of line (and ignore whitespace)
    while (input != '\n')
    {
      valueString.push_back(input);
      inputFile.get(input);
      if (!inputFile)
      {
        break;
      }
    }
    if (valueString.length() == 0)
    {
      // error reading int
      m_errorString = "Error reading value in file: " + filename + " after: " + constantName;
      inputFile.close();
      return false;
    }
    if ((input != '\n') && (!(isAlphaNum(input))))
    {
      m_errorString = "Error reading value in file: " + filename + " after: " + constantName;
      inputFile.close();
      return false;
    }

    // test for string or int
    if (isAlpha(valueString.at(0)))
    {
      // this is a string constant
      pair<string, string> entry(constantName, valueString);
      strValueMap.insert(entry);

    }
    else
    {
      // must begin with a number
      if(!isNumericString(valueString))
      {
        m_errorString = "Value after string: " + constantName + " in file: "
                        + filename + " begins with a number but is not an int.";
        inputFile.close();
        return false;
      }
      else
      {
        // int value is ok, add to valueMap
        stringstream ss(valueString);
        int constantValue;
        ss >> constantValue;

        pair<string, int> entry(constantName, constantValue);
        intValueMap.insert(entry);
      }
    }


    // remove whitespace
    while (isWhitespace(input))
    {
      inputFile.get(input);
      if (!inputFile)
      {
        break;
      }
    }

    // test for eof
    if (!inputFile)
    {
      break;
    }
    else
      // begin loop again
    {
      inputFile.get(input);
    }
  }

  // we've now parsed the input file
  inputFile.close();

  // add the constant valueMaps to the object's filemaps
  pair<string, map<string, int> > intFilemapEntry(filename, intValueMap);
  m_pIntFilemap->insert(intFilemapEntry);
  pair<string, map<string, string> > strFilemapEntry(filename, strValueMap);
  m_pStrFilemap->insert(strFilemapEntry);

  return true;
  
  */
  // VA : end of Original Code 
}

inline bool DataReader::isWhitespace(char c)
{
  return ((c == ' ') || (c == '\t'));
}

inline bool DataReader::isAlphaNum(char c)
{
  return (isAlpha(c) || isDigit(c));
}

inline bool DataReader::isAlpha(char c)
{
  return ((('A' <= c) && (c <= 'Z')) || (('a' <= c) && (c <= 'z')));
}

inline bool DataReader::isDigit(char c)
{
  return (('0' <= c) && (c <= '9'));
}

bool DataReader::isNumericString(string s)
{
  for (unsigned int i = 0; i < s.length(); i++)
  {
    if (!isDigit(s.at(i)))
    {
      return false;
    }
  }

  return true;
}
